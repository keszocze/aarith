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

/**
 * @brief Tests if two word arrays are unequal
 *
 * @tparam W Width of the first word array
 * @tparam V Width of the second word array
 * @tparam WordType Data type usd to store the actual data
 * @param a First word array
 * @param b Second word array
 * @return a != b
 */
template <size_t W, size_t V, typename WordType>
constexpr bool operator!=(const word_array<W, WordType>& a, const word_array<V, WordType>& b)
{
    // we do not care about speed and simply call the equality function....
    return !(a == b);
}

} // namespace aarith