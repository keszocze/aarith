#pragma once

namespace aarith {

template <size_t Width>
[[nodiscard]] auto operator&(const word_array<Width>& lhs, const word_array<Width>& rhs)
    -> word_array<Width>
{
    word_array<Width> bitwise_and;
    for (auto counter = 0U; counter < lhs.word_count(); ++counter)
    {
        bitwise_and.set_word(counter, lhs.word(counter) & rhs.word(counter));
    }
    return bitwise_and;
}

template <size_t Width>
auto operator|(const word_array<Width>& lhs, const word_array<Width>& rhs) -> word_array<Width>
{
    word_array<Width> bitwise_or;
    for (auto counter = 0U; counter < lhs.word_count(); ++counter)
    {
        bitwise_or.set_word(counter, lhs.word(counter) | rhs.word(counter));
    }
    return bitwise_or;
}

template <size_t Width>
auto operator^(const word_array<Width>& lhs, const word_array<Width>& rhs) -> word_array<Width>
{
    word_array<Width> bitwise_xor;
    for (auto counter = 0U; counter < lhs.word_count(); ++counter)
    {
        bitwise_xor.set_word(counter, lhs.word(counter) ^ rhs.word(counter));
    }
    return bitwise_xor;
}

template <size_t Width>
[[nodiscard]] auto operator~(const word_array<Width>& rhs) -> word_array<Width>
{
    word_array<Width> bitwise_not;
    for (auto counter = 0U; counter < rhs.word_count(); ++counter)
    {
        bitwise_not.set_word(counter, ~rhs.word(counter));
    }
    return bitwise_not;
}

/**
 * @brief  Counts the number of bits set to zero before the first one appears (from MSB to LSB)
 * @tparam Width Width of the word_array
 * @param value The word to count the leading zeroes in
 * @return
 */
template <size_t Width> auto count_leading_zeroes(const word_array<Width>& value) -> size_t
{
    for (auto i = Width; i > 0; --i)
    {
        if (value.bit(i - 1))
        {
            return (Width - i);
        }
    }
    return Width;
}

template <size_t Width>
[[nodiscard]] auto operator<<(const word_array<Width>& lhs, const size_t rhs) -> word_array<Width>
{
    if (rhs >= Width)
    {
        return word_array<Width>(0U);
    }
    if (rhs == 0)
    {
        return lhs;
    }
    word_array<Width> shifted;
    const auto skip_words = rhs / lhs.word_width();
    const auto shift_word_left = rhs - skip_words * lhs.word_width();
    const auto shift_word_right = lhs.word_width() - shift_word_left;

    for (auto counter = lhs.word_count(); counter > 0; --counter)
    {
        if (counter + skip_words < lhs.word_count())
        {
            typename word_array<Width>::word_type new_word;
            new_word = lhs.word(counter) << shift_word_left;
            if (shift_word_right < lhs.word_width())
            {
                new_word = new_word | (lhs.word(counter - 1) >> shift_word_right);
            }
            shifted.set_word(counter + skip_words, new_word);
        }
    }
    typename word_array<Width>::word_type new_word;
    new_word = lhs.word(0) << shift_word_left;
    shifted.set_word(skip_words, new_word);

    return shifted;
}

template <size_t Width>
auto operator>>(const word_array<Width>& lhs, const size_t rhs) -> word_array<Width>
{
    if (rhs >= Width)
    {
        return word_array<Width>(0U);
    }
    if (rhs == 0)
    {
        return lhs;
    }

    word_array<Width> shifted;
    const auto skip_words = rhs / lhs.word_width();
    const auto shift_word_right = rhs - skip_words * lhs.word_width();
    const auto shift_word_left = lhs.word_width() - shift_word_right;

    for (auto counter = skip_words; counter < lhs.word_count(); ++counter)
    {
        typename word_array<Width>::word_type new_word;
        new_word = lhs.word(counter) >> shift_word_right;
        if (shift_word_left < lhs.word_width() && counter + 1 < lhs.word_count())
        {
            new_word = new_word | (lhs.word(counter + 1) << shift_word_left);
        }
        shifted.set_word(counter - skip_words, new_word);
    }
    typename word_array<Width>::word_type new_word;
    new_word = lhs.word(lhs.word_count() - 1) >> shift_word_right;
    shifted.set_word(lhs.word_count() - skip_words - 1, new_word);

    return shifted;
}

/**
 * @brief Extracts a range from the word container
 *
 * Note that the indexing is done
 *  - zero based starting from the LSB
 *  - is inclusive (i.e. the start and end point are part of the range)
 *
 * @tparam S Starting index (inclusive, from left to right)
 * @tparam E  Ending index (inclusive, from left to right)
 * @tparam W Width of the word container that the range is taken from
 * @param w  Word container from which the range is taken from
 * @return Range word[S,E], including the
 */
template <size_t S, size_t E, size_t W> word_array<(S - E) + 1> bit_range(const word_array<W>& w)
{
    static_assert(S < W, "Range must start within the word");
    static_assert(E <= S, "Range must be positive (i.e. this method will not reverse the word");

    return width_cast<(S - E) + 1>(w >> E);
}

/**
 * @brief Splits the word container at the given splitting point
 * @tparam S Splitting point
 * @tparam W Width of the word container to be split
 * @param w Word container that is split
 * @return Pair of <word[W-1,S+1], word[S,0]>
 */
template <size_t S, size_t W>
std::pair<word_array<W - (S + 1)>, word_array<S + 1>> split(const word_array<W>& w)
{
    static_assert(S < W - 1 && S >= 0);

    const word_array<W - (S + 1)> lhs{width_cast<W - (S + 1)>(w >> (S + 1))};

    const word_array<S + 1> rhs = width_cast<S + 1>(w);

    return std::make_pair(lhs, rhs);
}
} // namespace aarith