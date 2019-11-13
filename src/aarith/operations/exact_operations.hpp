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
template <class UInteger> [[nodiscard]] auto exact_uint_add(const UInteger& a, const UInteger& b) -> UInteger
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
template <class UInteger> [[nodiscard]] auto exact_uint_sub(const UInteger& a, const UInteger& b) -> UInteger
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
 * @tparam UInteger The unsigned integer instance used for the operation
 * @param a First multiplicant
 * @param b Second multiplicant
 * @return Product of a and b
 */

template <class UInteger> [[nodiscard]] auto exact_uint_mul(const UInteger& a, const UInteger& b) -> UInteger
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

            const auto foo = [](const uint64_t x, const uint64_t y, const uint64_t carry_in) {
                const auto sum = x + y + carry_in;
                const uint32_t sum_carry = sum < x || sum < y || sum < carry_in;

                const auto [upper, lower] = split(sum);
                const auto carry_out = unsplit(sum_carry,upper);

                return std::make_pair(carry_out,lower);
            };


            const auto [inner_carry_1,column0_lower] = foo(pp1, 0, carry);
            
            const auto [inner_carry2,column1_lower] = foo(pp2, pp3, inner_carry_1);

            carry = pp4 + inner_carry2;

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
