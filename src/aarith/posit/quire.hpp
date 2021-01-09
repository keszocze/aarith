#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr quire<N, ES, WT> quire<N, ES, WT>::zero()
{
    return quire();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr quire<N, ES, WT> quire<N, ES, WT>::nar()
{
    quire q;

    q.value.set_bit(q.value.width - 1, true);

    return q;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr quire<N, ES, WT>
quire<N, ES, WT>::from(const quire<N, ES, WT>::storage_type& bits)
{
    quire q;
    q.value = bits;

    return q;
}

template <size_t N, size_t ES, typename WT> constexpr quire<N, ES, WT>::quire()
{
}

template <size_t N, size_t ES, typename WT>
constexpr quire<N, ES, WT>::quire(const quire<N, ES, WT>& other)
    : value(other.value)
{
}

template <size_t N, size_t ES, typename WT>
constexpr quire<N, ES, WT>::quire(const posit<N, ES, WT>& p)
{
    //
    // Handle special cases.
    //

    if (p.is_zero())
    {
        *this = zero();
        return;
    }

    if (p.is_nar())
    {
        *this = nar();
        return;
    }

    //
    // Handle regular cases.
    //

    // Represent as parameterized posit for easy conversion. We work with the
    // absolute value as that makes the conversion easier (less special
    // cases). At the end, we flip the sign if necessary.

    const posit_parameters parameterized(abs(p));

    // Construct integer and fraction parts of the fixed point value.  After
    // converting a posit to parameterized version, each non-special value
    // will have the integer part of the fraction set to 1 as every
    // non-special posit has a fraction 1.xxx.

    const auto posit_fraction = parameterized.get_fraction();
    const auto posit_fraction_bits = posit_fraction.fraction_bits();

    constexpr size_t non_fraction_width = QuireSize - FractionSize;
    const auto quire_integer = uinteger<non_fraction_width, WT>::one();

    uinteger<FractionSize> quire_fraction;

    if constexpr (FractionSize <= posit_fraction_bits.width())
    {
        constexpr size_t fraction_shift = posit_fraction_bits.width() - FractionSize;
        quire_fraction = width_cast<FractionSize>(posit_fraction_bits >> fraction_shift);
    }
    else
    {
        constexpr size_t fraction_shift = FractionSize - posit_fraction_bits.width();
        quire_fraction = width_cast<FractionSize>(posit_fraction_bits << fraction_shift);
    }

    // Construct bits for fixed point value with correct width. Then shift
    // according to scale.

    auto quire_full = concat(quire_integer, quire_fraction);

    const auto posit_scale = parameterized.get_scale();
    const auto abs_posit_scale = expanding_abs(posit_scale);

    if (posit_scale.is_negative())
    {
        quire_full = quire_full >> abs_posit_scale;
    }
    else
    {
        quire_full = quire_full << abs_posit_scale;
    }

    // Now we can construct the fixed point value.

    value = decltype(value)::from_bitstring(quire_full);

    if (p.is_negative())
    {
        value = -value;
    }
}

template <size_t N, size_t ES, typename WT>
quire<N, ES, WT>& quire<N, ES, WT>::operator=(const quire<N, ES, WT>& other)
{
    value = other.value;
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr quire<N, ES, WT>::operator posit<N, ES, WT>() const
{
    using Posit = posit<N, ES, WT>;
    using Params = posit_parameters<N, ES, WT>;
    using ParamsScale = typename Params::scale_type;
    using ParamsFraction = typename Params::fraction_type;

    // handle special cases

    if (is_zero())
    {
        return Posit::zero();
    }

    if (is_nar())
    {
        return Posit::nar();
    }

    // handle regular cases; start out by setting up params object

    auto param_fraction = ParamsFraction::zero();
    auto param_scale = ParamsScale::zero();

    // shift all fraction bits into the fractional part; this makes
    // for easier importing; sets the param_scale parameter

    auto myfixed = abs(value);

    while (myfixed.integer_part())
    {
        myfixed = myfixed >> 1;
        param_scale = param_scale + param_scale.one();
    }

    // create fraction bits for posit_fraction object; sets the param_fraction
    // parameter

    constexpr size_t myfixed_size = decltype(value)::frac_width;
    constexpr size_t param_fraction_size = param_fraction.FractionSize;

    uinteger<param_fraction_size, WT> param_fraction_bits;

    if constexpr (myfixed_size > param_fraction_size)
    {
        constexpr size_t shift = myfixed_size - param_fraction_size;
        param_fraction_bits = width_cast<param_fraction_size>(myfixed.fractional_part() << shift);
    }
    else
    {
        assert(param_fraction_size >= myfixed_size);

        constexpr size_t shift = param_fraction_size - myfixed_size;
        param_fraction_bits = width_cast<param_fraction_size>(myfixed.fractional_part()) << shift;
    }

    param_fraction = posit_fraction<N, ES, WT>(param_fraction_bits);

    // construct params

    const bool param_is_nar = false;
    const bool param_is_zero = false;
    const bool param_sign_bit = value.is_negative();

    auto params =
        posit_parameters<N, ES, WT>::from(param_is_nar, param_is_zero, param_sign_bit,
            param_scale, param_fraction);

    // return

    return posit<N, ES, WT>(params);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool quire<N, ES, WT>::is_zero() const
{
    return (*this == zero());
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool quire<N, ES, WT>::is_nar() const
{
    return (*this == nar());
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr const typename quire<N, ES, WT>::storage_type&
quire<N, ES, WT>::get_bits() const
{
    return value;
}

} // namespace aarith
