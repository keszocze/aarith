#pragma once

#include <aarith/posit.hpp>
#include <aarith/posit/posit_operations.hpp>
#include <cstdlib>

namespace aarith {

template <size_t N> constexpr std::ostream& operator<<(std::ostream& os, const binprod<N>& rhs)
{
    const double xd = internal::to_double(rhs.x);
    const double md = internal::to_double(rhs.m);
    const double B = xd * std::pow(2.0, md);

    return os << "(" << rhs.x << ", " << rhs.m << ", " << B << ")";
}

template <size_t N> constexpr binprod<N> operator+(const binprod<N>& lhs, const binprod<N>& rhs)
{
    // based on "Provably Correct Posit Arithmetic with Fixed-Point Big
    // Integer", Shin Yee Chung, 2018, p. 4

    // constants for arithmetic

    constexpr auto one = binprod<N>::storage_type::one();
    constexpr auto two = one + one;

    // easy to use aliases for readabilty

    const auto& xa = lhs.x;
    const auto& xb = rhs.x;

    const auto& ma = lhs.m;
    const auto& mb = rhs.m;

    // computation

    const auto mmax = max(ma, mb);

    auto x = xa * pow(two, mmax - mb) + xb * pow(two, mmax - ma);
    auto m = ma + mb - mmax;

    // normalization

    // returning

    return binprod<N>(x, m);
}

template <size_t N> constexpr binprod<N> operator*(const binprod<N>& lhs, const binprod<N>& rhs)
{
    const auto x = lhs.x * rhs.x;
    const auto m = lhs.m + rhs.m;

    return binprod<N>(x, m);
}

} // namespace aarith
