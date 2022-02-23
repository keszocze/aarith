#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/core/word_array.hpp>

namespace aarith {

/**
 * @brief Logical Left-shift assignment operator
 * @tparam W The word_container type to work on
 * @param lhs The word_container to be shifted
 * @param rhs The number of bits to shift
 * @return The shifted word_container
 */
template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
constexpr W logical_left_shift(W& lhs, const size_t rhs)
{
    static_assert(::aarith::is_word_array_v<W>);

    constexpr size_t width = W::width();
    using word_type = typename W::word_type;

    if (rhs >= width)
    {
        lhs.fill(word_type{0});
        return lhs;
    }
    if (rhs == 0)
    {
        return lhs;
    }

    const auto skip_words = rhs / lhs.word_width();
    const auto shift_word_left = rhs - skip_words * lhs.word_width();
    const auto shift_word_right = lhs.word_width() - shift_word_left;
    for (auto counter = lhs.word_count(); counter > 0; --counter)
    {
        if (counter + skip_words < lhs.word_count())
        {
            word_type new_word = lhs.word(counter) << shift_word_left;
            if (shift_word_right < lhs.word_width())
            {
                new_word = new_word | (lhs.word(counter - 1) >> shift_word_right);
            }
            lhs.set_word(counter + skip_words, new_word);
        }
    }
    word_type new_word = lhs.word(0) << shift_word_left;
    lhs.set_word(skip_words, new_word);

    for (size_t i = 0; i < skip_words; ++i)
    {
        lhs.set_word(i, word_type{0});
    }

    return lhs;
}

/**
 * @brief Left-shift assignment operator
 * @tparam W The word_container type to work on
 * @param lhs The word_container to be shifted
 * @param rhs The number of bits to shift
 * @return The shifted word_container
 */
template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
constexpr W operator<<=(W& lhs, const size_t rhs)
{
    return logical_left_shift(lhs, rhs);
}

/**
 * @brief Left-shift operator
 * @tparam W The word_container type to work on
 * @param lhs The word_container to be shifted
 * @param rhs The number of bits to shift
 * @return The shifted word_container
 */
template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
[[nodiscard]] constexpr auto operator<<(const W& lhs, const size_t rhs) -> W
{

    W shifted{lhs};
    shifted <<= rhs;

    return shifted;
}

/**
 * @brief Left-shift assignment operator
 * @tparam W The word_container type to work on
 * @param lhs The word_container to be shifted
 * @param rhs The number of bits to shift
 * @return The shifted word_container
 */
template <typename W, typename U,
          typename = std::enable_if_t<is_word_array_v<W> && is_unsigned_v<U>>>
constexpr W operator<<=(W& lhs, const U& rhs)
{
    return logical_left_shift(lhs, static_cast<size_t>(rhs));
}

/**
 * @brief Left-shift operator
 * @tparam W The word_container type to work on
 * @param lhs The word_container to be shifted
 * @param rhs The number of bits to shift
 * @return The shifted word_container
 */
template <typename W, typename U,
          typename = std::enable_if_t<is_word_array_v<W> && is_unsigned_v<U>>>
[[nodiscard]] constexpr auto operator<<(const W& lhs, const U& rhs) -> W
{

    W shifted{lhs};
    shifted <<= static_cast<size_t>(rhs);

    return shifted;
}

/**
 * @brief Logical right-shift assignment
 *
 * @tparam Width The width of the word_array
 * @param lhs The word_array that is to be shifted
 * @param rhs The number of bits to shift
 * @return The shifted word_array
 */
template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
auto constexpr logical_right_shift(W& lhs, const size_t rhs) -> W
{

    static_assert(::aarith::is_word_array_v<W>);

    constexpr size_t width = W::width();
    using word_type = typename W::word_type;

    if (rhs >= width)
    {
        lhs.fill(word_type{0});
        return lhs;
    }
    if (rhs == 0)
    {
        return lhs;
    }

    const auto skip_words = rhs / lhs.word_width();
    const auto shift_word_right = rhs - skip_words * lhs.word_width();
    const auto shift_word_left = lhs.word_width() - shift_word_right;

    using word_type = typename W::word_type;

    for (auto counter = skip_words; counter < lhs.word_count(); ++counter)
    {
        word_type new_word = lhs.word(counter) >> shift_word_right;
        if (shift_word_left < lhs.word_width() && counter + 1 < lhs.word_count())
        {
            new_word = new_word | (lhs.word(counter + 1) << shift_word_left);
        }
        lhs.set_word(counter - skip_words, new_word);
    }

    if (skip_words > 0)
    {
        word_type new_word = lhs.word(lhs.word_count() - 1) >> shift_word_right;
        lhs.set_word(lhs.word_count() - skip_words - 1, new_word);

        for (size_t i = lhs.word_count() - skip_words; i < lhs.word_count(); ++i)
        {
            lhs.set_word(i, word_type{0});
        }
    }
    return lhs;
}

/**
 * @brief Right-shift assignment operator
 * @tparam Width The width of the word_array
 * @param lhs The word_array that is to be shifted
 * @param rhs The number of bits to shift
 * @return The shifted word_array
 */
template <typename W, typename = std::enable_if_t<is_word_array_v<W> || is_unsigned_v<W>>>
auto constexpr operator>>=(W& lhs, const size_t rhs) -> W
{
    return logical_right_shift(lhs, rhs);
}

/**
 * @brief Right-shift operator
 * @tparam Width The width of the word_array
 * @param lhs The word_array that is to be shifted
 * @param rhs The number of bits to shift
 * @return The shifted word_array
 */
template <typename W, typename = std::enable_if_t<is_word_array_v<W> || is_unsigned_v<W>>>
auto constexpr operator>>(const W& lhs, const size_t rhs) -> W
{

    W shifted{lhs};
    shifted >>= rhs;

    return shifted;
}

/**
 * @brief 'Arithmetic' right-shift operator
 *
 * This shift shifts in the first bit, resulting in an arithmetic right shift for signed integers.
 *
 * @tparam Width The width of the word array
 * @param lhs The word array to be shifted
 * @param rhs The number of bits to be shifted
 * @return The shifted word array
 */
template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
auto constexpr arithmetic_right_shift(W& lhs, const size_t rhs) -> W&
{
    constexpr size_t Width = W::width();
    using WordType = typename W::word_type;

    const bool lhs_was_negative = lhs.msb();

    if (rhs >= Width)
    {
        // i.e. the "number" is negative
        if (lhs_was_negative)
        {
            const auto max = std::numeric_limits<WordType>::max();
            lhs.fill(max);
            return lhs;
        }
        else
        {
            lhs.fill(WordType{0});
            return lhs;
        }
    }

    if (rhs == 0 || lhs.is_zero())
    {
        return lhs;
    }

    const auto skip_words = rhs / lhs.word_width();
    const auto shift_word_right = rhs - skip_words * lhs.word_width();
    const auto shift_word_left = lhs.word_width() - shift_word_right;

    for (auto counter = skip_words; counter < lhs.word_count(); ++counter)
    {
        WordType new_word = (lhs.word(counter) >> shift_word_right);
        if (shift_word_left < lhs.word_width() && counter + 1 < lhs.word_count())
        {
            new_word = new_word | (lhs.word(counter + 1) << shift_word_left);
        }

        lhs.set_word(counter - skip_words, new_word);
    }

    if (skip_words > 0)
    {
        WordType new_word = (lhs.word(lhs.word_count() - 1) >> shift_word_right);
        lhs.set_word(lhs.word_count() - skip_words - 1, new_word);

        for (size_t i = lhs.word_count() - skip_words; i < lhs.word_count(); ++i)
        {
            const auto fill_word =
                lhs.is_negative() ? std::numeric_limits<WordType>::max() : WordType{0};
            lhs.set_word(i, fill_word);
        }
    }

    if (lhs_was_negative)
    {
        for (size_t i = (Width - 1); i >= (Width - rhs); --i)
        {
            lhs.set_bit(i);
        }
    }

    return lhs;
}

/**
 * @brief Rotates the word_array to the left using a specified carry-in
 * @tparam W
 */
template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
constexpr W& rotate_through_carry_left(W& lhs, const bool carry_in, const size_t rotate = 1)
{
    const size_t right_shift = lhs.width() - (rotate - 1);
    auto slice = lhs >> right_shift;
    slice.set_bit(rotate - 1, carry_in);
    lhs <<= rotate;
    lhs |= slice;

    return lhs;
}

/**
 * @brief Rotates the word_array to the left using a specified carry-in
 * @tparam W
 */
template <typename W, typename U,
          typename = std::enable_if_t<is_word_array_v<W> && is_unsigned_v<U>>>
constexpr W& rotate_through_carry_left(W& lhs, const bool carry_in, const U& rotate = U::one())
{
    return rotate_through_carry_left(lhs, carry_in, static_cast<size_t>(rotate));
}

/**
 * @brief Rotates the word_array to the right using a specified carry-in
 * @tparam W
 */
template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
constexpr W& rotate_through_carry_right(W& lhs, const bool carry_in, const size_t rotate = 1)
{

    const size_t left_shift = lhs.width() - (rotate - 1);
    auto slice = lhs << left_shift;
    slice.set_bit(lhs.width() - rotate, carry_in);
    lhs >>= rotate;
    lhs |= slice;

    return lhs;
}

/**
 * @brief Rotates the word_array to the right using a specified carry-in
 * @tparam W
 */
template <typename W, typename U,
          typename = std::enable_if_t<is_word_array_v<W> && is_unsigned_v<U>>>
constexpr W& rotate_through_carry_right(W& lhs, const bool carry_in, const U& rotate = U::one())
{
    return rotate_through_carry_right(lhs, carry_in, static_cast<size_t>(rotate));
}

/**
 * @brief Rotates the word_array to the left
 *
 * This shift preserves the signedness of the integer.
 *
 * @tparam Width The width of the signed integer
 * @param lhs The integer to be shifted
 * @param rhs The number of bits to be shifted
 * @return The shifted integer
 */
template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
constexpr W& rotate_left(W& lhs, size_t rotate = 1)
{

    // TODO (keszocze) offer this as a compile-time optimized version?
    //    constexpr size_t width = W::width();
    //    const auto slice = bit_range<width-1, width-R>(lhs);
    //
    //    lhs <<= R;
    //    lhs.set_bits(0, slice);

    const size_t right_shift = lhs.width() - rotate;
    const auto slice = lhs >> right_shift;
    lhs <<= rotate;
    lhs |= slice;

    return lhs;
}

/**
 * @brief Rotates the word_array to the left
 *
 * This shift preserves the signedness of the integer.
 *
 * @tparam Width The width of the signed integer
 * @param lhs The integer to be shifted
 * @param rhs The number of bits to be shifted
 * @return The shifted integer
 */
template <typename W, typename U,
          typename = std::enable_if_t<is_word_array_v<W> && is_unsigned_v<U>>>
constexpr W& rotate_left(W& lhs, const U& rotate = U::one())
{
    return rotate_left(lhs, static_cast<size_t>(rotate));
}

/**
 * @brief Rotates the word_array to the right
 *
 * This shift preserves the signedness of the integer.
 *
 * @tparam Width The width of the signed integer
 * @param lhs The integer to be shifted
 * @param rhs The number of bits to be shifted
 * @return The shifted integer
 */
template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
constexpr W& rotate_right(W& lhs, size_t rotate = 1)
{
    const size_t left_shift = lhs.width() - rotate;
    const auto slice = lhs << left_shift;
    lhs >>= rotate;
    lhs |= slice;

    return lhs;
}

/**
 * @brief Rotates the word_array to the right
 *
 * This shift preserves the signedness of the integer.
 *
 * @tparam Width The width of the signed integer
 * @param lhs The integer to be shifted
 * @param rhs The number of bits to be shifted
 * @return The shifted integer
 */
template <typename W, typename U,
          typename = std::enable_if_t<is_word_array_v<W> && is_unsigned_v<U>>>
constexpr W& rotate_right(W& lhs, const U& rotate = U::one())
{
    return rotate_right(lhs, static_cast<size_t>(rotate));
}

} // namespace aarith