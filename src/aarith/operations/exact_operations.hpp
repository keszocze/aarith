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
[[nodiscard]] auto exact_uint_add(const UInteger& a, const UInteger& b) -> UInteger
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
[[nodiscard]] auto exact_uint_sub(const UInteger& a, const UInteger& b) -> UInteger
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
 * This implements the most simply multiplication by adding up the number the correct number of
 * times. This is, of course, rather slow.
 *
 * @todo Actually complete the implementation
 *
 * @tparam UInteger The unsigned integer instance used for the operation
 * @param a First multiplicant
 * @param b Second multiplicant
 * @return Product of a and b
 */
template <class UInteger>[[nodiscard]] UInteger exact_uint_mul(const UInteger& a, const UInteger& b)
{
    static_assert(is_integral<UInteger>::value);

    // TODO does it make sense to count the ones first?

    UInteger shift_creation_mask = b;
    UInteger summand = a;
    UInteger result{0U};

    auto last_bit_set = [](const UInteger n) {
        typename UInteger::word_type one{1U};
        return (n.word(0) & one);
    };

    size_t to_shift = 0;

    while (!shift_creation_mask.is_zero())
    {
        if (last_bit_set(shift_creation_mask))
        {
            summand = (summand << to_shift);
            result = exact_uint_add(result, summand);
            to_shift = 1;
        }
        else
        {
            ++to_shift;
        }
        shift_creation_mask = (shift_creation_mask >> 1);
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
    if (UInteger::width() <= 32)
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
    auto res_div(const uinteger<W>& numerator, const uinteger<W>& denominator) -> std::pair<uinteger<W>,uinteger<W>>
    {

        if (denominator.is_zero()) {
            throw std::runtime_error("Attempted division by zero");
        }

        using UInteger = uinteger<W>;
        using LargeUInteger = uinteger<2 * W>;

        /*
         * According to Wikipedia https://en.wikipedia.org/wiki/Division_algorithm#Restoring_division the
         * denominator is expected to be smaller than the numerator. The following two special cases are
         * here to ensure this
         */

        if (numerator == denominator)
        {
            return std::make_pair(UInteger{1U},UInteger{0U});
        }

        if (numerator < denominator) {
            return std::make_pair(UInteger{0U},numerator);
        }




        const size_t n = numerator.width();

        LargeUInteger R = width_cast<2 * W>(numerator);
        LargeUInteger D = (width_cast<2*W>(denominator) << n);

        UInteger Q{0U};

        for (size_t i = 0; i<n; ++i)
        {
            size_t j = (n-1)-i;
            LargeUInteger TwoR = (R << 1);
            if (TwoR >= D)
            {
                R = exact_uint_sub(TwoR, D);
                Q.set_bit(j,true);
            }
            else
            {
                R = TwoR;
                Q.set_bit(j,false);
            }
        }

        uinteger<W> remainder = width_cast<W>(Q);
        return std::make_pair(Q, remainder);
    }

template <std::size_t W>
auto restoring_division(const uinteger<W>& numerator, const uinteger<W>& denominator) -> uinteger<W>
{
    auto [quotient, remainder] = res_div(numerator,denominator);
    return quotient;
}

} // namespace aarith
