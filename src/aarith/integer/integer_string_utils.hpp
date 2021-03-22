#pragma once

#include <aarith/core/core_string_utils.hpp>

namespace aarith {

/// Convert the given uinteger value into a string representation with base 2^N.
template <size_t N, size_t Width, typename WordType>
auto to_base_2n(const uinteger<Width, WordType>& value)
{
    static_assert(N <= 4);
    static constexpr std::array<char,16> digits{'0', '1', '2', '3', '4', '5', '6', '7',
                                        '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
    static constexpr auto digit_count = rounded_integer_division(Width, N);

    std::string result;
    for (auto i = digit_count; i > 0; --i)
    {
        auto const bits = value.template bits<N>((i - 1) * N);
        result += digits[bits.word(0)];
    }
    return result;
}

/// Convert the given uinteger value into a hexadecimal string representation.
template <size_t Width, typename WordType>
auto to_hex(const uinteger<Width, WordType>& value) -> std::string
{
    return to_base_2n<4>(value);
}

/// Convert the given uinteger value into a octal string representation.
template <size_t Width, typename WordType>
auto to_octal(const uinteger<Width, WordType>& value) -> std::string
{
    return to_base_2n<3>(value);
}

/// Convert the given integer value into a hexadecimal string representation.
template <size_t Width, typename WordType>
auto to_hex(const integer<Width, WordType>& value) -> std::string
{

    std::string res;
    if (value.is_negative())
    {
        res += "-";
    }
    const auto absval = expanding_abs(value);
    res += to_base_2n<4>(absval);
    return res;
}

/// Convert the given integer value into a octal string representation.
template <size_t Width, typename WordType>
auto to_octal(const integer<Width, WordType>& value) -> std::string
{

    std::string res;
    if (value.is_negative())
    {
        res += "-";
    }
    const auto absval = expanding_abs(value);
    res += to_base_2n<3>(absval);
    return res;
}

/// Convert the given uinteger value into a binary coded decimal (BCD), represented within a new
/// uinteger.
template <size_t Width, typename WordType>
auto to_bcd(const uinteger<Width, WordType>& num)
    -> uinteger<number_of_decimal_digits(Width) * 4, WordType>
{
    constexpr auto bcd_digits = number_of_decimal_digits(Width);
    constexpr auto bcd_width = bcd_digits * 4;

    uinteger<bcd_width, WordType> bcd;
    for (auto i = Width; i > 0; --i)
    {
        uinteger<bcd_width, WordType> three(3U);
        uinteger<bcd_width, WordType> four(4U);
        uinteger<bcd_width, WordType> mask(15U); // NOLINT

        for (auto c_bcd = 0U; c_bcd < bcd_digits; ++c_bcd)
        {
            auto masked = bcd & mask;
            if (masked > four)
            {
                bcd = add(bcd, three);
            }

            three = (three << 4);
            four = (four << 4);
            mask = (mask << 4);
        }

        bcd = (bcd << 1);
        const auto new_word = bcd.word(0) | (num.bit(i - 1));
        bcd.set_word(0, new_word);
    }
    return bcd;
}

/// Convert the given uinteger value into a decimal string representation.
template <size_t Width, typename WordType>
auto to_decimal(const uinteger<Width, WordType>& value) -> std::string
{
    return remove_leading_zeroes(to_hex(to_bcd(value)));
}

/// Convert the given integer value into a decimal string representation.
template <size_t Width, typename WordType>
auto to_decimal(const integer<Width, WordType>& value) -> std::string
{

    std::string res;
    if (value.is_negative())
    {
        res += "-";
    }
    const auto absval = expanding_abs(value);
    res += remove_leading_zeroes(to_hex(to_bcd(absval)));
    return res;
}

template <typename Integer, typename = std::enable_if_t<is_integral_v<Integer>>>
auto operator<<(std::ostream& out, const Integer& value) -> std::ostream&
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