#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/core/word_array.hpp>

namespace aarith {

/**
 *
 * @brief Expands/shrinks the bit width of a word_array or (unsigned) integer
 *
 * @note Expanding a negative signed number will change the stored value as the negative sign is
 * lost.
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
[[nodiscard]] auto constexpr logical_width_cast_left(const Container<SourceWidth, WordType>& source)
    -> Container<DestinationWidth, WordType>
{

    static_assert(is_word_array_v<Container<SourceWidth, WordType>>);

    if constexpr (SourceWidth == DestinationWidth)
    {
        return source;
    }
    else
    {
        Container<DestinationWidth, WordType> word_container;

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

/**
 *
 * @brief Expands/shrinks the bit width of a word_array or (unsigned) integer
 *
 *
 * The first bit (i.e. the MSB) will be used to fill the new bits in case of extenting the width of
 * the source. This means that for signed integers, the value of the number remains the same.
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
[[nodiscard]] auto constexpr arithmetic_width_cast_left(
    const Container<SourceWidth, WordType>& source) -> Container<DestinationWidth, WordType>
{

    static_assert(is_word_array_v<Container<SourceWidth, WordType>>);

    using D = Container<DestinationWidth, WordType>;

    word_array<DestinationWidth, WordType> result =
        logical_width_cast_left<DestinationWidth, word_array>(source);

    if constexpr (DestinationWidth > SourceWidth)
    {
        const bool is_negative = source.msb();

        if (is_negative)
        {
            for (size_t i = SourceWidth; i < DestinationWidth; ++i)
            {
                result.set_bit(i);
            }
        }
        return D{result};
    }
    else
    {
        return D{result};
    }
}

/**
 *
 * @brief Expands/shrinks the bit width of a word_array or (unsigned) integer
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
          size_t SourceWidth, typename WordType,
          typename = std::enable_if_t<is_word_array_v<Container<DestinationWidth, WordType>>>>
[[nodiscard]] auto constexpr width_cast(const Container<SourceWidth, WordType>& source)
    -> Container<DestinationWidth, WordType>
{
    if constexpr (is_signed_v<Container<SourceWidth, WordType>>)
    {
        return arithmetic_width_cast_left<DestinationWidth, Container, SourceWidth, WordType>(
            source);
    }
    else
    {
        return logical_width_cast_left<DestinationWidth, Container, SourceWidth, WordType>(source);
    }
}

/**
 *
 * @brief Expands/shrinks the bit width of a word_array or unsigned integer from the right
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
          size_t SourceWidth, typename WordType,
          typename = std::enable_if_t<is_word_array_v<Container<DestinationWidth, WordType>>>>
[[nodiscard]] auto constexpr width_cast_right(const Container<SourceWidth, WordType>& source)
    -> Container<DestinationWidth, WordType>
{
    if constexpr (SourceWidth == DestinationWidth)
    {
        return source;
    }
    else
    {
        Container<DestinationWidth, WordType> result;

        if constexpr (DestinationWidth >= SourceWidth)
        {
            result = source;
            result <<= (DestinationWidth - SourceWidth);
        }
        else
        {
            source >>= (SourceWidth - DestinationWidth);
            result = width_cast<DestinationWidth>(source);
        }
        return result;
    }
}

/**
 *
 * @tparam Left
 * @tparam Right
 * @tparam W
 * @tparam SourceWidth
 * @tparam WordType
 * @param source
 * @return
 */
template <size_t Left, size_t Right, template <size_t, typename> class W, size_t SourceWidth,
          typename WordType>
[[nodiscard]] constexpr auto expand(const W<SourceWidth, WordType>& source)
{

    using L = W<SourceWidth + Left, WordType>;
    using R = W<SourceWidth + Left + Right, WordType>;
    L left_expanded;

    if constexpr (Left > 0)
    {
        left_expanded = width_cast<SourceWidth + Left>(source);
    }
    else
    {
        left_expanded = source;
    }

    R right_expanded{left_expanded};

    if constexpr (Right > 0)
    {
        right_expanded <<= Right;
    }

    return right_expanded;
}

} // namespace aarith