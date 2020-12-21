#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/core/word_array.hpp>

namespace aarith {

/**
 *
 * @brief Expands/shrinks the bit width of a word_array or unsigned integer
 *
 * The value of an integer remains unchanged if the bit width is increased.
 *
 * @note Reducing the bit width performs a hard truncation.
 *
 * @tparam DestinationWidth The width to which the input is expanded/shrunk
 * @tparam SourceWidth The input width of the integer
 * @param source The integer that whose width is changed
 * @return integer with specified bit width
 */
template <size_t DestinationWidth, template <size_t, typename> typename Container,
          size_t SourceWidth, typename WordType>
[[nodiscard]] auto constexpr width_cast(const Container<SourceWidth, WordType>& source)
    -> Container<DestinationWidth, WordType>
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