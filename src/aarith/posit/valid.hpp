#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::from(const tile<N, ES, WT>& start,
                                                                const tile<N, ES, WT>& end)
{
    valid v;

    v.start = start;
    v.end = end;

    return v;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::zero()
{
    return from(tile_type::zero(), tile_type::zero());
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::one()
{
    return from(tile_type::one(), tile_type::one());
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::empty()
{
    // To represent the empty set, we can pick any posit p and return the
    // interval (p, p). We pick p = 0 here. This is arbitrary.

    const auto open_interval = tile_type::from(posit_type::zero(), true);
    return from(open_interval, open_interval);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::nar()
{
    const auto nar_tile = tile_type::nar();
    return from(nar_tile, nar_tile);
}

template <size_t N, size_t ES, typename WT> constexpr valid<N, ES, WT>::valid()
{
    ensure_canonicalized();
}

template <size_t N, size_t ES, typename WT>
constexpr valid<N, ES, WT>::valid(const valid& other)
    : start(other.start)
    , end(other.end)
{
    ensure_canonicalized();
}

template <size_t N, size_t ES, typename WT>
constexpr valid<N, ES, WT>::valid(const posit<N, ES, WT>& exact_value)
    : start(tile<N, ES, WT>::from(exact_value, true))
    , end(tile<N, ES, WT>::from(exact_value, true))
{
    ensure_canonicalized();
}

template <size_t N, size_t ES, typename WT> valid<N, ES, WT>::~valid()
{
}

template <size_t N, size_t ES, typename WT>
valid<N, ES, WT>& valid<N, ES, WT>::operator=(const valid& other)
{
    start = other.start;
    end = other.end;
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::operator==(const valid& other) const
{
    return start == other.start && end == other.start;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::operator!=(const valid& other) const
{
    return !(*this == other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::operator<(const valid& other) const
{
    // Based on "The End of Error", Gustafson, 2015, pp. 105.

    const valid& lhs = *this;
    const valid& rhs = other;

    if (lhs == rhs)
    {
        return false;
    }

    if (lhs.is_nar() || rhs.is_nar())
    {
        return false;
    }

    if (lhs.is_empty() || rhs.is_empty())
    {
        return false;
    }

    // const tile_type& lbound = lhs.end;
    // const tile_type& rbound = rhs.other.start;

    // return lbound < rbound;

    throw std::logic_error("valid::operator< not fully implemented");
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::operator<=(const valid& other) const
{
    return (*this < other) || (*this == other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::operator>(const valid& other) const
{
    return other < *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::operator>=(const valid& other) const
{
    return (*this > other) || (this == other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT>
valid<N, ES, WT>::operator+(const valid<N, ES, WT>& other) const
{
    throw std::logic_error("valid::operator+ not implemented");
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT>
valid<N, ES, WT>::operator-(const valid<N, ES, WT>& other) const
{
    throw std::logic_error("valid::operator- not implemented");
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT>
valid<N, ES, WT>::operator*(const valid<N, ES, WT>& other) const
{
    throw std::logic_error("valid::operator* not implemented");
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT>
valid<N, ES, WT>::operator/(const valid<N, ES, WT>& other) const
{
    throw std::logic_error("valid::operator/ not implemented");
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::is_zero() const
{
    return *this == zero();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::is_empty() const
{
    return start.is_uncertain() && end.is_uncertain() && start.value() == end.value();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::is_nar() const
{
    return *this == nar();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::inverse() const
{
    const tile_type inverse_start = end.incremented();
    const tile_type inverse_end = start.decremented();

    return from(inverse_start, inverse_end);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::contains(const tile<N, ES, WT>& t) const
{
    if (crosses_infinity())
    {
        return !inverse().contains(t);
    }
    else
    {
        return t >= start && t <= end;
    }
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const typename valid<N, ES, WT>::tile_type& valid<N, ES, WT>::get_start() const
{
    return start;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const typename valid<N, ES, WT>::tile_type& valid<N, ES, WT>::get_end() const
{
    return end;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::canonical_empty()
{
    return empty();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] bool valid<N, ES, WT>::crosses_infinity() const
{
    return !start.is_negative() && end.is_negative();
}

template <size_t N, size_t ES, typename WT> void valid<N, ES, WT>::ensure_canonicalized()
{
    if (is_empty())
    {
        *this = canonical_empty();
    }
}

} // namespace aarith
