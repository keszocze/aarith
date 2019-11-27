#pragma once

#include <aarith/types/integer.hpp>
#include <aarith/types/traits.hpp>
#include <aarith/utilities/bit_operations.hpp>

#include <iostream>

namespace aarith {

/**
 * @brief Adds two unsigned integers
 *
 * @note No Type conversion is performed. If the bit widths do not match, the code will not compile!
 *
 * @tparam UInteger The unsigned integer instance used for the operation
 * @param a First summand
 * @param b Second summand
 * @return Sum of a and b
 */
template <class UInteger>
[[nodiscard]] auto add(const UInteger& a, const UInteger& b) -> UInteger
{
    static_assert(is_integral<UInteger>::value);
    static_assert(is_unsigned<UInteger>::value);

    UInteger sum;
    typename UInteger::word_type carry{0};
    for (auto i = 0U; i < a.word_count(); ++i)
    {
        auto const partial_sum = a.word(i) + b.word(i) + carry;
        carry = (partial_sum < a.word(i) || partial_sum < b.word(i)) ? 1 : 0;
        sum.set_word(i, partial_sum);
    }
    return sum;
}

/**
 * @brief Computes the difference of two unsigned integers.
 *
 * @note No Type conversion is performed. If the bit widths do not match, the code will not compile!
 *
 * @tparam UInteger The unsigned integer instance used for the operation
 * @param a Minuend
 * @param b Subtrahend
 * @return Difference between a and b
 */
template <class UInteger>
[[nodiscard]] auto sub(const UInteger& a, const UInteger& b) -> UInteger
{
    static_assert(is_integral<UInteger>::value);
    static_assert(is_unsigned<UInteger>::value);

    UInteger sum;
    typename UInteger::word_type borrow{0};
    for (auto i = 0U; i < a.word_count(); ++i)
    {

        auto const a_word = a.word(i);
        auto const b_word = b.word(i);
        auto const subtrahend = b_word + borrow;
        auto const partial_diff = a_word - subtrahend;

        /*
         * The new borrow originates from either
         * a) the minuend being smaller than the subtrahend or
         * b) the subtrahend being smaller than the raw word of the uinteger b
         *
         * The case b) arises when the current word of b consists of ones only and there is
         * an "incoming" borrow.
         */

        borrow = ((a_word < subtrahend) || (subtrahend < b_word)) ? 1 : 0;
        sum.set_word(i, partial_diff);
    }
    return sum;
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
template <class UInteger>[[nodiscard]] UInteger mul(const UInteger& a, const UInteger& b)
{
    UInteger result{0U};
    if constexpr (UInteger::width() <= 32)
    {
        uint64_t result_uint64 = a.word(0) * b.word(0);
        result.set_word(0, result_uint64);
    }
    else
    {
        static_assert(is_integral<UInteger>::value);
        static_assert(is_unsigned<UInteger>::value);

        const auto leading_zeroes = count_leading_zeroes(b);
        auto bit_index = 0U;
        while (bit_index < leading_zeroes)
        {
            if (b.bit(bit_index))
            {
                result = add(result, a << bit_index);
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
 * The method implements Karatsuba's Algorithm https://en.wikipedia.org/wiki/Karatsuba_algorithm
 *
 * @todo Actually complete the implementation
 *
 * @tparam UInteger The unsigned integer instance used for the operation
 * @param a First multiplicant
 * @param b Second multiplicant
 * @return Product of a and b
 */
template <class UInteger>
[[nodiscard]] auto karatsuba(const UInteger& a, const UInteger& b) -> UInteger
{
    // base case, we can stop recursion now
    if constexpr (UInteger::width() <= 32)
    {
        uint64_t result_uint64 = a.word(0) * b.word(0);
        UInteger result;
        result.set_word(0, result_uint64);
        return result;
    }
    else
    {
        throw "currently unsupported";
    }
}

template <std::size_t W>
auto restoring_division(const uinteger<W>& numerator, const uinteger<W>& denominator)
    -> std::pair<uinteger<W>, uinteger<W>>
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
auto remainder(const UInteger& numerator, const UInteger& denominator) -> UInteger
{
    return restoring_division(numerator, denominator).second;
}

template <class UInteger>
auto div(const UInteger& numerator, const UInteger& denominator) -> UInteger
{
    return restoring_division(numerator, denominator).first;
}

} // namespace aarith

#include "aarith/types/integer.hpp"

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
