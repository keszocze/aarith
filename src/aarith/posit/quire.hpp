#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr quire<N, ES, WT> quire<N, ES, WT>::zero()
{
    throw std::logic_error("quire::zero not implemented");
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr quire<N, ES, WT> quire<N, ES, WT>::nar()
{
    throw std::logic_error("quire::nar not implemented");
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
[[nodiscard]] constexpr quire<N, ES, WT>::quire(const quire<N, ES, WT>& other)
    : value(other.value)
{
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr quire<N, ES, WT>::quire(const posit<N, ES, WT>& p)
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

    // Represent as parameterized posit for easy conversion.

    const posit_parameters parameterized(p);

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

    // Now we can finally construct the fixed point value.

    value = decltype(value)::from_bitstring(quire_full);

    // TODO (Schärt): Set sign bit in fixed point number
}

template <size_t N, size_t ES, typename WT>
quire<N, ES, WT>& quire<N, ES, WT>::operator=(const quire<N, ES, WT>& other)
{
    value = other.value;
    return *this;
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
[[nodiscard]] constexpr const typename quire<N, ES, WT>::storage_type& quire<N, ES, WT>::get_bits() const
{
    return value;
}

} // namespace aarith
