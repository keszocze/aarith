#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid_tile<N, ES, WT> valid_tile<N, ES, WT>::from(const posit<N, ES, WT>& p,
                                                                          bool u)
{
    valid_tile v;

    v.value() = p;
    v.uncertain = u;

    return v;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid_tile<N, ES, WT> valid_tile<N, ES, WT>::zero()
{
    constexpr auto p = posit<N, ES, WT>::zero();
    return from(p, false);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid_tile<N, ES, WT> valid_tile<N, ES, WT>::nar()
{
    constexpr auto p = posit<N, ES, WT>::nar();
    return from(p, false);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid_tile<N, ES, WT> valid_tile<N, ES, WT>::max()
{
    constexpr auto p = posit<N, ES, WT>::max();
    return from(p, true);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid_tile<N, ES, WT> valid_tile<N, ES, WT>::min()
{
    constexpr auto p = posit<N, ES, WT>::nar();
    return from(p, true);
}

template <size_t N, size_t ES, typename WT>
valid_tile<N, ES, WT>::valid_tile()
    : uncertain(false)
{
}

template <size_t N, size_t ES, typename WT>
valid_tile<N, ES, WT>::valid_tile(const valid_tile<N, ES, WT>& other)
    : posit<N, ES, WT>(other)
    , uncertain(other.uncertain)
{
}

template <size_t N, size_t ES, typename WT>
valid_tile<N, ES, WT>& valid_tile<N, ES, WT>::operator=(const valid_tile<N, ES, WT>& other)
{
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid_tile<N, ES, WT>::operator==(const valid_tile& other) const
{
    return (value() == other.value()) && (uncertain == other.uncertain);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid_tile<N, ES, WT>::operator!=(const valid_tile& other) const
{
    return !(*this == other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid_tile<N, ES, WT>::operator<(const valid_tile& other) const
{
    const valid_tile& lhs = *this;
    const valid_tile& rhs = other;

    const bool ul = lhs.is_uncertain();
    const bool ur = rhs.is_uncertain();

    //
    // Handle special cases related to NaR.
    //

    if (lhs.value().is_nar() || rhs.value().is_nar())
    {
        return false;
    }

    //
    // Handle the case of different signs.
    //

    if (lhs.is_negative() ^ rhs.is_negative())
    {
        // Every negative posit p is less than any positive posit q, no matter
        // the u-bit. So if the sign of lhs and rhs differs, we can just fall
        // back to comparing the value.
        return value() < other.value();
    }

    //
    // Handle the case of identical sign.
    //

    // We now know that the sign of both lhs and rhs are identical. Depending
    // on the u-bit, we have to do different comparisons.
    //
    // NOTE: The following code could be cut down quite a bit. However in its
    // current formulation it better illustrates the different possible cases.
    // So unless it proves to be a performance bottleneck, it is probably a
    // good idea to keep it as-is.

    assert(lhs.is_negative() == rhs.is_negative());
    const bool sign_bit = lhs.is_negative();

    if (ul && ur)
    {
        // Both lhs and rhs represent intervals of the same sign. We only have
        // to compare the underlying value.
        return value() < other.value();
    }
    else if (!ul && !ur)
    {
        // Both lhs and rhs represent concrete real values. We only have to
        // compare the underlying value.
        return value() < other.value();
    }
    else if (ul && !ur)
    {
        // While lhs is an interval, rhs is a concrete real value.

        if (lhs.value() == other.value())
        {
            // While the value matches, rhs is an interval. That means that
            // rhs is closer to NaR/infinity than lhs.

            return sign_bit;
        }
        else
        {
            return value() < other.value();
        }
    }
    else
    {
        assert(!ul && ur);

        // Identical to the ul && !ur case, just with lhs and rhs flipped.

        if (lhs.value() == other.value())
        {
            return !sign_bit;
        }
        else
        {
            return value() < other.value();
        }
    }
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid_tile<N, ES, WT>::operator<=(const valid_tile& other) const
{
    return (*this == other) || (*this < other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid_tile<N, ES, WT>::operator>(const valid_tile& other) const
{
    return other < *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid_tile<N, ES, WT>::operator>=(const valid_tile& other) const
{
    return (*this == other) || (*this > other);
}

template <size_t N, size_t ES, typename WT> void valid_tile<N, ES, WT>::mark_as_uncertain()
{
    uncertain = true;
}

template <size_t N, size_t ES, typename WT> void valid_tile<N, ES, WT>::mark_as_certain()
{
    uncertain = false;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid_tile<N, ES, WT>::is_uncertain() const
{
    return uncertain;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid_tile<N, ES, WT>::is_zero() const
{
    return *this == zero();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid_tile<N, ES, WT>::is_nar() const
{
    return *this == nar();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] posit<N, ES, WT>& valid_tile<N, ES, WT>::value()
{
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const posit<N, ES, WT>& valid_tile<N, ES, WT>::value() const
{
    return *this;
}

} // namespace aarith
