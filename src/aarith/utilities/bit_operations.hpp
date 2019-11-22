#pragma once

#include <cstddef>
#include <cstdint>
#include <tuple>

namespace aarith {

template <class T>[[nodiscard]] constexpr size_t size_in_words(const size_t w)
{
    const auto word_size = sizeof(T) * 8;
    return (w / word_size) + (w % word_size ? 1 : 0);
}

[[nodiscard]] constexpr std::pair<uint32_t, uint32_t> split(const uint64_t n)
{
    uint64_t bitmask{(1ULL << 32) - 1};

    return {static_cast<uint32_t>(n >> 32), static_cast<uint32_t>(n & bitmask)};
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
