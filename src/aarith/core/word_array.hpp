#pragma once

#include <aarith/core/traits.hpp>

#include <algorithm>
#include <array>
#include <climits>
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

    static_assert(::aarith::is_unsigned_int<WordType>,
                  "Only unsigned integers can be used as word types");

    /*
     * Constructors etc.
     */

    constexpr word_array() = default;

    constexpr word_array(WordType w)
    {
        this->words[0] = w & word_mask(0);
    }

    template <class... Args> constexpr word_array(WordType w, Args... args)
    {
        set_words(w, args...);
    }

    template <class... Args> static constexpr auto from_words(Args... args) -> word_array
    {
        word_array wc;
        wc.set_words(args...);
        return wc;
    }

    template <size_t V, typename T> constexpr word_array(const word_array<V, T>& other)
    {
        static_assert(V <= Width, "Can not create a word_array from larger container");
        for (auto i = 0U; i < other.word_count(); ++i)
        {
            set_word(i, other.word(i));
        }
    }

    template <size_t V, typename T>
    constexpr word_array<Width, T> operator=(const word_array<V, T>& other)
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

        if constexpr (word_array<Width, WordType>::word_count() >
                      word_array<V, WordType>::word_count())
        {
            for (size_t i = other.word_count(); i < this->word_count(); ++i)
            {
                set_word(i, 0U);
            }
        }

        return *this;
    }

    template <size_t V, typename T> void set_bits(const word_array<V, T>& other)
    {

        static_assert(V <= Width, "Can not create a word_array from larger container");

        for (size_t i = 0U; i < other.word_count(); ++i)
        {
            set_word(i, other.word(i));
        }

        if constexpr (word_array<Width, WordType>::word_count() >
                      word_array<V, WordType>::word_count())
        {
            for (size_t i = other.word_count(); i < this->word_count(); ++i)
            {
                set_word(i, 0U);
            }
        }
    }

    /**
     *
     * @tparam V
     * @tparam T
     * @param end
     * @param other
     */
    template <size_t V, typename T> void set_bits(size_t end, const word_array<V, T>& other)
    {

        static_assert(V <= Width, "Can not create a word_array from larger container");

        for (size_t i = 0; i < other.width(); i++)
        {
            const size_t index = i + end;
            set_bit(index, other.bit(i));
        }
    }

    /*
     * Getters
     */

    [[nodiscard]] static constexpr auto word_width() noexcept -> size_t
    {
        return sizeof(word_type) * CHAR_BIT;
    }

    [[nodiscard]] static constexpr auto word_count() noexcept -> size_t
    {
        const auto word_size = sizeof(word_type) * 8;
        return (Width / word_size) + (Width % word_size ? 1 : 0);
    }

    [[nodiscard]] static constexpr word_type word_mask(size_t index) noexcept
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

    [[nodiscard]] constexpr auto word(size_t index) const -> word_type
    {
        return words[index];
    }

    /**
     * @brief Returns ths most significant bit.
     *
     * The most significant bit is the Width's one (i.e. the one you can get via bit(Width-1)). This
     * method is simply there for convenience.
     */
    [[nodiscard]] constexpr auto msb() const -> bit_type
    {
        return bit(Width - 1);
    }

    /**
     * @brief Sets the value of the most significant bit (MSB)
     * @param b The value the MSB is set to
     */
    void constexpr set_msb(const bool b)
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
    auto constexpr bit(size_t index) const -> bit_type
    {
        auto const the_word = word(index / word_width());
        auto const masked_bit = the_word & (static_cast<word_type>(1) << (index % word_width()));
        return static_cast<bit_type>(masked_bit > 0 ? 1 : 0);
    }

    template <size_t Count> constexpr auto bits(size_t index) const -> word_array<Count, WordType>
    {
        word_array<Count, WordType> result;
        for (auto i = 0U; i < Count; ++i)
        {
            result.set_bit(i, bit(index + i));
        }
        return result;
    }

    /*
     * Setters
     */

    void constexpr set_bit(size_t index, bool value = true)
    {
        //        if (index >= width())
        //        {
        //
        //            std::string msg = gen_oob_msg(index, true);
        //            throw std::out_of_range(msg);
        //        }
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

    void constexpr set_word(const size_t index, const word_type value)
    {
        //        if (index >= word_count())
        //        {
        //            std::string msg = gen_oob_msg(index, false);
        //            throw std::out_of_range(msg);
        //        }
        const auto mask = word_mask(index);
        words[index] = value & mask;
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
     * @brief Assigns the specified value to all elements in the container.
     * @param value the value to assign to the elements
     */
    constexpr void fill(const word_type& value)
    {
        for (size_t i = 0; i < word_count(); ++i)
        {
            set_word(i, value);
        }
    }

    // Sets the words to the given values, where the right-most argument corresponds to word 0.
    template <class... Args> void constexpr set_words(Args... args)
    {
        set_word_recursively<0>(args...);
    }

    /*
     * Constants
     */

    /**
     * @brief Creates a word array consisting of ones only
     * @return <111.....11>
     */
    [[nodiscard]] static constexpr word_array<Width, WordType> all_ones()
    {
        word_array<Width, WordType> n;
        word_type ones = ~(static_cast<word_type>(0U));
        for (size_t i = 0; i < word_array::word_count(); ++i)
        {
            n.set_word(i, ones);
        }
        return n;
    }

    /**
     * @brief Creates a word array with only the most significant bit being one
     * @return <10000....00>
     */
    [[nodiscard]] static constexpr word_array<Width, WordType> msb_one()
    {
        word_array<Width, WordType> n;
        n.set_msb(true);
        return n;
    }

    /**
     * @brief Creates a word array consisting of zeroes only
     * @return <000...00>
     */
    [[nodiscard]] static constexpr word_array<Width, WordType> all_zeroes()
    {
        return word_array{0U};
    }

    /*
     * Utility stuff
     */

    /**
     * @brief Tests if all bits are zero
     * @return True iff all bits are zero
     */
    [[nodiscard]] bool constexpr is_zero() const noexcept
    {

        constexpr word_type zero{0U};
        for (size_t i = 0; i < word_count(); ++i)
        {
            if (word(i) != zero)
            {
                return false;
            }
        }
        return true;
    }

    [[nodiscard]] explicit constexpr operator bool() const noexcept
    {
        return !is_zero();
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
        return words.crbegin();
    }

    constexpr auto crend() const noexcept
    {
        return words.crend();
    }

private:
    template <size_t index, class... Args>
    auto constexpr set_word_recursively(word_type value, Args... args) -> size_t
    {
        static_assert(index < word_count(), "too many initializer words");
        auto const count = set_word_recursively<index + 1>(args...);
        words[count - index] = value & word_mask(count - index);
        return count;
    }

    template <size_t index> auto constexpr set_word_recursively(word_type value) -> size_t
    {
        static_assert(index < word_count(), "too many initializer words");
        words[0] = value & word_mask(0);
        return index;
    }

    std::string gen_oob_msg(size_t index, bool accessed_bit = true)
    {

        std::string msg;
        std::string head;
        std::string foot;
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

template <size_t Width, typename T> class is_word_array<word_array<Width, T>>
{
public:
    static constexpr bool value = true;
};

} // namespace aarith
