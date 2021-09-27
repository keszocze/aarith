#pragma once

#include <aarith/posit_no_operators.hpp>

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
inline std::ostream& operator<<(std::ostream& os, const quire<N, ES, WT>& q)
{
    if (q.is_nar())
    {
        os << "NaR";
    }
    else
    {
        os << q.get_bits();
    }

    return os;
}

} // namespace aarith
