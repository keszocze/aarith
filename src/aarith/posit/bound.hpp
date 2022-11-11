#pragma once

#include <aarith/posit_no_operators.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bound<N, ES, WT> bound<N, ES, WT>::from_left(const posit<N, ES, WT>& value,
                                                                     const interval_bound& u)
{
    if (is_open(u))
    {
        return bound(value, bound_sign::PLUS_EPS, true);
    }
    else
    {
        return bound(value, bound_sign::EXACT, true);
    }
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bound<N, ES, WT> bound<N, ES, WT>::from_right(const posit<N, ES, WT>& value,
                                                                      const interval_bound& u)
{
    if (is_open(u))
    {
        return bound(value, bound_sign::MINUS_EPS, false);
    }
    else
    {
        return bound(value, bound_sign::EXACT, false);
    }
}

template <size_t N, size_t ES, typename WT>
bound<N, ES, WT>::bound()
    : sign(bound_sign::EXACT)
    , is_left(true)
{
}

template <size_t N, size_t ES, typename WT>
bound<N, ES, WT>::bound(const posit<N, ES, WT>& new_value, bound_sign new_sign, bool new_is_left)
    : value(new_value)
    , sign(new_sign)
    , is_left(new_is_left)
{
}

template <size_t N, size_t ES, typename WT>
bound<N, ES, WT>::bound(const bound<N, ES, WT>& other)
    : value(other.value)
    , sign(other.sign)
    , is_left(other.is_left)
{
}

template <size_t N, size_t ES, typename WT> bound<N, ES, WT>::~bound()
{
}

template <size_t N, size_t ES, typename WT>
bound<N, ES, WT>& bound<N, ES, WT>::operator=(const bound<N, ES, WT>& other)
{
    this->value = other.value;
    this->sign = other.sign;
    this->is_left = other.is_left;
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool bound<N, ES, WT>::operator==(const bound<N, ES, WT>& other) const
{
    if (this->value.is_nar() || other.value.is_nar())
    {
        return this->value == other.value && this->sign == other.sign && this->is_left &&
               other.is_left;
    }
    else
    {
        return this->value == other.value && this->sign == other.sign;
    }
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool bound<N, ES, WT>::operator!=(const bound<N, ES, WT>& other) const
{
    return !(*this == other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bound<N, ES, WT>
bound<N, ES, WT>::operator*(const bound<N, ES, WT>& other) const
{
    // This implements the table defined in valid-bound-multiplication.ods.

    const bound& x = *this;
    const bound& y = other;

    //
    // Handle Infty Cases
    //

    if (x.value.is_nar() && y.value.is_nar())
    {
        if (x.is_left == y.is_left)
        {
            const bool both_left = x.is_left;

            if (both_left)
            {
                return bound(posit_type::nar(), bound_sign::MINUS_EPS, true);
            }
            else
            {
                return bound(posit_type::nar(), bound_sign::PLUS_EPS, false);
            }
        }
        else
        {
            return bound(posit_type::nar(), bound_sign::EXACT);
        }
    }

    if (x.value.is_nar() && !y.is_zero())
    {
        assert(!y.value.is_nar());
        return x;
    }

    if (!x.is_zero() && y.value.is_nar())
    {
        assert(!x.value.is_nar());
        return y;
    }

    //
    // Handle Regular Cases
    //

    const auto [xy, r] = mul(x.value, y.value);

    if (x.is_exact() && y.is_exact())
    {
        // x * y

        if (x.is_zero() || y.is_zero())
        {
            return bound(posit_type::zero(), bound_sign::EXACT);
        }
        else if (is_not_rounded(r))
        {
            return bound(xy, bound_sign::EXACT);
        }
        else if (is_rounded_down(r))
        {
            return bound(xy, bound_sign::PLUS_EPS);
        }
        else
        {
            assert(is_rounded_up(r));
            return bound(xy, bound_sign::MINUS_EPS);
        }
    }
    else if (x.is_exact() && y.is_plus_eps())
    {
        // x * (y + eps)

        if (x.is_zero())
        {
            return bound(posit_type::zero(), bound_sign::EXACT);
        }
        else if (is_not_rounded(r))
        {
            if (x.is_negative())
            {
                return bound(xy, bound_sign::MINUS_EPS);
            }
            else
            {
                return bound(xy, bound_sign::PLUS_EPS);
            }
        }
        else if (is_rounded_down(r))
        {
            return bound(xy, bound_sign::PLUS_EPS);
        }
        else
        {
            assert(is_rounded_up(r));
            return bound(xy, bound_sign::MINUS_EPS);
        }
    }
    else if (x.is_plus_eps() && y.is_exact())
    {
        // (x + eps) * y derived from previous case x * (y + eps)

        if (y.is_zero())
        {
            return bound(posit_type::zero(), bound_sign::EXACT);
        }
        else if (is_not_rounded(r))
        {
            if (y.is_negative())
            {
                return bound(xy, bound_sign::MINUS_EPS);
            }
            else
            {
                return bound(xy, bound_sign::PLUS_EPS);
            }
        }
        else if (is_rounded_down(r))
        {
            return bound(xy, bound_sign::PLUS_EPS);
        }
        else
        {
            assert(is_rounded_up(r));
            return bound(xy, bound_sign::MINUS_EPS);
        }
    }
    else if (x.is_exact() && y.is_minus_eps())
    {
        // x * (y - eps)

        if (x.is_zero())
        {
            return bound(posit_type::zero(), bound_sign::EXACT);
        }
        else if (is_not_rounded(r))
        {
            if (x.is_negative())
            {
                return bound(xy, bound_sign::PLUS_EPS);
            }
            else
            {
                return bound(xy, bound_sign::MINUS_EPS);
            }
        }
        else if (is_rounded_down(r))
        {
            return bound(xy, bound_sign::PLUS_EPS);
        }
        else
        {
            assert(is_rounded_up(r));
            return bound(xy, bound_sign::MINUS_EPS);
        }
    }
    else if (x.is_minus_eps() && y.is_exact())
    {
        // (x - eps) * y derived from previous case x * (y - eps)

        if (y.is_zero())
        {
            return bound(posit_type::zero(), bound_sign::EXACT);
        }
        else if (is_not_rounded(r))
        {
            if (y.is_negative())
            {
                return bound(xy, bound_sign::PLUS_EPS);
            }
            else
            {
                return bound(xy, bound_sign::MINUS_EPS);
            }
        }
        else if (is_rounded_down(r))
        {
            return bound(xy, bound_sign::PLUS_EPS);
        }
        else
        {
            assert(is_rounded_up(r));
            return bound(xy, bound_sign::MINUS_EPS);
        }
    }
    else if (x.is_plus_eps() && y.is_plus_eps())
    {
        // (x + eps) * (y + eta)

        if (x.is_zero() && y.is_zero())
        {
            return bound(posit_type::zero(), bound_sign::PLUS_EPS);
        }
        else if (x.is_zero() && y.is_non_negative())
        {
            return bound(posit_type::zero(), bound_sign::PLUS_EPS);
        }
        else if (x.is_zero() && y.is_negative())
        {
            return bound(posit_type::zero(), bound_sign::MINUS_EPS);
        }
        else if (x.is_non_negative() && y.is_zero())
        {
            return bound(posit_type::zero(), bound_sign::PLUS_EPS);
        }
        else if (x.is_negative() && y.is_zero())
        {
            return bound(posit_type::zero(), bound_sign::MINUS_EPS);
        }
        else if (x.is_non_negative() && y.is_non_negative())
        {
            if (is_not_rounded(r))
            {
                return bound(xy, bound_sign::PLUS_EPS);
            }
            else if (is_rounded_down(r))
            {
                return bound(xy, bound_sign::PLUS_EPS);
            }
            else
            {
                assert(is_rounded_up(r));
                return bound(xy, bound_sign::MINUS_EPS);
            }
        }
        else
        {
            if (is_not_rounded(r))
            {
                return bound(xy, bound_sign::UNSURE);
            }
            else if (is_rounded_down(r))
            {
                return bound(xy, bound_sign::PLUS_EPS);
            }
            else
            {
                assert(is_rounded_up(r));
                return bound(xy, bound_sign::MINUS_EPS);
            }
        }
    }
    else if (x.is_minus_eps() && y.is_minus_eps())
    {
        // (x - eps) * (y - eta)

        if (x.is_zero() && y.is_zero())
        {
            return bound(posit_type::zero(), bound_sign::PLUS_EPS);
        }
        else if (x.is_non_negative() && y.is_zero())
        {
            return bound(posit_type::zero(), bound_sign::UNSURE);
        }
        else if (x.is_negative() && y.is_zero())
        {
            return bound(posit_type::zero(), bound_sign::PLUS_EPS);
        }
        else if (x.is_zero() && y.is_non_negative())
        {
            return bound(posit_type::zero(), bound_sign::UNSURE);
        }
        else if (x.is_zero() && y.is_negative())
        {
            return bound(posit_type::zero(), bound_sign::PLUS_EPS);
        }
        else if (x.is_negative() && y.is_negative())
        {
            if (is_not_rounded(r))
            {
                return bound(xy, bound_sign::PLUS_EPS);
            }
            else if (is_rounded_down(r))
            {
                return bound(xy, bound_sign::PLUS_EPS);
            }
            else
            {
                assert(is_rounded_up(r));
                return bound(xy, bound_sign::MINUS_EPS);
            }
        }
        else
        {
            if (is_not_rounded(r))
            {
                return bound(xy, bound_sign::UNSURE);
            }
            else if (is_rounded_down(r))
            {
                return bound(xy, bound_sign::PLUS_EPS);
            }
            else
            {
                assert(is_rounded_up(r));
                return bound(xy, bound_sign::MINUS_EPS);
            }
        }
    }
    else
    {
        // (x + eps) * (y - eta) and (x - eps) * (y + eta)

        assert((x.is_plus_eps() && y.is_minus_eps()) || (y.is_plus_eps() && x.is_minus_eps()));

        if (x.is_zero() && y.is_zero())
        {
            return bound(posit_type::zero(), bound_sign::MINUS_EPS);
        }
        else if (x.is_zero() || y.is_zero())
        {
            return bound(posit_type::zero(), bound_sign::UNSURE);
        }
        else
        {
            if (is_not_rounded(r))
            {
                return bound(xy, bound_sign::UNSURE);
            }
            else if (is_rounded_down(r))
            {
                return bound(xy, bound_sign::PLUS_EPS);
            }
            else
            {
                assert(is_rounded_up(r));
                return bound(xy, bound_sign::MINUS_EPS);
            }
        }
    }
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const posit<N, ES, WT>& bound<N, ES, WT>::get_value() const
{
    return this->value;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const bound_sign& bound<N, ES, WT>::get_sign() const
{
    return this->sign;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const bool& bound<N, ES, WT>::get_is_left() const
{
    return this->is_left;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool bound<N, ES, WT>::is_exact() const
{
    return this->sign == bound_sign::EXACT;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool bound<N, ES, WT>::is_plus_eps() const
{
    return this->sign == bound_sign::PLUS_EPS;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool bound<N, ES, WT>::is_minus_eps() const
{
    return this->sign == bound_sign::MINUS_EPS;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool bound<N, ES, WT>::is_unsure() const
{
    return this->sign == bound_sign::UNSURE;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool bound<N, ES, WT>::is_zero() const
{
    return this->value.is_zero();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool bound<N, ES, WT>::is_negative() const
{
    return this->value.is_negative();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool bound<N, ES, WT>::is_non_negative() const
{
    return this->value.is_non_negative();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr std::tuple<posit<N, ES, WT>, interval_bound>
bound<N, ES, WT>::to_left() const
{
    posit_type left_value;
    interval_bound left_bound = interval_bound::CLOSED;

    if (this->value.is_nar())
    {
        if (this->is_left)
        {
            left_value = posit_type::nar();
            left_bound = interval_bound::OPEN;
        }
        else
        {
            left_value = posit_type::nar();
            left_bound = interval_bound::CLOSED;
        }
    }
    else
    {
        switch (this->sign)
        {
        case bound_sign::EXACT:
            left_value = this->value;
            left_bound = interval_bound::CLOSED;
            break;
        case bound_sign::PLUS_EPS:
            left_value = this->value;
            left_bound = interval_bound::OPEN;
            break;
        case bound_sign::MINUS_EPS:
        case bound_sign::UNSURE:
            left_value = this->value.decremented();
            left_bound = interval_bound::OPEN;
            break;
        };
    }

    return std::make_tuple(left_value, left_bound);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr std::tuple<posit<N, ES, WT>, interval_bound>
bound<N, ES, WT>::to_right() const
{
    posit_type right_value;
    interval_bound right_bound = interval_bound::CLOSED;

    if (this->value.is_nar())
    {
        if (this->is_left)
        {
            right_value = posit_type::nar();
            right_bound = interval_bound::CLOSED;
        }
        else
        {
            right_value = posit_type::nar();
            right_bound = interval_bound::OPEN;
        }
    }
    else
    {
        switch (this->sign)
        {
        case bound_sign::EXACT:
            right_value = this->value;
            right_bound = interval_bound::CLOSED;
            break;
        case bound_sign::MINUS_EPS:
            right_value = this->value;
            right_bound = interval_bound::OPEN;
            break;
        case bound_sign::PLUS_EPS:
        case bound_sign::UNSURE:
            right_value = this->value.incremented();
            right_bound = interval_bound::OPEN;
            break;
        };
    }

    return std::make_tuple(right_value, right_bound);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool bound<N, ES, WT>::lt_left(const bound<N, ES, WT>& lhs,
                                                       const bound<N, ES, WT>& rhs)
{
    return bound::lt(lhs, rhs, true);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool bound<N, ES, WT>::lt_right(const bound<N, ES, WT>& lhs,
                                                        const bound<N, ES, WT>& rhs)
{
    return bound::lt(lhs, rhs, false);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool bound<N, ES, WT>::lt(const bound<N, ES, WT>& lhs,
                                                  const bound<N, ES, WT>& rhs, bool is_left)
{
    if (lhs.value.is_nar() && rhs.value.is_nar())
    {
        return false;
    }

    if (lhs.value.is_nar())
    {
        assert(!rhs.value.is_nar());
        return lhs.is_left;
    }

    if (rhs.value.is_nar())
    {
        return !rhs.is_left;
    }

    if (lhs.value == rhs.value)
    {
        //
        //   ----------|----------|----------|----------
        //                        p
        //                 -eps       +eps
        //

        if (lhs.is_unsure())
        {
            return is_left;
        }
        if (lhs.is_minus_eps())
        {
            return rhs.is_exact() || rhs.is_plus_eps();
        }
        else if (lhs.is_exact())
        {
            return rhs.is_plus_eps();
        }
        else
        {
            return false;
        }
    }
    else
    {
        return lhs.value < rhs.value;
    }
}

} // namespace aarith
