#pragma once

#include <aarith/core/word_array.hpp>
#include <aarith/integer.hpp>
#include <cassert>
#include <cstdint>
#include <cstdlib>

namespace aarith {

template <size_t N, size_t ES, class WT>
constexpr posit<N, ES, WT> posit<N, ES, WT>::from_bits(const posit<N, ES, WT>::storage_type& bits)
{
    posit p;
    p.bits = bits;

    return p;
}

template <size_t N, size_t ES, class WT>
constexpr posit<N, ES, WT>::posit()
    : bits(0)
{
    static_assert_template_parameters();
}

template <size_t N, size_t ES, class WT>
constexpr posit<N, ES, WT>::posit(const posit& other)
    : bits(other.bits)
{
    static_assert_template_parameters();
}

template <size_t N, size_t ES, class WT>
constexpr posit<N, ES, WT>::posit(const posit&& other)
    : bits(std::move(other.bits))
{
    static_assert_template_parameters();
}

template <size_t N, size_t ES, class WT>
constexpr posit<N, ES, WT>& posit<N, ES, WT>::operator=(const posit& other)
{
    bits = other.bits;
    return *this;
}

template <size_t N, size_t ES, class WT>
constexpr posit<N, ES, WT>& posit<N, ES, WT>::operator=(const posit&& other)
{
    bits = std::move(other.bits);
    return *this;
}

template <size_t N, size_t ES, class WT>
constexpr posit<N, ES, WT>::posit(WT n)
    : bits(n)
{
}

template <size_t N, size_t ES, class WT>
constexpr bool posit<N, ES, WT>::operator==(const posit& other) const
{
    return bits == other.bits;
}

template <size_t N, size_t ES, class WT>
constexpr bool posit<N, ES, WT>::operator!=(const posit& other) const
{
    return !(*this == other);
}

template <size_t N, size_t ES, class WT>
constexpr bool posit<N, ES, WT>::operator<(const posit& other) const
{
    // special case complex infinity

    const auto inf = complex_infinity();

    // TODO (Schärtl) evtl. eine .is_inf() Methode hinzufügen?

    if (*this == inf || other == inf)
    {
        return false;
    }

    // regular cases

    auto mybits = get_bits();
    auto otherbits = other.get_bits();

    if (mybits.msb() && otherbits.msb())
    {
        mybits = twos_complement(mybits);
        otherbits = twos_complement(otherbits);

        return mybits > otherbits;
    }
    else if ((!mybits.msb()) && (!otherbits.msb()))
    {
        return mybits < otherbits;
    }
    else if (mybits.msb() && (!otherbits.msb()))
    {
        return true;
    }
    else
    {
        assert((!mybits.msb()) && otherbits.msb());
        return false;
    }
}

template <size_t N, size_t ES, class WT>
constexpr bool posit<N, ES, WT>::operator<=(const posit& other) const
{
    return (*this == other) || (*this < other);
}

template <size_t N, size_t ES, class WT>
constexpr bool posit<N, ES, WT>::operator>(const posit& other) const
{
    return other < *this;
}

template <size_t N, size_t ES, class WT>
constexpr bool posit<N, ES, WT>::operator>=(const posit& other) const
{
    return (*this == other) || (*this > other);
}

template <size_t N, size_t ES, class WT>
constexpr posit<N, ES, WT>& posit<N, ES, WT>::operator+() const
{
    return *this;
}

template <size_t N, size_t ES, class WT>
posit<N, ES, WT> posit<N, ES, WT>::operator+(const posit<N, ES, WT>& rhs) const
{
    using Posit = posit<N, ES, WT>;

    constexpr Posit zero = Posit::zero();
    constexpr Posit inf = Posit::complex_infinity();

    const Posit& lhs = *this;

    // special cases

    if (rhs == inf || lhs == inf)
    {
        return inf;
    }

    if (lhs == zero)
    {
        return rhs;
    }

    if (rhs == zero)
    {
        return lhs;
    }

    // regular cases

    const binprod<N> lhs_binprod(lhs);
    const binprod<N> rhs_binprod(rhs);

    const binprod<N> bsum = lhs_binprod + rhs_binprod;
    Posit psum = bsum.template to_posit<N, ES, WT>();

    // fix overflows

    if (rhs > zero && psum < lhs)
    {
        psum = psum.max();
    }

    // fix underflows

    if (rhs < zero && psum > lhs)
    {
        psum = psum.min();
    }

    return psum;
}

template <size_t N, size_t ES, class WT>
posit<N, ES, WT>& posit<N, ES, WT>::operator+=(const posit<N, ES, WT>& rhs)
{

    posit<N, ES, WT> sum = *this + rhs;
    this->bits = sum.get_bits();
    return *this;
}

template <size_t N, size_t ES, class WT> posit<N, ES, WT>& posit<N, ES, WT>::operator++()
{
    // ++x
    *this += this->one();
    return *this;
}

template <size_t N, size_t ES, class WT> posit<N, ES, WT> posit<N, ES, WT>::operator++(int)
{
    // x++
    auto copy = *this;
    *this += this->one();
    return *this;
}

template <size_t N, size_t ES, class WT>
constexpr posit<N, ES, WT> posit<N, ES, WT>::operator-() const
{
    if (*this == zero())
    {
        return *this;
    }

    if (*this == complex_infinity())
    {
        return *this;
    }

    const auto bits = twos_complement(this->get_bits());
    return posit<N, ES, WT>::from_bits(bits);
}

template <size_t N, size_t ES, class WT>
constexpr posit<N, ES, WT> posit<N, ES, WT>::operator-(const posit<N, ES, WT>& rhs) const
{
    return *this + (-rhs);
}

template <size_t N, size_t ES, class WT>
posit<N, ES, WT>& posit<N, ES, WT>::operator-=(const posit<N, ES, WT>& rhs)
{
    posit<N, ES, WT> sum = *this - rhs;
    this->bits = sum.get_bits();
    return *this;
}

template <size_t N, size_t ES, class WT> posit<N, ES, WT>& posit<N, ES, WT>::operator--()
{
    // --x
    *this -= one();
    return *this;
}

template <size_t N, size_t ES, class WT> posit<N, ES, WT> posit<N, ES, WT>::operator--(int)
{
    // x--
    auto copy = *this;
    *this -= this->one();
    return *this;
}

template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::min()
{
    // the min value is represented by 10..01

    posit p;

    p.bits.set_bit(N - 1, true);
    p.bits.set_bit(0, true);

    return p;
}

template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::minpos()
{
    // minpos is presented by 0..01

    posit p;

    p.bits.set_bit(0, true);

    return p;
}

template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::max()
{
    // the maximum value is represented by 01..1

    posit p;

    p.bits = p.bits.all_ones();
    p.bits.set_bit(N - 1, false);

    return p;
}

template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::zero()
{
    // zero is represented by all bits set to zero

    return posit();
}

template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::one()
{
    // one is represented by 010...0

    posit p;

    p.bits.set_bit(N - 2, true);

    return p;
}

template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::complex_infinity()
{
    // complex infinity is sign bit set to one and all
    // other bits set to zero

    posit p;
    p.bits.set_bit(N - 1, true);

    return p;
}

template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr uinteger<N + 1, WT> posit<N, ES, WT>::npat()
{
    // to support arbitrary template parameters, we need to be able to
    // return arbitrary large sizes; so instead of size_t we use uinteger
    // as a return type here

    using SizeType = uinteger<N + 1, WT>;
    const SizeType one = SizeType::one();

    return one << N;
}

template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr typename posit<N, ES, WT>::useed_type posit<N, ES, WT>::useed()
{
    return useed_type(1 << (1 << ES));
}

template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr bool posit<N, ES, WT>::is_negative() const
{
    if (*this == complex_infinity())
    {
        return false;
    }

    return get_sign_bit() == storage_type(1);
}

template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr typename posit<N, ES, WT>::storage_type
posit<N, ES, WT>::get_sign_bit() const
{
    return storage_type(bits.msb());
}

template <size_t N, size_t ES, class WT>
[[nodiscard]] constexpr typename posit<N, ES, WT>::storage_type posit<N, ES, WT>::get_bits() const
{
    return bits;
}

template <size_t N, size_t ES, class WT>
constexpr void posit<N, ES, WT>::static_assert_template_parameters() const
{
    static_assert(N >= 2, "number of bits needs to be at least 2");
    static_assert(ES <= 5, "exponent unreasonably large");
}

} // namespace aarith
