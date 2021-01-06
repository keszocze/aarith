#pragma once

#include <aarith/integer.hpp>
#include <utility>

namespace aarith {

/**
 * @brief Extracts a range from the fixed point number interpreted as a single bitstring
 *
 * Note that the indexing is done
 *  - zero based starting from the LSB
 *  - is inclusive (i.e. the start and end point are part of the range)
 *
 * @tparam S Starting index (inclusive, from left to right)
 * @tparam E  Ending index (inclusive, from left to right)
 * @tparam I Width of the integer part of the fixed point number
 * @tparam F Width of the fractional part of the fixed point number
 * @tparam B Underlying aarith integer type used (integer or uinteger)
 * @tparam WordType word type used to store the data
 * @param w Fixed point number from which the range is taken from
 * @return Range fixed_point[S,E], inclusive
 */
template <size_t S, size_t E, size_t I, size_t F, template <size_t, class> typename B,
          typename WordType>
[[nodiscard]] constexpr word_array<(S - E) + 1, WordType>
bit_range(const fixed<I, F, B, WordType>& w)
{
    return bit_range<S, E>(w.bits);
}

/**
 * @brief Adjusts the number of fraction bits of two numbers to be euqal
 * @tparam I1 Integer part width of the first number
 * @tparam F1 Fraction part width of the first number
 * @tparam I2 Integer part width of the second number
 * @tparam F2 Fraction part width of the second number
 * @tparam B The integer type used to store the number (signed or unsigned)
 * @tparam WordType The word type used to actually store the data
 */
template <size_t I1, size_t F1, size_t I2, size_t F2, template <size_t, typename> typename B,
          typename WordType = uint64_t>
constexpr std::pair<fixed<I1, std::max(F1, F2), B, WordType>,
                    fixed<I2, std::max(F1, F2), B, WordType>>
equalize_fraction_width(const fixed<I1, F1, B, WordType>& a, const fixed<I2, F2, B, WordType>& b)
{
    constexpr size_t F_expanded = std::max(F1, F2);

    const fixed<I1, F_expanded, B, WordType> a_expanded{width_cast<I1, F_expanded>(a)};
    const fixed<I2, F_expanded, B, WordType> b_expanded{width_cast<I2, F_expanded>(b)};

    return std::make_pair(a_expanded, b_expanded);
}

/**
 * @brief Performs an arithmetic right shift
 * @param lhs The fixed point number that is shifted
 * @param rhs The number of bits to shift
 * @return
 */
template <size_t I, size_t F, template <size_t, typename> typename B, typename WordType = uint64_t>
constexpr fixed<I, F, B, WordType> operator>>(const fixed<I, F, B, WordType>& lhs, const size_t rhs)
{
    return fixed<I, F, B, WordType>::from_bitstring(lhs.bits() >> rhs);
}

/**
 * @brief Performs a logical left shift
 * @param lhs The fixed point number that is shifted
 * @param rhs The number of bits to shift
 * @return
 */
template <size_t I, size_t F, template <size_t, typename> typename B, typename WordType = uint64_t>
constexpr fixed<I, F, B, WordType> operator<<(const fixed<I, F, B, WordType>& lhs, const size_t rhs)
{
    return fixed<I, F, B, WordType>::from_bitstring(lhs.bits() << rhs);
}

/**
 * @brief Changes the widths of the integer and fractional part.
 *
 * This method simply "cuts of" bits from the left and right if the target width is smaller than
 * the initial one. In case of signed number, cutting of bits might result in a flip of the sign.
 * In general, there are no guarantees on the value the number stores after a  reduction of any
 * of the widths.
 *
 * If the width of integer part is extended, the sign and the number (of the integer part) is
 * preserved. That means that if both widths are increased (or at least not decreased), the number
 * stored in the fixed point number remains the same and simply uses more bits.
 *
 * @tparam TargetI The target width of the integer part
 * @tparam TargetF The target width of the factional part
 * @param a The fixed point whose widhts are changed
 * @return A fixed point number with the specified widths
 */
template <size_t TargetI, size_t TargetF, size_t I, size_t F, template <size_t, class> class B,
          typename WordType = uint64_t>
constexpr auto width_cast(const fixed<I, F, B, WordType>& a) -> fixed<TargetI, TargetF, B, WordType>
{
    using ret_type = fixed<TargetI, TargetF, B, WordType>;

    auto fixed_i =
        fixed<TargetI, F, B, WordType>::from_bitstring(width_cast<TargetI + F>(a.bits()));
    constexpr size_t TargetW = TargetI + TargetF;
    if constexpr (TargetF > F)
    {
        size_t diff = TargetF - F;
        auto expanded = ret_type::from_bitstring(width_cast<TargetW>(fixed_i.bits()));
        return (expanded << diff);
    }
    else
    {
        size_t diff = F - TargetF;
        fixed_i = (fixed_i >> diff);
        return ret_type::from_bitstring(width_cast<TargetW>(fixed_i.bits()));
    }
}

/**
 * @brief Adds two fixed point numbers expanding the size to store the result.
 *
 * @tparam I1 Integer part width of the first number
 * @tparam F1 Fraction part width of the first number
 * @tparam I2 Integer part width of the second number
 * @tparam F2 Fraction part width of the second number
 * @tparam B The integer type used to store the number (signed or unsigned)
 * @tparam WordType The word type used to actually store the data
 * @param a First addend
 * @param b Second addend
 * @return The sum of both numbers using enough bits to correctly store the data
 */
template <size_t I1, size_t F1, size_t I2, size_t F2, template <size_t, typename> typename B,
          typename WordType = uint64_t>
auto constexpr expanding_add(const fixed<I1, F1, B, WordType>& a, const fixed<I2, F2, B, WordType>& b)
    -> fixed<std::max(I1, I2) + 1, std::max(F1, F2), B, WordType>
{
    constexpr size_t I_expanded = std::max(I1, I2) + 1;
    constexpr size_t F_expanded = std::max(F1, F2);

    const auto [expanded_a, expanded_b] = equalize_fraction_width(a, b);

    const auto tmp_result = expanding_add(expanded_a.bits(), expanded_b.bits());
    const auto result =
        fixed<I_expanded, F_expanded, B, WordType>::from_bitstring(tmp_result);

    return result;
}

/**
 * @brief Adds two fixed point numbers with over/underflow semantics and truncation on the right
 * @tparam I Integer part width
 * @tparam F Fraction part width
 * @tparam B The integer type used to store the number (signed or unsigned)
 * @tparam WordType The word type used to actually store the data
 * @param a The first addend
 * @param b The second addend
 * @return The (truncated) sum of both numbers
 */
template <size_t I, size_t F, template <size_t, typename> typename B, typename WordType = uint64_t>
auto add(const fixed<I, F, B, WordType>& a, const fixed<I, F, B, WordType>& b)
    -> fixed<I, F, B, WordType>
{
    const auto result = expanding_add(a, b);
    return width_cast<I, F>(result);
}

/**
 * @brief Subtracts two fixed point numbers expanding the size to store the result.
 *
 * @tparam I1 Integer part width of the first number
 * @tparam F1 Fraction part width of the first number
 * @tparam I2 Integer part width of the second number
 * @tparam F2 Fraction part width of the second number
 * @tparam B The integer type used to store the number (signed or unsigned)
 * @tparam WordType The word type used to actually store the data
 * @param a First number
 * @param b Second number
 * @return The difference of both numbers using enough bits to correctly store the data
 */
template <size_t I1, size_t F1, size_t I2, size_t F2, template <size_t, typename> typename B,
    typename WordType = uint64_t>
auto expanding_sub(const fixed<I1, F1, B, WordType>& a, const fixed<I2, F2, B, WordType>& b)
-> fixed<std::max(I1, I2) + 1, std::max(F1, F2), B, WordType>
{
    constexpr size_t I_expanded = std::max(I1, I2) + 1;

    const auto [expanded_a, expanded_b] = equalize_fraction_width(a, b);

    const auto tmp_result = expanding_sub(expanded_a.bits(), expanded_b.bits());
    const auto result =
        fixed<I_expanded, expanded_a.frac_width, B, WordType>::from_bitstring(tmp_result);

    return result;
}


/**
 * @brief Subtracts two fixed point numbers with over/underflow semantics and truncation on the right
 * @tparam I Integer part width
 * @tparam F Fraction part width
 * @tparam B The integer type used to store the number (signed or unsigned)
 * @tparam WordType The word type used to actually store the data
 * @param a The first addend
 * @param b The second addend
 * @return The (truncated) difference of both numbers
 */
template <size_t I, size_t F, template <size_t, typename> typename B, typename WordType = uint64_t>
auto sub(const fixed<I, F, B, WordType>& a, const fixed<I, F, B, WordType>& b)
-> fixed<I, F, B, WordType>
{
    const auto result = expanding_sub(a, b);
    return width_cast<I, F>(result);
}

template <size_t I, size_t F, template <size_t, typename> typename B, typename WT = uint64_t>
auto mul(const fixed<I, F, B, WT>& a, const fixed<I, F, B, WT>& b) -> fixed<I, F, B, WT>
{
    const size_t base_size = a.width;
    const size_t scratch_size = 2 * base_size;

    const auto a_extended = width_cast<scratch_size>(a.bits());
    const auto b_extended = width_cast<scratch_size>(b.bits());

    const auto result_bits_extended = (a_extended * b_extended) >> F;
    const auto result_bits = width_cast<base_size>(result_bits_extended);

    return fixed<I, F, B, WT>::from_bitstring(result_bits);
}

template <size_t I, size_t F, template <size_t, typename> typename B, typename WT = uint64_t>
auto div(const fixed<I, F, B, WT>& a, const fixed<I, F, B, WT>& b) -> fixed<I, F, B, WT>
{
    const size_t base_size = a.width;
    const size_t scratch_size = F + a.width;

    const auto a_extended = width_cast<scratch_size>(a.bits());
    const auto b_extended = width_cast<scratch_size>(b.bits());

    const auto result_bits_extended = (a_extended << F) / b_extended;
    const auto result_bits = width_cast<base_size>(result_bits_extended);

    return fixed<I, F, B, WT>::from_bitstring(result_bits);
}

} // namespace aarith
