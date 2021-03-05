#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <sstream>
#include <stdexcept>
#include <string>

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT>
valid<N, ES, WT>::from(const posit<N, ES, WT>& start_value, interval_bound start_bound,
                       const posit<N, ES, WT>& end_value, interval_bound end_bound)
{
    valid v;

    v.start_value = start_value;
    v.start_bound = start_bound;

    v.end_value = end_value;
    v.end_bound = end_bound;

    return v;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::zero()
{
    const auto pzero = posit_type::zero();
    const auto closed = interval_bound::CLOSED;

    return from(pzero, closed, pzero, closed);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::one()
{
    const auto pone = posit_type::one();
    const auto closed = interval_bound::CLOSED;

    return from(pone, closed, pone, closed);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::empty()
{
    const auto pzero = posit_type::zero();
    const auto open = interval_bound::OPEN;

    return from(pzero, open, pzero, open);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::all_reals()
{
    const auto infty = posit_type::nar();
    const auto open = interval_bound::OPEN;

    return from(infty, open, infty, open);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::full()
{
    const auto zero = posit_type::zero();
    const auto open = interval_bound::OPEN;
    const auto closed = interval_bound::CLOSED;

    return from(zero, closed, zero, open);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::max()
{
    const auto pmax = posit_type::max();
    const auto infty = posit_type::nar();
    const auto open = interval_bound::OPEN;

    return from(pmax, open, infty, open);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::min()
{
    const auto pmin = posit_type::min();
    const auto infty = posit_type::nar();
    const auto open = interval_bound::OPEN;

    return from(infty, open, pmin, open);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::nar()
{
    const auto infty = posit_type::nar();
    const auto closed = interval_bound::CLOSED;

    return from(infty, closed, infty, closed);
}

template <size_t N, size_t ES, typename WT>
constexpr valid<N, ES, WT>::valid()
    : start_value(posit_type::zero())
    , start_bound(interval_bound::CLOSED)
    , end_value(posit_type::zero())
    , end_bound(interval_bound::CLOSED)
{
}

template <size_t N, size_t ES, typename WT>
constexpr valid<N, ES, WT>::valid(const valid& other)
    : start_value(other.start_value)
    , start_bound(other.start_bound)
    , end_value(other.end_value)
    , end_bound(other.end_bound)
{
}

template <size_t N, size_t ES, typename WT>
constexpr valid<N, ES, WT>::valid(const posit<N, ES, WT>& exact_value)
    : start_value(exact_value)
    , start_bound(interval_bound::CLOSED)
    , end_value(exact_value)
    , end_bound(interval_bound::CLOSED)
{
}

template <size_t N, size_t ES, typename WT>
constexpr valid<N, ES, WT>::valid(const posit<N, ES, WT>& start, const posit<N, ES, WT>& end)
    : start_value(start)
    , start_bound(interval_bound::CLOSED)
    , end_value(end)
    , end_bound(interval_bound::CLOSED)
{
}

template <size_t N, size_t ES, typename WT> valid<N, ES, WT>::~valid()
{
}

template <size_t N, size_t ES, typename WT>
valid<N, ES, WT>& valid<N, ES, WT>::operator=(const valid& other)
{
    this->start_value = other.start_value;
    this->start_bound = other.start_bound;

    this->end_value = other.end_value;
    this->end_bound = other.end_bound;

    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::operator==(const valid& other) const
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

    if (lhs.is_full() || rhs.is_full())
    {
        return false;
    }

    if (lhs.is_all_reals() || rhs.is_all_reals())
    {
        return false;
    }

    if (lhs.is_irregular() || rhs.is_irregular())
    {
        return false;
    }

    // Here we are left comparing regular intervals {a, b} with {c, d}.
    // If b < c, we get {a, b} < {c, d}.

    const posit_type& b = lhs.get_end_value();
    const posit_type& c = rhs.get_start_value();

    if (b < c)
    {
        return true;
    }
    else if (b == c)
    {
        // If b == c, the kind of bound is the deciding factor.  We need to
        // check the cases, where v = {a, b} < w = {c, d}.
        //
        // {a, b] < (b, c}  ==  {a, b]        <  [b + eps, c}  ->  v < w
        // {a, b) < [b, c}  ==  {a, b - eps]  <  [b, c}        ->  v < w
        // {a, b) < (b, c}  ==  {a, b - eps]  <  [b + eps, c}  ->  v < w
        // {a, b] ≮ [b, c}  ==  {a, b         ≮  [b, c}        ->  v ≮ w

        constexpr interval_bound closed = interval_bound::CLOSED;
        return !(lhs.end_bound == closed && rhs.start_bound == closed);
    }
    else
    {
        // Either (1) b > c or (2) any of b or c are NaR.
        return false;
    }
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::operator<=(const valid& other) const
{
    return (*this == other) || (*this < other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::operator>(const valid& other) const
{
    return other < *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::operator>=(const valid& other) const
{
    return (*this == other) || (*this > other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::operator+() const
{
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT>
valid<N, ES, WT>::operator+(const valid<N, ES, WT>& other) const
{
    //
    // Compute the sum [a, b] + [c, d] = [l, r] where the endpoints can be
    // open, closed or mixed.  Based on "The End of Error", John L. Gustafson,
    // p. 113. Unfortunately this involves lots and lots of special cases.
    //

    constexpr auto open = interval_bound::OPEN;
    constexpr auto closed = interval_bound::CLOSED;

    constexpr auto not_rounded = rounding_event::NOT_ROUNDED;
    constexpr auto rounded_down = rounding_event::ROUNDED_DOWN;
    constexpr auto rounded_up = rounding_event::ROUNDED_UP;

    const posit_type& a = this->start_value;
    const interval_bound& au = this->start_bound;

    const posit_type& b = this->end_value;
    const interval_bound& bu = this->end_bound;

    const posit_type& c = other.start_value;
    const interval_bound& cu = other.start_bound;

    const posit_type& d = other.end_value;
    const interval_bound& du = other.end_bound;

    //
    // Start by looking at special cases that are easy to handle.
    //

    if (this->is_nar() || other.is_nar())
    {
        return nar();
    }

    if (this->is_empty() || other.is_empty())
    {
        return empty();
    }

    if (this->is_full() || other.is_full())
    {
        return full();
    }

    if (this->is_all_reals() || other.is_all_reals())
    {
        return all_reals();
    }

    //
    // For now we do not support addition of irregular intervals.
    // TODO(Schärtl): Define addition on irregular intervals.
    //

    if (this->is_irregular() || other.is_irregular())
    {
        throw std::logic_error("addition not defined on irregular arguments");
    }

    //
    // Now we deal with regular intervals. We have to apply the traditional
    // interval arithmetic rule {a, b} + {c, d} = {a + c, b + d} while taking
    // bounds and special value infinity into account.
    //

    valid sum;

    //
    // Compute left side of sum.
    //

    if (a.is_nar() || c.is_nar())
    {
        sum.start_value = sum.start_value.nar();
        sum.start_bound = open;
    }
    else
    {
        auto [ac_sum, ac_rbit] = add(a, c);
        interval_bound ac_bound = open;

        if (ac_rbit == rounded_up)
        {
            ac_sum = ac_sum.decremented();
        }
        else if (ac_rbit == not_rounded)
        {
            if (au == closed && cu == closed)
            {
                ac_bound = closed;
            }
        }

        sum.start_value = ac_sum;
        sum.start_bound = ac_bound;
    }

    //
    // Compute right side of sum.
    //

    if (b.is_nar() || d.is_nar())
    {
        sum.end_value = sum.end_value.nar();
        sum.end_bound = open;
    }
    else
    {
        auto [bd_sum, bd_rbit] = add(b, d);
        interval_bound bd_bound = open;

        if (bd_rbit == rounded_down)
        {
            bd_sum = bd_sum.incremented();
        }
        else if (bd_rbit == not_rounded)
        {
            if (bu == closed && du == closed)
            {
                bd_bound = closed;
            }
        }

        sum.end_value = bd_sum;
        sum.end_bound = bd_bound;
    }

    //
    // We can now return the sum.
    //

    return sum;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::operator-() const
{
    if (this->is_nar() || this->is_empty() || this->is_full() || this->is_all_reals())
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
[[nodiscard]] constexpr valid<N, ES, WT>
valid<N, ES, WT>::operator-(const valid<N, ES, WT>& other) const
{
    return *this + (-other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] /*constexpr*/ valid<N, ES, WT>
valid<N, ES, WT>::operator*(const valid<N, ES, WT>& other) const
{
    // We are multiplying v = {a, b} with w = {c, d}.

    const valid& v = *this;
    const valid& w = other;

    const tile_ref a = v.start();
    const tile_ref b = v.end();

    const tile_ref c = w.start();
    const tile_ref d = w.end();

    //
    // Start by looking at special cases that are easy to handle.
    //

    if (this->is_nar() || other.is_nar())
    {
        return nar();
    }

    if (this->is_empty() || other.is_empty())
    {
        return empty();
    }

    if (this->is_full() || other.is_full())
    {
        return full();
    }

    if (this->is_all_reals() || other.is_all_reals())
    {
        return nar();
    }

    //
    // Valid multiplication requires us to consider all possible choices ac,
    // ad, bc and bd.
    //

    valid product;

    {
        std::array<group_result, 4> choices = {tile_mul(a, c), tile_mul(a, d), tile_mul(b, c),
                                               tile_mul(b, d)};

        std::sort(choices.begin(), choices.end(), valid::lt_left);
        product.start_value = choices.front().product;
        product.start_bound = merge_bounds_from(choices.front());

        std::sort(choices.begin(), choices.end(), valid::lt_right);
        product.end_value = choices.back().product;
        product.end_bound = merge_bounds_from(choices.back());
    }

    return product;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT>
valid<N, ES, WT>::operator/(const valid<N, ES, WT>& other) const
{
    throw std::logic_error("not implemented");
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::is_zero() const
{
    return *this == zero();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::is_empty() const
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
[[nodiscard]] constexpr bool valid<N, ES, WT>::is_all_reals() const
{
    return *this == all_reals();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::is_full() const
{
    // Any interval [p, p) and (p, p] represents the full set if p is non-NaR.

    if (start_value.is_nar() || end_value.is_nar())
    {
        return false;
    }

    return (start_value == end_value) && (start_bound != end_bound);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::is_exact_real() const
{
    if (this->is_nar())
    {
        return false;
    }

    const auto closed = interval_bound::CLOSED;

    return start_bound == closed && end_bound == closed && start_value == end_value;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::is_nar() const
{
    return *this == nar();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::contains(const posit<N, ES, WT>& value) const
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
[[nodiscard]] constexpr bool valid<N, ES, WT>::is_regular() const
{
    // The full set and {NaR} both contain NaR, as such they are not regular.

    if (this->is_full() || this->is_nar())
    {
        return false;
    }

    // Any interval [NaR, q} and {p, NaR] definitely contains NaR and as such
    // is not regular.

    constexpr interval_bound closed = interval_bound::CLOSED;

    if (start_value.is_nar() && start_bound == closed)
    {
        return false;
    }

    if (end_value.is_nar() && end_bound == closed)
    {
        return false;
    }

    // Empty set, (-∞, ∞) and [p, p] for non-NaR posit p are all sets that do
    // not contain NaR. As such they are regular valids.

    if (this->is_empty() || this->is_all_reals() || this->is_exact_real())
    {
        return true;
    }

    // Interval (-∞, q} with non-NaR posit q is some real interval on the
    // reals.  It does not contain valid NaR and as such is regular.

    if (start_value.is_nar() && !end_value.is_nar())
    {
        return true;
    }

    // Just as above, {p, ∞) with non-NaR posit p represent some
    // interval {p, ∞) which does not include valid NaR. Such valid are
    // also regular.

    if (!start_value.is_nar() && end_value.is_nar())
    {
        return true;
    }

    return start_value < end_value;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::is_irregular() const
{
    return !this->is_regular();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> valid<N, ES, WT>::inverse() const
{
    const posit_type& new_start_value = this->end_value;
    const interval_bound new_start_bound = negate(this->end_bound);

    const posit_type& new_end_value = this->start_value;
    const interval_bound new_end_bound = negate(this->start_bound);

    return from(new_start_value, new_start_bound, new_end_value, new_end_bound);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const typename valid<N, ES, WT>::posit_type& valid<N, ES, WT>::get_start_value() const
{
    return start_value;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const typename valid<N, ES, WT>::posit_type& valid<N, ES, WT>::get_end_value() const
{
    return end_value;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const interval_bound& valid<N, ES, WT>::get_start_bound() const
{
    return start_bound;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const interval_bound& valid<N, ES, WT>::get_end_bound() const
{
    return end_bound;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> valid<N, ES, WT>::as_exact_real() const
{
    if (!this->is_exact_real())
    {
        throw std::logic_error("valid::exact_real called on valid that is not an exact real");
    }

    return start_value;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] std::string valid<N, ES, WT>::in_interval_notation() const
{
    std::stringstream ss;
    ss << *this;
    return ss.str();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] std::string valid<N, ES, WT>::in_tile_notation() const
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
constexpr valid<N, ES, WT>::group_result::group_result()
    : rounding(rounding_event::NOT_ROUNDED)
    , lhs_bound(interval_bound::OPEN)
    , rhs_bound(interval_bound::OPEN)
{
}

template <size_t N, size_t ES, typename WT>
constexpr valid<N, ES, WT>::group_result::group_result(const posit<N, ES, WT>& p, rounding_event r,
                                                       interval_bound lu, interval_bound ru)
    : product(p)
    , rounding(r)
    , lhs_bound(lu)
    , rhs_bound(ru)
{
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] std::string valid<N, ES, WT>::in_tile_notation(const posit<N, ES, WT>& p,
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

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr typename valid<N, ES, WT>::group_result
valid<N, ES, WT>::tile_mul(const valid<N, ES, WT>::tile_ref& lhs,
                           const valid<N, ES, WT>::tile_ref& rhs)
{
    const auto [product, rounding] = mul(lhs.value, rhs.value);
    return group_result{product, rounding, lhs.bound, rhs.bound};
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const typename valid<N, ES, WT>::tile_ref valid<N, ES, WT>::start() const
{
    return {this->start_value, this->start_bound};
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const typename valid<N, ES, WT>::tile_ref valid<N, ES, WT>::end() const
{
    return {this->end_value, this->end_bound};
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr interval_bound valid<N, ES, WT>::merge_bounds_from(interval_bound u0,
                                                                           interval_bound u1)
{
    constexpr auto open = interval_bound::OPEN;
    constexpr auto closed = interval_bound::CLOSED;

    if (is_open(u0) || is_open(u1))
    {
        return open;
    }
    else
    {
        return closed;
    }
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] interval_bound
valid<N, ES, WT>::merge_bounds_from(const valid<N, ES, WT>::group_result& group)
{
    return valid::merge_bounds_from(group.lhs_bound, group.rhs_bound);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] bool valid<N, ES, WT>::lt_left(const valid<N, ES, WT>::group_result& lhs,
                                             const valid<N, ES, WT>::group_result& rhs)
{
    return valid::lt(lhs, rhs, true);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] bool valid<N, ES, WT>::lt_right(const valid<N, ES, WT>::group_result& lhs,
                                              const valid<N, ES, WT>::group_result& rhs)
{
    return valid::lt(lhs, rhs, false);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] bool valid<N, ES, WT>::lt(const valid<N, ES, WT>::group_result& lhs,
                                        const valid<N, ES, WT>::group_result& rhs, bool left)
{
    if (lhs.product.is_nar() || rhs.product.is_nar())
    {
        return false;
    }

    if (lhs.product != rhs.product)
    {
        return lhs.product < rhs.product;
    }
    else
    {
        assert(lhs.product == rhs.product);

        const interval_bound lhs_bound = valid::merge_bounds_from(lhs);
        const interval_bound rhs_bound = valid::merge_bounds_from(rhs);

        if (left)
        {
            // [x is less than (x.
            return is_closed(lhs_bound) && is_open(rhs_bound);
        }
        else
        {
            // x) is less than x].
            return is_open(lhs_bound) && is_closed(rhs_bound);
        }
    }
}

} // namespace aarith
