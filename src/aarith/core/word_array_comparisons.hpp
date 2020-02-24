#pragma once

#pragma once

#include "traits.hpp"
#include "word_array.hpp"
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
template <size_t W, size_t V>
bool operator==(const word_array<W>& a, const word_array<V>& b)
{
    if constexpr (W != V)
    {
        return false;
    }
    else
    {
        for (auto i = 0U; i < word_array<W>::word_count(); ++i)
        {
            if (a.word(i) != b.word(i))
            {
                return false;
            }
        }
        return true;
    }
}


template <size_t W, size_t V>
bool operator!=(const word_array<W>& a, const word_array<V>& b)
{
    // we do not care about speed and simply call the equality function....
    return !(a == b);
}

} // namespace aarith