#pragma once

#include <sstream>
#include <stdexcept>
#include <string>

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr ivalid<N, ES, WT>
ivalid<N, ES, WT>::from(const posit<N, ES, WT>& start_value, interval_bound start_bound,
                        const posit<N, ES, WT>& end_value, interval_bound end_bound)
{
    ivalid v;

    v.start_value = start_value;
    v.start_bound = start_bound;

    v.end_value = end_value;
    v.end_bound = end_bound;

    return v;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr ivalid<N, ES, WT> ivalid<N, ES, WT>::zero()
{
    const auto pzero = posit_type::zero();
    const auto closed = interval_bound::CLOSED;

    return from(pzero, closed, pzero, closed);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr ivalid<N, ES, WT> ivalid<N, ES, WT>::one()
{
    const auto pone = posit_type::one();
    const auto closed = interval_bound::CLOSED;

    return from(pone, closed, pone, closed);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr ivalid<N, ES, WT> ivalid<N, ES, WT>::empty()
{
    const auto pzero = posit_type::zero();
    const auto open = interval_bound::OPEN;

    return from(pzero, open, pzero, open);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr ivalid<N, ES, WT> ivalid<N, ES, WT>::full()
{
    const auto infty = posit_type::nar();
    const auto open = interval_bound::OPEN;

    return from(infty, open, infty, open);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr ivalid<N, ES, WT> ivalid<N, ES, WT>::max()
{
    const auto pmax = posit_type::max();
    const auto infty = posit_type::nar();
    const auto open = interval_bound::OPEN;

    return from(pmax, open, infty, open);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr ivalid<N, ES, WT> ivalid<N, ES, WT>::min()
{
    const auto pmin = posit_type::min();
    const auto infty = posit_type::nar();
    const auto open = interval_bound::OPEN;

    return from(infty, open, pmin, open);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr ivalid<N, ES, WT> ivalid<N, ES, WT>::nar()
{
    const auto infty = posit_type::nar();
    const auto closed = interval_bound::CLOSED;

    return from(infty, closed, infty, closed);
}

template <size_t N, size_t ES, typename WT>
constexpr ivalid<N, ES, WT>::ivalid()
    : start_value(posit_type::zero())
    , start_bound(interval_bound::CLOSED)
    , end_value(posit_type::zero())
    , end_bound(interval_bound::CLOSED)
{
}

template <size_t N, size_t ES, typename WT>
constexpr ivalid<N, ES, WT>::ivalid(const ivalid& other)
    : start_value(other.start_value)
    , start_bound(other.start_bound)
    , end_value(other.end_value)
    , end_bound(other.end_bound)
{
}

template <size_t N, size_t ES, typename WT>
constexpr ivalid<N, ES, WT>::ivalid(const posit<N, ES, WT>& exact_value)
    : start_value(exact_value)
    , start_bound(interval_bound::CLOSED)
    , end_value(exact_value)
    , end_bound(interval_bound::CLOSED)
{
}

template <size_t N, size_t ES, typename WT>
constexpr ivalid<N, ES, WT>::ivalid(const posit<N, ES, WT>& start, const posit<N, ES, WT>& end)
    : start_value(start)
    , start_bound(interval_bound::CLOSED)
    , end_value(end)
    , end_bound(interval_bound::CLOSED)
{
}

template <size_t N, size_t ES, typename WT> ivalid<N, ES, WT>::~ivalid()
{
}

template <size_t N, size_t ES, typename WT>
ivalid<N, ES, WT>& ivalid<N, ES, WT>::operator=(const ivalid& other)
{
    this->start_value = other.start_value;
    this->start_bound = other.start_bound;

    this->end_value = other.end_value;
    this->end_bound = other.end_bound;

    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool ivalid<N, ES, WT>::operator==(const ivalid& other) const
{
    // Handle special cases where two valid v, w can be equal even though
    // their bit patterns do not match.

    if (this->is_empty() && other.is_empty())
    {
        return true;
    }

    if (this->is_full() && other.is_full())
    {
        return true;
    }

    // Compare start bound for bitwise match.

    if (this->start_value != other.start_value || this->start_bound != other.start_bound)
    {
        return false;
    }

    // Compare end bound for bitwise match.

    if (this->end_value != other.end_value || this->end_bound != other.end_bound)
    {
        return false;
    }

    // start and end are identical. The valids are the equal.

    return true;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool ivalid<N, ES, WT>::operator!=(const ivalid& other) const
{
    return !(*this == other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool ivalid<N, ES, WT>::operator<(const ivalid& other) const
{
    const ivalid& lhs = *this;
    const ivalid& rhs = other;

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

    // Here we are left comparing regular intervals {a, b} with {c, d}.
    // If b < c, we get {a, b} < {c, d}.

    const posit_type& b = lhs.end_value();
    const posit_type& c = rhs.start_value();

    if (b < c)
    {
        return true;
    }

    // Finally, if b == c, the kind of bound is the deciding factor.
    // We need to check the cases, where {a, b} < {c, d}.
    //
    // {a, b] < (b, c}  ==  {a, b]         < [b + eps, c}  -> ok
    // {a, b) < [b, c}  ==  {a, b - eps]   < [b, c}        -> ok
    // {a, b) < (b, c}  ==  {a, b - eps]   < [b + eps, c}  -> ok
    // {a, b] < [b, c}  ==  {a, b         !< [b, c}        -> not ok

    if (lhs.end_bound == interval_bound::CLOSED && rhs.start_bound == interval_bound::CLOSED)
    {
        return false;
    }

    return true;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool ivalid<N, ES, WT>::operator<=(const ivalid& other) const
{
    return (*this == other) || (*this < other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool ivalid<N, ES, WT>::operator>(const ivalid& other) const
{
    return other < *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool ivalid<N, ES, WT>::operator>=(const ivalid& other) const
{
    return (*this == other) || (*this > other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr ivalid<N, ES, WT> ivalid<N, ES, WT>::operator+() const
{
    throw std::logic_error("not implemented");
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr ivalid<N, ES, WT>
ivalid<N, ES, WT>::operator+(const ivalid<N, ES, WT>& other) const
{
    throw std::logic_error("not implemented");
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr ivalid<N, ES, WT> ivalid<N, ES, WT>::operator-() const
{
    if (this->is_full() || this->is_empty())
    {
        return *this;
    }

    const posit_type new_start_value = -this->end_value;
    const interval_bound new_start_bound = this->end_bound;

    const posit_type new_end_value = -this->start_value;
    const interval_bound new_end_bound = this->start_bound;

    return from(new_start_value, new_start_bound, new_end_value, new_end_bound);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr ivalid<N, ES, WT>
ivalid<N, ES, WT>::operator-(const ivalid<N, ES, WT>& other) const
{
    return *this + (-other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr ivalid<N, ES, WT>
ivalid<N, ES, WT>::operator*(const ivalid<N, ES, WT>& other) const
{
    throw std::logic_error("not implemented");
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr ivalid<N, ES, WT>
ivalid<N, ES, WT>::operator/(const ivalid<N, ES, WT>& other) const
{
    throw std::logic_error("not implemented");
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool ivalid<N, ES, WT>::is_zero() const
{
    return *this == zero();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool ivalid<N, ES, WT>::is_empty() const
{
    // All intervals (p, p) are empty except when p = nar because interval
    // (nar, nar) is the full set.

    const auto infty = posit_type::nar();
    const auto open = interval_bound::OPEN;

    if (start_bound != open || end_bound != open)
    {
        return false;
    }

    if (start_value == infty && end_value == infty)
    {
        return false;
    }

    return start_value == end_value;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool ivalid<N, ES, WT>::is_full() const
{
    // Interval (nar, nar) is the full set, that is it is all the real numbers
    // as we interpret (nar, nar) as (-infty, infty).

    const auto infty = posit_type::nar();
    const auto open = interval_bound::OPEN;

    return start_bound == open && end_bound == open && start_value == infty && end_value == infty;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool ivalid<N, ES, WT>::is_exact_real() const
{
    if (this->is_nar())
    {
        return false;
    }

    const auto closed = interval_bound::CLOSED;

    return start_bound == closed && end_bound == closed && start_value == end_value;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool ivalid<N, ES, WT>::is_nar() const
{
    return *this == nar();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool ivalid<N, ES, WT>::contains(const posit<N, ES, WT>& value) const
{
    if (this->is_empty())
    {
        // If the valid is empty, it contains no elements and in particular it
        // does not contain "value".
        return false;
    }

    if (this->is_nar())
    {
        // If this valid represents special case NaR, it only contains posit
        // NaR.
        return value.is_nar();
    }

    if (this->is_full())
    {
        // If this valid represents any numbers on the number line, it
        // contains "value" as long as it represents an actual real.
        return !value.is_nar();
    }

    // We handled all special cases. Now we take a look at the regular case
    // where for interval (p, q) we have to check whether p < value < q (and
    // all other variations of open/closed intervals).

    if (this->is_regular())
    {
        bool start_ok = false;
        bool end_ok = false;

        if (this->start_bound == interval_bound::OPEN)
        {
            start_ok = (this->start_value < value);
        }
        else
        {
            start_ok = (this->start_value <= value);
        }

        if (this->end_bound == interval_bound::OPEN)
        {
            end_ok = (value < this->end_value);
        }
        else
        {
            end_ok = (value <= this->end_value);
        }

        return start_ok && end_ok;
    }
    else
    {
        return !this->inverse().contains(value);
    }
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool ivalid<N, ES, WT>::is_regular() const
{
    if (this->is_empty() || this->is_nar())
    {
        return false;
    }

    if (this->is_full() || this->is_exact_real())
    {
        return true;
    }

    return start_value < end_value;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool ivalid<N, ES, WT>::is_irregular() const
{
    return !this->is_regular();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr ivalid<N, ES, WT> ivalid<N, ES, WT>::inverse() const
{
    const posit_type& new_start_value = this->end_value;
    const interval_bound new_start_bound = negate(this->end_bound);

    const posit_type& new_end_value = this->start_value;
    const interval_bound new_end_bound = negate(this->start_bound);

    return from(new_start_value, new_start_bound, new_end_value, new_end_bound);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const typename ivalid<N, ES, WT>::posit_type&
ivalid<N, ES, WT>::get_start_value() const
{
    return start_value;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const typename ivalid<N, ES, WT>::posit_type& ivalid<N, ES, WT>::get_end_value() const
{
    return end_value;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const interval_bound& ivalid<N, ES, WT>::get_start_bound() const
{
    return start_bound;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const interval_bound& ivalid<N, ES, WT>::get_end_bound() const
{
    return end_bound;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] std::string ivalid<N, ES, WT>::in_interval_notation() const
{
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] std::string ivalid<N, ES, WT>::in_tile_notation() const
{
    std::stringstream ss;

    ss << "{";
    ss << in_tile_notation(start_value, start_bound);
    ss << "; ";
    ss << in_tile_notation(end_value, end_bound);
    ss << "}";

    return ss.str();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] std::string ivalid<N, ES, WT>::in_tile_notation(const posit<N, ES, WT>& p,
                                                              const interval_bound& u)
{
    std::stringstream ss;

    if (u == interval_bound::OPEN)
    {
        ss << p << "ᵘ";
    }
    else
    {
        ss << p;
    }

    return ss.str();
}

} // namespace aarith
