#pragma once

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <string>

#include <aarith/fixed_point.hpp>

namespace aarith {

template <size_t I, size_t F, template <size_t, class> typename B, typename WordType>
auto fraction_as_integer(const fixed<I, F, B, WordType>& value)
{
    // TODO (Schärtl): Find real bound for width of this buffer.

    using Int = uinteger<2 * F>;

    // Understanding the conversion can be a bit tricky. It is important to
    // realize that
    //
    //     1/2 = 5/10     = 500/1000
    //     1/4 = 25/100   = 250/1000
    //   + 1/8 = 125/1000 = 125/1000
    //     -------------------------
    //     0.875            875/1000
    //
    // and so on. Knowing this, we keep computing the numerator (example value
    // is 875) in variable "int_fraction".

    constexpr Int five = Int(5U);
    constexpr Int ten = Int(10U);

    const auto fraction = value.fractional_part();
    const ssize_t last_one_idx = count_leading_zeroes(flip(fraction));

    Int p = five;
    Int q = ten;
    Int int_fraction;

    for (ssize_t i = fraction.width() - 1; i >= last_one_idx; --i)
    {
        if (fraction.bit(i))
        {
            int_fraction = int_fraction + p;
        }

        int_fraction = int_fraction * ten;
        p = p * five;
        q = q * ten;
    }

    // Now change to string. Pad with leading zeroes if necessary.

    const ssize_t decimal_places = fraction.width() - last_one_idx + 1;
    const std::string int_fraction_str = to_decimal(int_fraction);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(decimal_places) << int_fraction_str;
    return ss.str();
}

template <size_t I, size_t F, template <size_t, class> typename B, typename WordType>
auto operator<<(std::ostream& out, const fixed<I, F, B, WordType>& value) -> std::ostream&
{
    if (value.is_negative())
    {
        out << "-";
    }

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
        // We always have to print the integer part.
        out << to_decimal(abs(value).integer_part());

        // We only output a fraction if it is non-zero. To check, we first
        // render the fraction in a string and then, if it contains non-zero
        // characters, write to to os.

        const std::string fs = fraction_as_integer(abs(value));
        const auto is_zero_char = [](const char c) -> bool { return c == '0'; };

        if (!std::all_of(fs.begin(), fs.end(), is_zero_char))
        {
            out << ".";
            out << fs;
        }
    }

    return out;
}

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

} // namespace aarith
