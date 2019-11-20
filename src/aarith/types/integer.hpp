#pragma once

#include "aarith/utilities/bit_operations.hpp"
#include "traits.hpp"
#include <array>
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <stdexcept>

namespace aarith {

template <size_t Width> class uinteger
{
public:
    using word_type = uint64_t;
    using bit_type = word_type;
    static_assert(Width > 0, "Width must be at least 1 (bit)");

    uinteger() = default;

    template <class T>
    explicit uinteger(T n)
        : words{{0}}
    {
        static_assert(std::is_integral<T>::value, "Only integral values are supported");
        static_assert(!std::is_signed<T>::value, "Only unsigned numbers are supported");
        static_assert(sizeof(T) * 8 <= sizeof(word_type) * 8,
                      "Only up to 64 bit integers are supported");
        // TODO understand, why this has to be commented out!
        // static_assert(sizeof(T) * 8 <= Width, "Data type can not fit provided number");

        words[0] = n;
    }

    template <class... Args> static auto from_words(Args... args) -> uinteger
    {
        uinteger uint;
        uint.set_words(args...);
        return uint;
    }

    static constexpr auto word_width() -> size_t
    {
        return sizeof(word_type) * 8;
    }

    static constexpr auto word_count() -> size_t
    {
        return size_in_words<word_type>(Width);
    }

    static constexpr auto word_mask(size_t index) -> word_type
    {
        constexpr word_type other_masks = static_cast<word_type>(-1); // all ones, e.g. no masking
        constexpr word_type last_mask =
            (width() % word_width() != 0)
                ? (static_cast<word_type>(1) << (width() % word_width())) - 1
                : static_cast<word_type>(-1);
        return (index == word_count() - 1) ? last_mask : other_masks;
    };

    static constexpr auto width() -> size_t
    {
        return Width;
    }

    auto word(size_t index) const -> word_type
    {
        return words[index];
    }

    void set_word(size_t index, word_type value)
    {
        if (index >= word_count()) {
            std::string msg;
            msg += "Trying to access word with index ";
            msg += std::to_string(index);
            msg +=" for uinteger<";
            msg += std::to_string(width());
            msg +="> with max index ";
            msg += std::to_string(word_count()-1);


            throw std::out_of_range(msg);
        }
        words[index] = value & word_mask(index);
    }

    /// Sets the words to the given values, where the rightern-most argument corresponds to word 0.
    template <class... Args> void set_words(Args... args)
    {
        set_word_recursively<0>(args...);
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

    auto bit(size_t index) const -> bit_type
    {
        auto const the_word = word(index / word_width());
        auto const masked_bit = the_word & (static_cast<word_type>(1) << (index % word_width()));
        return static_cast<bit_type>(masked_bit > 0 ? 1 : 0);
    }

    template <size_t Count> auto bits(size_t index) const -> uinteger<Count>
    {
        uinteger<Count> result;
        for (auto i = 0U; i < Count; ++i)
        {
            result.set_bit(i, bit(index + i));
        }
        return result;
    }

    auto operator<<=(const size_t shift_by) -> uinteger&
    {
        return *this = *this << shift_by;
    }

    auto operator>>=(const size_t shift_by) -> uinteger&
    {
        return *this = *this >> shift_by;
    }

    auto operator+=(const uinteger<Width> addend) -> uinteger&
    {
        return *this = *this + addend;
    }

private:
    template <size_t index, class... Args>
    auto set_word_recursively(word_type value, Args... args) -> size_t
    {
        static_assert(index < word_count(), "too many initializer words");
        auto const count = set_word_recursively<index + 1>(args...);
        words[count - index] = value;
        return count;
    }

    template <size_t index> auto set_word_recursively(word_type value) -> size_t
    {
        static_assert(index < word_count(), "too many initializer words");
        words[0] = value;
        return index;
    }

    std::array<word_type, word_count()> words{{0}};
};

template <size_t Width> class is_integral<uinteger<Width>>
{
public:
    static constexpr bool value = true;
};

template <size_t Width> class is_unsigned<uinteger<Width>>
{
public:
    static constexpr bool value = true;
};

template <size_t DestinationWidth, size_t SourceWidth>
auto width_cast(const uinteger<SourceWidth>& source) -> uinteger<DestinationWidth>
{
    uinteger<DestinationWidth> destination;
    if constexpr (DestinationWidth >= SourceWidth)
    {
        for (auto i = 0U; i < source.word_count(); ++i)
        {
            destination.set_word(i, source.word(i));
        }
    }
    else
    {
        for (auto i = 0U; i < destination.word_count(); ++i)
        {
            destination.set_word(i, source.word(i));
        }
    }
    return destination;
}

template <size_t Width> auto operator<<(const uinteger<Width>& lhs, size_t rhs) -> uinteger<Width>
{
    if (rhs >= Width)
    {
        return uinteger<Width>(0U);
    }
    if (rhs == 0)
    {
        return lhs;
    }

    uinteger<Width> shifted;
    const auto skip_words = rhs / lhs.word_width();
    const auto shift_word_left = rhs - skip_words * lhs.word_width();
    const auto shift_word_right = lhs.word_width() - shift_word_left;

    for (auto counter = lhs.word_count(); counter > 0; --counter)
    {
        if (counter + skip_words < lhs.word_count())
        {
            typename uinteger<Width>::word_type new_word;
            new_word = lhs.word(counter) << shift_word_left;
            if (shift_word_right < lhs.word_width())
            {
                new_word = new_word | (lhs.word(counter - 1) >> shift_word_right);
            }
            shifted.set_word(counter + skip_words, new_word);
        }
    }
    typename uinteger<Width>::word_type new_word;
    new_word = lhs.word(0) << shift_word_left;
    shifted.set_word(skip_words, new_word);

    return shifted;
}

template <size_t Width>
auto operator>>(const uinteger<Width>& lhs, const size_t rhs) -> uinteger<Width>
{
    if (rhs >= Width)
    {
        return uinteger<Width>(0U);
    }
    if (rhs == 0)
    {
        return lhs;
    }

    uinteger<Width> shifted;
    const auto skip_words = rhs / lhs.word_width();
    const auto shift_word_right = rhs - skip_words * lhs.word_width();
    const auto shift_word_left = lhs.word_width() - shift_word_right;

    for (auto counter = 0U; counter < lhs.word_count(); ++counter)
    {
        if (skip_words <= counter)
        {
            typename uinteger<Width>::word_type new_word;
            new_word = lhs.word(counter) >> shift_word_right;
            if (shift_word_left < lhs.word_width() && counter + 1 < lhs.word_count())
            {
                new_word = new_word | (lhs.word(counter + 1) << shift_word_left);
            }
            shifted.set_word(counter - skip_words, new_word);
        }
    }
    typename uinteger<Width>::word_type new_word;
    new_word = lhs.word(lhs.word_count() - 1) >> shift_word_right;
    shifted.set_word(lhs.word_count() - skip_words - 1, new_word);

    return shifted;
}

template <size_t Width>
auto operator&(const uinteger<Width>& lhs, const uinteger<Width>& rhs) -> uinteger<Width>
{
    uinteger<Width> logical_and;
    for (auto counter = 0U; counter < lhs.word_count(); ++counter)
    {
        logical_and.set_word(counter, lhs.word(counter) & rhs.word(counter));
    }
    return logical_and;
}

template <size_t Width>
auto operator|(const uinteger<Width>& lhs, const uinteger<Width>& rhs) -> uinteger<Width>
{
    uinteger<Width> logical_or;
    for (auto counter = 0U; counter < lhs.word_count(); ++counter)
    {
        logical_or.set_word(counter, lhs.word(counter) | rhs.word(counter));
    }
    return logical_or;
}

template <size_t Width> auto operator~(const uinteger<Width>& rhs) -> uinteger<Width>
{
    uinteger<Width> logical_not;
    for (auto counter = 0U; counter < rhs.word_count(); ++counter)
    {
        logical_not.set_word(counter, ~rhs.word(counter));
    }
    return logical_not;
}

template <size_t Width> auto abs_two_complement(const uinteger<Width>& value) -> uinteger<Width>
{
    if (value.bit(Width - 1) == 1)
    {
        const uinteger<Width> one(1U);
        return ~value + one;
    }
    return value;
}

} // namespace aarith
