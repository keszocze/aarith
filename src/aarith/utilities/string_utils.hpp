#pragma once

#include "aarith/operations/comparisons.hpp"
#include "aarith/operations/exact_operations.hpp"
#include "aarith/types/integer.hpp"
#include "aarith/utilities/bit_operations.hpp"
#include <iostream>
#include <string>

namespace aarith {

template <size_t Width> auto to_hex(const uinteger<Width>& value) -> std::string
{
    static constexpr char digits[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    static constexpr auto hex_digit_count = rounded_integer_division(Width, 4);

    std::string result;
    for (auto i = hex_digit_count; i > 0; --i)
    {
        auto const bits = value.bits<4>((i - 1) * 4);
        result += digits[bits.word(0)];
    }
    return result;
}

template <size_t Width> auto to_octal(const uinteger<Width>& value) -> std::string
{
    static constexpr char digits[16] = {'0', '1', '2', '3', '4', '5', '6', '7'};
    static constexpr auto hex_digit_count = rounded_integer_division(Width, 3);

    std::string result;
    for (auto i = hex_digit_count; i > 0; --i)
    {
        auto const bits = value.bits<3>((i - 1) * 3);
        result += digits[bits.word(0)];
    }
    return result;
}

template <size_t Width>
auto to_bcd(const uinteger<Width>& num) -> uinteger<number_of_decimal_digits(Width) * 4>
{
    using namespace aarith::exact_operators;

    constexpr auto bcd_digits = number_of_decimal_digits(Width);
    constexpr auto bcd_width = bcd_digits * 4;

    uinteger<bcd_width> bcd;
    for (auto i = Width; i > 0; --i)
    {
        uinteger<bcd_width> three(3U);
        uinteger<bcd_width> four(4U);
        uinteger<bcd_width> mask(15U);

        for (auto c_bcd = 0U; c_bcd < bcd_digits; ++c_bcd)
        {
            if ((bcd & mask) > four)
            {
                bcd = bcd + three;
            }

            three <<= 4;
            four <<= 4;
            mask <<= 4;
        }

        bcd <<= 1;
        const auto new_word = bcd.word(0) | (num.bit(i - 1));
        bcd.set_word(0, new_word);
    }
    return bcd;
}

template <size_t Width> auto to_binary(const uinteger<Width>& value) -> std::string
{
    std::string result;
    for (auto i = Width; i > 0; --i)
    {
        result += value.bit(i - 1);
    }
    return result;
}

inline auto remove_leading_zeroes(const std::string& number) -> std::string
{
    for (auto i = 0U; i < number.length(); ++i)
    {
        if (number[i] != '0')
        {
            return number.substr(i);
        }
    }
    return "0";
}

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
        result += separator;
    }
    for (auto i = partial_group_size; i <= number.length(); i += group_size)
    {
        result += number.substr(number.length() - i - group_size, group_size);
        result += separator;
    }
    return result;
}

template <size_t Width> auto to_decimal(const uinteger<Width>& value) -> std::string
{
    return remove_leading_zeroes(to_hex(to_bcd(value)));
}

template <size_t Width>
auto operator<<(std::ostream& out, const uinteger<Width>& value) -> std::ostream&
{
    if (out.flags() & std::ios::hex)
    {
        out << to_hex(value);
    }
    else if (out.flags() & std::ios::oct)
    {
        out << to_octal(value);
    }
    else
    {
        out << to_decimal(value);
    }
    return out;
}

} // namespace aarith
