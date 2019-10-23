#pragma once

#include <array>
#include <cstdint>
#include <type_traits>

template <class T> constexpr size_t size_in_words(const size_t w)
{
    const auto word_size = sizeof(T) * 8;
    return (w / word_size) + (w % word_size ? 1 : 0);
}

template <size_t Width> class uinteger
{
private:
    using integer_type = uint64_t;
    static_assert(Width > 0, "Width must be at least 1 (bit)");
    static constexpr size_t array_size = size_in_words<integer_type>(Width);

    std::array<integer_type, array_size> value;

public:
    template <class T> uinteger(T n)
    {
        static_assert(std::is_integral<T>::value, "Only integral values are supported");
        static_assert(!std::is_signed<T>::value, "Only unsigned numbers are supported");
        static_assert(sizeof(T) * 8 <= sizeof(integer_type) * 8,
                      "Only up to 64 bit integers are supported");
        static_assert(sizeof(T) * 8 <= Width, "Data type can not fit provided number");

        value[0] = n;
    }
};
