#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/core/word_array.hpp>

namespace aarith {
template <size_t DestinationWidth, size_t SourceWidth, typename WordType>
[[nodiscard]] auto constexpr width_cast(const word_array<SourceWidth, WordType>& source)
    -> word_array<DestinationWidth, WordType>
{
    if constexpr (SourceWidth == DestinationWidth)
    {
        return source;
    }
    else
    {
        word_array<DestinationWidth, WordType> word_container;

        if constexpr (DestinationWidth >= SourceWidth)
        {
            for (auto i = 0U; i < source.word_count(); ++i)
            {
                word_container.set_word(i, source.word(i));
            }
        }
        else
        {
            for (auto i = 0U; i < word_container.word_count(); ++i)
            {
                word_container.set_word(i, source.word(i));
            }
        }
        return word_container;
    }
}
} // namespace aarith