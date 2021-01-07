#pragma once

#include <aarith/fixed_point.hpp>

namespace aarith {

template <size_t I, size_t F, template <size_t, typename> typename B, typename WT>
auto operator+(const fixed<I, F, B, WT>& a, const fixed<I, F, B, WT>& b)
{
    return add(a, b);
}

template <size_t I, size_t F, template <size_t, typename> typename B, typename WT>
auto operator-(const fixed<I, F, B, WT>& a, const fixed<I, F, B, WT>& b)
{
    return sub(a, b);
}

template <size_t I, size_t F, template <size_t, typename> typename B, typename WT>
auto operator*(const fixed<I, F, B, WT>& a, const fixed<I, F, B, WT>& b)
{
    return mul(a, b);
}

template <size_t I, size_t F, template <size_t, typename> typename B, typename WT>
auto operator/(const fixed<I, F, B, WT>& a, const fixed<I, F, B, WT>& b)
{
    return div(a, b);
}

} // namespace aarith
