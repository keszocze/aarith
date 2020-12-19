#pragma once

#include <aarith/integer.hpp>
#include <aarith/posit.hpp>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <stdexcept>

namespace aarith {

namespace internal {

template <size_t N, typename WT> [[nodiscard]] constexpr double to_double(const uinteger<N, WT>& n)
{
    const uint64_t n64 = narrow_cast<uint64_t>(n);
    return static_cast<double>(n64);
}

template <size_t N, typename WT> [[nodiscard]] constexpr double to_double(const integer<N, WT>& n)
{
    const int64_t i64 = narrow_cast<int64_t>(n);
    return static_cast<double>(i64);
}

} // namespace internal

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr size_t get_num_regime_bits(const posit<N, ES, WT>& p)
{
    // special cases

    if (p.is_zero() || p.is_nar())
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

template <size_t N, size_t ES, typename WT>
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

template <size_t N, size_t ES, typename WT>
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

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr integer<N, WT> get_sign_value(const posit<N, ES, WT>& p)
{
    using Integer = integer<N, WT>;

    if (p.is_negative())
    {
        return Integer::one();
    }
    else
    {
        return Integer::zero();
    }
}

template <size_t N, size_t ES, typename WT>
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

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr uinteger<N, WT> get_exponent_value(const posit<N, ES, WT>& p)
{
    const size_t nexp = get_num_exponent_bits<N, ES, WT>(p);
    const size_t nfrac = get_num_fraction_bits<N, ES, WT>(p);

    auto bits = p.get_bits();

    if (bits.msb())
    {
        bits = twos_complement(bits);
    }

    // mask out the relevant exponent bits

    const auto mask = uinteger<N, WT>(get_low_mask<N, WT>(nexp));
    const auto exponent_bits = (bits >> nfrac) & mask;

    // the exponent value is *always* an ES-wide integer; if nexp=1 but ES=1,
    // that one single exponent bit q needs to be understood as the msb of an
    // ES-wide integer, that is in this particular case, the exponent value is
    // 1- = 10 = 2 and not just 1; this cost me a lot of time

    const size_t shift = ES - nexp;
    return (exponent_bits << shift);
}

template <size_t N, size_t ES, typename WT>
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

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr double to_double(const posit<N, ES, WT>& p)
{
    if (p.is_zero())
    {
        return 0.0;
    }

    if (p.is_nar())
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

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> from_double(const double x)
{
    using Posit = posit<N, ES, WT>;

    // first consider the two possible special cases, zero NaR

    if (x == 0.0)
    {
        return Posit::zero();
    }

    if (std::isnan(x))
    {
        return Posit::nar();
    }

    if (std::isinf(x))
    {
        return Posit::nar();
    }

    // prepare some types and constants for later

    using StorageType = uinteger<N, WT>;
    constexpr StorageType zero = StorageType::zero();
    constexpr StorageType one = StorageType::one();
    constexpr StorageType two = one + one;

    const double useed = std::pow(2, std::pow(2, ES));

    // in this function we build up the underlying bitstring "p" bit by bit
    // (as explained in Posit Arithmetic, Gustafson, October 2017, pp. 25)

    StorageType p;
    double y = std::abs(x);
    size_t i = 0;

    // (1) First divide by useed or multiply by useed until x is in the
    // interval [1, useed).

    if (y >= 1.0)
    {
        // north east

        p = one;

        i = 2;

        while (y >= useed && i < N)
        {
            p = (p << 1) + one;
            y = y / useed;

            i += 1;
        }

        p = p << 1;
        i += 1;
    }
    else
    {
        // south east

        p = zero;
        i = 1;

        while (y < 1.0 && i <= N)
        {
            y = y * useed;
            i += 1;
        }

        if (i >= N)
        {
            p = two;
            i = N + 1;
        }
        else
        {
            p = one;
            i += 1;
        }
    }

    // (2) Then the value is repeatedly divided by 2 until it is in the
    // interval [1, 2) and that determines the exponent.

    double e = std::pow(2, ES - 1);

    while (e > 1.0 / 2.0 && i <= N)
    {
        p = p << 1;

        if (y >= std::pow(2.0, e))
        {
            y /= std::pow(2.0, e);
            p = p + one;
        }

        e = e / 2.0;
        i += 1;
    }

    y = y - 1.0;

    // (3) The fraction always has a leading 1 to the left of the binary
    // point, eliminating the need to handle subnormal exception values that
    // have a 0 bit to the left of the binary point.

    while (y > 0.0 && i <= N)
    {
        y = y * 2.0;

        p = (p << 1);

        if (std::floor(y))
        {
            p = p + one;
        }

        y -= std::floor(y);
        i += 1;
    }

    p = p << N + 1 - i;
    i += 1;

    // (4) Round to nearest even and fill up the last bits.

    const auto isb = p.lsb();
    p = p >> one;

    if (isb)
    {
        if (y == 1 || y == 0)
        {
            if (p.lsb())
            {
                p = p + one;
            }
        }
        else
        {
            p = p + one;
        }
    }

    // (5) If we were converting a negative number, we now need
    // to take the 2s complement.

    if (x < 0.0)
    {
        p = twos_complement(p);
    }

    // (6) Now that we have the bitstring, wrap it in the posit type and we
    // are ready to return.

    return Posit::from_bits(p);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> abs(const posit<N, ES, WT>& p)
{
    if (p < p.zero())
    {
        return -p;
    }
    else
    {
        return p;
    }
}

template <size_t N, size_t ES, typename WT>
constexpr integer<N> get_global_exponent(const posit<N, ES, WT>& p)
{
    using Integer = integer<N>;

    constexpr auto rscale = Integer(1 << ES);

    const auto R = Integer(get_regime_value<N, ES, WT>(p));
    const auto E = Integer(get_exponent_value<N, ES, WT>(p));

    return rscale * R + E;
}

} // namespace aarith
