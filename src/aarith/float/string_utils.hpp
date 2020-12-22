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

struct decimal_conversion
{
    size_t dec_exponent;
    double conversion;
    bool neg_exponent;
    bool not_supported = false;
};

template <size_t E, size_t M, typename WordType>
auto compute_nearest_exponent10(const normalized_float<E, M, WordType>& nf) -> decimal_conversion
{
    // TODO float pow and log functions
    if constexpr (E > sizeof(size_t) * 8)
    {
        decimal_conversion conv{0U, 0., false, true};
        return conv;
    }

    auto const exponent_nf = nf.get_exponent();
    size_t exponent_i = exponent_nf.word(0);
    auto const bias = nf.bias;
    size_t bias_i = bias.word(0);

    bool is_neg = false;
    size_t exponent = exponent_i - bias_i;
    if (exponent_i < bias_i)
    {
        exponent = bias_i - exponent_i;
        is_neg = true;
    }

    auto const convert_2exp = log10(2.);
    auto decimal_exponent = static_cast<double>(exponent);
    decimal_exponent *= convert_2exp;

    double rounded_exponent = std::floor(decimal_exponent);
    if ((decimal_exponent - rounded_exponent) > 0.5)
    {
        rounded_exponent = std::ceil(decimal_exponent);
    }
    auto diff = decimal_exponent - rounded_exponent;
    if (is_neg)
    {
        diff = rounded_exponent - decimal_exponent;
    }

    auto conversion10 = std::pow(10., diff);

    decimal_conversion conv_struct{static_cast<size_t>(rounded_exponent), conversion10, is_neg};

    return conv_struct;
}

/// Convert the given normalized_float to a scientific string representation
template <size_t E, size_t M, typename WordType>
auto to_sci_string(const normalized_float<E, M, WordType> nf) -> std::string
{
    std::stringstream str;

    if (nf.is_nan())
    {
        str << "NaN";
        return str.str();
    }

    if (nf.is_negative())
    {
        str << "-";
    }

    if (nf.is_zero())
    {
        str << "0";
        return str.str();
    }
    if (nf.is_inf())
    {
        str << "inf";
        return str.str();
    }

    if constexpr (E <= 8 && M <= 23)
    {
        auto f = static_cast<float>(nf);
        str << f;
        return str.str();
    }
    else if constexpr (E <= 11 && M <= 52)
    {
        auto f = static_cast<double>(nf);
        str << f;
        return str.str();
    }
    else
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
        // construct a float with the given mantissa and an exponent of 0
        // to leech on float's inherent decimal output
        uint32_t ui_mantissa = (static_cast<uint32_t>(flc_mantissa.word(0)) & 0x7fffff) | 0x3f800000;
        float* mantissa = reinterpret_cast<float*>(&ui_mantissa);
        auto conv = compute_nearest_exponent10(nf);
        *mantissa = *mantissa * conv.conversion;

        const integer<E + 1, WordType> exp{nf.get_exponent()};
        const integer<E + 1, WordType> bias{nf.bias};

        if (conv.not_supported)
        {
            str << "E = " << E << " is not supported for to_sci_string()";
            return str.str();
        }

        if (*mantissa <= 1.f && conv.dec_exponent != 0)
        {
            *mantissa *= 10.f;
            if (conv.neg_exponent)
            {
                conv.dec_exponent += 1;
            }
            else
            {
                conv.dec_exponent -= 1;
            }
        }
        str << *mantissa;
        if (conv.dec_exponent != 0)
        {
            str << "e" << (conv.neg_exponent ? "-" : "+") << conv.dec_exponent;
        }

        return str.str();
    }
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
