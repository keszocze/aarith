#pragma once

#include <array>
#include <cstdint>
#include <iostream>
#include <type_traits>

namespace aarith {

template <class T> constexpr size_t size_in_words(const size_t w)
{
    const auto word_size = sizeof(T) * 8;
    return (w / word_size) + (w % word_size ? 1 : 0);
}

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
        auto const last_mask = (static_cast<word_type>(1) << (Width % word_width())) - 1;
        auto const other_masks = static_cast<word_type>(-1);
        return index == word_count() - 1 ? last_mask : other_masks;
    };

    auto word(size_t index) const -> word_type
    {
        return words[index];
    }

    void set_word(size_t index, word_type value)
    {
        words[index] = value & word_mask(index);
    }

    auto bit(size_t index) const -> bit_type
    {
        auto const the_word = word(index / word_width());
        auto const masked_bit = the_word & (static_cast<word_type>(1) << (index % word_width()));
        return static_cast<bit_type>(masked_bit > 0 ? 1 : 0);
    }

private:
    std::array<word_type, word_count()> words{0};
};

template <class Type> class is_integral
{
public:
    static constexpr bool value = false;
};

template <size_t Width> class is_integral<uinteger<Width>>
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
        for (auto i = 0U; i < SourceWidth; ++i)
        {
            destination.set_word(i, source.word(i));
        }
    }
    else
    {
        for (auto i = 0U; i < DestinationWidth; ++i)
        {
            destination.set_word(i, source.word(i));
        }
    }
}

template <size_t Width>
auto operator<<(const uinteger<Width>& value, std::ostream& out) -> std::ostream&
{
    for (auto i = Width; i > 0; --i)
    {
        std::cout << value.bit(i - 1);
    }
    return out;
}

} // namespace aarith