#pragma once

#include <aarith/core/word_array.hpp>
#include <aarith/integer.hpp>
#include <cassert>
#include <cstdint>
#include <cstdlib>

namespace aarith {

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT> posit<N, ES, WT>::from_bits(const posit<N, ES, WT>::storage_type& bits)
{
    posit p;
    p.bits = bits;

    return p;
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>::posit()
    : bits(0)
{
    static_assert_template_parameters();
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>::posit(const posit& other)
    : bits(other.bits)
{
    static_assert_template_parameters();
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>::posit(const posit&& other)
    : bits(std::move(other.bits))
{
    static_assert_template_parameters();
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>& posit<N, ES, WT>::operator=(const posit& other)
{
    bits = other.bits;
    return *this;
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>& posit<N, ES, WT>::operator=(const posit&& other)
{
    bits = std::move(other.bits);
    return *this;
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>::posit(WT n)
    : bits(n)
{
}

template <size_t N, size_t ES, typename WT>
constexpr bool posit<N, ES, WT>::operator==(const posit& other) const
{
    return bits == other.bits;
}

template <size_t N, size_t ES, typename WT>
constexpr bool posit<N, ES, WT>::operator!=(const posit& other) const
{
    return !(*this == other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] posit<N, ES, WT> posit<N, ES, WT>::incremented() const
{
    auto bits = get_bits();
    return from_bits(bits + bits.one());
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] posit<N, ES, WT> posit<N, ES, WT>::decremented() const
{
    auto bits = get_bits();
    return from_bits(bits - bits.one());
}

template <size_t N, size_t ES, typename WT>
constexpr bool posit<N, ES, WT>::operator<(const posit& other) const
{
    // special case NaR

    const auto inf = nar();

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

template <size_t N, size_t ES, typename WT>
constexpr bool posit<N, ES, WT>::operator<=(const posit& other) const
{
    return (*this == other) || (*this < other);
}

template <size_t N, size_t ES, typename WT>
constexpr bool posit<N, ES, WT>::operator>(const posit& other) const
{
    return other < *this;
}

template <size_t N, size_t ES, typename WT>
constexpr bool posit<N, ES, WT>::operator>=(const posit& other) const
{
    return (*this == other) || (*this > other);
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>& posit<N, ES, WT>::operator+() const
{
    return *this;
}

template <size_t N, size_t ES, typename WT>
posit<N, ES, WT> posit<N, ES, WT>::operator+(const posit<N, ES, WT>& rhs) const
{
    using Posit = posit<N, ES, WT>;

    constexpr Posit zero = Posit::zero();
    constexpr Posit inf = Posit::nar();

    const Posit& lhs = *this;

    // special cases

    if (rhs == inf || lhs == inf)
    {
        return inf;
    }

    if (lhs.is_zero())
    {
        return rhs;
    }

    if (rhs.is_zero())
    {
        return lhs;
    }

    // regular cases

    const binprod<N> lhs_binprod(lhs);
    const binprod<N> rhs_binprod(rhs);

    const binprod<N> bsum = lhs_binprod + rhs_binprod;
    Posit psum = bsum.template to_posit<N, ES, WT>();

    std::cout << lhs_binprod << " " << rhs_binprod << " " << bsum << std::endl;

    // fix overflows

    if (rhs > zero && psum < lhs)
    {
        std::cout << "[preventing overflow]" << std::endl;
        psum = psum.max();
    }

    // fix underflows

    if (rhs < zero && psum > lhs)
    {
        std::cout << "[preventing underflow]" << std::endl;
        psum = psum.min();
    }

    // return

    return psum;
}

template <size_t N, size_t ES, typename WT>
posit<N, ES, WT>& posit<N, ES, WT>::operator+=(const posit<N, ES, WT>& rhs)
{

    posit<N, ES, WT> sum = *this + rhs;
    this->bits = sum.get_bits();
    return *this;
}

template <size_t N, size_t ES, typename WT> posit<N, ES, WT>& posit<N, ES, WT>::operator++()
{
    // ++x
    *this += this->one();
    return *this;
}

template <size_t N, size_t ES, typename WT> posit<N, ES, WT> posit<N, ES, WT>::operator++(int)
{
    // x++
    auto copy = *this;
    *this += this->one();
    return *this;
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT> posit<N, ES, WT>::operator-() const
{
    if (is_zero())
    {
        return *this;
    }

    if (is_nar())
    {
        return *this;
    }

    const auto bits = twos_complement(this->get_bits());
    return posit<N, ES, WT>::from_bits(bits);
}

template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT> posit<N, ES, WT>::operator-(const posit<N, ES, WT>& rhs) const
{
    return *this + (-rhs);
}

template <size_t N, size_t ES, typename WT>
posit<N, ES, WT>& posit<N, ES, WT>::operator-=(const posit<N, ES, WT>& rhs)
{
    posit<N, ES, WT> sum = *this - rhs;
    this->bits = sum.get_bits();
    return *this;
}

template <size_t N, size_t ES, typename WT> posit<N, ES, WT>& posit<N, ES, WT>::operator--()
{
    // --x
    *this -= one();
    return *this;
}

template <size_t N, size_t ES, typename WT> posit<N, ES, WT> posit<N, ES, WT>::operator--(int)
{
    // x--
    auto copy = *this;
    *this -= this->one();
    return *this;
}

template <size_t N, size_t ES, typename WT>
posit<N, ES, WT> posit<N, ES, WT>::operator*(const posit<N, ES, WT>& rhs) const
{
    using Posit = posit<N, ES, WT>;

    constexpr Posit zero = Posit::zero();
    constexpr Posit inf = Posit::nar();

    const Posit& lhs = *this;

    // special cases

    if (rhs == inf || lhs == inf)
    {
        return inf;
    }

    if (lhs.is_zero() || rhs.is_zero())
    {
        return zero;
    }

    // regular cases

    const binprod<N> lhs_binprod(lhs);
    const binprod<N> rhs_binprod(rhs);

    const binprod<N> bprod = lhs_binprod * rhs_binprod;
    Posit pprod = bprod.template to_posit<N, ES, WT>();

    // fix overflows

    if (rhs > zero && pprod < lhs)
    {
        pprod = pprod.max();
    }

    // fix underflows

    if (rhs < zero && pprod > lhs)
    {
        pprod = pprod.min();
    }

    // return

    return pprod;
}

template <size_t N, size_t ES, typename WT>
posit<N, ES, WT>& posit<N, ES, WT>::operator*=(const posit<N, ES, WT>& rhs)
{
    const auto product = *this * rhs;
    bits = product.get_bits();
    return *this;
}

template <size_t N, size_t ES, typename WT> [[nodiscard]] constexpr size_t posit<N, ES, WT>::width()
{
    return N;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr size_t posit<N, ES, WT>::exponent_size()
{
    return ES;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::min()
{
    // the min value is represented by 10..01

    posit p;

    p.bits.set_bit(N - 1, true);
    p.bits.set_bit(0, true);

    return p;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::minpos()
{
    // minpos is presented by 0..01

    posit p;

    p.bits.set_bit(0, true);

    return p;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::max()
{
    // the maximum value is represented by 01..1

    posit p;

    p.bits = p.bits.all_ones();
    p.bits.set_bit(N - 1, false);

    return p;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::zero()
{
    // zero is represented by all bits set to zero

    return posit();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::one()
{
    // one is represented by 010...0

    posit p;

    p.bits.set_bit(N - 2, true);

    return p;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> posit<N, ES, WT>::nar()
{
    // NaR is sign bit set to one and all
    // other bits set to zero

    posit p;
    p.bits.set_bit(N - 1, true);

    return p;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr uinteger<N + 1, WT> posit<N, ES, WT>::npat()
{
    // to support arbitrary template parameters, we need to be able to
    // return arbitrary large sizes; so instead of size_t we use uinteger
    // as a return type here

    using SizeType = uinteger<N + 1, WT>;
    const SizeType one = SizeType::one();

    return one << N;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr typename posit<N, ES, WT>::useed_type posit<N, ES, WT>::useed()
{
    return useed_type(1 << (1 << ES));
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool posit<N, ES, WT>::is_negative() const
{
    if (is_nar())
    {
        return false;
    }

    return get_sign_bit() == storage_type(1);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool posit<N, ES, WT>::is_zero() const
{
    return (*this == zero());
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool posit<N, ES, WT>::is_nar() const
{
    return (*this == nar());
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr typename posit<N, ES, WT>::storage_type
posit<N, ES, WT>::get_sign_bit() const
{
    return storage_type(bits.msb());
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr typename posit<N, ES, WT>::storage_type posit<N, ES, WT>::get_bits() const
{
    return bits;
}

template <size_t N, size_t ES, typename WT>
constexpr void posit<N, ES, WT>::static_assert_template_parameters() const
{
    static_assert(N >= 2, "number of bits needs to be at least 2");
    static_assert(ES <= 5, "exponent unreasonably large");
}

} // namespace aarith
