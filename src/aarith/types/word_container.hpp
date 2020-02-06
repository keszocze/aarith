#pragma once

#include "aarith/utilities/bit_operations.hpp"
#include "traits.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace aarith {

template <size_t Width, class WordType = uint64_t> class word_container
{
public:
    using word_type = WordType;
    using bit_type = WordType;
    static_assert(Width > 0, " Width must be at least 1 (bit)");

    /*
     * Constructors etc.
     */

    word_container() = default;

    word_container(WordType w)
    {
        this->words[0] = w & word_mask(0);
    }

    template <class... Args> word_container(WordType w, Args... args)
    {
        set_words(w, args...);
    }

    template <class... Args> static auto from_words(Args... args) -> word_container
    {
        word_container wc;
        wc.set_words(args...);
        return wc;
    }

    template <size_t V> word_container(const word_container<V>& other)
    {
        static_assert(V <= Width, "Can not create a word_container from larger container");

        for (auto i = 0U; i < other.word_count(); ++i)
        {
            set_word(i, other.word(i));
        }
    }
    template <size_t V> word_container<Width> operator=(const word_container<V>& other)
    {
        static_assert(V <= Width, "Can not create a word_container from larger container");

        if constexpr (V == Width)
        {
            if (&other == this)
            {
                return *this;
            }
        }

        for (size_t i = 0U; i < other.word_count(); ++i)
        {
            set_word(i, other.word(i));
        }

        if constexpr (this->word_count() > other.word_count())
        {
            for (size_t i = other.word_count(); i < this->word_count(); ++i)
            {
                set_word(i, 0U);
            }
        }
        return *this;
    }

    /*
     * Getters
     */

    [[nodiscard]] static constexpr auto word_width() noexcept -> size_t
    {
        return sizeof(word_type) * 8;
    }

    [[nodiscard]] static constexpr auto word_count() noexcept -> size_t
    {
        return size_in_words<word_type>(Width);
    }

    [[nodiscard]] static constexpr auto word_mask(size_t index) noexcept -> word_type
    {

        constexpr word_type other_masks = static_cast<word_type>(-1); // all ones, e.g. no masking
        constexpr word_type last_mask =
            (width() % word_width() != 0)
                ? (static_cast<word_type>(1) << (width() % word_width())) - 1
                : static_cast<word_type>(-1);
        return (index == word_count() - 1) ? last_mask : other_masks;
    };

    [[nodiscard]] static constexpr auto width() noexcept -> size_t
    {
        return Width;
    }

    [[nodiscard]] auto word(size_t index) const -> word_type
    {
        return words[index];
    }

    /**
     * @brief Returns ths most significant bit.
     *
     * The most significant bit is the Width's one (i.e. the one you can get via bit(Width-1)). This
     * method is simply there for convenience.
     */
    [[nodiscard]] auto msb() const -> bit_type
    {
        return bit(Width - 1);
    }


    /**
     * @brief Sets the value of the most significant bit (MSB)
     * @param b The value the MSB is set to
     */
    void set_msb(const bool b)
    {
        set_bit(Width - 1, b);
    }

    /**
     * @brief Returns bit at given index.
     *
     * @note No bounds checking is performed! If your index is too large bad things will happen!
     *
     * @param index The index for which the bit is to be returned
     * @return  The bit at the indexed position
     */
    auto bit(size_t index) const -> bit_type
    {
        auto const the_word = word(index / word_width());
        auto const masked_bit = the_word & (static_cast<word_type>(1) << (index % word_width()));
        return static_cast<bit_type>(masked_bit > 0 ? 1 : 0);
    }

    template <size_t Count> auto bits(size_t index) const -> word_container<Count>
    {
        word_container<Count> result;
        for (auto i = 0U; i < Count; ++i)
        {
            result.set_bit(i, bit(index + i));
        }
        return result;
    }

    /*
     * Setters
     */

    void set_bit(size_t index, bool value = true)
    {
        if (index >= width())
        {

            std::string msg = gen_oob_msg(index, true);
            throw std::out_of_range(msg);
        }
        const size_t word_index = index / word_width();
        const size_t inner_word_index = index % word_width();
        word_type mask = (1ULL << inner_word_index);

        if (value)
        {
            words[word_index] |= mask;
        }
        else
        {
            words[word_index] &= ~mask;
        }
    }

    static constexpr auto word_index(size_t bit_index) -> size_t
    {
        return bit_index / word_width();
    }

    void set_bit(size_t index, bit_type value)
    {
        auto const the_word = word(word_index(index));
        auto const masked_word = the_word & ~(static_cast<word_type>(1) << (index % word_width()));
        set_word(word_index(index),
                 masked_word | (static_cast<word_type>(value & 1) << (index % word_width())));
    }

    void set_word(size_t index, word_type value)
    {
        if (index >= word_count())
        {
            std::string msg = gen_oob_msg(index, false);
            throw std::out_of_range(msg);
        }
        words[index] = value & word_mask(index);
    }

    // Sets the words to the given values, where the right-most argument corresponds to word 0.
    template <class... Args> void set_words(Args... args)
    {
        set_word_recursively<0>(args...);
    }

    /*
     * Constants
     */

    [[nodiscard]] static constexpr word_container all_ones()
    {
        word_container n;
        word_type ones = ~(static_cast<word_type>(0U));
        for (size_t i = 0; i < n.word_count(); ++i)
        {
            n.set_word(i, ones);
        }
        return n;
    }

    [[nodiscard]] static constexpr word_container all_zeroes()
    {
        return word_container{0U};
    }

    /*
     * Utility stuff
     */

    [[nodiscard]] bool is_zero() const noexcept
    {
        return std::all_of(words.begin(), words.end(), [](const word_type& w) {
            word_type zero = 0U;
            return w == zero;
        });
    }

    [[nodiscard]] explicit operator bool() const noexcept
    {
        return std::any_of(words.begin(), words.end(), [](const word_type& w) {
            word_type zero = 0U;
            return w != zero;
        });
    }

    constexpr auto begin() const noexcept
    {
        return words.begin();
    }

    constexpr auto end() const noexcept
    {
        return words.end();
    }

    constexpr auto cbegin() const noexcept
    {
        return words.cbegin();
    }

    constexpr auto cend() const noexcept
    {
        return words.cend();
    }

    constexpr auto rbegin() const noexcept
    {
        return words.rbegin();
    }

    constexpr auto rend() const noexcept
    {
        return words.rend();
    }

    constexpr auto crbegin() const noexcept
    {
        return words.bcregin();
    }

    constexpr auto crend() const noexcept
    {
        return words.crend();
    }

private:
    template <size_t index, class... Args>
    auto set_word_recursively(word_type value, Args... args) -> size_t
    {
        static_assert(index < word_count(), "too many initializer words");
        auto const count = set_word_recursively<index + 1>(args...);
        words[count - index] = value & word_mask(count - index);
        return count;
    }

    template <size_t index> auto set_word_recursively(word_type value) -> size_t
    {
        static_assert(index < word_count(), "too many initializer words");
        words[0] = value & word_mask(0);
        return index;
    }

    std::string gen_oob_msg(size_t index, bool accessed_bit = true)
    {

        std::string msg, head, foot;
        if (accessed_bit)
        {
            head = "Trying to access bit with index ";
            foot = std::to_string(width() - 1);
        }
        else
        {
            head = "Trying to access word with index ";
            foot = std::to_string(word_count() - 1);
        }

        msg += head;
        msg += std::to_string(index);
        msg += " for word_container<";
        msg += std::to_string(width());
        msg += "> with max index ";
        msg += foot;

        return msg;
    }

    std::array<word_type, word_count()> words{{0}};
};

template <size_t DestinationWidth, size_t SourceWidth>
[[nodiscard]] auto width_cast(const word_container<SourceWidth>& source)
    -> word_container<DestinationWidth>
{
    word_container<DestinationWidth> word_container;
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

template <size_t Width>
[[nodiscard]] auto operator&(const word_container<Width>& lhs, const word_container<Width>& rhs)
    -> word_container<Width>
{
    word_container<Width> logical_and;
    for (auto counter = 0U; counter < lhs.word_count(); ++counter)
    {
        logical_and.set_word(counter, lhs.word(counter) & rhs.word(counter));
    }
    return logical_and;
}

template <size_t Width>
auto operator|(const word_container<Width>& lhs, const word_container<Width>& rhs)
    -> word_container<Width>
{
    word_container<Width> logical_or;
    for (auto counter = 0U; counter < lhs.word_count(); ++counter)
    {
        logical_or.set_word(counter, lhs.word(counter) | rhs.word(counter));
    }
    return logical_or;
}

template <size_t Width>
[[nodiscard]] auto operator~(const word_container<Width>& rhs) -> word_container<Width>
{
    word_container<Width> logical_not;
    for (auto counter = 0U; counter < rhs.word_count(); ++counter)
    {
        logical_not.set_word(counter, ~rhs.word(counter));
    }
    return logical_not;
}

/**
 * @brief  Counts the number of bits set to zero before the first one appears (from MSB to LSB)
 * @tparam Width Width of the word_container
 * @param value The word to count the leading zeroes in
 * @return
 */
template <size_t Width> auto count_leading_zeroes(const word_container<Width>& value) -> size_t
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
[[nodiscard]] auto operator<<(const word_container<Width>& lhs, const size_t rhs)
    -> word_container<Width>
{
    if (rhs >= Width)
    {
        return word_container<Width>(0U);
    }
    if (rhs == 0)
    {
        return lhs;
    }
    word_container<Width> shifted;
    const auto skip_words = rhs / lhs.word_width();
    const auto shift_word_left = rhs - skip_words * lhs.word_width();
    const auto shift_word_right = lhs.word_width() - shift_word_left;

    for (auto counter = lhs.word_count(); counter > 0; --counter)
    {
        if (counter + skip_words < lhs.word_count())
        {
            typename word_container<Width>::word_type new_word;
            new_word = lhs.word(counter) << shift_word_left;
            if (shift_word_right < lhs.word_width())
            {
                new_word = new_word | (lhs.word(counter - 1) >> shift_word_right);
            }
            shifted.set_word(counter + skip_words, new_word);
        }
    }
    typename word_container<Width>::word_type new_word;
    new_word = lhs.word(0) << shift_word_left;
    shifted.set_word(skip_words, new_word);

    return shifted;
}

template <size_t Width>
auto operator>>(const word_container<Width>& lhs, const size_t rhs) -> word_container<Width>
{
    if (rhs >= Width)
    {
        return word_container<Width>(0U);
    }
    if (rhs == 0)
    {
        return lhs;
    }

    word_container<Width> shifted;
    const auto skip_words = rhs / lhs.word_width();
    const auto shift_word_right = rhs - skip_words * lhs.word_width();
    const auto shift_word_left = lhs.word_width() - shift_word_right;

    for (auto counter = skip_words; counter < lhs.word_count(); ++counter)
    {
        typename word_container<Width>::word_type new_word;
        new_word = lhs.word(counter) >> shift_word_right;
        if (shift_word_left < lhs.word_width() && counter + 1 < lhs.word_count())
        {
            new_word = new_word | (lhs.word(counter + 1) << shift_word_left);
        }
        shifted.set_word(counter - skip_words, new_word);
    }
    typename word_container<Width>::word_type new_word;
    new_word = lhs.word(lhs.word_count() - 1) >> shift_word_right;
    shifted.set_word(lhs.word_count() - skip_words - 1, new_word);

    return shifted;
}

} // namespace aarith
