#pragma once

namespace aarith {


// TODO implement something reasonable here!
//template <size_t I, size_t F, template <size_t, class> typename B, typename WordType>
//auto operator<<(std::ostream& out, const fixed<I, F, B, WordType>& value) -> std::ostream&
//{
//    if (out.flags() & std::ios::hex)
//    {
//        out << to_hex(value.integer_part());
//        out << ".";
//        out << to_hex(value.fractional_part());
//    }
//    else if (out.flags() & std::ios::oct)
//    {
//        out << to_octal(value.integer_part());
//        out << ".";
//        out << to_octal(value.fractional_part());
//    }
//    else
//    {
//        out << to_decimal(value.integer_part());
//        out << ".";
//        out << to_decimal(value.fractional_part());
//    }
//    return out;
//}

/**
 * @brief Creates a string of bits representing the fixed point number using a '.' to indicate the
 * decimal point.
 *
 * @tparam Fixed
 * @param value The fixed point number to be transformed into a binary representation
 * @return String representing the number as binary digits
 */
template <typename Fixed, typename = std::enable_if_t<::aarith::is_fixed_point_v<Fixed>>>
[[nodiscard]] std::string to_binary(const Fixed& value)
{
    std::string result{to_binary(value.integer_part())};
    result += ".";
    result += to_binary(value.fractional_part());
    return result;
}

template <size_t I, size_t F, template <size_t, typename> typename BaseInt, typename WordType = uint64_t>
std::ostream& operator<<(std::ostream& out, const fixed<I,F,BaseInt, WordType>& value) {
    out << to_binary(value);
    return out;
}

} // namespace aarith