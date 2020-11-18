#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/integer/integers.hpp>

namespace aarith {

/**
 *
 * @note Two numbers can be equal even though they have different bit widths!
 */
template <size_t W, size_t V, typename T, template <size_t, typename> typename Int>
constexpr bool operator==(const Int<W, T>& a, const Int<V, T>& b)
{
    constexpr size_t max_width = std::max(W, V);

    Int<max_width, T> a_ = width_cast<max_width>(a);
    Int<max_width, T> b_ = width_cast<max_width>(b);

    for (size_t i = 0U; i < Int<max_width, T>::word_count(); ++i)
    {
        if (a_.word(i) != b_.word(i))
        {
            return false;
        }
    }

    return true;
}

template <size_t W, size_t V, typename WordType>
constexpr bool operator<(const uinteger<W, WordType>& a, const uinteger<V, WordType>& b)
{

    constexpr size_t words_W = integer<W, WordType>::word_count();
    constexpr size_t words_V = integer<V, WordType>::word_count();

    if constexpr (words_W == words_V)
    {
        for (auto i = words_W; i > 0; --i)
        {
            WordType const word_a = a.word(i - 1);
            WordType const word_b = b.word(i - 1);

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

        integer<max_width, WordType> a_ = width_cast<max_width>(a);
        integer<max_width, WordType> b_ = width_cast<max_width>(b);

        for (auto i = integer<max_width, WordType>::word_count(); i > 0; --i)
        {
            WordType const word_a = a_.word(i - 1);

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

template <typename W, typename V> constexpr bool operator<=(const W& a, const V& b)
{
    static_assert(same_signedness<W, V>);
    return (a < b) || (a == b);
}

template <typename W, typename V> constexpr bool operator>=(const W& a, const V& b)
{
    static_assert(same_signedness<W, V>);
    return b <= a;
}

template <typename W, typename V> constexpr bool operator>(const W& a, const V& b)
{
    static_assert(same_signedness<W, V>);
    return b < a;
}

template <size_t W, size_t V, typename WordType>
constexpr bool operator<(const integer<W, WordType>& a, const integer<V, WordType>& b)
{

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
            auto const word_a = static_cast<WordType>(a.word(i));
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
            auto const word_b = static_cast<WordType>(b.word(i));
            if (word_b > 0U)
            {
                return both_positive;
            }
        }
    }

    for (auto i = min_count - 1; i >= 0; --i)
    {
        auto const word_a = static_cast<WordType>(a.word(i));
        auto const word_b = static_cast<WordType>(b.word(i));

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

template <typename Integer> const Integer& min(const Integer& a, const Integer& b)
{
    return (a < b) ? a : b;
}

template <typename Integer> const Integer& max(const Integer& a, const Integer& b)
{
    return (a < b) ? b : a;
}

} // namespace aarith
