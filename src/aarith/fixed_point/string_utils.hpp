#pragma once

namespace aarith {

template <size_t I, size_t F, template <size_t, class> typename B, typename WordType>
auto operator<<(std::ostream& out, const fixed<I, F, B, WordType>& value) -> std::ostream&
{
    if (out.flags() & std::ios::hex)
    {
        out << to_hex(value.integer_part());
        out << ".";
        out << to_hex(value.fractional_part());
    }
    else if (out.flags() & std::ios::oct)
    {
        out << to_octal(value.integer_part());
        out << ".";
        out << to_octal(value.fractional_part());
    }
    else
    {
        out << to_decimal(value.integer_part());
        out << ".";
        out << to_decimal(value.fractional_part());
    }
    return out;
}

template <size_t I, size_t F, template <size_t, class> typename B, typename WordType>
[[nodiscard]] std::string to_binary(const fixed<I, F, B, WordType>& value)
{
    std::string result{to_binary(value.integer_part())};
    result += ".";
    result += to_binary(value.fractional_part());
    return result;
}

} // namespace aarith