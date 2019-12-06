#pragma once

#include "aarith/utilities/bit_operations.hpp"
#include "traits.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
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
        : words{{0}}
    {
        static_assert(std::is_integral<T>::value, "Only integral values are supported");
        static_assert(!std::is_signed<T>::value, "Only unsigned numbers are supported");
        static_assert(sizeof(T) * 8 <= sizeof(word_type) * 8,
                      "Only up to 64 bit integers are supported");
        //        static_assert(sizeof(T) * 8 <= Width, "Data type can not fit provided number");

        words[0] = n;
    }

    template <size_t V> uinteger(const uinteger<V>& other)
    {
        static_assert(V <= Width, "Can not create an uinteger from larger uinteger");

        for (auto i = 0U; i < other.word_count(); ++i)
        {
            set_word(i, other.word(i));
        }
    }
    template <size_t V> uinteger<Width> operator=(const uinteger<V>& other)
    {
        static_assert(V <= Width, "Can not create an uinteger from larger uinteger");

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

    void set_bit(size_t index, bool value = true)
    {
        if (index >= width())
        {
            std::string msg;
            msg += "Trying to access bit with index ";
            msg += std::to_string(index);
            msg += " for uinteger<";
            msg += std::to_string(width());
            msg += "> with max index ";
            msg += std::to_string(width() - 1);
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

    void set_word(size_t index, word_type value)
    {
        if (index >= word_count())
        {
            std::string msg;
            msg += "Trying to access word with index ";
            msg += std::to_string(index);
            msg += " for uinteger<";
            msg += std::to_string(width());
            msg += "> with max index ";
            msg += std::to_string(word_count() - 1);

            throw std::out_of_range(msg);
        }
        words[index] = value & word_mask(index);
    }

    // Sets the words to the given values, where the rightern-most argument corresponds to word 0.
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


    [[nodiscard]] static constexpr uinteger min() {
        uinteger n;
        return n;
    }

    [[nodiscard]] static constexpr uinteger zero() {
        return min();
    }

    [[nodiscard]] static constexpr uinteger one() {
        uinteger n;
        n.set_bit(0);
        return n;
    }

    [[nodiscard]] static constexpr uinteger max()
    {
        uinteger n;
        word_type ones = ~(static_cast<word_type>(0U));
        for (size_t i = 0; i < n.word_count(); ++i)
        {
            n.set_word(i, ones);
        }
        return n;
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
        words[count - index] = value & word_mask(index);
        return count;
    }

    template <size_t index> auto set_word_recursively(word_type value) -> size_t
    {
        static_assert(index < word_count(), "too many initializer words");
        words[0] = value & word_mask(index);
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
[[nodiscard]] auto operator<<(const uinteger<Width>& lhs, const size_t rhs) -> uinteger<Width>
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
auto operator|(const uinteger<Width>& lhs, const uinteger<Width>& rhs) -> uinteger<Width>
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

template <size_t Width> auto abs_two_complement(const uinteger<Width>& value) -> uinteger<Width>
{
    if (value.bit(Width - 1) == 1)
    {
        const uinteger<Width> one(1U);
        return ~value + one;
    }
    return value;
}

template <size_t Width> auto count_leading_zeroes(const uinteger<Width>& value) -> size_t
{
    for (auto i = Width; i > 0; --i)
    {
        if (value.bit(i - 1))
        {
            return i;
        }
    }
    return Width;
}

} // namespace aarith

// We are only allowed to extend std with specializations
// https://en.cppreference.com/w/cpp/language/extending_std
template <size_t W> class std::numeric_limits<aarith::uinteger<W>>
{
public:
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = false;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;
    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    static constexpr bool is_bounded = true;
    static constexpr std::float_denorm_style has_denorm = std::denorm_absent;
    static constexpr bool has_denorm_loss = false;
    static constexpr std::float_round_style round_style =
        std::round_toward_zero; // TODO do we need to takte that into account somewhere?
    static constexpr bool is_iec559 = false;
    static constexpr bool is_module = true;
    static constexpr int radix = 2;
    static constexpr int digits = W; // TODO what happens if W > max_int?
    static constexpr int digits10 = std::numeric_limits<aarith::uinteger<W>>::digits *
                                    std::log10(std::numeric_limits<aarith::uinteger<W>>::radix);

    // weird decision but https://en.cppreference.com/w/cpp/types/numeric_limits/max_digits10 says
    // so
    static constexpr int max_digits10 = 0;

    static constexpr int min_exponent = 0;
    static constexpr int min_exponent10 = 0;
    static constexpr int max_exponent = 0;
    static constexpr int max_exponent10 = 0;

    // Is this correct? At least division by zero throws an exception, so this value is, most
    // likely, correct.
    static constexpr bool traps = true;

    static constexpr bool tinyness_before = false;

    static constexpr aarith::uinteger<W> min() noexcept
    {
        return aarith::uinteger<W>{0U};
    }

    static constexpr aarith::uinteger<W> lowest() noexcept
    {
        return aarith::uinteger<W>{0U};
    }

    static constexpr aarith::uinteger<W> max() noexcept
    {
        return aarith::uinteger<W>::max();
    }


};
