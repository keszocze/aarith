#pragma once

#include "aarith/core/traits.hpp"
#include "uinteger.hpp"
#include <cstdint>

namespace aarith {

/**
 *
 * @note Two numbers can be equal even though they have different bit widths!
 */
template <size_t W, size_t V> bool operator==(const uinteger<W>& a, const uinteger<V>& b)
{

    // TODO remove when clang implements the stuff as constexpr
#ifndef __clang__
    constexpr auto min_count = std::min(a.word_count(), b.word_count());
    constexpr auto max_count = std::max(a.word_count(), b.word_count());
#else
    const auto min_count = std::min(a.word_count(), b.word_count());
    const auto max_count = std::max(a.word_count(), b.word_count());
#endif

    for (auto i = 0U; i < min_count; ++i)
    {
        if (a.word(i) != b.word(i))
        {
            return false;
        }
    }

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

    return true;
}

template <size_t W, size_t V> bool operator<(const uinteger<W>& a, const uinteger<V>& b)
{

    const auto min_count = std::min(a.word_count(), b.word_count());
    const auto max_count = std::max(a.word_count(), b.word_count());

    if constexpr (W > V)
    {
        for (size_t i = max_count - 1; i >= min_count; --i)
        {
            auto const word_a = a.word(i);
            if (word_a > 0U)
            {
                return false;
            }
        }
    }
    else
    {
        for (size_t i = max_count - 1; i >= min_count; --i)
        {
            auto const word_b = b.word(i);
            if (word_b > 0U)
            {
                return true;
            }
        }
    }

    for (auto i = min_count; i > 0; --i)
    {
        auto const word_a = a.word(i - 1);
        auto const word_b = b.word(i - 1);
        if (word_a < word_b)
        {
            return true;
        }
        else if (word_a > word_b)
        {
            return false;
        }
    }

    return false;
}

template <typename W, typename V> bool operator<=(const W& a, const V& b)
{
    static_assert(same_sign<W, V>);
    return (a < b) || (a == b);
}

template <typename W, typename V> bool operator>=(const W& a, const V& b)
{
    static_assert(same_sign<W, V>);
    return b <= a;
}

template <typename W, typename V> bool operator>(const W& a, const V& b)
{
    static_assert(same_sign<W, V>);
    return b < a;
}

} // namespace aarith
