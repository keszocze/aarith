#pragma once

#include <aarith/integer.hpp>
#include <aarith/posit.hpp>
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>

#include <iostream>

namespace aarith {

/**
 * @param n The integer to compute the twos complement of.
 * @return The twos complement of n.
 */
template <size_t N, class WT>
[[nodiscard]] constexpr uinteger<N, WT> twos_complement(const uinteger<N, WT>& n)
{
    return (~n) + n.one();
}

/**
 * @return The number of regime bits in the given posit.
 */
template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr size_t get_num_regime_bits(const posit<N, ES, WT>& p)
{
    // special case

    if (p == p.complex_infinity())
    {
        return N;
    }

    // regular cases

    size_t nregime = 0;

    auto bits = p.get_bits();

    // if msb == 1, apply twos complement

    if (bits.msb())
    {
        bits = twos_complement<N, WT>(bits);
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

/**
 * @return The number of exponent bits in the given posit.
 */
template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr size_t get_num_exponent_bits(const posit<N, ES, WT>& p)
{
    const size_t nregime = get_num_regime_bits<N, ES, WT>(p);

    // We have zero exponent bits if (1) the posit consists of just regime
    // bits (nregime == N), if (2) the posit consists of just regime and sign
    // bits (nregime == N - 1) or if (3) the posit consists of sign bit, regime
    // bits and a single separation bit (nregime == N - 2).

    if (nregime >= N - 2)
    {
        return 0;
    }

    // Otherwise the number of exponent bit is determined by the number of
    // remaining usable bits and the exponent size as defined by template
    // parameter ES. The remaining bits are N minus the bits for regime,
    // minus one bit for sign and minus one bit for separation bit.

    const size_t nremaining = N - nregime - 2;
    return std::min(nremaining, ES);
}

/**
 * @return The number of fraction bits in the given posit.
 */
template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr size_t get_num_fraction_bits(const posit<N, ES, WT>& p)
{
    const size_t nregime = get_num_regime_bits<N, ES, WT>(p);

    // Just like when calculating the number of exponent bits, we have to
    // check if sign + regime + separation bit take up the whole posit.
    // In that case, we have zero fraction bits.

    if (nregime >= N - 2)
    {
        return 0;
    }

    // We do have some bits left aside from sign + regime and separation bit.

    const size_t nsign = 1;
    const size_t nseperator = 1;
    const size_t nexp = get_num_exponent_bits<N, ES, WT>(p);

    const size_t nused = nsign + nregime + nseperator + nexp;

    return N - nused;
}

/**
 * @return The regime value of the given posit.
 */
template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr integer<N, WT> get_regime_value(const posit<N, ES, WT>& p)
{
    // Getting the regime value means counting the number of bits in the
    // regime. Also note that "for positive exponents, the regime value is one
    // less than the number of bits in the run, since we need to be able to
    // represent a value of zero" (Posit Arithmetic, Gustafson, October 2017).

    const ssize_t nregime = get_num_regime_bits<N, ES, WT>(p);

    if (p.get_sign_bit())
    {
        return integer<N, WT>(nregime - 1);
    }
    else
    {
        return integer<N, WT>((-1) * nregime);
    }
}

/**
 * @return The exponent value of the given posit.
 */
template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr uinteger<N, WT> get_exponent_value(const posit<N, ES, WT>& p)
{
    const size_t nexp = get_num_exponent_bits<N, ES, WT>(p);
    const size_t nfrac = get_num_fraction_bits<N, ES, WT>(p);

    auto bits = p.get_bits();

    if (bits.msb())
    {
        bits = twos_complement<N, WT>(bits);
    }

    const auto mask = uinteger<N, WT>(get_low_mask<N, WT>(nexp));
    return (bits >> nfrac) & mask;
}

/**
 * Return the fraction of the given posit. The fraction is the fractional part
 * of the posit interpreted as an unsigned integer.
 *
 * @return The fraction of the given posit.
 */
template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr uinteger<N, WT> get_fraction_value(const posit<N, ES, WT>& p)
{
    const size_t nfrac = get_num_fraction_bits<N, ES, WT>(p);

    auto bits = p.get_bits();

    if (bits.msb())
    {
        bits = twos_complement<N, WT>(bits);
    }

    const auto mask = uinteger<N, WT>(get_low_mask<N, WT>(nfrac));
    return bits & mask;
}

} // namespace aarith
