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
    : start(tile<N, ES, WT>::from(exact_value, false))
    , end(tile<N, ES, WT>::from(exact_value, false))
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

    if (lhs.crosses_infinity() || rhs.crosses_infinity())
    {
        return false;
    }

    const tile_type& lbound = lhs.end;
    const tile_type& rbound = rhs.start;

    return lbound < rbound;
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
[[nodiscard]] /*constexpr*/ valid<N, ES, WT>
valid<N, ES, WT>::operator+(const valid<N, ES, WT>& other) const
{
    // Compute the sum [a, b] + [c, d] = [l, r] where the endpoints can be
    // open, closed or mixed.  Based on "The End of Error", John L. Gustafson,
    // p. 113.

    const tile_type& a = start;
    const tile_type& b = end;

    const tile_type& c = other.start;
    const tile_type& d = other.end;

    tile_type l, r;

    // Handle special cases that throw exceptions. They are the NaR
    // cases of the original formulation.

    // TODO (Schärtl): Handle signaling NaR.

    // Handle the cases that do not signal failure. First compute the left
    // bound "l". If you are confused, this code makes a lot more sense if you
    // look up the table from "The End of Error", p. 113.

    if (a == closed_neg_inf())
    {
        l = closed_neg_inf();
    }
    else if (a == open_neg_inf())
    {
        if (c == closed_neg_inf())
        {
            l = closed_neg_inf();
        }
        else
        {
            l = open_neg_inf();
        }
    }
    else if (c == closed_neg_inf())
    {
        l = closed_neg_inf();
    }
    else if (c == open_neg_inf())
    {
        l = open_neg_inf();
    }
    else
    {
        const posit_type lsum = a.value() + b.value();
        const bool u = a.is_uncertain() || b.is_uncertain();

        l = tile_type::from(lsum, u);
    }

    // Now compute the right bound "r".

    if (b == open_pos_inf())
    {
        if (d == closed_pos_inf())
        {
            r = closed_pos_inf();
        }
        else
        {
            r = open_pos_inf();
        }
    }
    else if (b == closed_pos_inf())
    {
        r = closed_pos_inf();
    }
    else if (d == open_pos_inf())
    {
        r = open_pos_inf();
    }
    else if (d == closed_pos_inf())
    {
        r = closed_pos_inf();
    }
    else
    {
        const posit_type rsum = c.value() + d.value();
        const bool u = c.is_uncertain() || d.is_uncertain();

        r = tile_type::from(rsum, u);
    }

    // Now that we have both bounds, we can construct the valid.

    return from(l, r);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT>
valid<N, ES, WT>::operator-(const valid<N, ES, WT>& other) const
{
    const tile_type& a = start;
    const tile_type& b = end;

    const tile_type& c = other.start;
    const tile_type& d = other.end;

    // TODO (Schärtl): Handle signaling NaR

    tile_type l, r;

    if (a == closed_neg_inf())
    {
        l = closed_neg_inf();
    }
    else if (a == open_neg_inf())
    {
        if (d == closed_pos_inf())
        {
            l = closed_neg_inf();
        }
        else
        {
            l = open_neg_inf();
        }
    }
    else if (d == open_pos_inf())
    {
        l = open_neg_inf();
    }
    else if (d == closed_pos_inf())
    {
        l = closed_neg_inf();
    }
    else
    {
        const posit_type lsum = a.value() - d.value();
        const bool u = a.is_uncertain() || d.is_uncertain();

        l = tile_type::from(lsum, u);
    }

    if (b == closed_pos_inf())
    {
        r = closed_pos_inf();
    }
    else if (b == open_pos_inf())
    {
        if (c == closed_neg_inf())
        {
            r = closed_pos_inf();
        }
        else
        {
            r = open_pos_inf();
        }
    }
    else if (c == closed_neg_inf())
    {
        r = closed_pos_inf();
    }
    else if (c == open_neg_inf())
    {
        r = open_neg_inf();
    }
    else
    {
        const posit_type rsum = b.value() - c.value();
        const bool u = b.is_uncertain() || c.is_uncertain();

        r = tile_type::from(rsum, u);
    }

    return from(l, r);
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
[[nodiscard]] constexpr typename valid<N, ES, WT>::tile_type valid<N, ES, WT>::closed_neg_inf()
{
    constexpr auto bound = posit_type::nar();
    return tile_type::from(bound, false);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr typename valid<N, ES, WT>::tile_type valid<N, ES, WT>::open_neg_inf()
{
    constexpr auto bound = posit_type::nar();
    return tile_type::from(bound, true);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr typename valid<N, ES, WT>::tile_type valid<N, ES, WT>::closed_pos_inf()
{
    constexpr auto bound = posit_type::nar();
    return tile_type::from(bound, false);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr typename valid<N, ES, WT>::tile_type valid<N, ES, WT>::open_pos_inf()
{
    constexpr auto bound = posit_type::max();
    return tile_type::from(bound, true);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::canonical_empty()
{
    return empty();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] bool valid<N, ES, WT>::crosses_infinity() const
{
    // TODO (Schärtl): Fix!

    if (!start.value().msb() && end.value().msb())
    {
        return true;
    }

    return false;
}

template <size_t N, size_t ES, typename WT> void valid<N, ES, WT>::ensure_canonicalized()
{
    if (is_empty())
    {
        *this = canonical_empty();
    }
}

} // namespace aarith
