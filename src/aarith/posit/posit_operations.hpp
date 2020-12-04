#pragma once

#include <aarith/integer.hpp>
#include <aarith/posit.hpp>
#include <cassert>
#include <cstdint>
#include <cstdlib>

namespace aarith {

template <size_t N, class WT = uint64_t>
[[nodiscard]] constexpr uinteger<N, WT> get_inverted_if_msb_is_set(const uinteger<N, WT>& n)
{
    return n.msb() == 0 ? n : ~n;
}

template <size_t N, size_t ES, size_t Start, size_t End, size_t E, size_t M, typename WT>
[[nodiscard]] constexpr word_array<(Start - End) + 1, WT> bit_range(const posit<N, ES, WT>& p)
{
    return bit_range<Start, End>(p.as_word_array());
}

template <size_t N, size_t ES, class WT = uint64_t>
[[nodiscard]] constexpr size_t get_num_regime_bits(const posit<N, ES, WT>& p)
{
    const auto bits = p.get_bits();
    const auto ibits = get_inverted_if_msb_is_set(bits);
    const size_t leading_zeroes = count_leading_zeroes(ibits);

    // because ibits should have at least one 0 at the beginning, the number
    // of leading zeroes has to be at least one
    assert(leading_zeroes > 0);

    return leading_zeroes - 1;
}

template <size_t N, size_t ES, class WT = uint64_t>
[[nodiscard]] constexpr uinteger<N, WT> get_regime(const posit<N, ES, WT>& p)
{
    const size_t nregime = get_num_regime_bits(p);

    const size_t shift = N - nregime - 1;
    const uinteger<N, WT> mask = get_low_mask<N, WT>(nregime);

    return (p.get_bits() >> shift) & mask;
}

} // namespace aarith
