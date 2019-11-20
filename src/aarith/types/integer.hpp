#pragma once

#include "aarith/utilities/bit_operations.hpp"
#include "traits.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <type_traits>

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
        : words{0}
    {
        static_assert(std::is_integral<T>::value, "Only integral values are supported");
        static_assert(!std::is_signed<T>::value, "Only unsigned numbers are supported");
        static_assert(sizeof(T) * 8 <= sizeof(word_type) * 8,
                      "Only up to 64 bit integers are supported");
        static_assert(sizeof(T) * 8 <= Width, "Data type can not fit provided number");

        words[0] = n;
    }

    template <class... Args> static auto from_words(Args... args) -> uinteger
    {
        uinteger uint;
        uint.set_words(args...);
        return uint;
    }

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

    void set_word(size_t index, word_type value)
    {
        words[index] = value & word_mask(index);
    }

    // Sets the words to the given values, where the rightern-most argument corresponds to word 0.
    template <class... Args> void set_words(Args... args)
    {
        set_word_recursively<0>(args...);
    }

    auto bit(size_t index) const -> bit_type
    {
        auto const the_word = word(index / word_width());
        auto const masked_bit = the_word & (static_cast<word_type>(1) << (index % word_width()));
        return static_cast<bit_type>(masked_bit > 0 ? 1 : 0);
    }

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

    constexpr auto  crbegin() const noexcept
    {
        return words.bcregin();
    }

    constexpr auto crend() const noexcept
    {
        return words.crend();
    }

    auto operator<<=(const size_t shift_by) -> uinteger&
    {
        return *this = *this << shift_by;
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

    std::array<word_type, word_count()> words{0};
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
[[nodiscard]] auto width_cast(const uinteger<SourceWidth>& source) -> uinteger<DestinationWidth>
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

template <size_t Width>
auto operator<<(std::ostream& out, const uinteger<Width>& value) -> std::ostream&
{
    for (auto i = Width; i > 0; --i)
    {
        out << value.bit(i - 1);
    }
    return out;
}

template <size_t Width>
[[nodiscard]] auto operator<<(const uinteger<Width>& lhs, const uint32_t rhs) -> uinteger<Width>
{
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
            new_word = new_word | (lhs.word(counter - 1) >> shift_word_right);
            shifted.set_word(counter + skip_words, new_word);
        }
    }
    typename uinteger<Width>::word_type new_word;
    new_word = lhs.word(0) << shift_word_left;
    shifted.set_word(skip_words, new_word);

    return shifted;
}

template <size_t Width>
[[nodiscard]] auto operator&(const uinteger<Width>& lhs, const uinteger<Width>& rhs)
    -> uinteger<Width>
{
    uinteger<Width> logical_and;
    for (auto counter = 0U; counter < lhs.word_count(); ++counter)
    {
        logical_and.set_word(counter, lhs.word(counter) & rhs.word(counter));
    }
    return logical_and;
}

template <size_t Width>
[[nodiscard]] auto operator|(const uinteger<Width>& lhs, const uinteger<Width>& rhs)
    -> uinteger<Width>
{
    uinteger<Width> logical_or;
    for (auto counter = 0U; counter < lhs.word_count(); ++counter)
    {
        logical_or.set_word(counter, lhs.word(counter) | rhs.word(counter));
    }
    return logical_or;
}

template <size_t Width>[[nodiscard]] auto operator~(const uinteger<Width>& rhs) -> uinteger<Width>
{
    uinteger<Width> logical_not;
    for (auto counter = 0U; counter < rhs.word_count(); ++counter)
    {
        logical_not.set_word(counter, ~rhs.word(counter));
    }
    return logical_not;
}

} // namespace aarith
