#pragma once

#include <aarith/posit/binprod.hpp>
#include <aarith/posit/posit.hpp>
#include <aarith/posit/posit_internal.hpp>
#include <aarith/posit/posit_operations.hpp>
#include <iostream>
#include <ostream>

namespace aarith {

namespace internal {

template <typename Integer> constexpr Integer pow2(const Integer& y)
{
    Integer p = Integer::one();

    for (Integer i = Integer::zero(); i < y; ++i)
    {
        p = p << 1;
    }

    return p;
}

} // namespace internal

/**
 * @brief Return the global exponent that unifies regime and exponent.
 *
 * Posit values are a fraction m raised to some power 2^k where
 * k is made up of useed^R times 2^E. This function returns k.
 */
template <size_t N, size_t ES, class WT>
constexpr integer<N> get_global_exponent(const posit<N, ES, WT>& p)
{
    using Integer = integer<N>;

    constexpr auto rscale = Integer(1 << ES);

    const auto R = Integer(get_regime_value<N, ES, WT>(p));
    const auto E = Integer(get_exponent_value<N, ES, WT>(p));

    return rscale * R + E;
}

template <size_t N, size_t ES, class WT = uint64_t>
constexpr posit<N, ES, WT>& operator+(const posit<N, ES, WT>& p)
{
    return p;
}

template <size_t N, size_t ES, class WT = uint64_t>
posit<N, ES, WT> operator+(const posit<N, ES, WT>& lhs, const posit<N, ES, WT>& rhs)
{
    using Posit = posit<N, ES, WT>;

    constexpr Posit zero = Posit::zero();
    constexpr Posit inf = Posit::complex_infinity();

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
posit<N, ES, WT>& operator+=(posit<N, ES, WT>& lhs, const posit<N, ES, WT>& rhs)
{
    posit<N, ES, WT> sum = lhs + rhs;
    lhs.bits = sum.get_bits();
    return lhs;
}

template <size_t N, size_t ES, class WT> posit<N, ES, WT>& operator++(posit<N, ES, WT>& p)
{
    // ++x
    p += p.one();
    return p;
}

template <size_t N, size_t ES, class WT> posit<N, ES, WT> operator++(posit<N, ES, WT>& p, int)
{
    // x++
    auto copy = p;
    p += p.one();
    return copy;
}

template <size_t N, size_t ES, class WT>
constexpr posit<N, ES, WT> operator-(const posit<N, ES, WT>& arg)
{
    if (arg == arg.zero())
    {
        return arg;
    }

    if (arg == arg.complex_infinity())
    {
        return arg;
    }

    const auto bits = twos_complement(arg.get_bits());
    return arg.from_bits(bits);
}

template <size_t N, size_t ES, class WT>
constexpr posit<N, ES, WT> operator-(const posit<N, ES, WT>& lhs, const posit<N, ES, WT>& rhs)
{
    return lhs + (-rhs);
}

template <size_t N, size_t ES, class WT>
posit<N, ES, WT>& operator-=(posit<N, ES, WT>& lhs, const posit<N, ES, WT>& rhs)
{
    posit<N, ES, WT> sum = lhs - rhs;
    lhs.bits = sum.get_bits();
    return lhs;
}

template <size_t N, size_t ES, class WT> posit<N, ES, WT>& operator--(posit<N, ES, WT>& p)
{
    // --x
    p -= p.one();
    return p;
}

template <size_t N, size_t ES, class WT> posit<N, ES, WT> operator--(posit<N, ES, WT>& p, int)
{
    // x--
    auto copy = p;
    p -= p.one();
    return copy;
}

template <size_t N, size_t ES, class WT>
std::ostream& operator<<(std::ostream& os, const posit<N, ES, WT>& p)
{
    // TODO(Schärtl): actually print decimal representation; to do this
    // however we need to have arithmetic operators implemented first

    return os << to_binary(p.get_bits());
}

} // namespace aarith
