#pragma once

#include <aarith/types/sinteger.hpp>
#include <aarith/types/traits.hpp>
#include <cstdint>

#include <bitset>
#include <iostream>

namespace aarith {

template <size_t W, size_t V> bool operator==(const sinteger<W>& a, const sinteger<V>& b)
{

#ifndef __clang__
    constexpr auto min_count = std::min(a.word_count(), b.word_count());
    constexpr auto max_count = std::max(a.word_count(), b.word_count());
#else
    const auto min_count = std::min(a.word_count(), b.word_count());
    const auto max_count = std::max(a.word_count(), b.word_count());
#endif

    if (a.is_negative() != b.is_negative())
    {
        return false;
    }

    const bool numbers_are_negative = a.is_negative(); // we can pick either a or b

    using word_type = typename sinteger<W>::word_type;

    if constexpr (W > V)
    {

        for (size_t i = min_count; i < max_count; ++i)
        {
            /*
             * We have to check whether the leading bits (within the range of the mask/the number of
             * specified bits) consists of ones only. This is necessary for negative numbers only
             */
            if (numbers_are_negative)
            {
                const word_type mask = sinteger<W>::word_mask(i);
                if (a.word(i) != mask)
                {
                    return false;
                }
            }
            else
            {
                /*
                 * If the numbers are not negative, there is no need to check against ones (as
                 * provided by the mask) being set as the unused bits are already correctly filled
                 * with zeroes (at least, if the numbers are identical)
                 */
                if (a.word(i) != 0U)
                {
                    return false;
                }
            };
        }
    }
    else
    {
        for (size_t i = min_count; i < max_count; ++i)
        {
            if (numbers_are_negative)
            {
                const word_type mask = sinteger<V>::word_mask(i);
                if (b.word(i) != mask)
                {
                    return false;
                }
            }
            else
            {
                if (b.word(i) != 0U)
                {
                    return false;
                }
            };
        }
    }

    for (auto i = 0U; i < min_count; ++i)
    {
        if (numbers_are_negative)
        {
            if constexpr (W > V)
            {
                if ((a.word(i) & b.word_mask(i)) != b.word(i))
                {
                    return false;
                }
            }
            else
            {
                if (a.word(i) != (b.word(i) & a.word_mask(i)))
                {
                    return false;
                }
            }
        }
        else
        {
            if (a.word(i) != b.word(i))
            {
                return false;
            }
        }
    }

    return true;
}

template <size_t W, size_t V> bool operator<(const sinteger<W>& a, const sinteger<V>& b)
{

    using word_type = typename sinteger<W>::word_type;
    const auto min_count = std::min(a.word_count(), b.word_count());
    const auto max_count = std::max(a.word_count(), b.word_count());

    if (a.is_negative() && !b.is_negative())
    {
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
        for (size_t i = max_count - 1; i >= min_count; --i)
        {
            auto const word_a = static_cast<word_type>(a.word(i));
            if (word_a > 0U)
            {
                return !both_positive;
            }
        }
    }
    else
    {
        for (size_t i = max_count - 1; i >= min_count; --i)
        {
            auto const word_b = static_cast<word_type>(b.word(i));
            if (word_b > 0U)
            {
                return both_positive;
            }
        }
    }

    for (auto i = min_count - 1; i >= 0; --i)
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

} // namespace aarith