#pragma once

#pragma once

#include "aarith/types/traits.hpp"
#include "aarith/types/word_container.hpp"
#include <cstdint>

namespace aarith {

    /**
     * @brief Compares two word_containers bit-wise
     * @tparam W Width of the left word_container
     * @tparam V Width of the right word_container
     * @param a Left word_container
     * @param b Right word_container
     * @return true iff the word containers are identical in every bit
     */
template <size_t W, size_t V>
bool operator==(const word_container<W>& a, const word_container<V>& b)
{
    if constexpr (W != V)
    {
        return false;
    }
    else
    {
        for (auto i = 0U; i < word_container<W>::word_count(); ++i)
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
bool operator!=(const word_container<W>& a, const word_container<V>& b)
{
    // we do not care about speed and simply call the equality function....
    return !(a == b);
}

} // namespace aarith