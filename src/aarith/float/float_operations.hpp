#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/float/normalized_float.hpp>

namespace aarith {

/**
 * @brief Addition of two normalized_floats using the FAU adder: lhs+rhs
 *
 * @param lhs The first number that is to be summed up
 * @param rhs The second number that is to be summed up
 * @param bits The number of most-significant bits that are calculated of the mantissa addition
 * @tparam E Width of exponent
 * @tparam M Width of mantissa including the leading 1
 *
 * @return The sum
 *
 */
template <size_t E, size_t M, class Function_add, class Function_sub>
[[nodiscard]] auto add_(const normalized_float<E, M> lhs, const normalized_float<E, M> rhs,
                        Function_add fun_add, Function_sub fun_sub) -> normalized_float<E, M>
{
    //order operands
    if (abs(lhs) < abs(rhs))
    {
        return add_(rhs, lhs, fun_add, fun_sub);
    }

    //sub if 2nd operand is negative
    if (lhs.get_sign() != rhs.get_sign())
    {
        auto swap_sign = rhs;
        swap_sign.set_sign(~swap_sign.get_sign());
        return sub_(lhs, swap_sign, fun_add, fun_sub);
    }

    const auto exponent_delta = sub(width_cast<E+1>(lhs.get_exponent()), width_cast<E+1>(rhs.get_exponent()));
    //check for underflow
    if(exponent_delta.bit(E) == 1)
    {
        
    }
    const auto new_mantissa = rhs.get_full_mantissa() >> exponent_delta.word(0);
    const auto mantissa_sum = fun_add(lhs.get_full_mantissa(), new_mantissa);

    normalized_float<E, mantissa_sum.width() - 1> sum;
    sum.set_sign(lhs.get_sign());
    sum.set_exponent(lhs.get_exponent());
    sum.set_full_mantissa(mantissa_sum);

    return normalize<E, mantissa_sum.width() - 1, M>(sum);
}

/**
 * @brief Subtraction with normalized_floats using the FAU adder: lhs-rhs.
 *
 * @param lhs The minuend
 * @param rhs The subtrahend
 * @param bits The number of most-significant bits that are calculated of the mantissa subtraction
 * @tparam E Width of exponent
 * @tparam M Width of mantissa including the leading 1
 *
 * @return The difference lhs-rhs
 *
 */
template <size_t E, size_t M, class Function_add, class Function_sub>
[[nodiscard]] auto sub_(const normalized_float<E, M> lhs, const normalized_float<E, M> rhs,
                        Function_add fun_add, Function_sub fun_sub) -> normalized_float<E, M>
{
    if (abs(lhs) < abs(rhs))
    {
        auto swap = rhs;
        swap.set_sign(~swap.get_sign());
        return add(swap, lhs);
    }

    if (lhs.get_sign() != rhs.get_sign())
    {
        auto swap_sign = rhs;
        swap_sign.set_sign(~swap_sign.get_sign());
        return add_(lhs, swap_sign, fun_add, fun_sub);
    }

    const auto exponent_delta = sub(lhs.get_exponent(), rhs.get_exponent());
    const auto new_mantissa = rhs.get_full_mantissa() >> exponent_delta.word(0);
    const auto mantissa_sum = fun_sub(lhs.get_full_mantissa(), new_mantissa);

    normalized_float<E, mantissa_sum.width() - 1> sum;
    sum.set_sign(lhs.get_sign());
    if (mantissa_sum != uinteger<mantissa_sum.width()>::all_zeroes())
    {
        sum.set_exponent(lhs.get_exponent());
    }
    sum.set_full_mantissa(mantissa_sum);

    return normalize<E, mantissa_sum.width() - 1, M>(sum);
}

/**
 * @brief Adds to normalized_floats.
 *
 * @param lhs The first number that is to be summed up
 * @param rhs The second number that is to be summed up
 * @tparam E Width of exponent
 * @tparam M Width of mantissa including the leading 1
 *
 * @return The sum
 *
 */
template <size_t E, size_t M>
[[nodiscard]] auto add(const normalized_float<E, M> lhs, const normalized_float<E, M> rhs)
    -> normalized_float<E, M>
{
    // return add_<E, M, class aarith::uinteger<M+1, long unsigned int> (*)(const class
    // aarith::uinteger<M+1>&, const class aarith::uinteger<M+1>&, const bool)>(lhs, rhs,
    // expanding_add<M+1, M+1>, expanding_sub<M+1, M+1>);
    return add_<
        E, M,
        class aarith::uinteger<M + 2, uint64_t> (*)(const aarith::uinteger<M + 1, uint64_t>&,
                                                    const aarith::uinteger<M + 1, uint64_t>&),
        class aarith::uinteger<M + 1, uint64_t> (*)(const aarith::uinteger<M + 1, uint64_t>&,
                                                    const aarith::uinteger<M + 1, uint64_t>&)>(
        lhs, rhs, expanding_add<uinteger<M + 1>, uinteger<M + 1>>,
        expanding_sub<uinteger<M + 1>, uinteger<M + 1>>);

    // return add_<E, M>(lhs, rhs, expanding_add<uinteger<M+1>, uinteger<M+1>>,
    // expanding_sub<uinteger<M+1>, uinteger<M+1>>);
}

/**
 * @brief Subtraction with normalized_floats: lhs-rhs.
 *
 * @param lhs The minuend
 * @param rhs The subtrahend
 * @tparam E Width of exponent
 * @tparam M Width of mantissa including the leading 1
 *
 * @return The difference lhs-rhs
 *
 */
template <size_t E, size_t M>
[[nodiscard]] auto sub(const normalized_float<E, M> lhs, const normalized_float<E, M> rhs)
    -> normalized_float<E, M>
{
    // return sub_<E, M>(lhs, rhs, expanding_add<uinteger<M+1>, uinteger<M+1>>,
    // expanding_sub<uinteger<M+1>, uinteger<M+1>>);
    return sub_<
        E, M,
        class aarith::uinteger<M + 2, uint64_t> (*)(const aarith::uinteger<M + 1, uint64_t>&,
                                                    const aarith::uinteger<M + 1, uint64_t>&),
        class aarith::uinteger<M + 1, uint64_t> (*)(const aarith::uinteger<M + 1, uint64_t>&,
                                                    const aarith::uinteger<M + 1, uint64_t>&)>(
        lhs, rhs, expanding_add<uinteger<M + 1>, uinteger<M + 1>>,
        expanding_sub<uinteger<M + 1>, uinteger<M + 1>>);
}

/**
 * @brief Multiplication with normalized_floats: lhs*rhs.
 *
 * @param lhs The multiplicand
 * @param rhs The multiplicator
 * @tparam E Width of exponent
 * @tparam M Width of mantissa including the leading 1
 *
 * @return The product lhs*rhs
 *
 */
template <size_t E, size_t M, typename WordType>
[[nodiscard]] auto mul(const normalized_float<E, M, WordType> lhs,
                       const normalized_float<E, M, WordType> rhs)
    -> normalized_float<E, M, WordType>
{
    //compute sign
    auto sign = lhs.get_sign() ^ rhs.get_sign();

    //compute exponent
    auto ext_esum = expanding_add(lhs.get_exponent(), rhs.get_exponent());
    bool overflow = ext_esum.bit(E) == 1;
    ext_esum = sub(ext_esum, width_cast<E + 1>(lhs.bias));
    bool underflow = (not overflow) and ext_esum.bit(E) == 1;
    overflow = overflow and ext_esum.bit(E) == 1;

    //compute mantissa
    auto mproduct = schoolbook_expanding_mul(lhs.get_full_mantissa(), rhs.get_full_mantissa());
    mproduct = mproduct >> M;

    //check for over or underflow and break
    if(underflow || overflow)
    {
        normalized_float<E, M> product;
        product.set_sign(sign);
        if(overflow)
        {
            product.set_exponent(uinteger<E>::all_ones());
        }
        else
        {
            ext_esum = ~ext_esum;
            ext_esum = add(ext_esum, uinteger<ext_esum.width()>(2));
            if(ext_esum < uinteger<64>(M+1))
            {
                mproduct = mproduct >> ext_esum.word(0);
                product.set_full_mantissa(width_cast<M+1>(mproduct));
            }

        }
        return normalize<E, M, M>(product);
    }

    auto esum = width_cast<E>(ext_esum);

    normalized_float<E, mproduct.width() - 1> product;
    product.set_mantissa(mproduct);
    product.set_exponent(esum);
    product.set_sign(sign);

    return normalize<E, mproduct.width() - 1, M>(product);
}

/**
 * @brief Division with normalized_floats: lhs/rhs.
 *
 * @param lhs The dividend
 * @param rhs The divisor
 * @tparam E Width of exponent
 * @tparam M Width of mantissa including the leading 1
 *
 * @return The quotient lhs/rhs
 *
 */
template <size_t E, size_t M, typename WordType>
[[nodiscard]] auto div(const normalized_float<E, M, WordType> lhs,
                       const normalized_float<E, M, WordType> rhs)
    -> normalized_float<E, M, WordType>
{
    if(lhs.is_inf())
    {
        return lhs;
    }

    if(rhs.is_inf())
    {
        return rhs;
    }

    auto dividend = width_cast<2 * (M + 1) + 3>(lhs.get_full_mantissa());
    auto divisor = width_cast<2 * (M + 1) + 3>(rhs.get_full_mantissa());
    dividend = (dividend << M + 4);
    auto mquotient = div(dividend, divisor);
    // mquotient >>= 1;
    auto rdmquotient = rshift_and_round(mquotient, 4);

    //auto esum = width_cast<E>(
    //    sub(expanding_add(lhs.get_exponent(), lhs.bias), width_cast<E + 1>(rhs.get_exponent())));
    auto esum = expanding_add(lhs.get_exponent(), lhs.bias);
    if (esum <= rhs.get_exponent()) 
    {
        esum = esum.zero();
    }
    else
    {
        esum = width_cast<E>(sub(esum, width_cast<E + 1>(rhs.get_exponent())));
    }
    auto sign = lhs.get_sign() ^ rhs.get_sign();

    normalized_float<E, rdmquotient.width() - 1> quotient;
    quotient.set_full_mantissa(rdmquotient);
    quotient.set_exponent(esum);
    quotient.set_sign(sign);

    return normalize<E, rdmquotient.width() - 1, M>(quotient);
}

/**
 * @brief Extracts a bitstring range from the bit representation of the float
 *
 * Note that the indexing is done
 *  - zero based starting from the LSB
 *  - is inclusive (i.e. the start and end point are part of the range)
 *
 * @tparam Start Starting index (inclusive, from left to right)
 * @tparam Eend  Ending index (inclusive, from left to right)
 * @tparam E Width of the exponent
 * @tparam M Width of the mantissa
 * @param f  Float from which the range is taken from
 * @return Range float[End,Start], inclusive
 */
template <size_t Start, size_t End, size_t E, size_t M, typename WordType>
[[nodiscard]] constexpr word_array<(Start - End) + 1, WordType>
bit_range(const normalized_float<E, M, WordType>& f)
{
    return bit_range<Start, End>(f.as_word_array());
}

namespace float_operators {

template <size_t E, size_t M, typename WordType>
auto operator+(const normalized_float<E, M, WordType>& lhs,
               const normalized_float<E, M, WordType>& rhs) -> normalized_float<E, M, WordType>
{
    return add(lhs, rhs);
}

template <size_t E, size_t M, typename WordType>
auto operator-(const normalized_float<E, M, WordType>& lhs,
               const normalized_float<E, M, WordType>& rhs) -> normalized_float<E, M, WordType>
{
    return sub(lhs, rhs);
}

template <size_t E, size_t M, typename WordType>
auto operator*(const normalized_float<E, M, WordType>& lhs,
               const normalized_float<E, M, WordType>& rhs) -> normalized_float<E, M, WordType>
{
    return mul(lhs, rhs);
}

template <size_t E, size_t M, typename WordType>
auto operator/(const normalized_float<E, M, WordType>& lhs,
               const normalized_float<E, M, WordType>& rhs) -> normalized_float<E, M, WordType>
{
    return div(lhs, rhs);
}

template <size_t E, size_t M, typename WordType>
auto operator%(const normalized_float<E, M, WordType>& lhs,
               const normalized_float<E, M, WordType>& rhs) -> normalized_float<E, M, WordType>
{
    return remainder(lhs, rhs);
}

} // namespace float_operators
} // namespace aarith
