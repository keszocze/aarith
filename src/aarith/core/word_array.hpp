#pragma once

#include "traits.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace aarith {

template <size_t Width, class WordType = uint64_t> class word_array
{
public:
    using word_type = WordType;
    using bit_type = WordType;
    static_assert(Width > 0, " Width must be at least 1 (bit)");

    /*
     * Constructors etc.
     */

    word_array() = default;

    word_array(WordType w)
    {
        this->words[0] = w & word_mask(0);
    }

    template <class... Args> word_array(WordType w, Args... args)
    {
        set_words(w, args...);
    }

    template <class... Args> static auto from_words(Args... args) -> word_array
    {
        word_array wc;
        wc.set_words(args...);
        return wc;
    }

    template <size_t V> word_array(const word_array<V>& other)
    {
        static_assert(V <= Width, "Can not create a word_array from larger container");

        for (auto i = 0U; i < other.word_count(); ++i)
        {
            set_word(i, other.word(i));
        }
    }
    template <size_t V> word_array<Width> operator=(const word_array<V>& other)
    {
        static_assert(V <= Width, "Can not create a word_array from larger container");

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

        if constexpr (word_array<Width, WordType>::word_count() > word_array<V>::word_count())
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
        const auto word_size = sizeof(word_type) * 8;
        return (Width / word_size) + (Width % word_size ? 1 : 0);
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

    template <size_t Count> auto bits(size_t index) const -> word_array<Count>
    {
        word_array<Count> result;
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

    void set_word(const size_t index, const word_type value)
    {
        if (index >= word_count())
        {
            std::string msg = gen_oob_msg(index, false);
            throw std::out_of_range(msg);
        }
        words[index] = value & word_mask(index);
    }

    /**
     * @brief Returns a const reference to the element at specified location pos, with bounds
     checking.
     *
     * If pos is not within the range of the container, an exception of type std::out_of_range is
     thrown.
     *
     * @param pos position of the element to return

     * @return Const reference to the requested element.
     */
    auto at(size_t pos) const
    {
        return words.at(pos);
    }

    /**
     * @brief Returns a reference to the element at specified location pos. No bounds checking is
     * performed.
     * @param pos position of the element to return
     * @return Const reference to the requested element.
     */
    auto operator[](size_t pos) const
    {
        return words[pos];
    }

    /**
     * @brief Checks if the container has no elements
     *
     * @return false
     */
    [[nodiscard]] constexpr bool empty() const noexcept
    {
        return false;
    }

    /**
     * @brief Returns a const reference to the first element in the container.
     * @return Const reference to the first element
     */
    auto front() const
    {
        return words.front();
    }

    /**
     *
     * @return Const reference to the last element.
     *
     **/
    auto back() const
    {
        return words.back();
    }

    /**
     * @brief Returns the number of words in the array
     *
     * @note The most significant word might be be used entirely. The most significant bits might be
     * masked away.
     *
     * @return The number of words used store the number
     */
    [[nodiscard]] constexpr size_t size() const noexcept
    {
        return word_count();
    }

    /**
     * @brief Assigns the given value value to all elements in the container.
     * @param valuethe value to assign to the elements
     */
    constexpr void fill(const word_type& value)
    {
        for (size_t i = 0; i < word_count(); ++i)
        {
            set_word(i, value);
        }
    }

    /**
     * @brief Returns the maxmimum number of elements that can be stored in this word_array
     *
     * This number is fixed.
     *
     * @return The maximum number of elements
     */
    constexpr size_t max_size() const noexcept
    {
        return word_count();
    }

    // Sets the words to the given values, where the right-most argument corresponds to word 0.
    template <class... Args> void set_words(Args... args)
    {
        set_word_recursively<0>(args...);
    }

    /*
     * Constants
     */

    [[nodiscard]] static constexpr word_array all_ones()
    {
        word_array n;
        word_type ones = ~(static_cast<word_type>(0U));
        for (size_t i = 0; i < n.word_count(); ++i)
        {
            n.set_word(i, ones);
        }
        return n;
    }

    [[nodiscard]] static constexpr word_array all_zeroes()
    {
        return word_array{0U};
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
        msg += " for word_array<";
        msg += std::to_string(width());
        msg += "> with max index ";
        msg += foot;

        return msg;
    }

    std::array<word_type, word_count()> words{{0}};
};

template <size_t DestinationWidth, size_t SourceWidth>
[[nodiscard]] auto width_cast(const word_array<SourceWidth>& source) -> word_array<DestinationWidth>
{
    word_array<DestinationWidth> word_container;
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

} // namespace aarith
