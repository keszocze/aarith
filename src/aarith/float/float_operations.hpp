#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/float/floating_point.hpp>

namespace aarith {

/**
 * @brief Generic addition of two `floating_point` values
 *
 * This method computes the sum of two floating-point values using the provided functions `fun_add`
 * and `fun_sub` to compute the new mantissa. This generic function allows to easily implement
 * own adders, e.g. to develop new hardware implementations.
 *
 * @note As an end-user of aarith, you will, most likely, never need to call this function.
 *
 * @tparam E Exponent width
 * @tparam M Mantissa width
 * @tparam Function_add Function object type for  performing an addition
 * @tparam Function_sub Function object fype for performing a subtraction
 * @param lhs Left-hand side argument of the usm
 * @param rhs Right-hand side argument of the sum
 * @param fun_add Function performing the addition of the mantissae
 * @param fun_sub Function performing the subtraction of the mantissae
 * @return The sum of lhs + rhs using the provided functions
 */
template <size_t E, size_t M, class Function_add, class Function_sub>
[[nodiscard]] auto add_(const floating_point<E, M> lhs, const floating_point<E, M> rhs,
                        Function_add fun_add, Function_sub fun_sub) -> floating_point<E, M>
{
    // order operands
    if (abs(lhs) < abs(rhs))
    {
        return add_(rhs, lhs, fun_add, fun_sub);
    }

    // sub if 2nd operand is negative
    if (lhs.get_sign() != rhs.get_sign())
    {
        auto swap_sign = rhs;
        swap_sign.set_sign(~swap_sign.get_sign());
        return sub_(lhs, swap_sign, fun_add, fun_sub);
    }

    const auto exponent_delta =
        sub(width_cast<E + 1>(lhs.get_exponent()), width_cast<E + 1>(rhs.get_exponent()));

    auto extra_shift = 0U;
    if (lhs.is_normalized() && !rhs.is_normalized())
    {
        extra_shift = 1;
    }

    const auto new_mantissa = rhs.get_full_mantissa() >> (exponent_delta.word(0) - extra_shift);
    const auto mantissa_sum = fun_add(lhs.get_full_mantissa(), new_mantissa);

    floating_point<E, mantissa_sum.width() - 1> sum(lhs.get_sign(), lhs.get_exponent(),
                                                    mantissa_sum);

    return normalize<E, mantissa_sum.width() - 1, M>(sum);
}

/**
 * @brief Generic subtraction of two `floating_point` values
 *
 * This method computes the difference of two floating-point values using the provided functions `fun_add`
 * and `fun_sub` to compute the new mantissa. This generic function allows to easily implement
 * own adders, e.g. to develop new hardware implementations.*
 *
 * @note As an end-user of aarith, you will, most likely, never need to call this function.
 *
 * @tparam E Exponent width
 * @tparam M Mantissa width
 * @tparam Function_add Function object type for  performing an addition
 * @tparam Function_sub Function object fype for performing a subtraction
 * @param lhs Left-hand side argument of the usm
 * @param rhs Right-hand side argument of the sum
 * @param fun_add Function performing the addition of the mantissae
 * @param fun_sub Function performing the subtraction of the mantissae
 * @return The sum of lhs + rhs using the provided functions
 */
template <size_t E, size_t M, class Function_add, class Function_sub>
[[nodiscard]] auto sub_(const floating_point<E, M> lhs, const floating_point<E, M> rhs,
                        Function_add fun_add, Function_sub fun_sub) -> floating_point<E, M>
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
    auto extra_shift = 0U;
    if (lhs.is_normalized() && !rhs.is_normalized())
    {
        extra_shift = 1;
    }

    const auto new_mantissa = rhs.get_full_mantissa() >> (exponent_delta.word(0) - extra_shift);
    const auto mantissa_sum = fun_sub(lhs.get_full_mantissa(), new_mantissa);

    floating_point<E, mantissa_sum.width() - 1> sum(lhs.get_sign(), lhs.get_exponent(),
                                                    mantissa_sum);

    return normalize<E, mantissa_sum.width() - 1, M>(sum);
}

/**
 * @brief Adds two `floating_point` values
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
[[nodiscard]] auto add(const floating_point<E, M> lhs, const floating_point<E, M> rhs)
    -> floating_point<E, M>
{
    if (lhs.is_nan())
    {
        return lhs.make_quiet_nan();
    }

    if (rhs.is_nan())
    {
        return rhs.make_quiet_nan();
    }

    if ((lhs.is_nan() || rhs.is_nan()) || (lhs.is_neg_inf() && rhs.is_pos_inf()) ||
        (lhs.is_pos_inf() && rhs.is_neg_inf()))
    {
        return floating_point<E, M>::NaN();
    }

    if (lhs.is_inf())
    {
        return lhs;
    }
    if (rhs.is_inf())
    {
        return rhs;
    }


    return add_<
        E, M,
        class aarith::uinteger<M + 2, uint64_t> (*)(const aarith::uinteger<M + 1, uint64_t>&,
                                                    const aarith::uinteger<M + 1, uint64_t>&),
        class aarith::uinteger<M + 1, uint64_t> (*)(const aarith::uinteger<M + 1, uint64_t>&,
                                                    const aarith::uinteger<M + 1, uint64_t>&)>(
        lhs, rhs, expanding_add<uinteger<M + 1>, uinteger<M + 1>>,
        expanding_sub<uinteger<M + 1>, uinteger<M + 1>>);
}

/**
 * @brief Subtract two `floating_point` values
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
[[nodiscard]] auto sub(const floating_point<E, M> lhs, const floating_point<E, M> rhs)
    -> floating_point<E, M>
{
    if (lhs.is_nan())
    {
        return lhs.make_quiet_nan();
    }

    if (rhs.is_nan())
    {
        return rhs.make_quiet_nan();
    }

    if ((lhs.is_nan() || rhs.is_nan()) || (lhs.is_pos_inf() && rhs.is_pos_inf()) ||
        (lhs.is_neg_inf() && rhs.is_neg_inf()))
    {
        return floating_point<E, M>::NaN();
    }

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
 * @brief Multiplies two `floating_point` numbers
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
[[nodiscard]] auto mul(const floating_point<E, M, WordType> lhs,
                       const floating_point<E, M, WordType> rhs) -> floating_point<E, M, WordType>
{
    if (lhs.is_nan())
    {
        return lhs.make_quiet_nan();
    }

    if (rhs.is_nan())
    {
        return rhs.make_quiet_nan();
    }

    if ((lhs.is_nan() || rhs.is_nan()) || (lhs.is_zero() && rhs.is_inf()) ||
        (lhs.is_inf() && rhs.is_zero()))
    {
        return floating_point<E, M>::NaN();
    }

    // compute sign
    auto sign = lhs.get_sign() ^ rhs.get_sign();

    if (lhs.is_inf() || rhs.is_inf())
    {
        return sign ? floating_point<E, M>::neg_infinity() : floating_point<E, M>::pos_infinity();
    }

    // compute exponent
    auto ext_esum = expanding_add(lhs.get_exponent(), rhs.get_exponent());
    bool overflow = ext_esum.bit(E) == 1;
    ext_esum = sub(ext_esum, width_cast<E + 1>(lhs.bias));
    bool underflow = (not overflow) and ext_esum.bit(E) == 1;
    overflow = overflow and ext_esum.bit(E) == 1;

    // compute mantissa
    auto mproduct = schoolbook_expanding_mul(lhs.get_full_mantissa(), rhs.get_full_mantissa());
    mproduct = mproduct >> M;

    // check for over or underflow and break
    if (underflow || overflow)
    {
        floating_point<E, M> product;
        product.set_sign(sign);
        if (overflow)
        {
            product.set_exponent(uinteger<E>::all_ones());
        }
        else
        {
            ext_esum = ~ext_esum;
            ext_esum = add(ext_esum, uinteger<ext_esum.width()>(2));
            if (ext_esum < uinteger<64>(M + 1)) // TODO remove this number? it should bit bit-width of size_t?
            {
                mproduct = mproduct >> ext_esum.word(0);
                product.set_full_mantissa(width_cast<M + 1>(mproduct));
            }
        }
        // return normalize<E, M, M>(product);
        return product;
    }

    auto esum = width_cast<E>(ext_esum);

    floating_point<E, mproduct.width() - 1> product(sign, esum, mproduct);

    return normalize<E, mproduct.width() - 1, M>(product);
}

/**
 * @brief Division with floating_points: lhs/rhs.
 *
 * @param lhs The dividend
 * @param rhs The divisor
 * @tparam E Width of exponent
 * @tparam M Width of mantissa including the leading 1
 * @tparam WordType The word type used to internally store the data
 * @return The quotient lhs/rhs
 *
 */
template <size_t E, size_t M, typename WordType>
[[nodiscard]] auto div(const floating_point<E, M, WordType> lhs,
                       const floating_point<E, M, WordType> rhs) -> floating_point<E, M, WordType>
{

    /*=================================
     * 7.2. Invalid Operation
     */
    // TODO (keszocze) make this calls use the corresponding 754 std function once the branch is
    // merged
    if (lhs.is_nan())
    {
        return lhs.make_quiet_nan();
    }

    if (rhs.is_nan())
    {
        return rhs.make_quiet_nan();
    }

    if ((lhs.is_zero() && rhs.is_zero()) || (lhs.is_inf() && rhs.is_inf()))
    {
        return floating_point<E, M>::NaN();
    }
    //==========================================

    const auto result_is_negative = lhs.get_sign() ^ rhs.get_sign();

    if (rhs.is_zero())
    {
        return result_is_negative ? floating_point<E, M>::neg_infinity()
                                  : floating_point<E, M>::pos_infinity();
    }

    if (lhs.is_inf())
    {
        // due to the checks above, we already know that rhs is finite
        return result_is_negative ? floating_point<E, M>::neg_zero() : floating_point<E, M>::zero();
    }



    if (rhs.is_inf() || lhs.is_zero())
    {
        return result_is_negative ? floating_point<E, M>::neg_zero() : floating_point<E, M>::zero();
    }

    size_t denorm_exponent_lhs = 0;
    const bool lhs_is_denormal = !lhs.is_normalized();
    if (lhs_is_denormal)
    {
        // more precision of the computation with denormal numbers
        //        denorm_exponent_lhs = M - find_leading_one(lhs.get_full_mantissa());

        denorm_exponent_lhs = [&lhs]() {
            const std::optional<size_t> one_at = first_set_bit(lhs.get_full_mantissa());
            if (one_at)
            {
                return size_t(M - (*one_at));
            }
            else
            {
                return size_t(0);
            }
        }();
    }

    auto dividend = width_cast<2 * M + 1>(lhs.get_full_mantissa());
    auto divisor = width_cast<2 * M + 1>(rhs.get_full_mantissa());
    // shift to use integer division for producing 1,M
    dividend = dividend << M + denorm_exponent_lhs;
    auto mquotient = div(dividend, divisor);

    auto exponent_tmp = expanding_add(lhs.get_exponent(), lhs.bias);
    if (lhs_is_denormal)
    {
        denorm_exponent_lhs -= 1U;
        exponent_tmp = sub(exponent_tmp, uinteger<E + 1>(denorm_exponent_lhs));
    }

    bool overflow = exponent_tmp.bit(E) == 1;

    exponent_tmp = sub(exponent_tmp, width_cast<E + 1>(rhs.get_exponent()));
    uinteger<E + 1> denorm_exponent_correction{1U};
    if (!rhs.is_normalized())
    {
        exponent_tmp = sub(exponent_tmp, denorm_exponent_correction);
    }

    auto esum = width_cast<E>(exponent_tmp);
    overflow &= exponent_tmp.bit(E) == 1;
    const bool underflow = !overflow && exponent_tmp.bit(E) == 1;

    // check for over or underflow and break
    if (underflow || overflow)
    {
        floating_point<E, M> quotient;
        quotient.set_sign(result_is_negative);
        // apparently float div does not produce -0
        if (overflow)
        {
            quotient.set_exponent(uinteger<E>::all_ones());
        }
        else
        {
            // shift mantissa of subnormal number
            // in case some part of the mantissa can be expressed as subnormal number
            exponent_tmp = ~exponent_tmp;
            exponent_tmp = add(exponent_tmp, uinteger<exponent_tmp.width()>(2));
            if (exponent_tmp < uinteger<sizeof(M) * 8>(M + 1)) // TODO where does the 8 come from?
            {
                mquotient = rshift_and_round(mquotient, exponent_tmp.word(0));
                quotient.set_full_mantissa(width_cast<M + 1>(mquotient));
            }
        }
        return quotient;
    }
    else if (esum == uinteger<esum.width()>::zero())
    {
        floating_point<E, M> quotient{result_is_negative, esum,
                                      width_cast<M + 1>(rshift_and_round(mquotient, 1))};
        return quotient;
    }

    floating_point<E, mquotient.width() - 1> quotient(result_is_negative, esum, mquotient);

    return normalize<E, mquotient.width() - 1, M>(quotient);
}

/**
 * @brief Computes the negative value of the floating-point number
 *
 * Quoting the standard: copies a floating-point operand x to a destination in the same format,
 * reversing the sign bit. negate(x) is not the same as subtraction(0, x)
 *
 * @note This method ignores NaN values in the sense that they are also copied and the sign bit
 * flipped.
 *
 * @tparam E Width of exponent
 * @tparam M Width of mantissa
 * @tparam WordType The word type used to internally store the data
 * @return The negated value of the provided number
 */
template <size_t E, size_t M, typename WordType = uint64_t>
[[nodiscard]] constexpr floating_point<E, M, WordType>
negate(const floating_point<E, M, WordType>& x)
{
    floating_point<E, M, WordType> negated{x};

    negated.set_sign(!x.get_sign());

    return negated;
}

/**
 * @brief Copies the floating-point number
 *
 * Quoting the standard: copies a floating-point operand x to a destination in the same format,
 * with no change to the sign bit.
 *
 * @note This method ignores NaN values in the sense that they are also copied not signalling any
 * error.
 *
 * @note This is a rather useless method that only exists to be more compliant with the IEEE 754
 * (2019) standard.
 *
 * @tparam E Width of exponent
 * @tparam M Width of mantissa
 * @tparam WordType The word type used to internally store the data
 * @return The copied value
 */
template <size_t E, size_t M, typename WordType = uint64_t>
[[nodiscard]] constexpr floating_point<E, M, WordType> copy(const floating_point<E, M, WordType>& x)
{
    floating_point<E, M, WordType> copied{x};

    return copied;
}

/**
 * @brief Copies a floating-point number using the sign of another number
 *
 * Quoting the standard: copies a floating-point operand x to a destination in the same format as
 * x, but with the sign bit of y.
 *
 * @note This method ignores NaN values in the sense that they are also copied not signalling any
 * error.
 *
 * @tparam E Width of exponent
 * @tparam M Width of mantissa
 * @tparam WordType The word type used to internally store the data
 * @return The copied value
 */
template <size_t E, size_t M, typename WordType = uint64_t>
[[nodiscard]] constexpr floating_point<E, M, WordType>
copySign(const floating_point<E, M, WordType>& x, const floating_point<E, M, WordType>& y)
{
    floating_point<E, M, WordType> copied{x};

    copied.set_sign(y.get_sign());

    return copied;
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
bit_range(const floating_point<E, M, WordType>& f)
{
    return bit_range<Start, End>(f.as_word_array());
}


/**
 * This additional nesting of a namespace allows to include aarith without having the usual
 * operator names imported as well.
 *
 * The use case for this is to allow explicitly replace the conventional arithmetic operations
 * with sepcialized ones. This can, e.g., be used when evaluating approximate operations in the
 * context of neural networks. The name lookup of C++ makes it necessary not to see the operators
 * earlier.
 */
namespace float_operators {

template <size_t E, size_t M, typename WordType>
auto operator+(const floating_point<E, M, WordType>& lhs, const floating_point<E, M, WordType>& rhs)
    -> floating_point<E, M, WordType>
{
    return add(lhs, rhs);
}

template <size_t E, size_t M, typename WordType>
auto operator-(const floating_point<E, M, WordType>& lhs, const floating_point<E, M, WordType>& rhs)
    -> floating_point<E, M, WordType>
{
    return sub(lhs, rhs);
}

template <size_t E, size_t M, typename WordType>
auto operator*(const floating_point<E, M, WordType>& lhs, const floating_point<E, M, WordType>& rhs)
    -> floating_point<E, M, WordType>
{
    return mul(lhs, rhs);
}

template <size_t E, size_t M, typename WordType>
auto operator/(const floating_point<E, M, WordType>& lhs, const floating_point<E, M, WordType>& rhs)
    -> floating_point<E, M, WordType>
{
    return div(lhs, rhs);
}

template <size_t E, size_t M, typename WordType>
auto operator-(const floating_point<E, M, WordType>& x) -> floating_point<E, M, WordType>
{
    return negate(x);
}

} // namespace float_operators
} // namespace aarith
