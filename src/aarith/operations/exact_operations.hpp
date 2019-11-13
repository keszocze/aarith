#pragma once

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
template <class UInteger> auto exact_uint_add(const UInteger& a, const UInteger& b) -> UInteger
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
template <class UInteger> auto exact_uint_sub(const UInteger& a, const UInteger& b) -> UInteger
{
    static_assert(is_integral<UInteger>::value);
    static_assert(is_unsigned<UInteger>::value);

    UInteger sum;
    typename UInteger::word_type borrow{0};
    for (auto i = 0U; i < a.word_count(); ++i)
    {
        auto const partial_diff = a.word(i) - (b.word(i) + borrow);
        borrow = (partial_diff > a.word(i) || partial_diff > b.word(i)) ? 1 : 0;
        sum.set_word(i, partial_diff);
    }
    return sum;
}

/**
 * @brief Multiplies two unsigned integers.
 *
 * @note No Type conversion is performed. If the bit widths do not match, the code will not compile!
 *
 * @tparam UInteger The unsigned integer instance used for the operation
 * @param a First multiplicant
 * @param b Second multiplicant
 * @return Product of a and b
 */
template <class UInteger> auto exact_uint_mul(const UInteger& a, const UInteger& b) -> UInteger
{

    static_assert(is_integral<UInteger>::value);
    static_assert(is_unsigned<UInteger>::value);

    UInteger product;
    typename UInteger::word_type carry{0};

    for (auto j = 0U; j < b.word_count(); ++j)
    {

        const auto b_word = b.word(j);

        UInteger partial_product;
        for (auto i = 0U; (i + j) < a.word_count(); ++i)
        {

            const auto [a_upper, a_lower] = split(a.word(i));
            const auto [b_upper, b_lower] = split(b_word);

            const typename UInteger::word_type pp1 = a_lower * b_lower;
            const typename UInteger::word_type pp2 = a_lower * b_upper;
            const typename UInteger::word_type pp3 = a_upper * b_lower;
            const typename UInteger::word_type pp4 = a_upper * b_upper;

            const auto column0 = pp1 + carry;
            const uint32_t column0_carry = column0 < pp1 || column0 < carry;

            const auto [column0_upper, column0_lower] = split(pp1);
            carry = unsplit(column0_carry, column0_upper);

            const auto column1_sum = carry + pp2 + pp3;
            const auto [column1_upper, column1_lower] = split(column1_sum);

            const uint32_t column1_carry =
                column1_sum < carry || column1_sum < pp2 || column1_sum < pp3;
            carry = unsplit(column1_carry, column1_upper);

            carry = pp4 + carry;

            partial_product.set_word(i + j, unsplit(column1_lower, column0_lower));
        }

        product = exact_uint_add(product, partial_product);
    }
    return product;
}
/*
template <class UInteger> class exact_integer_operations
{
public:
    static_assert(is_integral<UInteger>::value);

    void add(const UInteger& a, const UInteger& b) {}
};
*/

} // namespace aarith
