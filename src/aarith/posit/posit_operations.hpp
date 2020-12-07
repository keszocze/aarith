#pragma once

#include <aarith/integer.hpp>
#include <aarith/posit.hpp>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>

#include <iostream>

namespace aarith {

template <size_t N, class WT = uint64_t>
[[nodiscard]] constexpr uinteger<N, WT> get_twos_complement_if_msb_is_set(const uinteger<N, WT>& n)
{
    if (n.msb() == 0)
    {
        // sign bit is not set; return as-is
        return n;
    }
    else
    {
        // sign bit *is* set; return twos complement
        return (~n) + uinteger<N, WT>(1);
    }
}

template <size_t N, size_t ES, size_t Start, size_t End, size_t E, size_t M, typename WT>
[[nodiscard]] constexpr word_array<(Start - End) + 1, WT> bit_range(const posit<N, ES, WT>& p)
{
    return bit_range<Start, End>(p.as_word_array());
}

template <size_t N, size_t ES, class WT = uint64_t>
[[nodiscard]] constexpr size_t get_num_regime_bits(const posit<N, ES, WT>& p)
{
    // special case

    if (p == p.complex_infinity())
    {
        return N;
    }

    // regular cases

    size_t nregime;

    auto bits = p.get_bits();

    // if msb == 1, apply twos complement

    if (bits.msb())
    {
        bits = (~bits) + bits.one();
    }

    // now count leading digits; regime is always >= 1

    if (bits.bit(N - 2))
    {
        nregime = count_leading_ones(bits, 1);
    }
    else
    {
        nregime = count_leading_zeroes(bits, 1);
    }

    return nregime;
}

template <size_t N, size_t ES, class WT = uint64_t>
[[nodiscard]] constexpr size_t get_num_exponent_bits(const posit<N, ES, WT>& p)
{
    throw std::logic_error("not implemented");
}

template <size_t N, size_t ES, class WT = uint64_t>
[[nodiscard]] constexpr size_t get_num_fraction_bits(const posit<N, ES, WT>& p)
{
    throw std::logic_error("not implemented");
}

template <size_t N, size_t ES, class WT = uint64_t>
[[nodiscard]] constexpr integer<N, WT> get_regime_value(const posit<N, ES, WT>& p)
{
    // Getting the regime value means counting the number of bits in the
    // regime. Also note that "for positive exponents, the regime value is one
    // less than the number of bits in the run, since we need to be able to
    // represent a value of zero" (Posit Arithmetic, Gustafson, October 2017).

    const ssize_t nregime = get_num_regime_bits(p);

    if (p.get_sign_bit())
    {
        return integer<N, WT>(nregime - 1);
    }
    else
    {
        return integer<N, WT>((-1) * nregime);
    }
}

} // namespace aarith
