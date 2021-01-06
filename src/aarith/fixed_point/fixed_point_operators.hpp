#pragma once

#include <aarith/fixed_point/fixed_point_operations.hpp>

namespace aarith {

namespace fixed_point_operators {

template <size_t I, size_t F, template <size_t, typename> typename B, typename WordType = uint64_t>
auto operator+(const fixed<I, F, B, WordType>& a, const fixed<I, F, B, WordType>& b)
{
    return add(a, b);
}

template <size_t I, size_t F, template <size_t, typename> typename B, typename WordType = uint64_t>
auto operator-(const fixed<I, F, B, WordType>& a, const fixed<I, F, B, WordType>& b)
{
    return sub(a, b);
}

template <size_t I, size_t F, template <size_t, typename> typename B, typename WordType = uint64_t>
auto operator*(const fixed<I, F, B, WordType>& a, const fixed<I, F, B, WordType>& b)
{
    return mul(a, b);
}

template <size_t I, size_t F, template <size_t, typename> typename B, typename WordType = uint64_t>
auto operator/(const fixed<I, F, B, WordType>& a, const fixed<I, F, B, WordType>& b)
{
    return div(a, b);
}

} // namespace fixed_point_operators

} // namespace aarith
