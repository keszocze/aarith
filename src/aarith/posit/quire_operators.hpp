#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool operator==(const quire<N, ES, WT>& lhs, const quire<N, ES, WT>& rhs)
{
    return lhs.get_bits() == rhs.get_bits();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool operator!=(const quire<N, ES, WT>& lhs, const quire<N, ES, WT>& rhs)
{
    return !(lhs.get_bits() == rhs.get_bits());
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool operator>(const quire<N, ES, WT>& lhs, const quire<N, ES, WT>& rhs)
{
    return lhs.get_bits() > rhs.get_bits();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool operator>=(const quire<N, ES, WT>& lhs, const quire<N, ES, WT>& rhs)
{
    return lhs.get_bits() >= rhs.get_bits();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool operator<(const quire<N, ES, WT>& lhs, const quire<N, ES, WT>& rhs)
{
    return lhs.get_bits() < rhs.get_bits();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool operator<=(const quire<N, ES, WT>& lhs, const quire<N, ES, WT>& rhs)
{
    return lhs.get_bits() <= rhs.get_bits();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr auto operator+(const quire<N, ES, WT>& lhs, const quire<N, ES, WT>& rhs)
    -> quire<N, ES, WT>
{
    quire sum = lhs;
    sum += rhs;
    return sum;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr auto operator-(const quire<N, ES, WT>& q) -> quire<N, ES, WT>
{
    const auto value = q.get_bits();
    return quire<N, ES, WT>::from(-value);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr auto operator-(const quire<N, ES, WT>& lhs, const quire<N, ES, WT>& rhs)
    -> quire<N, ES, WT>
{
    quire diff = lhs;
    diff -= rhs;
    return diff;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr auto operator*(const quire<N, ES, WT>& lhs, const quire<N, ES, WT>& rhs)
    -> quire<N, ES, WT>
{
    quire prod = lhs;
    prod *= rhs;
    return prod;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr auto operator/(const quire<N, ES, WT>& lhs, const quire<N, ES, WT>& rhs)
    -> quire<N, ES, WT>
{
    quire quot = lhs;
    quot /= rhs;
    return quot;
}

template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const quire<N, ES, WT>& q)
{
    return os << q.get_bits();
}

} // namespace aarith
