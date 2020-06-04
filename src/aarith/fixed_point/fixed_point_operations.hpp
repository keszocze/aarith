#pragma once

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




} // namespace aarith