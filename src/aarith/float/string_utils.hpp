#pragma once

#include <aarith/integer.hpp>

#include <aarith/core/string_utils.hpp>

#include <sstream>

namespace aarith {

/// Convert the given normalized_float to a string with digits to the power of 2
template <size_t N, size_t E, size_t M>
auto to_base_2n(const normalized_float<E, M> nf) -> std::string
{
    std::string str;

    str = std::to_string(nf.get_sign()) + " " + to_base_2n<N>(nf.get_exponent()) + " " +
          to_base_2n<N>(nf.get_mantissa());
    return str;
}

/// Convert the given normalized_float value into a hexadecimal string representation.
template <size_t E, size_t M> auto to_hex(const normalized_float<E, M>& value) -> std::string
{
    return to_base_2n<4>(value);
}

/// Convert the given normalized_float value into a octal string representation.
template <size_t E, size_t M> auto to_octal(const normalized_float<E, M>& value) -> std::string
{
    return to_base_2n<3>(value);
}

/// Convert the given normalized_float value into a binary string representation.
template <size_t E, size_t M> auto to_binary(const normalized_float<E, M>& value) -> std::string
{
    return to_base_2n<1>(value);
}

/// Convert the given normalized_float to a representation that can be posted to a calculator
template <size_t E, size_t M> auto to_compute_string(const normalized_float<E, M> nf) -> std::string
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

/// Convert the given normalized_float to a scientific string representation
template <size_t E, size_t M> auto to_sci_string(const normalized_float<E, M> nf) -> std::string
{
    uinteger<M> fl_mantissa = nf.get_mantissa();
    uinteger<24> flc_mantissa;
    if constexpr (M >= 24)
    {
        auto shift_mantissa = M - 24;
        fl_mantissa = fl_mantissa >> shift_mantissa;
        flc_mantissa = width_cast<24, M>(fl_mantissa);
    }
    else
    {
        auto shift_mantissa = 24 - M;
        flc_mantissa = width_cast<24, M>(fl_mantissa);
        flc_mantissa = (flc_mantissa << shift_mantissa);
    }
    uint32_t ui_mantissa = (static_cast<uint32_t>(flc_mantissa.word(0)) & 0x7fffff) | 0x3f800000;
    float* mantissa = reinterpret_cast<float*>(&ui_mantissa);

    auto const exponent = sub(nf.get_exponent(), nf.get_bias());

    const integer<E> s_exponent(exponent);
    auto const s_abs_exponent = abs(s_exponent);
    const uinteger<E> abs_exponent(s_abs_exponent);

    std::stringstream str;
    str << ((nf.get_sign() == 1) ? "-" : "") << *mantissa << "E"
        << (exponent.bit(E - 1) == 1 ? "-" : "") << to_decimal(abs_exponent);

    return str.str();
}

template <size_t E, size_t M>
auto operator<<(std::ostream& out, const normalized_float<E, M>& value) -> std::ostream&
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