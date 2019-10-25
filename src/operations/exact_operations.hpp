#pragma once

namespace aarith {

template <class UInteger> auto exact_uint_add(const UInteger& a, const UInteger& b) -> UInteger
{
    static_assert(is_integral<UInteger>::value);
    //static_assert(is_unsigned<UInteger>::value);

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

/*
template <class UInteger> class exact_integer_operations
{
public:
    static_assert(is_integral<UInteger>::value);

    void add(const UInteger& a, const UInteger& b) {}
};
*/

} // namespace aarith