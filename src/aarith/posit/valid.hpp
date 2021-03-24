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

        return is_open(lhs.end_bound) || is_open(rhs.start_bound);
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

    [[maybe_unused]] constexpr auto open = interval_bound::OPEN;
    [[maybe_unused]] constexpr auto closed = interval_bound::CLOSED;

    [[maybe_unused]] constexpr auto not_rounded = rounding_event::NOT_ROUNDED;
    [[maybe_unused]] constexpr auto rounded_down = rounding_event::ROUNDED_DOWN;
    [[maybe_unused]] constexpr auto rounded_up = rounding_event::ROUNDED_UP;

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
        const auto [ac_sum, ac_rbit] = add(a, c);
        const tile left = adapt_left(ac_sum, ac_rbit, merge(au, cu));

        sum.start_value = left.p;
        sum.start_bound = left.u;
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
        const auto [bd_sum, bd_rbit] = add(b, d);
        const tile right = adapt_right(bd_sum, bd_rbit, merge(bu, du));

        sum.end_value = right.p;
        sum.end_bound = right.u;
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

namespace Impl {

template <typename T, size_t N, typename FilterFunc>
[[nodiscard]] constexpr std::vector<T> filter(const std::array<T, N>& a, FilterFunc f)
{
    std::vector<T> ret;
    std::copy_if(a.begin(), a.end(), std::back_inserter(ret), f);
    return ret;
}

} // namespace Impl

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT>
valid<N, ES, WT>::operator*(const valid<N, ES, WT>& other) const
{
    //
    // Start by looking at special cases that are easy to handle.
    //

    if (this->is_nar() || other.is_nar())
    {
        // If any of the two operands indicates error, that is the sign to
        // stop the computation as the algorithm is faulty.

        return nar();
    }

    if (this->is_empty() || other.is_empty())
    {
        // Multiplying nothing with something is still nothing. I think.

        return empty();
    }

    if (this->is_full() || other.is_full())
    {
        // The full set is weird because it's actually the union of all the
        // reals with special error state NaR. The only reasonable answer we
        // can make if any of the operators is the full set is again the full
        // set.
        //
        // N.B.: We do know that if one operand is zero and the other operand
        // is the full set, the result is only either 0 or NaR. But we cannot
        // represent the set {NaR, 0}; the closest thing we have is the full
        // set.

        return full();
    }

    if (this->is_zero() || other.is_zero())
    {
        // Multiplying any real with zero results in zero. The encoding of all
        // reals requires this special case.

        return zero();
    }

    if (this->is_all_reals() || other.is_all_reals())
    {
        // Multiplying any arbitrary real with something could result in any
        // real on the number line. The encoding of all reals requires this
        // special case.

        return all_reals();
    }

    //
    // Valid multiplication requires us to consider all possible choices ac,
    // ad, bc and bd.  Pick out the biggest and smallest value and the convert
    // them back to valid bounds.
    //

    valid product;
    std::array<bound_type, 4> choices = valid::get_mult_choices(*this, other);

    if (this->is_irregular() || other.is_irregular())
    {
        // For [irregular valids], we use a pessimistic bound that wraps
        // around from the top.

        {
            auto left_choices = Impl::filter(choices, [](const bound_type& elem) -> bool {
                return elem.get_value().is_non_negative();
            });

            if (left_choices.empty())
            {
                product.end_value = posit_type::nar();
                product.end_bound = interval_bound::CLOSED;
            }
            else
            {
                std::sort(left_choices.begin(), left_choices.end(), bound_type::lt_left);
                const auto [lvalue, lbound] = left_choices.front().to_left();
                product.start_value = lvalue;
                product.start_bound = lbound;
            }
        }

        {
            auto right_choices = Impl::filter(choices, [](const bound_type& elem) -> bool {
                return elem.get_value().is_negative();
            });

            if (right_choices.empty())
            {
                product.start_value = posit_type::nar();
                product.start_bound = interval_bound::CLOSED;
            }
            else
            {
                std::sort(right_choices.begin(), right_choices.end(), bound_type::lt_right);
                const auto [rvalue, rbound] = right_choices.back().to_right();
                product.end_value = rvalue;
                product.end_bound = rbound;
            }
        }
    }
    else
    {
        // For [regular valids], we can use traditional interval arithmetic
        // rules.

        {
            std::sort(choices.begin(), choices.end(), bound_type::lt_left);
            const auto [lvalue, lbound] = choices.front().to_left();
            product.start_value = lvalue;
            product.start_bound = lbound;
        }

        {
            std::sort(choices.begin(), choices.end(), bound_type::lt_right);
            const auto [rvalue, rbound] = choices.back().to_right();
            product.end_value = rvalue;
            product.end_bound = rbound;
        }
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
[[nodiscard]] constexpr bool valid<N, ES, WT>::is_exact() const
{
    return is_closed(start_bound) && is_closed(end_bound) && start_value == end_value;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid<N, ES, WT>::is_exact_real() const
{
    return !this->is_nar() && this->is_exact();
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

        if (is_open(this->start_bound))
        {
            start_ok = (this->start_value < value);
        }
        else
        {
            start_ok = (this->start_value <= value);
        }

        if (is_open(this->end_bound))
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

    if (start_value.is_nar() && is_closed(start_bound))
    {
        return false;
    }

    if (end_value.is_nar() && is_closed(end_bound))
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
[[nodiscard]] constexpr posit<N, ES, WT> valid<N, ES, WT>::as_exact() const
{
    if (!this->is_exact())
    {
        throw std::logic_error("valid::as_exact called on valid that is not an exact posit");
    }

    return start_value;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> valid<N, ES, WT>::as_exact_real() const
{
    if (!this->is_exact_real())
    {
        throw std::logic_error("valid::as_exact_real called on valid that is not an exact real");
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
[[nodiscard]] std::string valid<N, ES, WT>::in_tile_notation(const posit<N, ES, WT>& p,
                                                             const interval_bound& u)
{
    std::stringstream ss;

    if (is_open(u))
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
[[nodiscard]] constexpr typename valid<N, ES, WT>::tile
valid<N, ES, WT>::adapt(const posit_type& value, const rounding_event rvalue,
                        const interval_bound desired, bool is_left)
{
    if (is_left)
    {
        return adapt_left(value, rvalue, desired);
    }
    else
    {
        return adapt_right(value, rvalue, desired);
    }
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr typename valid<N, ES, WT>::tile
valid<N, ES, WT>::adapt_left(const posit_type& value, const rounding_event rvalue,
                             const interval_bound desired)
{
    posit_type result_value = value;
    interval_bound result_bound = interval_bound::OPEN;

    if (is_rounded_up(rvalue))
    {
        result_value = value.decremented();
        result_bound = interval_bound::OPEN;
    }
    else if (is_not_rounded(rvalue))
    {
        // We are only assured to get "desired" when posit arithmetic did not
        // round.
        result_bound = desired;
    }

    return {result_value, result_bound};
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr typename valid<N, ES, WT>::tile
valid<N, ES, WT>::adapt_right(const posit_type& value, const rounding_event rvalue,
                              const interval_bound desired)
{
    posit_type result_value = value;
    interval_bound result_bound = interval_bound::OPEN;

    if (is_rounded_down(rvalue))
    {
        result_value = value.incremented();
        result_bound = interval_bound::OPEN;
    }
    else if (is_not_rounded(rvalue))
    {
        // We are only assured to get "desired" when posit arithmetic did not
        // round.
        result_bound = desired;
    }

    return {result_value, result_bound};
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr std::array<typename valid<N, ES, WT>::bound_type, 4>
valid<N, ES, WT>::get_mult_choices(const valid<N, ES, WT>& lhs, const valid<N, ES, WT>& rhs)
{
    const bound_type a = bound_type::from_left(lhs.start_value, lhs.start_bound);
    const bound_type b = bound_type::from_right(lhs.end_value, lhs.end_bound);

    const bound_type c = bound_type::from_left(rhs.start_value, rhs.start_bound);
    const bound_type d = bound_type::from_right(rhs.end_value, rhs.end_bound);

    return {a * c, a * d, b * c, b * d};
}

} // namespace aarith
