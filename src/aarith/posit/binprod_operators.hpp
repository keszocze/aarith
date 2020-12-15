#pragma once

#include <aarith/posit.hpp>
#include <cstdlib>

namespace aarith {

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

} // namespace aarith
