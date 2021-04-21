#pragma once

#include <aarith/posit.hpp>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <sstream>
#include <stdexcept>

namespace aarith {

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
        bits = negate(bits);
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

    // regime is never more than N - 1 as the sign bit is always separate

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
        bits = negate(bits);
    }

    const auto R0 = bits.bit(N - 2);

    // general case where we have to subtract the separator bit from the
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
        bits = negate(bits);
    }

    // mask out the relevant exponent bits

    const auto mask = uinteger<N, WT>(get_low_mask<N, WT>(nexp));
    const auto exponent_bits = (bits >> nfrac) & mask;

    // the exponent value is *always* an ES-wide integer; if nexp=1 but ES=2,
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
        bits = negate(bits);
    }

    const auto mask = uinteger<N, WT>(get_low_mask<N, WT>(nfrac));
    return bits & mask;
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
[[nodiscard]] constexpr integer<N> get_scale_value(const posit<N, ES, WT>& p)
{
    using Integer = integer<N>;

    constexpr Integer rscale = Integer(1 << ES);

    const Integer R = Integer(get_regime_value<N, ES, WT>(p));
    const Integer E = Integer(get_exponent_value<N, ES, WT>(p));

    return rscale * R + E;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> sqrt(const posit<N, ES, WT>& x)
{
    using Posit = posit<N, ES, WT>;

    // Handle special cases.

    if (!x.is_non_negative())
    {
        return Posit::nar();
    }

    // Naive Newton-Raphson Method. We iterate until convergence.

    const Posit two = Posit(2);

    Posit rprev;
    Posit rcur = x;

    while (rprev != rcur)
    {
        const Posit rnext = (rcur + x / rcur) / two;

        rprev = rcur;
        rcur = rnext;
    }

    return rcur;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> log(const posit<N, ES, WT>& x)
{
    using Posit = posit<N, ES, WT>;

    //
    // Handle special cases.
    //

    if (x.is_nar() || x <= Posit::zero())
    {
        return Posit::nar();
    }

    //
    // Approximate log x with algorithm from
    //
    //   https://doi.org/10.1090/S0025-5718-1972-0307438-2
    //
    // summarized on Stackoverflow,
    //
    //   https://math.stackexchange.com/a/75398
    //

    const Posit one = Posit(1);
    const Posit two = Posit(2);

    Posit a = (one + x) / two;
    Posit b = sqrt(x);

    const int niterations = 50;

    for (int i = 0; i < niterations; ++i)
    {
        const Posit anext = (a + b) / two;
        const Posit bnext = sqrt(anext * b);

        a = anext;
        b = bnext;
    }

    return two * ((x - one) / (a + b));
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> factorial(const posit<N, ES, WT>& x)
{
    using Posit = posit<N, ES, WT>;

    if (x.is_negative() || x.is_nar())
    {
        return Posit::nar();
    }

    if (x.is_zero())
    {
        return Posit::one();
    }

    Posit accu = Posit::one();

    for (Posit i = Posit::one(); i <= x; ++i)
    {
        accu *= i;
    }

    return accu;
}

template <size_t N, size_t ES, typename WT, size_t IN, typename IWT>
[[nodiscard]] constexpr posit<N, ES, WT> pow(const posit<N, ES, WT>& base,
                                             integer<IN, IWT> exponent)
{
    using Posit = posit<N, ES, WT>;

    // Handle special cases.

    if (base.is_nar())
    {
        return Posit::nar();
    }

    if (base.is_zero() && exponent.is_zero())
    {
        return Posit::nar();
    }

    if (exponent.is_zero())
    {
        return Posit::one();
    }

    // Do the iteration. If we have a positive exponent, we multiply by
    // base. If on the other hand, exponent is negative, we divide instead.

    const auto total_steps = expanding_abs(exponent);
    const bool exponent_negative = exponent.is_negative();

    Posit power = Posit::one();

    for (auto i = total_steps.zero(); i < total_steps; ++i)
    {
        if (exponent_negative)
        {
            power = power / base;
        }
        else
        {
            power = power * base;
        }
    }

    return power;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> max(const posit<N, ES, WT>& p, const posit<N, ES, WT>& q)
{
    if (p.is_nar())
    {
        return q;
    }

    if (q.is_nar())
    {
        return p;
    }

    if (p > q)
    {
        return p;
    }
    else
    {
        return q;
    }
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> fmax(const posit<N, ES, WT>& p, const posit<N, ES, WT>& q)
{
    return max(p, q);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> min(const posit<N, ES, WT>& p, const posit<N, ES, WT>& q)
{
    if (p.is_nar())
    {
        return q;
    }

    if (q.is_nar())
    {
        return p;
    }

    if (p < q)
    {
        return p;
    }
    else
    {
        return q;
    }
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> fmin(const posit<N, ES, WT>& p, const posit<N, ES, WT>& q)
{
    return min(p, q);
}

template <size_t N, size_t ES, typename WT>
void dump_meta(std::ostream& os, const posit<N, ES, WT>& p)
{
    const auto bits = p.get_bits();

    auto decoded = bits;
    if (p.is_negative())
    {
        decoded = negate(decoded);
    }

    const auto regime = get_regime_value(p);
    const auto exponent = get_exponent_value(p);
    const auto fraction = get_fraction_value(p);
    const auto scale = get_scale_value(p);

    os << to_binary(bits) << " decoded=" << to_binary(decoded) << " regime=" << regime
       << " exponent=" << exponent << " fraction=" << fraction << " scale=" << scale << std::endl;
    ;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] std::string dump_string(const posit<N, ES, WT>& p)
{
    std::stringstream ss;
    dump_meta(ss, p);
    return ss.str();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit_result<N, ES, WT> add(const posit<N, ES, WT>& lhs,
                                                    const posit<N, ES, WT>& rhs)
{
    const auto lhs_params = posit_parameters<N, ES, WT>(lhs);
    const auto rhs_params = posit_parameters<N, ES, WT>(rhs);
    const auto sum_params = lhs_params + rhs_params;

    return sum_params.to_posit();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit_result<N, ES, WT> mul(const posit<N, ES, WT>& lhs,
                                                    const posit<N, ES, WT>& rhs)
{
    const auto lhs_params = posit_parameters<N, ES, WT>(lhs);
    const auto rhs_params = posit_parameters<N, ES, WT>(rhs);
    const auto prod_params = lhs_params * rhs_params;

    return prod_params.to_posit();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit_result<N, ES, WT> div(const posit<N, ES, WT>& lhs,
                                                    const posit<N, ES, WT>& rhs)
{
    const auto lhs_params = posit_parameters<N, ES, WT>(lhs);
    const auto rhs_params = posit_parameters<N, ES, WT>(rhs);
    const auto quot_params = lhs_params / rhs_params;

    return quot_params.to_posit();
}

} // namespace aarith
