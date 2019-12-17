#pragma once

#include <aarith/types/sinteger.hpp>
#include <aarith/types/traits.hpp>
#include <cstdint>



namespace aarith {

template <size_t W, size_t V> bool operator==(const sinteger<W>& a, const sinteger<V>& b)
{
    const auto min_count = std::min(a.word_count(), b.word_count());
    const auto max_count = std::max(a.word_count(), b.word_count());

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

}