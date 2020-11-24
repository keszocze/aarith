#pragma once

#include <aarith/core/string_utils.hpp>

#include <sstream>

namespace aarith {

/// Convert the given normalized_float to a string with digits to the power of 2
template <size_t N, size_t E, size_t M, typename WordType>
auto to_base_2n(const normalized_float<E, M, WordType> nf, const bool show_hidden_bit = false)
    -> std::string
{
    std::string str;

    if (show_hidden_bit)
    {
        str = std::to_string(nf.get_sign()) + " " + to_base_2n<N>(nf.get_exponent()) + " " +
              to_base_2n<N>(nf.get_full_mantissa());
    }
    else
    {
        str = std::to_string(nf.get_sign()) + " " + to_base_2n<N>(nf.get_exponent()) + " " +
              to_base_2n<N>(nf.get_mantissa());
    }
    return str;
}

/// Convert the given normalized_float value into a hexadecimal string representation.
template <size_t E, size_t M, typename WordType>
auto to_hex(const normalized_float<E, M, WordType>& value, const bool show_hidden_bit = false)
    -> std::string
{
    return to_base_2n<4>(value, show_hidden_bit);
}

/// Convert the given normalized_float value into a octal string representation.
template <size_t E, size_t M, typename WordType>
auto to_octal(const normalized_float<E, M, WordType>& value, const bool show_hidden_bit = false)
    -> std::string
{
    return to_base_2n<3>(value, show_hidden_bit);
}

/// Convert the given normalized_float value into a binary string representation.
template <size_t E, size_t M, typename WordType>
auto to_binary(const normalized_float<E, M, WordType>& value, const bool show_hidden_bit = false)
    -> std::string
{
    return to_base_2n<1>(value, show_hidden_bit);
}

template <size_t E, size_t M, typename WordType>
auto to_compute_string(const normalized_float<E, M, WordType> nf) -> std::string
{

    if (nf.is_nan())
    {
        return "NaN";
    }

    std::stringstream stream("");

    const bool neg = nf.is_negative();

    // print the sign part
    stream << (neg ? "-" : "");

    if (nf.is_zero())
    {
        stream << "0";
        return stream.str();
    }

    stream << (neg ? "(" : ""); // might need to add enclosing paranthesis

    // print exponent
    if (nf.is_normalized())
    {
        stream << "2^(" << nf.unbiased_exponent() << ")";
    }
    else
    {
        stream << "2^(" << nf.denorm_exponent() << ")";
    }

    // print the mantissa part, here, actual computations are necessary
    stream << " * (" << (nf.is_normalized() ? "1" : "0");

    const auto mantissa = nf.get_mantissa();

    integer<65> iM{M};
    for (int64_t i = M; i > 0; --i)
    {
        auto curr_bit = mantissa.bit(i);
        if (curr_bit == 1)
        {
            integer<65> I{i};
            stream << " + 2^(" << sub(I, iM) << ")";
        }
    }
    stream << ")";

    stream << (neg ? ")" : ""); // might need to add enclosing paranthesis
    return stream.str();
}

/// Convert the given normalized_float to a scientific string representation
template <size_t E, size_t M, typename WordType>
auto to_sci_string(const normalized_float<E, M, WordType> nf) -> std::string
{
    auto fl_mantissa = nf.get_mantissa();
    uinteger<23, WordType> flc_mantissa;
    if constexpr (M >= 23)
    {
        auto shift_mantissa = M - 23;
        fl_mantissa = fl_mantissa >> shift_mantissa;
        flc_mantissa = width_cast<23, M>(fl_mantissa);
    }
    else
    {
        auto shift_mantissa = 23 - M;
        flc_mantissa = width_cast<23, M>(fl_mantissa);
        flc_mantissa = (flc_mantissa << shift_mantissa);
    }
    uint32_t ui_mantissa = (static_cast<uint32_t>(flc_mantissa.word(0)) & 0x7fffff) | 0x3f800000;
    float* mantissa = reinterpret_cast<float*>(&ui_mantissa);

    const integer<E + 1, WordType> exp{nf.get_exponent()};
    const integer<E + 1, WordType> bias{nf.bias};
    const integer<E + 1, WordType> unbiased_exponent = sub(exp, bias);

    std::stringstream str;
    str << ((nf.get_sign() == 1) ? "-" : "") << *mantissa << "E" << unbiased_exponent;

    return str.str();
}

template <size_t E, size_t M, typename WordType>
auto operator<<(std::ostream& out, const normalized_float<E, M, WordType>& value) -> std::ostream&
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
