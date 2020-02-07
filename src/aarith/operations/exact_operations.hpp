#pragma once

#include <aarith/types/uinteger.hpp>
#include <aarith/types/normfloat.hpp>
#include <aarith/types/traits.hpp>
#include <aarith/utilities/bit_operations.hpp>
#include <aarith/operations/comparisons.hpp>

#include <iostream>

namespace aarith {

/**
 * @brief Adds two unsigned integers of, possibly, different bit widths.
 *
 * @tparam W Width of the first summand
 * @tparam V Width of the second summand
 * @param a First summand
 * @param b Second summand
 * @param initial_carry True if there is an initial carry coming in
 * @return Sum of correct maximal bit width
 */
template <size_t W, size_t V>
[[nodiscard]] uinteger<std::max(W, V) + 1> expanding_add(const uinteger<W>& a, const uinteger<V>& b,
                                                         const bool initial_carry = false)
{
    static_assert(is_integral<uinteger<W>>::value);
    static_assert(is_unsigned<uinteger<W>>::value);
    static_assert(is_integral<uinteger<V>>::value);
    static_assert(is_unsigned<uinteger<V>>::value);

    constexpr size_t res_width = std::max(W, V) + 1U;

    uinteger<res_width> sum;
    using word_type = typename uinteger<res_width>::word_type;
    word_type carry{0U};
    if (initial_carry)
    {
        carry = 1U;
    }

    /*
     * If the bit widths are not the same we actually have to check that we don't access values
     * outside the underlying word container.
     */
    if constexpr (uinteger<W>::word_count() != uinteger<V>::word_count())
    {
        for (auto i = 0U; i < sum.word_count(); ++i)
        {

            word_type a_{0U};
            word_type b_{0U};
            if (i < a.word_count())
            {
                a_ = a.word(i);
            }
            if (i < b.word_count())
            {
                b_ = b.word(i);
            }

            auto const partial_sum = a_ + b_ + carry;
            carry = (partial_sum < a_ || partial_sum < b_) ? 1U : 0U;
            sum.set_word(i, partial_sum);
        }
    }
    // Here we can simple iterate until we reached the end of either of the two uintegers
    else
    {
        for (auto i = 0U; i < a.word_count(); ++i)
        {
            auto const partial_sum = a.word(i) + b.word(i) + carry;
            carry = (partial_sum < a.word(i) || partial_sum < b.word(i)) ? 1U : 0U;
            sum.set_word(i, partial_sum);
        }

        // we check whether an the additional bit results in an additional word and only propagate
        // the carry if this word exists
        if constexpr (uinteger<W>::word_count() < uinteger<res_width>::word_count())
        {
            sum.set_word(sum.word_count() - 1, carry);
        }
    }

    return sum;
}

/**
 * @brief Adds two unsigned integers
 *
 * @tparam UInteger The unsigned integer instance used for the addition
 * @param a First summand
 * @param b Second summand
 * @return Sum of a and b
 */
template <size_t W>[[nodiscard]] uinteger<W> add(const uinteger<W>& a, const uinteger<W>& b)
{
    uinteger<W + 1> result = expanding_add<W, W>(a, b);
    return width_cast<W>(result);
}

/**
 * @brief Computes the difference of two unsigned integers.
 *
 * @tparam W the bit width of the operands
 * @param a Minuend
 * @param b Subtrahend
 * @return Difference between a and b
 */
template <size_t W>[[nodiscard]] auto sub(const uinteger<W>& a, const uinteger<W>& b) -> uinteger<W>
{
    static_assert(is_integral<uinteger<W>>::value);
    static_assert(is_unsigned<uinteger<W>>::value);

    uinteger<W> result;
    uinteger<W> minus_b = add(~b, uinteger<W>::one());
    result = add(a, minus_b);
    return result;
}

/**
 * @brief Multiplies two unsigned integers expanding the bit width so that the result fits.
 *
 *
 * @tparam UInteger The unsigned integer instance used for the operation
 * @param a First multiplicant
 * @param b Second multiplicant
 * @return Product of a and b
 */
template <std::size_t W, std::size_t V>
[[nodiscard]] uinteger<W + V> expanding_mul(const uinteger<W>& a, const uinteger<V>& b)
{
    constexpr std::size_t res_width = W + V;
    uinteger<res_width> result{0U};
    if constexpr (res_width <= 64)
    {
        uint64_t result_uint64 = a.word(0) * b.word(0);
        result.set_word(0, result_uint64);
    }
    else
    {
        static_assert(is_integral<uinteger<res_width>>::value);
        static_assert(is_unsigned<uinteger<res_width>>::value);

        const auto leading_zeroes = V - count_leading_zeroes(b);
        uinteger<res_width> a_ = width_cast<res_width>(a);

        auto bit_index = 0U;
        while (bit_index < leading_zeroes)
        {
            if (b.bit(bit_index))
            {
                result = add(result, a_ << bit_index);
            }
            ++bit_index;
        }
    }
    return result;
}

/**
 * @brief Multiplies two unsigned integers.
 *
 * @note No Type conversion is performed. If the bit widths do not match, the code will not compile!
 *
 * This implements the simplest multiplication algorithm (binary "long multiplication") that adds up
 * the partial products everywhere where the first multiplicand has a 1 bit. The simplicity, of
 * course, comes at the cost of performance.
 *
 * @tparam UInteger The unsigned integer instance used for the operation
 * @param a First multiplicant
 * @param b Second multiplicant
 * @return Product of a and b
 */
template <size_t W>[[nodiscard]] uinteger<W> mul(const uinteger<W>& a, const uinteger<W>& b)
{
    return width_cast<W>(expanding_mul(a, b));
}

/**
 * @brief Implements the restoring division algorithm.
 *
 * @see https://en.wikipedia.org/wiki/Division_algorithm#Restoring_division
 *
 * @param numerator The number that is to be divided
 * @param denominator The number that devides the other number
 * @tparam W Width of the numbers used in division.
 *
 * @return Pair of (quotient, remainder)
 *
 */
template <std::size_t W, std::size_t V>
[[nodiscard]] std::pair<uinteger<W>, uinteger<W>> restoring_division(const uinteger<W>& numerator,
                                                                     const uinteger<V>& denominator)

{
    using UInteger = uinteger<W>;
    using LargeUInteger = uinteger<2 * W>;

    if (denominator.is_zero())
    {
        throw std::runtime_error("Attempted division by zero");
    }

    // Cover some special cases in order to speed everything up
    if (numerator == denominator)
    {
        return std::make_pair(UInteger{1U}, UInteger{0U});
    }
    if (numerator < denominator)
    {
        return std::make_pair(UInteger{0U}, numerator);
    }
    if (denominator == UInteger{1U})
    {
        return std::make_pair(numerator, UInteger{0U});
    }

    // Perform restoring division in all other cases
    const auto n = numerator.width();
    const LargeUInteger D = (width_cast<2 * W>(denominator) << n);
    LargeUInteger R = width_cast<2 * W>(numerator);
    UInteger Q{0U};

    for (size_t i = 0; i < n; ++i)
    {
        const auto bit = (n - 1) - i;
        const LargeUInteger TwoR = (R << 1);
        if (TwoR >= D)
        {
            R = sub(TwoR, D);
            Q.set_bit(bit, true);
        }
        else
        {
            R = TwoR;
            Q.set_bit(bit, false);
        }
    }

    const uinteger<W> remainder = width_cast<W>(R >> n);

    return std::make_pair(Q, remainder);
}

template <class UInteger>
[[nodiscard]] auto remainder(const UInteger& numerator, const UInteger& denominator) -> UInteger
{
    return restoring_division(numerator, denominator).second;
}

template <class UInteger>
[[nodiscard]] auto div(const UInteger& numerator, const UInteger& denominator) -> UInteger
{
    return restoring_division(numerator, denominator).first;
}

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
[[nodiscard]] auto add(const normfloat<E, M> lhs, const normfloat<E, M> rhs)
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

    normfloat<E, mantissa_sum.width()> sum(0.f);
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

    normfloat<E, mantissa_sum.width()> sum(0.f);
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

#include "aarith/types/uinteger.hpp"

namespace aarith::exact_operators {

template <size_t Width>
auto operator+(const uinteger<Width>& lhs, const uinteger<Width>& rhs) -> uinteger<Width>
{
    return add(lhs, rhs);
}

template <size_t Width>
auto operator-(const uinteger<Width>& lhs, const uinteger<Width>& rhs) -> uinteger<Width>
{
    return sub(lhs, rhs);
}

template <size_t Width>
auto operator*(const uinteger<Width>& lhs, const uinteger<Width>& rhs) -> uinteger<Width>
{
    return mul(lhs, rhs);
}

template <size_t Width>
auto operator/(const uinteger<Width>& lhs, const uinteger<Width>& rhs) -> uinteger<Width>
{
    return div(lhs, rhs);
}

template <size_t Width>
auto operator%(const uinteger<Width>& lhs, const uinteger<Width>& rhs) -> uinteger<Width>
{
    return remainder(lhs, rhs);
}

} // namespace aarith::exact_operators
