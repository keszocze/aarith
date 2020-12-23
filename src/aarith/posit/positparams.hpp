#pragma once

#include <aarith/posit.hpp>
#include <sstream>

namespace aarith {

template <size_t N, size_t ES, typename WT>
constexpr positparams<N, ES, WT>::positparams(const posit<N, ES, WT>& p)
{
    // posit -> positparams

    if (p.is_nar())
    {
        is_nar = true;
        is_zero = false;
    }
    else if (p.is_zero())
    {
        is_nar = false;
        is_zero = true;
    }
    else
    {
        is_nar = false;
        is_zero = false;
        sign_bit = p.is_negative();
        scale = get_scale_value(p);
        fraction = fractional(p);
    }
}

template <size_t N, size_t ES, typename WT>
constexpr positparams<N, ES, WT>::positparams(const positparams<N, ES, WT>& other)
    : is_nar(other.is_nar)
    , is_zero(other.is_zero)
    , sign_bit(other.sign_bit)
    , scale(other.scale)
    , fraction(other.fraction)
{
}

template <size_t N, size_t ES, typename WT>
constexpr positparams<N, ES, WT>&
positparams<N, ES, WT>::operator=(const positparams<N, ES, WT>& other)
{
    is_nar = other.is_nar;
    is_zero = other.is_zero;
    sign_bit = other.sign_bit;
    scale = other.scale;
    fraction = other.fraction;

    return *this;
}

template <size_t N, size_t ES, typename WT> positparams<N, ES, WT>::~positparams()
{
}

template <size_t N, size_t ES, typename WT>
constexpr positparams<N, ES, WT>::operator posit<N, ES, WT>() const
{
    // positparams -> posit

    using Posit = posit<N, ES, WT>;
    using Integer = integer<N, WT>;

    //
    // if the result is nar or zero, things are easy
    //

    if (is_nar)
    {
        return Posit::nar();
    }

    if (is_zero)
    {
        return Posit::zero();
    }

    //
    // compute parameters
    //

    constexpr Integer powes = Integer(1 << ES);
    constexpr ssize_t es = static_cast<ssize_t>(ES);

    const Integer regime = floordiv(scale, powes);
    const Integer exponent = absmod(scale, powes);

    //
    // set bits; keep count of the currently bit being set with 'i'
    //

    uinteger<N + ES + 3> bits;
    ssize_t i = bits.width() - 1;

    //
    // set sign bit; as we are dealing with the absolute value, there is
    // actually nothing to do but increment i
    //

    i -= 1;

    //
    // set regime bits
    //

    Integer nregime;
    bool first_regime_bit;

    if (scale < scale.zero())
    {
        // north east

        nregime = abs(regime) + regime.one();
        first_regime_bit = false;
    }
    else
    {
        // south east

        nregime = regime + regime.one() + regime.one();
        first_regime_bit = true;
    }

    for (auto ridx = nregime.zero(); ridx < nregime && i >= 0; ++ridx, --i)
    {
        const bool last_regime_bit = (ridx == (nregime - nregime.one()));

        if (last_regime_bit)
        {
            bits.set_bit(i, !first_regime_bit);
        }
        else
        {
            bits.set_bit(i, first_regime_bit);
        }
    }

    //
    // set exponent bits
    //

    const ssize_t nexp = es;
    assert(nexp == es); // should always have enough space in bits as it's extra large

    for (ssize_t eprinted = 0; eprinted < nexp && i >= 0; ++eprinted, --i)
    {
        const ssize_t eidx = es - 1 - eprinted;
        const auto exponent_bit = exponent.bit(eidx);
        bits.set_bit(i, exponent_bit);
    }

    //
    // set fraction bits
    //

    const auto fraction_bits = fraction.fraction_bits();

    for (ssize_t fidx = N - 1; fidx >= 0 && i >= 0; --fidx, --i)
    {
        const auto fraction_bit = fraction_bits.bit(fidx);
        bits.set_bit(i, fraction_bit);
    }

    //
    // construct posit and parameters for rounding
    //

    const uinteger<N, WT> posit_bits = width_cast<N>(bits >> (ES + 3));
    const uinteger<ES + 3, WT> truncated = width_cast<ES + 3>(bits);

    const Posit x = Posit::from_bits(posit_bits);
    const positparams<N, ES, WT> Px(x);

    const bool last = posit_bits.bit(0);
    const bool after = truncated.bit(truncated.width() - 1);
    const bool sticky = !width_cast<ES + 3 - 1>(truncated).is_zero(); // maybe -2 instead of - 1...

    //
    // do exponent rounding
    //

    if ((last && after) || (after && sticky))
    {
        if ((last && after))
        {
            return x.incremented_real();
        }
    }

    //
    // do fraction rounding
    //

    if (fraction != Px.fraction)
    {
        if ((last && after) || (after && sticky))
        {
            return x.incremented_real();
        }
    }

    //
    // return; set sign bit if necessary
    //

    if (sign_bit)
    {
        return -x;
    }
    else
    {
        return x;
    }
}

template <size_t N, size_t ES, typename WT>
bool positparams<N, ES, WT>::operator==(const positparams<N, ES, WT>& other) const
{
    return is_nar == other.is_nar && is_zero == other.is_zero && sign_bit == other.sign_bit &&
           scale == other.scale && fraction == other.fraction;
}

template <size_t N, size_t ES, typename WT>
bool positparams<N, ES, WT>::operator!=(const positparams<N, ES, WT>& other) const
{
    return !(*this == other);
}

template <size_t N, size_t ES, typename WT>
positparams<N, ES, WT> positparams<N, ES, WT>::operator+(const positparams<N, ES, WT>& other) const
{
    //
    // first we handle special cases; zero as arguments and result in
    // particular are always kind of special when it comes to posit arithmetic
    //

    if (is_nar || other.is_nar)
    {
        return *this;
    }

    if (is_zero)
    {
        return other;
    }

    if (other.is_zero)
    {
        return *this;
    }

    const bool value_equal = (scale == other.scale && fraction == other.fraction);
    const bool product_sign = sign_bit ^ other.sign_bit;

    if (value_equal && product_sign)
    {
        return zero();
    }

    //
    // create copies for working on; this might seem wasteful but remember
    // that these objects are only as big as some integers
    //

    positparams<N, ES, WT> lhs = *this;
    positparams<N, ES, WT> rhs = other;

    //
    // do addition
    //

    match_scale_of(lhs, rhs);

    positparams<N, ES, WT> sum;
    sum_fractions(sum, lhs, rhs);

    return sum;
}

template <size_t N, size_t ES, typename WT>
constexpr positparams<N, ES, WT>::positparams()
    : is_nar(false)
    , is_zero(false)
    , sign_bit(false)
{
}

template <size_t N, size_t ES, typename WT> positparams<N, ES, WT> positparams<N, ES, WT>::zero()
{
    positparams<N, ES, WT> result;
    result.is_zero = true;
    return result;
}

template <size_t N, size_t ES, typename WT>
std::tuple<positparams<N, ES, WT>*, positparams<N, ES, WT>*>
positparams<N, ES, WT>::ordered(positparams<N, ES, WT>* p, positparams<N, ES, WT>* q)
{
    if (p->scale > q->scale)
    {
        return std::make_tuple(p, q);
    }
    else
    {
        return std::make_tuple(q, p);
    }
}

template <size_t N, size_t ES, typename WT>
void positparams<N, ES, WT>::match_scale_of(positparams<N, ES, WT>& p, positparams<N, ES, WT>& q)
{
    auto [bigger, smaller] = ordered(&p, &q);

    const auto scale_diff = uinteger<N, WT>(bigger->scale - smaller->scale);

    smaller->scale = bigger->scale;
    smaller->fraction = smaller->fraction >> scale_diff;
}

template <size_t N, size_t ES, typename WT>
void positparams<N, ES, WT>::sum_fractions(positparams<N, ES, WT>& dest,
                                           const positparams<N, ES, WT>& lhs,
                                           const positparams<N, ES, WT>& rhs)
{
    dest.is_nar = false;
    dest.is_zero = false;
    dest.sign_bit = false;

    assert(lhs.scale == rhs.scale);
    dest.scale = lhs.scale; // == rhs.scale

    if (lhs.sign_bit && rhs.sign_bit)
    {
        // (-p) + (-q) == - (p + q)

        add_fractions(dest, lhs.fraction, rhs.fraction);
        dest.sign_bit = true;
    }
    else if (!lhs.sign_bit && !rhs.sign_bit)
    {
        // (+p) + (+q) == p + q

        add_fractions(dest, lhs.fraction, rhs.fraction);
        dest.sign_bit = false;
    }
    else if (!lhs.sign_bit && rhs.sign_bit)
    {
        // (+p) + (-q)

        if (lhs.fraction > rhs.fraction)
        {
            sub_fractions(dest, lhs.fraction, rhs.fraction);
            dest.sign_bit = false;
        }
        else if (lhs.fraction == rhs.fraction)
        {
            dest.fraction = dest.fraction.zero();
            dest.is_zero = true;
            dest.sign_bit = false;
        }
        else
        {
            assert(lhs.fraction < rhs.fraction);

            sub_fractions(dest, rhs.fraction, lhs.fraction);
            dest.sign_bit = true;
        }
    }
    else
    {
        assert(lhs.sign_bit && !rhs.sign_bit);

        // (-p) + (+q)

        if (lhs.fraction < rhs.fraction)
        {
            sub_fractions(dest, rhs.fraction, lhs.fraction);
            dest.sign_bit = false;
        }
        else if (lhs.fraction == rhs.fraction)
        {
            dest.fraction = dest.fraction.zero();
            dest.is_zero = true;
            dest.sign_bit = false;
        }
        else
        {
            assert(lhs.fraction > rhs.fraction);

            sub_fractions(dest, lhs.fraction, rhs.fraction);
            dest.sign_bit = true;
        }
    }
}

template <size_t N, size_t ES, typename WT>
void positparams<N, ES, WT>::add_fractions(positparams<N, ES, WT>& dest,
                                           const fractional<N, ES, WT>& lfrac,
                                           const fractional<N, ES, WT>& rfrac)
{
    dest.fraction = lfrac + rfrac;

    // handle overflow

    if (dest.fraction.integer_bits().bit(1))
    {
        dest.fraction = dest.fraction >> 1;
        dest.scale = dest.scale + dest.scale.one();
    }
}

template <size_t N, size_t ES, typename WT>
void positparams<N, ES, WT>::sub_fractions(positparams<N, ES, WT>& dest,
                                           const fractional<N, ES, WT>& lfrac,
                                           const fractional<N, ES, WT>& rfrac)
{
    dest.fraction = lfrac - rfrac;

    // TODO (Schärtl): handle underflow(?)
}

template <size_t SN, size_t SES, typename SWT>
std::ostream& operator<<(std::ostream& os, const positparams<SN, SES, SWT>& p)
{
    return os << "(nar=" << p.is_nar << " is_zero=" << p.is_zero << " sign=" << p.sign_bit
              << " scale=" << p.scale << " fraction=" << p.fraction << ")";
}

} // namespace aarith
