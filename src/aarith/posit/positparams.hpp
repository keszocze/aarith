#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
constexpr positparams<N, ES, WT>::positparams(const posit<N, ES, WT>& p)
{
    is_nar = p.is_nar();
    is_zero = p.is_zero();
    sign_bit = p.is_negative();
    scale = get_scale_value(p);
    fraction = fractional(p);

    ensure_standard_form();
}

template <size_t N, size_t ES, typename WT>
template <size_t ValueWidth, typename ValueWordType>
constexpr positparams<N, ES, WT>::positparams(const integer<ValueWidth, ValueWordType>& value)
{
    //
    // initalize flags
    //

    is_nar = false;
    is_zero = value.is_zero();
    sign_bit = value.is_negative();

    //
    // handle edge cases
    //

    if (value.is_zero())
    {
        return;
    }

    //
    // convert integer to fraction
    //

    const auto abs_value = expanding_abs(value);

    constexpr size_t fs = decltype(fraction)::FractionSize;
    const auto intfraction = width_cast<fs>(abs_value);

    const auto leading_zeroes = count_leading_zeroes(intfraction);
    fraction = fractional<N, ES, WT>(intfraction << leading_zeroes);

    //
    // set correct scale
    //

    scale = ilog2(abs_value) + abs_value.one();

    //
    // normalize
    //

    ensure_standard_form();
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
positparams<N, ES, WT>& positparams<N, ES, WT>::operator=(const positparams<N, ES, WT>& other)
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
[[nodiscard]] constexpr positparams<N, ES, WT>::operator posit<N, ES, WT>() const
{
    using Posit = posit<N, ES, WT>;
    using Integer = integer<N, WT>;

    //
    // If the result is nar or zero, things are easy.
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
    // Compute parameters necessary to construct the posit.
    //

    constexpr Integer powes = Integer(1 << ES);
    constexpr ssize_t es = static_cast<ssize_t>(ES);

    const Integer regime = floordiv(scale, powes);
    const Integer exponent = absmod(scale, powes);

    //
    // Set bits. Keep count of the currently bit being set with 'i'.
    //

    uinteger<N + ES + N> bits;
    ssize_t i = bits.width() - 1;

    //
    // Set sign bit. As we are dealing with the absolute value and only set
    // the sign bit at the end, there is actually nothing to do but increment
    // the counter i.
    //

    i -= 1;

    //
    // Set regime bits.
    //

    Integer nregime;
    bool first_regime_bit = false;

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
    // Set exponent bits.
    //

    for (ssize_t eprinted = 0; eprinted < es && i >= 0; ++eprinted, --i)
    {
        const ssize_t eidx = es - 1 - eprinted;
        const auto exponent_bit = exponent.bit(eidx);
        bits.set_bit(i, exponent_bit);
    }

    //
    // Set fraction bits.
    //

    const auto fraction_bits = fraction.fraction_bits();

    for (ssize_t fidx = fraction_bits.width() - 1; fidx >= 0 && i >= 0; --fidx, --i)
    {
        const auto fraction_bit = fraction_bits.bit(fidx);
        bits.set_bit(i, fraction_bit);
    }

    //
    // Split up bitstring into "posit_bits" which we use to construct the
    // posit and "truncated" which we have to take a look at for rounding.
    //

    constexpr size_t posit_bits_width = N;
    constexpr size_t truncated_width = bits.width() - posit_bits_width;
    constexpr size_t tail_width = truncated_width - 1;

    const uinteger<posit_bits_width, WT> posit_bits =
        width_cast<posit_bits_width>(bits >> truncated_width);
    const uinteger<truncated_width, WT> truncated = width_cast<truncated_width>(bits);
    const uinteger<tail_width, WT> tail = width_cast<tail_width>(truncated);

    //
    // Construct the unrounded posit.
    //

    Posit x = Posit::from(posit_bits);

    //
    // Do rounding if necessary.
    //

    const bool last = posit_bits.bit(0);
    const bool after = truncated.bit(truncated.width() - 1);
    const bool tailbit = !tail.is_zero();

    if ((last && after) || (after && tailbit))
    {
        x = x.incremented_real();
    }

    //
    // Handle special case zero in return. Zero should only be returned when
    // the result is actually zero (e.g. when multiplying with zero). When the
    // result is very close to zero, but not quite zero, we have to return
    // minpos instead to indicate that the result is something close but not
    // quite zero.
    //

    if (x.is_zero())
    {
        assert(!is_zero); // already taken care of at the beginning
        x = x.minpos();
    }

    //
    // Apply twos complement for negative values.
    //

    if (sign_bit)
    {
        x = -x;
    }

    //
    // return
    //

    return x;
}

template <size_t N, size_t ES, typename WT>
template <size_t TargetWidth, typename TargetWordType>
[[nodiscard]] constexpr positparams<N, ES, WT>::operator integer<TargetWidth, TargetWordType>()
    const
{
    // We need at least one sign bit and one "payload bit". Honestly for small
    // values like N=2 or N=3 we will hardly get reasonable results in most
    // cases, but it is up to the user to decide what they want.
    static_assert(TargetWidth >= 2, "conversion to integer requires at least two integer bits");

    using TargetType = integer<TargetWidth, TargetWordType>;

    //
    // handle special values zero and NaR
    //

    if (is_zero)
    {
        return TargetType::zero();
    }

    if (is_nar)
    {
        // NaR cannot be represented by an integer; unlike floating points
        // there is no NaN or smiliar to return
        throw nar_error("tried to convert NaR to integer");
    }

    //
    // handle regular case
    //

    // To get the integer, all we really need to do is take the fraction and
    // shift it to fit the scale. Say we represent a value of
    //
    //   1.01010010 ✕ 2³.
    //
    // We first set variable "intfrac" to represent that number without a
    // seperation dot, e.g.
    //
    //   101010010 ✕ 2³⁻⁸,
    //
    // where we subtract 8 from the scale as the dot was shifted 8 places
    // (FractionSize) to the right.

    constexpr integer<N, WT> fraction_size(fraction.FractionSize);

    auto intfrac = fraction.scratch_bits();
    integer<N, WT> intscale = scale - fraction_size;

    // Now as we want to return an integer, we only have to shift "intfrac" such
    // that intscale is zero. This might result in bits being truncated.

    const auto shift = expanding_abs(intscale);

    if (intscale.is_negative())
    {
        intfrac = intfrac >> shift;
    }
    else
    {
        intfrac = intfrac << shift;
    }

    // Truncate to expected size. From the TargetWidth-many bits we can
    // use TargetWidth - 1 bits to represent the absolute value. One bit
    // is reserved for the sign which is masked away.

    TargetType abs_result = width_cast<TargetType::width()>(intfrac);
    abs_result.set_bit(intfrac.width() - 1, false);

    // Now that we have constructed the absolute value, we can handle negative
    // values and return.

    if (sign_bit)
    {
        return twos_complement(abs_result);
    }
    else
    {
        return abs_result;
    }
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool
positparams<N, ES, WT>::operator==(const positparams<N, ES, WT>& other) const
{
    return is_nar == other.is_nar && is_zero == other.is_zero && sign_bit == other.sign_bit &&
           scale == other.scale && fraction == other.fraction;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool
positparams<N, ES, WT>::operator!=(const positparams<N, ES, WT>& other) const
{
    return !(*this == other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr positparams<N, ES, WT>
positparams<N, ES, WT>::operator+(const positparams<N, ES, WT>& other) const
{
    //
    // special arguments, that is either NaR or zero, both of which are weird
    // and special
    //

    if (is_nar || other.is_nar)
    {
        return nar();
    }

    if (is_zero)
    {
        return other;
    }

    if (other.is_zero)
    {
        return *this;
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

    //
    // return standard representation
    //

    sum.ensure_standard_form();
    return sum;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr positparams<N, ES, WT>
positparams<N, ES, WT>::operator*(const positparams<N, ES, WT>& other) const
{
    //
    // handle special cases NaR and zero
    //

    if (is_nar || other.is_nar)
    {
        return nar();
    }

    if (is_zero || other.is_zero)
    {
        return zero();
    }

    //
    // do multiplication
    //

    positparams<N, ES, WT> lhs = *this;
    positparams<N, ES, WT> rhs = other;
    positparams<N, ES, WT> prod;

    prod.sign_bit = lhs.sign_bit ^ rhs.sign_bit;
    prod.scale = lhs.scale + rhs.scale;
    prod.fraction = lhs.fraction * rhs.fraction;

    prod.ensure_standard_form();

    return prod;
}

template <size_t N, size_t ES, typename WT>
constexpr positparams<N, ES, WT>::positparams()
    : is_nar(false)
    , is_zero(false)
    , sign_bit(false)
{
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr positparams<N, ES, WT> positparams<N, ES, WT>::zero()
{
    positparams<N, ES, WT> result;
    result.is_zero = true;
    return result;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr positparams<N, ES, WT> positparams<N, ES, WT>::nar()
{
    positparams<N, ES, WT> result;
    result.is_nar = true;
    return result;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr positparams<N, ES, WT> positparams<N, ES, WT>::minpos()
{
    return positparams(posit<N, ES, WT>::minpos());
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

    // handle overflow aka normalize

    while (dest.fraction.integer_bits().bit(1))
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

    // handle underflow aka normalize

    if (!dest.fraction.fraction_bits().is_zero())
    {
        while (!dest.fraction.integer_bits().bit(0))
        {
            dest.fraction = dest.fraction << 1;
            dest.scale = dest.scale - dest.scale.one();
        }
    }
}

template <size_t SN, size_t SES, typename SWT>
std::ostream& operator<<(std::ostream& os, const positparams<SN, SES, SWT>& p)
{
    return os << "(nar=" << p.is_nar << " is_zero=" << p.is_zero << " sign=" << p.sign_bit
              << " scale=" << p.scale << " fraction=" << p.fraction << ")";
}

template <size_t N, size_t ES, typename WT> void positparams<N, ES, WT>::ensure_standard_form()
{
    if (fraction.integer_bits())
    {
        // integer part is non-zero; shift fraction around such that we get
        // the standard form 0001.xxxx

        constexpr auto hidden_bit_format = decltype(fraction.integer_bits())::one();

        while (fraction.integer_bits() != hidden_bit_format)
        {
            fraction = fraction >> 1;
            scale = scale + scale.one();
        }
    }
    else if (fraction.fraction_bits())
    {
        // the integer part is zero, but the fraction part is non-zero; shift
        // fraction around such that we get the standard form 0001.xxxx

        assert(!fraction.integer_bits());

        while (!fraction.integer_bits().lsb())
        {
            fraction = fraction << 1;
            scale = scale - scale.one();
        }
    }
}

} // namespace aarith
