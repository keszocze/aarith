#pragma once

#include <string>

namespace aarith {

/**
 * @brief
 * @param bits
 * @return
 */
inline constexpr auto number_of_decimal_digits(size_t bits) -> size_t
{
    // When converted to decimal, an n-bit binary numeral will have at most k*n decimal digits,
    // rounded up, where k = log_10 2 ~ 0.301.
    return (bits * 301) / 1000 + ((bits * 301) % 1000 == 0 ? 0 : 1); // NOLINT
}

/**
 * @brief
 * @tparam T
 * @tparam U
 * @param dividend
 * @param divisor
 * @return
 */
template <class T, class U>
constexpr auto rounded_integer_division(T dividend, U divisor) -> decltype(T{} / U{})
{
    return dividend / divisor + (dividend % divisor ? 1 : 0);
}

/**
 * @brief Removes leading zeroes from a string
 *
 * @note It is assumed that the string contains a number. The method works on all contents though.
 *
 * This method, by default, replaces empty strings with a "0".
 *
 * @param number The number for which leading zeroes should be removed
 * @param can_be_empty Whether an empty string may be returned
 * @return
 */
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

/**
 * @brief Group digits within a given string storing a number.
 *
 * @note It is assumed that the string contains a number. The method works on all contents though.
 *
 * @param number A string storing the number to be grouped
 * @param group_size The number of digits in a group
 * @param separator The separator to insert between the groups (default: space)
 * @return
 */
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

/**
 * @brief Converts the word array into a bitstring
 * @tparam Width Width of the word array
 * @tparam WordType Word type used to store the data in the word array
 * @param value The word array
 * @return The string of bits of the stored value
 */
template <size_t Width, typename WordType>
auto to_binary(const word_array<Width, WordType>& value) -> std::string
{
    std::string result;
    for (size_t i = Width; i > 0; --i)
    {
        result += value.bit(i - 1) ? '1' : '0';
    }
    return result;
}

/**
 * @brief Outputs a `word_array` to an output stream using the convenient << operator form
 * @tparam W Width of the word array
 * @tparam WordType The type used to store the actual data
 * @tparam WA Generic word array type
 */
template <size_t W, typename WordType, template <size_t, typename> class WA,
          typename =
              std::enable_if_t<is_word_array_v<WA<W, WordType>> && !is_integral_v<WA<W, WordType>>>>
auto operator<<(std::ostream& out, const WA<W, WordType>& value) -> std::ostream&
{
    out << group_digits(to_binary(value), WA<W, WordType>::word_width());
    return out;
}

} // namespace aarith