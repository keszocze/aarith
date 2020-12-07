#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/core/word_array.hpp>

namespace aarith {

template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
constexpr W& operator&=(W& a, const W& b)
{
    for (auto counter = 0U; counter < a.word_count(); ++counter)
    {
        a.set_word(counter, a.word(counter) & b.word(counter));
    }
    return a;
}

template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
[[nodiscard]] constexpr auto operator&(const W& lhs, const W& rhs) -> W
{
    W bitwise_and{lhs};
    bitwise_and &= rhs;
    return bitwise_and;
}

template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
constexpr W& operator|=(W& a, const W& b)
{
    for (auto counter = 0U; counter < a.word_count(); ++counter)
    {
        a.set_word(counter, a.word(counter) | b.word(counter));
    }
    return a;
}

template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
auto constexpr operator|(const W& lhs, const W& rhs) -> W
{
    W bitwise_or{lhs};
    bitwise_or |= rhs;
    return bitwise_or;
}

template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
constexpr W& operator^=(W& a, const W& b)
{
    for (auto counter = 0U; counter < a.word_count(); ++counter)
    {
        a.set_word(counter, a.word(counter) ^ b.word(counter));
    }
    return a;
}

template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
constexpr auto operator^(const W& lhs, const W& rhs) -> W
{
    W bitwise_xor{lhs};
    bitwise_xor ^= rhs;
    return bitwise_xor;
}

template <typename W>[[nodiscard]] constexpr auto operator~(const W& rhs) -> W
{
    static_assert(::aarith::is_word_array_v<W>);
    W bitwise_not;
    for (auto counter = 0U; counter < rhs.word_count(); ++counter)
    {
        bitwise_not.set_word(counter, ~rhs.word(counter));
    }
    return bitwise_not;
}

/**
 * @brief Counts the number of bits set to zero before the first one appears
 * (from MSB to LSB).
 *
 * Optional parameter offset makes it possible to skip the first offset-many
 * MSB. For example, calling count_leading_zeroes(0b100111, offset=1) would
 * return 2 as the first one is skipped.
 *
 * @tparam Width Width of the word_array
 * @param value The word to count the leading zeroes in
 * @param offset The number of MSB to skip before conuting.
 * @return The number of zeroes
 */
template <size_t Width, typename WordType>
constexpr size_t count_leading_zeroes(const word_array<Width, WordType>& value, size_t offset=0)
{
    // if offset is chosen too big, skip all bits, counting none

    if (offset >= Width) {
        return 0;
    }

    // if offset is set to a reasonable value, count bit by bit

    const size_t start = Width - offset;

    for (auto i = start; i > 0; --i)
    {
        if (value.bit(i - 1))
        {
            return (Width - i - offset);
        }
    }

    return Width - offset;
}

/**
 * @brief Counts the number of bits set to ones before the first zero appears
 * (from MSB to LSB).
 *
 * Optional parameter offset makes it possible to skip the first offset-many
 * MSB. For example, calling count_leading_ones(0b011000, offset=1) would
 * return 2 as the first zero is skipped.
 *
 * @tparam Width Width of the word_array
 * @param value The word to count the leading ones in
 * @param offset The number of MSB to skip before conuting.
 * @return The number of ones
 */
template <size_t Width, typename WordType>
constexpr size_t count_leading_ones(const word_array<Width, WordType>& value, size_t offset=0)
{
    return count_leading_zeroes(~value, offset);
}

/**
 * @brief Computes the position of the first set bit (i.e. a bit set to one) in the word_array from
 * MSB to LSB
 *
 * @tparam Width Width of the word_array
 * @param value The word_array whose first set bit should be found
 * @return The index of the first set bit in value
 */
template <size_t Width, typename WordType>
size_t first_set_bit(const word_array<Width, WordType>& value)
{
    size_t first_bit = 0UL;

    // we have the operator bool checking whether (value >> first_bit) != 0
    while (value >> first_bit)
    {
        ++first_bit;
    }
    return first_bit;
}

/**
 * @brief Return a mask that has the n least significant bits set to one
 * and all other bits set to zero.
 *
 * If n is greater than Width, this function returns a word array with
 * all Width-many bits set to one.
 *
 * @tparam Width Width of the returned word_array
 * @param n The number of ones in the returned array
 * @return The mask with n-many bits set to one.
 */
template <size_t Width, typename WordType>
[[nodiscard]] constexpr word_array<Width, WordType> get_low_mask(size_t n)
{
    word_array<Width, WordType> w(0);

    for (size_t idx = 0; idx < n && idx < Width; ++idx)
    {
        w.set_bit(idx, true);
    }

    return w;
}

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
 * @brief Arithmetic right-shift operator
 *
 * This shift preserves the signedness of the integer.
 *
 * @tparam Width The width of the signed integer
 * @param lhs The integer to be shifted
 * @param rhs The number of bits to be shifted
 * @return The shifted integer
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
        if (lhs.msb())
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
 * @brief Sets the most significant bit to one
 * @tparam W Type that behaves like a word array (can, e.g., also be an uinteger)
 * @tparam V The bit width of the type W
 * @tparam WordType The word type the data is stored in
 * @param w The word array like parameter
 * @return w with the most significant bit set to one
 */
template <template <size_t, typename> class W, size_t V, typename WordType = uint64_t>
[[nodiscard]] constexpr W<V, WordType> msb_one(const W<V, WordType>& w)
{
    constexpr W<V, WordType> msbone{word_array<V, WordType>::msb_one()};
    return (w | msbone);
}

/**
 * @brief Extracts a range from the word array
 *
 * Note that the indexing is done
 *  - zero based starting from the LSB
 *  - is inclusive (i.e. the start and end point are part of the range)
 *
 * @tparam S Starting index (inclusive, from left to right)
 * @tparam E  Ending index (inclusive, from left to right)
 * @tparam W Width of the word container that the range is taken from
 * @param w  Word container from which the range is taken from
 * @return Range word[E,S] inclusive
 */
template <size_t S, size_t E, size_t W, typename WordType>
[[nodiscard]] constexpr word_array<(S - E) + 1, WordType>
bit_range(const word_array<W, WordType>& w)
{
    static_assert(S < W, "Range must start within the word");
    static_assert(E <= S, "Range must be positive (i.e. this method will not reverse the word");

    return width_cast<(S - E) + 1, W, WordType>(w >> E);
}

template <size_t W, size_t V, typename WordType>
word_array<W + V, WordType> concat(const word_array<W, WordType>& w,
                                   const word_array<V, WordType>& v)
{
    word_array<W + V, WordType> result{w};
    result = result << V;
    result = result | word_array<W + V, WordType>{v};
    return result;
}

/**
 * @brief Splits the word container at the given splitting point
 * @tparam S Splitting point
 * @tparam W Width of the word container to be split
 * @param w Word container that is split
 * @return Pair of <word[W-1,S+1], word[S,0]>
 */
template <size_t S, size_t W, typename WordType>
[[nodiscard]] constexpr std::pair<word_array<W - (S + 1), WordType>, word_array<S + 1, WordType>>
split(const word_array<W, WordType>& w)
{
    static_assert(S < W - 1 && S >= 0);

    const word_array<W - (S + 1), WordType> lhs{width_cast<W - (S + 1)>(w >> (S + 1))};

    const word_array<S + 1, WordType> rhs = width_cast<S + 1>(w);

    return std::make_pair(lhs, rhs);
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

    // TODO offer this as a compile-time optimized version?
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

/**
 * @brief Applies a given function to all words within the word_array
 * @tparam F "Catch-all" parameter for functions operating on the words of the word_array
 * @tparam W Bit width of the word_array to operate on
 * @param w The word_array to operate on
 * @param f Function of type word_array<W>::word_type -> word_array<W>::word_type
 * @return A new word_array with the transformed words
 */
template <class F, size_t W, typename WordType>
[[nodiscard]] constexpr word_array<W, WordType> map(const word_array<W, WordType>& w, const F f)
{

    word_array<W, WordType> result;
    for (size_t i = 0; i < w.word_count(); ++i)
    {
        result.set_word(i, f(w.word(i)));
    }
    return result;
}

/**
 * @brief Applies a function to all words of two word_arrays and zips them to a new
 * word_array
 *
 * If the word_arrays have a different number of words, the result will be of size min(W,V);
 *
 * @tparam F "Catch-all" parameter for functions operating on the words of the word_array
 * @tparam W Bit width of the first word_array to operate on
 * @tparam V Bit width of the second word_array to operate on
 * @param w The first word_array to operate on
 * @param v The second word_array to operate on
 * @param f Function of type (word_array<W>::word_type, word_array<W>::word_type) ->
 * word_array<W>
 * @return The newly created, zipped word_array
 */
template <class F, size_t W, size_t V, typename WordType>
[[nodiscard]] constexpr word_array<std::min(W, V), WordType>
zip_with(const word_array<W, WordType>& w, const word_array<V, WordType>& v, F f)
{
    constexpr size_t L = std::min(W, V);
    word_array<L, WordType> result;

    for (size_t i = 0; i < result.word_count(); ++i)
    {
        result.set_word(i, f(w.word(i), v.word(i)));
    }
    return result;
}

/**
 * @brief Applies a function to all words of two word_arrays and zips them to a new
 * word_array carrying along a state
 *
 * The state that is carry will be parsed to the next function call on the two words of the
 * container and may be updated in the process (e.g. generating a carry that propagates through the
 * container).
 *
 * If the word_arrays have a different number of words, the result will be of size min(W,V);
 *
 * @tparam F "Catch-all" parameter for functions operating on the words of the word_array
 * @tparam W Bit width of the first word_array to operate on
 * @tparam V Bit width of the secnd word_array to operate on
 * @param w The first word_array to operate on
 * @param v The second word_array to operate on
 * @param f Function of type (word_array<W>::word_type, word_array<W>::word_type,
 * word_array<W>::word_type) -> std::pari<word_array<W>,word_array<W>::word_type>
 * @param initial_state The initial state to be passed into the operation
 * @return The newly created, zipped word_array
 */
template <class F, size_t W, size_t V, typename WordType>
[[nodiscard]] constexpr word_array<std::min(W, V), WordType>
zip_with_state(const word_array<W, WordType>& w, const word_array<V, WordType>& v, const F f,
               const typename word_array<std::min(W, V), WordType>::word_type initial_state =
                   typename word_array<std::min(W, V), WordType>::word_type())
{
    constexpr size_t L = std::min(W, V);
    word_array<L, WordType> result;

    using wt = typename word_array<L, WordType>::word_type;

    wt state = initial_state;

    for (size_t i = 0; i < result.word_count(); ++i)
    {
        const auto fun_res = f(w.word(i), v.word(i), state);
        state = fun_res.second;
        wt new_word = fun_res.first;
        result.set_word(i, new_word);
    }
    return result;
}

/**
 * @brief Zips two word containers that might have a different number of words, expanding missing
 * words with zeroes.
 *
 * If the word_arrays have a different number of words, the "missing" words are filled up with
 * zeroes such that this method will return a word_array that has the max width of both input
 * word_arrays.
 *
 * @tparam F "Catch all" type for the zip with function
 * @tparam W Width of the first word_array
 * @tparam V Width of the second word_array
 * @param w First word_array
 * @param v Second word_array
 * @param f The zip_with function of type (word_array<W>::word_type,
 * word_array<W>::word_type) -> word_array<W>
 * @return
 */
template <class F, size_t W, size_t V, typename WordType>
[[nodiscard]] constexpr word_array<std::max(W, V), WordType>
zip_with_expand(const word_array<W, WordType>& w, const word_array<V, WordType>& v, const F f)
{
    constexpr size_t wc = word_array<W, WordType>::word_count();
    constexpr size_t vc = word_array<V, WordType>::word_count();

    // simply return the "standard" zip_with when the word widths are identical
    if constexpr (wc == vc)
    {
        return zip_with(w, v, f);
    }
    else
    {

        constexpr size_t max = std::max(W, V);

        word_array<max, WordType> w_{w};
        word_array<max, WordType> v_{v};

        return zip_with(w_, v_, f);
    }
}

/**
 * @brief Zips two word containers that might have a different number of words, expanding missing
 * words with zeroes carrying along a state.
 *
 * The state that is carry will be parsed to the next function call on the two words of the
 * container and may be updated in the process (e.g. generating a carry that propagates through the
 * container).
 *
 *
 * If the word_arrays have a different number of words, the "missing" words are filled up with
 * zeroes such that this method will return a word_array that has the max width of both input
 * word_arrays.
 *
 * @tparam F "Catch all" type for the zip with function
 * @tparam W Width of the first word_array
 * @tparam V Width of the second word_array
 * @param w First word_array
 * @param v Second word_array
 * @param f Function of type (word_array<W>::word_type, word_array<W>::word_type,
 * word_array<W>::word_type) -> std::pari<word_array<W>,word_array<W>::word_type>
 * @param initial_state The initial state to be passed into the operation
 * @return
 */
template <class F, size_t W, size_t V, typename WordType>
[[nodiscard]] constexpr word_array<std::max(W, V), WordType>
zip_with_state_expand(const word_array<W, WordType>& w, const word_array<V, WordType>& v, const F f,
                      const typename word_array<std::max(W, V), WordType>::word_type initial_state =
                          typename word_array<std::max(W, V), WordType>::word_type())
{
    constexpr size_t wc = word_array<W, WordType>::word_count();
    constexpr size_t vc = word_array<V, WordType>::word_count();

    // simply return the "standard" zip_with when the word widths are identical
    if constexpr (wc == vc)
    {
        return zip_with_state(w, v, f);
    }
    else
    {

        constexpr size_t max = std::max(W, V);

        word_array<max, WordType> w_{w};
        word_array<max, WordType> v_{v};

        return zip_with_state(w_, v_, f, initial_state);
    }
}

/**
 *
 * @brief Reduces the vector using a given two-ary function.
 *
 * This is also known as a fold.
 *
 * @tparam R Return type of the reducing function
 * @tparam F "Catch all" type for the reducing function
 * @tparam W The width of the word_array
 * @param w The word_array
 * @param f The reducing function of type (word_array<W>::word_type, R) -> R
 * @param initial_value The initial value that is fed into f
 * @return The value from reducing the word_array using f and the given initial value
 */
template <class R, class F, size_t W, typename WordType>
[[nodiscard]] R constexpr reduce(const word_array<W, WordType>& w, const F f, const R initial_value)
{
    R result = initial_value;

    for (size_t i = 0; i < w.word_count(); ++i)
    {
        result = static_cast<R>(f(w.word(i), result));
    }

    return result;
}

/**
 * @brief zips and reduces two word_arrays
 *
 * @tparam R The return type of the reducing function
 * @tparam F "Catch all" type for the reducing function
 * @tparam W The width of the first word_array
 * @tparam V The width of the second word_array
 * @param w The first word_array
 * @param v The second word_array
 * @param f The reducing function of type (word_array<W>::word_type,
 * word_array<V>__word_type, R) -> R
 * @param initial_value The initial value that is fed into f
 * @return The value from zipping and reducing the two word_arrays
 */
template <class R, class F, size_t W, size_t V, typename WordType>
[[nodiscard]] R constexpr zip_reduce(const word_array<W, WordType>& w,
                                     const word_array<V, WordType>& v, const F f,
                                     const R initial_value)
{
    R result = initial_value;

    constexpr size_t L = std::min(W, V);

    for (size_t i = 0; i < word_array<L, WordType>::word_count(); ++i)
    {
        result = f(w.word(i), v.word(i), result);
    }

    return result;
}

/**
 * @brief zips and reduces two word_arrays that might have a different word count
 *
 * This method expands missing words by using zeros instead. This means that no values are skipped.
 *
 * @tparam R The return type of the reducing function
 * @tparam F "Catch all" type for the reducing function
 * @tparam W The width of the first word_array
 * @tparam V The width of the second word_array
 * @param w The first word_array
 * @param v The second word_array
 * @param f The reducing function of type (word_array<W>::word_type,
 * word_array<V>__word_type, R) -> R
 * @param initial_value The initial value that is fed into f
 * @return The value from zipping and reducing the two word_arrays
 */
template <class R, class F, size_t W, size_t V, typename WordType>
[[nodiscard]] R zip_reduce_expand(const word_array<W, WordType>& w,
                                  const word_array<V, WordType>& v, const F f,
                                  const R initial_value)
{

    R result = initial_value;

    constexpr size_t wc = w.word_count();
    constexpr size_t vc = v.word_count();

    if constexpr (wc == vc)
    {
        return zip_reduce(w, v, f, initial_value);
    }
    else
    {

        constexpr size_t max = std::max(W, V);

        word_array<max, WordType> w_{w};
        word_array<max, WordType> v_{v};

        return zip_reduce(w_, v_, f, initial_value);
    }
}

} // namespace aarith
