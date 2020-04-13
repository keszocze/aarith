#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/integer/integers.hpp>
#include <cstdint>

#include <bitset>
#include <iostream>

namespace aarith {

/**
 *
 * @note Two numbers can be equal even though they have different bit widths!
 */
template <size_t W, size_t V> bool constexpr operator==(const uinteger<W>& a, const uinteger<V>& b)
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

/**
 * @brief Strictly tests to aarith integers for equality
 *
 * This method does not only check the stored value but also tests whether the bit-widths are
 * identical. If only a semantic comparison is to be performed, use {@see operator==} instead. If
 * the bit widths of the operands do not match, the result is immediately determined to be false.
 *
 * @tparam Integer Integer type
 * @tparam W Bitwidth of the first integer
 * @tparam V Bitwidth of the second integer
 * @param a First integer to compare
 * @param b Second integer to compare
 * @return Whether the two integers are strictly equal
 */
template <template <size_t> class Integer, size_t W, size_t V>
constexpr bool strict_eq(const Integer<W>& a, const Integer<V>& b)
{
    // we only want to work on our own types...
    static_assert(::aarith::is_integral_v<Integer<W>>);

    if constexpr (W != V)
    {
        return false;
    }
    return a == b;
}

/**
 * @brief Strictly compares to aarith integers for inequality
 *
 * This method does not only check the stored value but also tests whether the bit-widths are
 * identical. If only a semantic comparison is to be performed, use {@see operator!=} instead. If
 * the bit widths of the operands do not match, the result is immediately determined to be true.
 *
 * @tparam Integer Integer type
 * @tparam W Bitwidth of the first integer
 * @tparam V Bitwidth of the second integer
 * @param a First integer to compare
 * @param b Second integer to compare
 * @return Whether the two integers are strictly equal
 */
template <template <size_t> class Integer, size_t W, size_t V>
constexpr bool strict_not_eq(const Integer<W>& a, const Integer<V>& b)
{
    // we only want to work on our own types...
    static_assert(::aarith::is_integral_v<Integer<W>>);

    if constexpr (W != V)
    {
        return true;
    }

    return !(a == b);
}

template <size_t W, size_t V> constexpr bool operator<(const uinteger<W>& a, const uinteger<V>& b)
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

template <typename W, typename V> constexpr bool operator<=(const W& a, const V& b)
{
    static_assert(same_sign<W, V>);
    return (a < b) || (a == b);
}

template <typename W, typename V> constexpr bool operator>=(const W& a, const V& b)
{
    static_assert(same_sign<W, V>);
    return b <= a;
}

template <typename W, typename V> constexpr bool operator>(const W& a, const V& b)
{
    static_assert(same_sign<W, V>);
    return b < a;
}

template <size_t W, size_t V> constexpr bool operator==(const integer<W>& a, const integer<V>& b)
{
    // TODO remove when clang implements the stuff as constexpr
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

    using word_type = typename integer<W>::word_type;

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
                const word_type mask = integer<W>::word_mask(i);
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
                const word_type mask = integer<V>::word_mask(i);
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

template <size_t W, size_t V> constexpr bool operator<(const integer<W>& a, const integer<V>& b)
{

    using word_type = typename integer<W>::word_type;
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

} // namespace aarith