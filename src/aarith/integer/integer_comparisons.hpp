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
template <template <size_t> typename Int, size_t W, size_t V>
bool operator==(const Int<W>& a, const Int<V>& b)
{

    constexpr size_t max_width = std::max(W, V);

    Int<max_width> a_ = width_cast<max_width>(a);
    Int<max_width> b_ = width_cast<max_width>(b);

    for (size_t i = 0U; i < integer<max_width>::word_count(); ++i)
    {
        if (a_.word(i) != b_.word(i))
        {
            return false;
        }
    }

    return true;
}

template <size_t W, size_t V> bool operator<(const uinteger<W>& a, const uinteger<V>& b)
{

    using word_type = typename integer<W>::word_type;

    constexpr size_t words_W = integer<W>::word_count();
    constexpr size_t words_V = integer<V>::word_count();

    if constexpr (words_W == words_V)
    {
        for (auto i = words_W; i > 0; --i)
        {
            word_type const word_a = a.word(i - 1);
            word_type const word_b = b.word(i - 1);

            if (word_a < word_b)
            {
                return true;
            }
            if (word_a > word_b)
            {
                return false;
            }
        }
    }
    else
    {
        // if there really was a performance bottleneck, we could split this into the two cases
        // words_W < words_V and words_V < word_W and create special purpose code. currently we
        // do not care too much about speed

        const size_t max_width =
            std::max(W, V); // TODO make constexpr the moment clang supports this

        integer<max_width> a_ = width_cast<max_width>(a);
        integer<max_width> b_ = width_cast<max_width>(b);

        for (auto i = integer<max_width>::word_count(); i > 0; --i)
        {
            word_type const word_a = a_.word(i - 1);
            word_type const word_b = b_.word(i - 1);

            if (word_a < word_b)
            {
                return true;
            }
            if (word_a > word_b)
            {
                return false;
            }
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

// template <size_t W, size_t V> bool operator==(const integer<W>& a, const integer<V>& b)
//{
//
//    if (a.is_negative() != b.is_negative())
//    {
//        return false;
//    }
//
//    constexpr size_t max_width = std::max(W, V);
//
//    integer<max_width> a_ = width_cast<max_width>(a);
//    integer<max_width> b_ = width_cast<max_width>(b);
//
//    for (size_t i = 0U; i < integer<max_width>::word_count(); ++i)
//    {
//        if (a_.word(i) != b_.word(i))
//        {
//            return false;
//        }
//    }
//
//    return true;
//}

template <size_t W, size_t V> bool operator<(const integer<W>& a, const integer<V>& b)
{

    using word_type = typename integer<W>::word_type;

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

    constexpr size_t words_W = integer<W>::word_count();
    constexpr size_t words_V = integer<V>::word_count();

    if constexpr (words_W == words_V)
    {
        for (auto i = words_W; i > 0; --i)
        {
            word_type const word_a = a.word(i - 1);
            word_type const word_b = b.word(i - 1);

            if (word_a > word_b)
            {
                return !both_positive;
            }
            if (word_a < word_b)
            {
                return both_positive;
            }
        }
    }
    else
    {

        // if there really was a performance bottleneck, we could split this into the two cases
        // words_W < words_V and words_V < word_W and create special purpose code. currently we
        // do not care too much about speed

        const size_t max_width =
            std::max(W, V); // TODO make constexpr the moment clang supports this

        integer<max_width> a_ = width_cast<max_width>(a);
        integer<max_width> b_ = width_cast<max_width>(b);

        for (auto i = integer<max_width>::word_count(); i > 0; --i)
        {
            word_type const word_a = a_.word(i - 1);
            word_type const word_b = b_.word(i - 1);

            if (word_a > word_b)
            {
                return !both_positive;
            }
            if (word_a < word_b)
            {
                return both_positive;
            }
        }
    }

    return false;
}

} // namespace aarith