#pragma once

#include <aarith/integer/uinteger.hpp>
#include <aarith/float/normfloat.hpp>
#include <aarith/integer/uinteger_comparisons.hpp>
#include <aarith/integer/uinteger_operations.hpp>
#include <aarith/core/traits.hpp>

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
template<size_t E, size_t M>
[[nodiscard]] auto add(const normfloat<E, M>& lhs, const normfloat<E, M>& rhs)
-> normfloat<E, M>
{
    if(abs(lhs) < abs(rhs))
    {
        return add(rhs, lhs);
    }

    if(lhs.get_sign() != rhs.get_sign())
    {
        auto swap_sign = rhs;
        swap_sign.set_sign(~swap_sign.get_sign());
        return sub(lhs, swap_sign);
    }

    const auto exponent_delta = sub(lhs.get_exponent(), rhs.get_exponent());
    const auto new_mantissa = rhs.get_mantissa() >> exponent_delta.word(0);
    const auto mantissa_sum = expanding_add(lhs.get_mantissa(),  new_mantissa);

    normfloat<E, mantissa_sum.width()> sum;
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
template<size_t E, size_t M>
[[nodiscard]] auto sub(const normfloat<E, M> lhs, const normfloat<E, M> rhs)
-> normfloat<E, M>
{
    if(abs(lhs) < abs(rhs))
    {
        auto swap = rhs;
        swap.set_sign(~swap.get_sign());
        return add(swap, lhs);
    }

    if(lhs.get_sign() != rhs.get_sign())
    {
        auto swap_sign = rhs;
        swap_sign.set_sign(~swap_sign.get_sign());
        return add(lhs, swap_sign);
    }

    const auto exponent_delta = sub(lhs.get_exponent(), rhs.get_exponent());
    const auto new_mantissa = rhs.get_mantissa() >> exponent_delta.word(0);
    const auto mantissa_sum = sub(lhs.get_mantissa(), new_mantissa);

    normfloat<E, mantissa_sum.width()> sum;
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
template<size_t E, size_t M>
[[nodiscard]] auto mul(const normfloat<E, M> lhs, const normfloat<E, M> rhs)
-> normfloat<E, M>
{
    auto mproduct = expanding_mul(lhs.get_mantissa(), rhs.get_mantissa());
    mproduct = mproduct >> (M-1);
    auto esum = width_cast<E>(sub(expanding_add(lhs.get_exponent(), rhs.get_exponent()), width_cast<E+1>(lhs.get_bias())));
    auto sign = lhs.get_sign() ^ rhs.get_sign();

    normfloat<E, mproduct.width()> product;
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
template<size_t E, size_t M>
[[nodiscard]] auto div(const normfloat<E, M> lhs, const normfloat<E, M> rhs)
-> normfloat<E, M>
{
    auto dividend = width_cast<2*M+3>(lhs.get_mantissa());
    auto divisor = width_cast<2*M+3>(rhs.get_mantissa());
    dividend <<= M+3;
    auto mquotient = div(dividend, divisor);
    //mquotient >>= 1;
    auto rdmquotient = rshift_and_round(mquotient, 4);

    auto esum = width_cast<E>(sub(expanding_add(lhs.get_exponent(), lhs.get_bias()), width_cast<E+1>(rhs.get_exponent())));
    auto sign = lhs.get_sign() ^ rhs.get_sign();

    normfloat<E, rdmquotient.width()> quotient;
    quotient.set_mantissa(rdmquotient);
    quotient.set_exponent(esum);
    quotient.set_sign(sign);

    return normalize<E, rdmquotient.width(), M>(quotient);
}

} // namespace aarith

#include "aarith/integer/uinteger.hpp"

namespace aarith::exact_operators {

template<size_t E, size_t M>
auto operator+(const normfloat<E, M>& lhs, const normfloat<E, M>& rhs) -> normfloat<E, M>
{
    return add(lhs, rhs);
}

template<size_t E, size_t M>
auto operator-(const normfloat<E, M>& lhs, const normfloat<E, M>& rhs) -> normfloat<E, M>
{
    return sub(lhs, rhs);
}

template<size_t E, size_t M>
auto operator*(const normfloat<E, M>& lhs, const normfloat<E, M>& rhs) -> normfloat<E, M>
{
    return mul(lhs, rhs);
}

template<size_t E, size_t M>
auto operator/(const normfloat<E, M>& lhs, const normfloat<E, M>& rhs) -> normfloat<E, M>
{
    return div(lhs, rhs);
}

template<size_t E, size_t M>
auto operator%(const normfloat<E, M>& lhs, const normfloat<E, M>& rhs) -> normfloat<E, M>
{
    return remainder(lhs, rhs);
}

} // namespace aarith::exact_operators
