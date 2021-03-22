#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
bound<N, ES, WT>::bound()
    : sign(bound_sign::EXACT)
{
}

template <size_t N, size_t ES, typename WT>
bound<N, ES, WT>::bound(const posit<N, ES, WT>& new_value, bound_sign new_sign)
    : value(new_value)
    , sign(new_sign)
{
}

template <size_t N, size_t ES, typename WT>
bound<N, ES, WT>::bound(const bound<N, ES, WT>& other)
    : value(other.value)
    , sign(other.sign)
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
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool bound<N, ES, WT>::operator==(const bound<N, ES, WT>& other) const
{
    return this->value == other.value && this->sign == other.sign;
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

} // namespace aarith
