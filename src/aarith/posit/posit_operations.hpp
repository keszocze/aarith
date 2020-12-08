#pragma once

#include <aarith/integer.hpp>
#include <aarith/posit.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
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

    // Also refer to Posit Standard Documentation, Release 4.9-draft, November
    // 10, 2020, page 7.

    const ssize_t nregime = get_num_regime_bits<N, ES, WT>(p);

    auto bits = p.get_bits();
    const auto R0 = bits.bit(N - 1);

    if (R0 == 0)
    {
        return - integer<N, WT>(nregime);
    }
    else
    {
        // R0 == 1
        return integer<N, WT>(nregime) - integer<N, WT>::one();
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

namespace internal
{

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
 * Evaluate the given posit to compute the represented real value.  Uses
 * double precision IEEE floats for computation.
 *
 * The special value posit::complex_infinity gets translated to NaN
 * by this function.
 *
 * @ The real value of p, represented as a double precision float.
 */
template <size_t N, size_t ES, class WT>
constexpr double to_double(const posit<N, ES, WT>& p)
{
    // [Posit Arithmetic, Gustafson, October 2017, p. 13]

    if (p == p.zero())
    {
        return 0.0;
    }

    if (p == p.complex_infinity())
    {
        return NAN;
    }

    const double sign = p.is_negative() ? -1.0 : 1.0;
    const double useed = std::pow(2.0, std::pow(2.0, static_cast<double>(ES)));
    const double k = internal::to_double(get_regime_value(p));
    const double e = internal::to_double(get_exponent_value(p));

    const double fval = internal::to_double(get_fraction_value(p));
    const double nfrac = static_cast<double>(get_num_fraction_bits(p));
    const double f = (fval == 0.0) ? (1.0) : (1 + fval * std::pow(2, (-1.0) * nfrac));

    const double res =  sign * std::pow(useed, k) * std::pow(2, e) * f;
    return res;
}

} // namespace aarith
