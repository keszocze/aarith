#pragma once

#include "aarith/operations/float_comparisons.hpp"
#include "aarith/operations/float_operations.hpp"
#include "aarith/operations/sinteger_operations.hpp"
#include "aarith/operations/uinteger_comparisons.hpp"
#include "aarith/operations/uinteger_operations.hpp"
#include "aarith/types/normfloat.hpp"
#include "aarith/types/sinteger.hpp"
#include "aarith/types/uinteger.hpp"
#include "aarith/utilities/bit_operations.hpp"
#include <iostream>
#include <sstream>
#include <string>

namespace aarith {

/// Convert the given uinteger value into a string representation with base 2^N.
template <size_t N, size_t Width> auto to_base_2n(const uinteger<Width>& value)
{
    static_assert(N <= 4);
    static constexpr char digits[16] = {'0', '1', '2', '3', '4', '5', '6', '7',
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
template <size_t Width> auto to_hex(const uinteger<Width>& value) -> std::string
{
    return to_base_2n<4>(value);
}

/// Convert the given uinteger value into a octal string representation.
template <size_t Width> auto to_octal(const uinteger<Width>& value) -> std::string
{
    return to_base_2n<3>(value);
}

/// Convert the given uinteger value into a binary coded decimal (BCD), represented within a new
/// uinteger.
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

/// Convert the given uinteger value into a binary string representation.
template <size_t Width> auto to_binary(const word_container<Width>& value) -> std::string
{
    std::string result;
    for (auto i = Width; i > 0; --i)
    {
        result += value.bit(i - 1) ? '1' : '0';
    }
    return result;
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

/// Convert the given uinteger value into a decimal string representation.
template <size_t Width> auto to_decimal(const uinteger<Width>& value) -> std::string
{
    return remove_leading_zeroes(to_hex(to_bcd(value)));
}

/// Convert the given normfloat to a string with digits to the power of 2
template <size_t N, size_t E, size_t M> auto to_base_2n(const normfloat<E, M> nf) -> std::string
{
    std::string str;

    str = std::to_string(nf.get_sign()) + " " + to_base_2n<N>(nf.get_exponent()) + " " +
          to_base_2n<N>(nf.get_mantissa());
    return str;
}

/// Convert the given normfloat value into a hexadecimal string representation.
template <size_t E, size_t M> auto to_hex(const normfloat<E, M>& value) -> std::string
{
    return to_base_2n<4>(value);
}

/// Convert the given normfloat value into a octal string representation.
template <size_t E, size_t M> auto to_octal(const normfloat<E, M>& value) -> std::string
{
    return to_base_2n<3>(value);
}

/// Convert the given normfloat value into a binary string representation.
template <size_t E, size_t M> auto to_binary(const normfloat<E, M>& value) -> std::string
{
    return to_base_2n<1>(value);
}

/// Convert the given normfloat to a representation that can be posted to a calculator
template <size_t E, size_t M> auto to_compute_string(const normfloat<E, M> nf) -> std::string
{
    std::stringstream stream("");
    stream << "(-1)^" << nf.get_sign() << " * 2^(";
    auto first = true;

    auto ones = 0U;
    auto zeroes = 0U;
    for (auto counter = E + M; counter > M; --counter)
    {
        if (nf.bit(counter - 1) == 0)
        {
            zeroes++;
        }
        else
        {
            ones++;
        }
    }

    if (ones < zeroes)
    {
        for (auto counter = E + M; counter > M; --counter)
        {
            stream << ((nf.bit(counter - 1) == 1 && !first) ? " + " : "")
                   << ((nf.bit(counter - 1) == 1) ? (("2^") + std::to_string(counter - M - 1))
                                                  : "");
            first &= nf.bit(counter - 1) == 0;
        }
        stream << " - (2^" << (E - 1) << " - 1)";
    }
    else
    {
        for (auto counter = E + M; counter > M; --counter)
        {
            stream << ((nf.bit(counter - 1) == 0 && !first) ? " - " : "")
                   << ((nf.bit(counter - 1) == 1)
                           ? ((counter == E + M) ? (("2^") + std::to_string(counter - M - 1)) : "")
                           : ((counter == E + M) ? ("0")
                                                 : ("2^" + std::to_string(counter - M - 1))));
            first = false;
        }
    }

    stream << ") * (";
    first = true;
    for (auto counter = M; counter > 0; --counter)
    {
        if (nf.bit(counter - 1) == 1)
        {
            stream << (first ? "" : " + ") << "2^(" << std::to_string(static_cast<int>(counter - M))
                   << ")";
            first = false;
        }
    }
    stream << ")";

    return stream.str();
}

/// Convert the given normfloat to a scientific string representation
template <size_t E, size_t M> auto to_sci_string(const normfloat<E, M> nf) -> std::string
{
    uinteger<M> fl_mantissa = nf.get_mantissa();
    uinteger<24> flc_mantissa;
    if (M >= 24)
    {
        auto shift_mantissa = M - 24;
        fl_mantissa = fl_mantissa >> shift_mantissa;
        flc_mantissa = width_cast<24, M>(fl_mantissa);
    }
    else
    {
        auto shift_mantissa = 24 - M;
        flc_mantissa = width_cast<24, M>(fl_mantissa);
        flc_mantissa <<= shift_mantissa;
    }
    uint32_t ui_mantissa = static_cast<uint32_t>(flc_mantissa.word(0)) & 0x7fffff | 0x3f800000;
    float* mantissa = reinterpret_cast<float*>(&ui_mantissa);

    auto const exponent = sub(nf.get_exponent(), nf.get_bias());

    const sinteger<E> s_exponent(exponent);
    auto const s_abs_exponent = abs(s_exponent);
    const uinteger<E> abs_exponent(abs_exponent);

    std::stringstream str;
    str << ((nf.get_sign() == 1) ? "-" : "") << *mantissa << "E"
        << (exponent.bit(E - 1) == 1 ? "-" : "") << to_decimal(abs_exponent);

    return str.str();
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

template <size_t E, size_t M>
auto operator<<(std::ostream& out, const normfloat<E, M>& value) -> std::ostream&
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
        out << to_sci_string(value);
    }
    return out;
}

} // namespace aarith
