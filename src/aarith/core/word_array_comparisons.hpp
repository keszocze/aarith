#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/core/word_array.hpp>
#include <cstdint>

namespace aarith {

/**
 * @brief Compares two word_containers bit-wise
 * @tparam W Width of the left word_array
 * @tparam V Width of the right word_array
 * @param a Left word_array
 * @param b Right word_array
 * @return true iff the word containers are identical in every bit
 */
template <size_t W, size_t V, typename WordType>
constexpr bool operator==(const word_array<W, WordType>& a, const word_array<V, WordType>& b)
{
    if constexpr (W != V)
    {
        return false;
    }
    else
    {
        for (auto i = 0U; i < word_array<W, WordType>::word_count(); ++i)
        {
            if (a.word(i) != b.word(i))
            {
                return false;
            }
        }
        return true;
    }
}

template <typename W, typename V> constexpr bool operator!=(const W& a, const V& b)
{
    static_assert(::aarith::is_word_array_v<W>);
    static_assert(::aarith::is_word_array_v<V>);

    // we do not care about speed and simply call the equality function....
    return !(a == b);
}

} // namespace aarith