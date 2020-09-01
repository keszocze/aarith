#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/float/normalized_float.hpp>
#include <aarith/integer.hpp>

namespace aarith {

/**
 * @brief Adds to normfloats.
 *
 * @param lhs The first number that is to be summed up
 * @param rhs The second number that is to be summed up
 * @tparam E Width of exponent
 * @tparam M Width of mantissa including the leading 1
 *
 * @return The sum
 *
 */
template <size_t E, size_t M, typename WordType>
[[nodiscard]] auto add(const normalized_float<E, M, WordType>& lhs,
                       const normalized_float<E, M, WordType>& rhs)
    -> normalized_float<E, M, WordType>
{
    if (abs(lhs) < abs(rhs))
    {
        return add(rhs, lhs);
    }

    if (lhs.get_sign() != rhs.get_sign())
    {
        auto swap_sign = rhs;
        swap_sign.set_sign(~swap_sign.get_sign());
        return sub(lhs, swap_sign);
    }

    const auto exponent_delta = sub(lhs.get_exponent(), rhs.get_exponent());
    const auto new_mantissa = rhs.get_full_mantissa() >> exponent_delta.word(0);
    const auto mantissa_sum = expanding_add(lhs.get_full_mantissa(), new_mantissa);

    normalized_float<E, mantissa_sum.width()> sum;
    sum.set_sign(lhs.get_sign());
    sum.set_exponent(lhs.get_exponent());
    sum.set_mantissa(mantissa_sum);

    return normalize<E, mantissa_sum.width(), M>(sum);
}

/**
 * @brief Subtraction with normfloats: lhs-rhs.
 *
 * @param lhs The minuend
 * @param rhs The subtrahend
 * @tparam E Width of exponent
 * @tparam M Width of mantissa including the leading 1
 *
 * @return The difference lhs-rhs
 *
 */
template <size_t E, size_t M, typename WordType>
[[nodiscard]] auto sub(const normalized_float<E, M, WordType> lhs,
                       const normalized_float<E, M, WordType> rhs)
    -> normalized_float<E, M, WordType>
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
        return add(lhs, swap_sign);
    }

    const auto exponent_delta = sub(lhs.get_exponent(), rhs.get_exponent());
    const auto new_mantissa = rhs.get_full_mantissa() >> exponent_delta.word(0);
    const auto mantissa_sum = sub(lhs.get_full_mantissa(), new_mantissa);

    normalized_float<E, mantissa_sum.width()> sum;
    sum.set_sign(lhs.get_sign());
    sum.set_exponent(lhs.get_exponent());
    sum.set_mantissa(mantissa_sum);

    return normalize<E, mantissa_sum.width(), M>(sum);
}

/**
 * @brief Multiplication with normfloats: lhs*rhs.
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
    auto mproduct = expanding_mul(lhs.get_full_mantissa(), rhs.get_full_mantissa());
    mproduct = mproduct >> M;
    auto esum = width_cast<E>(sub(expanding_add(lhs.get_exponent(), rhs.get_exponent()),
                                  width_cast<E + 1>(lhs.get_bias())));
    auto sign = lhs.get_sign() ^ rhs.get_sign();

    normalized_float<E, mproduct.width()> product;
    product.set_mantissa(mproduct);
    product.set_exponent(esum);
    product.set_sign(sign);

    return normalize<E, mproduct.width(), M>(product);
}

/**
 * @brief Division with normfloats: lhs/rhs.
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
    auto dividend = width_cast<2 * (M + 1) + 3>(lhs.get_full_mantissa());
    auto divisor = width_cast<2 * (M + 1) + 3>(rhs.get_full_mantissa());
    dividend = (dividend << M + 4);
    auto mquotient = div(dividend, divisor);
    // mquotient >>= 1;
    auto rdmquotient = rshift_and_round(mquotient, 4);

    auto esum = width_cast<E>(sub(expanding_add(lhs.get_exponent(), lhs.get_bias()),
                                  width_cast<E + 1>(rhs.get_exponent())));
    auto sign = lhs.get_sign() ^ rhs.get_sign();

    normalized_float<E, rdmquotient.width()> quotient;
    quotient.set_mantissa(rdmquotient);
    quotient.set_exponent(esum);
    quotient.set_sign(sign);

    return normalize<E, rdmquotient.width(), M>(quotient);
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

} // namespace aarith

namespace aarith::arithmetic_operators {

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

} // namespace aarith::arithmetic_operators
