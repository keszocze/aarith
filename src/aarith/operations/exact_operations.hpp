#pragma once

#include "aarith/types/traits.hpp"

namespace aarith {

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


    template <class UInteger> auto exact_uint_mul(const UInteger& a, const UInteger& b) -> UInteger
    {

        static_assert(is_integral<UInteger>::value);
        static_assert(is_unsigned<UInteger>::value);

        UInteger product;
        uint64_t carry{0};

        for (auto i = 0U; i < a.word_count(); ++i)
        {

            const auto [a_upper,a_lower] = split(a.word(i));
            const auto [b_upper,b_lower] = split(b.word(i));

            const uint64_t pp1 = a_lower * b_lower;
            const uint64_t pp2 = a_lower * b_upper;
            const uint64_t pp3 = a_upper * b_lower;
            const uint64_t pp4 = a_upper * b_upper;

            

            auto const partial_sum = a.word(i) + b.word(i) + carry;
            carry = (partial_sum < a.word(i) || partial_sum < b.word(i)) ? 1 : 0;
            product.set_word(i, partial_sum);
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
