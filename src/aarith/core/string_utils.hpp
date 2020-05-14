#pragma once

#include <string>

namespace aarith {

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

/// Remove all leading zeroes from a string that represents a number.
inline auto remove_leading_zeroes(const std::string& number, bool can_be_empty = false)
    -> std::string
{
    for (auto i = 0U; i < number.length(); ++i)
    {
        if (number[i] != '0')
        {
            return number.substr(i);
        }
    }
    return can_be_empty ? "" : "0";
}

/// Group digits within the given string that represents a number using the given separator.
/// For example, `group_digits("1000", 3, '.')` yields `"1.000"`.
inline auto group_digits(const std::string& number, size_t group_size, char separator = ' ')
    -> std::string
{
    if (group_size == 0)
    {
        return number;
    }

    std::string result;
    auto const partial_group_size = number.length() % group_size;
    if (partial_group_size != 0)
    {
        result += number.substr(0, partial_group_size);
    }
    for (auto i = partial_group_size; i < number.length(); i += group_size)
    {
        if (i > 0)
        {
            result += separator;
        }
        result += number.substr(i, group_size);
    }
    return result;
}

/// Convert the given uinteger value into a binary string representation
template <size_t Width, typename WordType>
auto to_binary(const word_array<Width, WordType>& value) -> std::string
{
    std::string result;
    for (auto i = Width; i > 0; --i)
    {
        result += value.bit(i - 1) ? '1' : '0';
    }
    return result;
}

} // namespace aarith