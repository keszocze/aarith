#pragma once

#include <aarith/types/sinteger.hpp>
#include <aarith/types/traits.hpp>
#include <cstdint>



namespace aarith {

template <size_t W, size_t V> bool operator==(const sinteger<W>& a, const sinteger<V>& b)
{
    const auto min_count = std::min(a.word_count(), b.word_count());
    const auto max_count = std::max(a.word_count(), b.word_count());

    if constexpr (W > V)
    {
        for (size_t i = min_count; i < max_count; ++i)
        {
            if (a.word(i) != 0U)
            {
                return false;
            }
        }
    }
    else
    {
        for (size_t i = min_count; i < max_count; ++i)
        {
            if (b.word(i) != 0U)
            {
                return false;
            }
        }
    }    

    for (auto i = 0U; i < min_count; ++i)
    {
        if (a.word(i) != b.word(i))
        {
            return false;
        }
    }



    return true;
}



template <size_t W, size_t V> bool operator<(const sinteger<W>& a, const sinteger<V>& b)
{

    using word_type = typename sinteger<W>::word_type;
    const auto min_count = std::min(a.word_count(), b.word_count());
    const auto max_count = std::max(a.word_count(), b.word_count());

    if (a.is_negative() && !b.is_negative()) {
        return true;
    }
    if (!a.is_negative() && b.is_negative()) 
    {
        return false;
    }


    // from here on, the signs of the numbers are identical
    const bool both_positive = !a.is_negative();

    if constexpr (W > V)
    {
        for (size_t i = max_count -1 ; i >= min_count; --i)
        {
            auto const word_a = static_cast<word_type>(a.word(i));
            if (word_a > 0U) {
                return !both_positive;
            }
        }
    }
    else
    {
        for (size_t i = max_count -1 ; i >= min_count; --i)
        {
            auto const word_b = static_cast<word_type>(b.word(i));
            if (word_b > 0U) {
                return both_positive;
            }
        }
    }

    for (auto i=min_count - 1; i >= 0;--i)
    {
        auto const word_a = static_cast<word_type>(a.word(i));
        auto const word_b = static_cast<word_type>(b.word(i));

        if (word_a > word_b)
        {
            return !both_positive;
        }
        else if (word_a < word_b)
        {
            return both_positive;
        }
    }



    return false;
}




template <size_t W, size_t V> bool operator!=(const sinteger<W>& a, const sinteger<V>& b) 
{
    return !(a == b);
}

template <size_t W, size_t V> bool operator<=(const sinteger<W>& a, const sinteger<V>& b)
{
    return (a < b) || (a == b);
}

template <size_t W, size_t V> bool operator>=(const sinteger<W>& a, const sinteger<V>& b)
{
    return b <= a;
}

template <size_t W, size_t V> bool operator>(const sinteger<W>& a, const sinteger<V>& b)
{
    return b < a;
}

}