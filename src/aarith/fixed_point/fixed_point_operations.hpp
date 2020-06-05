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

template <size_t I, size_t F, template <size_t, typename> typename B, typename WordType = uint64_t>
constexpr fixed<I,F,B,WordType> operator>>(const fixed<I,F,B,WordType>& lhs, const size_t rhs)
{
    return fixed<I,F,B,WordType>::from_bitstring(lhs.bits() >> rhs);
}

template <size_t I, size_t F, template <size_t, typename> typename B, typename WordType = uint64_t>
constexpr fixed<I,F,B,WordType> operator<<(const fixed<I,F,B,WordType>& lhs, const size_t rhs)
{
    return fixed<I,F,B,WordType>::from_bitstring(lhs.bits() << rhs);
}

template <size_t TargetI, size_t TargetF, size_t I, size_t F, template <size_t, class> class B,
          typename WordType = uint64_t>
constexpr auto width_cast(fixed<I, F, B, WordType> a) -> fixed<TargetI, TargetF, B, WordType>
{
    using ret_type = fixed<TargetI, TargetF, B, WordType>;

    auto fixed_i = fixed<TargetI, F, B, WordType>::from_bitstring(width_cast<TargetI + F>(a.bits()));
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
//
// template <size_t I1, size_t F1, size_t I2, size_t F2, template <size_t, class> B,
//          typename WordType = uint64_t>
// expanding_add(fixed<I1, F1, B, WordType> a, fixed<I2, F2, B, WordType> b)
//    ->fixed<std::max(I1, I2) + 1, std::max(F1, F2), B, WordType>
//{
//    // TODO größen angleichen
//    auto tmp_result = add(a_expanded, b_expanded);
//    // TODO reinterpret Funktion bauen (oder doch per Konstruktor? mittels "bool raw=false" extra
//    // param?)
//    return reinterpret<std::max(I1, I2) + 1, std::max(F1, F2)>(tmp_result);
//}

} // namespace aarith