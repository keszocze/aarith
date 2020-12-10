#pragma once

#include <aarith/integer.hpp>
#include <aarith/posit.hpp>
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdexcept>

#include <iostream>

namespace aarith {

namespace internal {

template <size_t N, class WT> [[nodiscard]] constexpr double to_double(const uinteger<N, WT>& n)
{
    const uint64_t n64 = narrow_cast<uint64_t>(n);
    return static_cast<double>(n64);
}

template <size_t N, class WT> [[nodiscard]] constexpr double to_double(const integer<N, WT>& n)
{
    const int64_t i64 = narrow_cast<int64_t>(n);
    return static_cast<double>(i64);
}

} // namespace internal

/**
 * @return The number of regime bits in the given posit.
 */
template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr size_t get_num_regime_bits(const posit<N, ES, WT>& p)
{
    // special cases

    if (p == p.zero() || p == p.complex_infinity())
    {
        return N - 1;
    }

    // regular case

    size_t nregime = 0;
    auto bits = p.get_bits();

    if (bits.msb())
    {
        bits = twos_complement(bits);
    }

    // now count leading digits; regime is always >= 1

    const auto R0 = bits.bit(N - 2);

    if (R0 == 1)
    {
        nregime = count_leading_ones(bits, 1);
    }
    else
    {
        nregime = count_leading_zeroes(bits, 1);
    }

    // regime is never more than N - 1 as the sign bit is always seperate

    return std::min(N - 1, nregime + 1);
}

/**
 * @return The number of exponent bits in the given posit.
 */
template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr size_t get_num_exponent_bits(const posit<N, ES, WT>& p)
{
    const size_t nregime = get_num_regime_bits<N, ES, WT>(p);

    // We have zero exponent bits if (1) the posit consists of just regime
    // bits (nregime == N) or if (2) the posit consists of just regime and sign
    // bits (nregime == N - 1).

    if (nregime >= N - 1)
    {
        return 0;
    }

    // Otherwise the number of exponent bit is determined by the number of
    // remaining usable bits and the exponent size as defined by template
    // parameter ES. The remaining bits are N minus the bits for regime minus
    // one bit for sign bit.

    const size_t nremaining = N - nregime - 1;
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
    // check if sign + regime bit takes up the whole posit.  In that case, we
    // have zero fraction bits.

    if (nregime >= N - 1)
    {
        return 0;
    }

    // We do have some bits left aside from sign + regime.

    const size_t nsign = 1;
    const size_t nexp = get_num_exponent_bits<N, ES, WT>(p);
    const size_t nused = nsign + nregime + nexp;

    if (nused >= N)
    {
        return 0;
    }

    return N - nused;
}

/**
 * @return The regime value of the given posit.
 */
template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr integer<N, WT> get_regime_value(const posit<N, ES, WT>& p)
{
    // Getting the regime value means counting the number of bits in the
    // regime. Also note that "for positive exponents [sic], the regime value
    // is one less than the number of bits in the run, since we need to be
    // able to represent a value of zero" (Posit Arithmetic, Gustafson,
    // October 2017).

    // Also refer to Posit Standard Documentation, Release 4.9-draft, November
    // 10, 2020, page 7.

    // get bits; if negative apply 2s complement

    auto bits = p.get_bits();

    if (bits.msb())
    {
        bits = twos_complement(bits);
    }

    const auto R0 = bits.bit(N - 2);

    // general case where we have to subtract the seperator bit from the
    // number of regime bit; what we understand as the number of regime bits
    // is called "r + 1" in the draft

    const size_t nregime = get_num_regime_bits<N, ES, WT>(p);
    ssize_t r = static_cast<ssize_t>(nregime) - 1;

    // special case where the entire string of regime bits is just
    // one specific bit (i.e. either all one or all zero)

    if (nregime <= N - 1)
    {
        const auto regime_bits = bit_range<N - 2, 0>(bits);

        if (regime_bits == regime_bits.all_zeroes() || regime_bits == regime_bits.all_ones())
        {
            r = static_cast<ssize_t>(nregime);
        }
    }

    // as per the standard we look at the MSG of the regime and depending on
    // its value return the regime value as negative integer or as r minus one

    if (R0 == 0)
    {
        return -integer<N, WT>(r);
    }
    else // R0 == 1
    {
        return integer<N, WT>(r) - integer<N, WT>::one();
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
        bits = twos_complement(bits);
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
        bits = twos_complement(bits);
    }

    const auto mask = uinteger<N, WT>(get_low_mask<N, WT>(nfrac));
    return bits & mask;
}

/**
 * Evaluate the given posit to compute the represented real value.  Uses
 * double precision IEEE floats for computation.
 *
 * The special value posit::complex_infinity gets translated to NaN
 * by this function.
 *
 * @ The real value of p, represented as a double precision float.
 */
template <size_t N, size_t ES, class WT> [[nodiscard]] double to_double(const posit<N, ES, WT>& p)
{
    if (p == p.zero())
    {
        return 0.0;
    }

    if (p == p.complex_infinity())
    {
        return NAN;
    }

    const double sign = p.is_negative() ? -1.0 : 1.0;
    const double useed = std::pow(2.0, std::pow(2.0, ES));
    const double k = internal::to_double(get_regime_value(p));
    const double e = internal::to_double(get_exponent_value(p));

    const double F = internal::to_double(get_fraction_value(p));
    const double nF = get_num_fraction_bits(p);
    const double f = (nF == 0.0) ? 1.0 : 1.0 + F / std::pow(2.0, nF);

    return sign * std::pow(useed, k) * std::pow(2, e) * f;
}

} // namespace aarith
