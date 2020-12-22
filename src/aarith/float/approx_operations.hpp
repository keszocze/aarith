#pragma once

#include <aarith/float.hpp>
#include <aarith/integer/approx_operations.hpp>

namespace aarith {

/**
 * @brief Addition of two normfloats using anytime addition: lhs+rhs
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
template <size_t E, size_t M>
[[nodiscard]] auto anytime_add(const floating_point<E, M> lhs, const floating_point<E, M> rhs,
                               const unsigned int bits = M + 1) -> floating_point<E, M>
{
    if (abs(lhs) < abs(rhs))
    {
        return anytime_add(rhs, lhs, bits);
    }

    if (lhs.get_sign() != rhs.get_sign())
    {
        auto swap_sign = rhs;
        swap_sign.set_sign(~swap_sign.get_sign());
        return anytime_sub(lhs, swap_sign, bits);
    }

    const auto exponent_delta =
        sub(width_cast<E + 1>(lhs.get_exponent()), width_cast<E + 1>(rhs.get_exponent()));

    const auto new_mantissa = rhs.get_full_mantissa() >> exponent_delta.word(0);
    const auto mantissa_sum =
        approx_expanding_add_post_masking(lhs.get_full_mantissa(), new_mantissa, bits);

    floating_point<E, mantissa_sum.width() - 1> sum(lhs.get_sign(), lhs.get_exponent(),
                                                      mantissa_sum);

    return normalize<E, mantissa_sum.width() - 1, M>(sum);
}

/**
 * @brief Subtraction with floating_points: lhs-rhs.
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
template <size_t E, size_t M>
[[nodiscard]] auto anytime_sub(const floating_point<E, M> lhs, const floating_point<E, M> rhs,
                               const unsigned int bits = M + 1) -> floating_point<E, M>
{
    if (abs(lhs) < abs(rhs))
    {
        auto swap = rhs;
        swap.set_sign(~swap.get_sign());
        return anytime_add(swap, lhs, bits);
    }

    if (lhs.get_sign() != rhs.get_sign())
    {
        auto swap_sign = rhs;
        swap_sign.set_sign(~swap_sign.get_sign());
        return anytime_add(lhs, swap_sign, bits);
    }

    const auto exponent_delta = sub(lhs.get_exponent(), rhs.get_exponent());
    const auto new_mantissa = rhs.get_full_mantissa() >> exponent_delta.word(0);
    const auto mantissa_sum =
        approx_expanding_sub_post_masking(lhs.get_full_mantissa(), new_mantissa, bits + 1);

    floating_point<E, mantissa_sum.width() - 1> sum(lhs.get_sign(), lhs.get_exponent(),
                                                      mantissa_sum);

    return normalize<E, mantissa_sum.width() - 1, M>(sum);
}

/**
 * @brief Multiplication with floating_points: lhs*rhs.
 *
 * @param lhs The multiplicand
 * @param rhs The multiplicator
 * @param bits The number of most-significant bits that are calculated of the mantissa
 * multiplication
 * @tparam E Width of exponent
 * @tparam M Width of mantissa including the leading 1
 *
 * @return The product lhs*rhs
 *
 */
template <size_t E, size_t M>
[[nodiscard]] auto anytime_mul(const floating_point<E, M> lhs, const floating_point<E, M> rhs,
                               const unsigned int bits = 2 * M) -> floating_point<E, M>
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
        return sign ? floating_point<E, M>::neg_infinity()
                    : floating_point<E, M>::pos_infinity();
    }

    // compute exponent
    auto ext_esum = expanding_add(lhs.get_exponent(), rhs.get_exponent());
    bool overflow = ext_esum.bit(E) == 1;
    ext_esum = sub(ext_esum, width_cast<E + 1>(lhs.bias));
    bool underflow = (not overflow) and ext_esum.bit(E) == 1;
    overflow = overflow and ext_esum.bit(E) == 1;

    auto mproduct = approx_expanding_mul_post_masking(lhs.get_full_mantissa(),
                                                      rhs.get_full_mantissa(), bits + 1);
    mproduct = mproduct >> (M);

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
            if (ext_esum < uinteger<64>(M + 1))
            {
                mproduct = mproduct >> ext_esum.word(0);
                product.set_full_mantissa(width_cast<M + 1>(mproduct));
            }
        }
        return product;
    }

    auto esum = width_cast<E>(ext_esum);

    floating_point<E, mproduct.width() - 1> product(sign, esum, mproduct);

    return normalize<E, mproduct.width() - 1, M>(product);
}

/**
 * @brief Anytime division with floating_points: lhs/rhs.
 *
 * @param lhs The dividend
 * @param rhs The divisor
 * @param bits The number of most-significant bits that are calculated of the mantissa division
 * @tparam E Width of exponent
 * @tparam M Width of mantissa including the leading 1
 *
 * @return The quotient lhs/rhs
 *
 */
template <size_t E, size_t M>
[[nodiscard]] auto anytime_div(const floating_point<E, M> lhs, const floating_point<E, M> rhs,
                               const unsigned int bits = M + 1) -> floating_point<E, M>
{
    auto dividend = width_cast<2 * (M + 1) + 3>(lhs.get_full_mantissa());
    auto divisor = width_cast<2 * (M + 1) + 3>(rhs.get_full_mantissa());
    dividend = dividend << (M + 1) + 3;
    auto mquotient = approx_div_post_masking(dividend, divisor, bits + (M + 1) + 3);

    auto rdmquotient = rshift_and_round(mquotient, 4);

    auto esum = width_cast<E>(
        expanding_sub(expanding_add(lhs.get_exponent(), lhs.bias), rhs.get_exponent()));
    auto sign = lhs.get_sign() ^ rhs.get_sign();

    floating_point<E, rdmquotient.width() - 1> quotient;
    quotient.set_mantissa(rdmquotient);
    quotient.set_exponent(esum);
    quotient.set_sign(sign);

    return normalize<E, rdmquotient.width() - 1, M>(quotient);
}

/**
 * @brief Addition of two floating_points using the FAU adder: lhs+rhs
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
template <size_t E, size_t M, size_t LSP, size_t SHARED>
[[nodiscard]] auto FAU_add(const floating_point<E, M> lhs, const floating_point<E, M> rhs)
    -> floating_point<E, M>
{
    return add_<E, M>(lhs, rhs, FAUadder<M + 1, LSP, SHARED>, FAU_sub<M + 1, LSP, SHARED>);
}

/**
 * @brief Subtraction with floating_points using the FAU adder: lhs-rhs.
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
template <size_t E, size_t M, size_t LSP, size_t SHARED>
[[nodiscard]] auto FAU_sub(const floating_point<E, M> lhs, const floating_point<E, M> rhs)
    -> floating_point<E, M>
{
    return sub_<E, M>(lhs, rhs, FAUadder<M + 1, LSP, SHARED>, FAU_sub<M + 1, LSP, SHARED>);
}

} // namespace aarith
