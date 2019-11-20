#pragma once

#include <cstddef>

namespace aarith {

template <class T> constexpr size_t size_in_words(const size_t w)
{
    const auto word_size = sizeof(T) * 8;
    return (w / word_size) + (w % word_size ? 1 : 0);
}

inline constexpr auto number_of_decimal_digits(size_t bits) -> size_t
{
    // When converted to decimal, an n-bit binary numeral will have at most k*n decimal digits,
    // rounded up, where k = log_10 2 ~ 0.301.
    return (bits * 301) / 1000 + ((bits * 301) % 1000 == 0 ? 0 : 1);
}

template <class T, class U>
constexpr auto rounded_integer_division(T dividend, U divisor) -> decltype(T{} / U{})
{
    return dividend / divisor + (dividend % divisor ? 1 : 0);
}

} // namespace aarith
