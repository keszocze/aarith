#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr tile<N, ES, WT> tile<N, ES, WT>::from(const posit<N, ES, WT>& p,
                                                                          bool u)
{
    tile v;

    v.value() = p;
    v.uncertain = u;

    return v;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr tile<N, ES, WT> tile<N, ES, WT>::zero()
{
    constexpr auto p = posit<N, ES, WT>::zero();
    return from(p, false);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr tile<N, ES, WT> tile<N, ES, WT>::nar()
{
    constexpr auto p = posit<N, ES, WT>::nar();
    return from(p, false);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr tile<N, ES, WT> tile<N, ES, WT>::max()
{
    constexpr auto p = posit<N, ES, WT>::max();
    return from(p, true);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr tile<N, ES, WT> tile<N, ES, WT>::min()
{
    constexpr auto p = posit<N, ES, WT>::nar();
    return from(p, true);
}

template <size_t N, size_t ES, typename WT>
tile<N, ES, WT>::tile()
    : uncertain(false)
{
}

template <size_t N, size_t ES, typename WT>
tile<N, ES, WT>::tile(const tile<N, ES, WT>& other)
    : posit<N, ES, WT>(other)
    , uncertain(other.uncertain)
{
}

template <size_t N, size_t ES, typename WT>
tile<N, ES, WT>& tile<N, ES, WT>::operator=(const tile<N, ES, WT>& other)
{
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool tile<N, ES, WT>::operator==(const tile& other) const
{
    return (value() == other.value()) && (uncertain == other.uncertain);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool tile<N, ES, WT>::operator!=(const tile& other) const
{
    return !(*this == other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool tile<N, ES, WT>::operator<(const tile& other) const
{
    const tile& lhs = *this;
    const tile& rhs = other;

    //
    // Handle special cases related to NaR.
    //

    if (lhs.is_nar() || rhs.is_nar())
    {
        // NaR is never less than or greater anything. The result is always
        // false.

        return false;
    }

    if (lhs == min() && rhs != min())
    {
        // Normal posit comparison returns false for any lt-comparison w/ NaR.
        // Because the minimum possible tile has value NaR but u-bit set to
        // '1', we have to handle lhs == min() as a special case.

        return true;
    }

    //
    // Handle standard cases.
    //

    // Comparing two tiles is equivalent to comparing two posits for most cases.
    // Only when the u-bit differs do we have to run some special logic.
    //
    // Given two tiles with identical values but different u-bits, the tile
    // with u-bit set to '1' is always greater than the tile with u-bit set to
    // '0'. This is because intervals, i.e. tiles with u-bit='1', drift to the
    // right towards +∞, here illustrated on the number line:
    //
    // <----- -1 ----- 0 ---- 1 ---- 2 ---->
    //   -2U      -1U     OU     1U     2U

    const bool ul = lhs.is_uncertain();
    const bool ur = rhs.is_uncertain();

    if ((ul != ur) && (lhs.value() == rhs.value()))
    {
        // u-bits differ but value is identical. lhs is less than rhs iff
        // rhs has u-bit set.

        return ur;
    }
    else
    {
        return lhs.value() < rhs.value();
    }
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool tile<N, ES, WT>::operator<=(const tile& other) const
{
    return (*this == other) || (*this < other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool tile<N, ES, WT>::operator>(const tile& other) const
{
    return other < *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool tile<N, ES, WT>::operator>=(const tile& other) const
{
    return (*this == other) || (*this > other);
}

template <size_t N, size_t ES, typename WT> void tile<N, ES, WT>::mark_as_uncertain()
{
    uncertain = true;
}

template <size_t N, size_t ES, typename WT> void tile<N, ES, WT>::mark_as_certain()
{
    uncertain = false;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool tile<N, ES, WT>::is_uncertain() const
{
    return uncertain;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool tile<N, ES, WT>::is_zero() const
{
    return *this == zero();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool tile<N, ES, WT>::is_nar() const
{
    return *this == nar();
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] posit<N, ES, WT>& tile<N, ES, WT>::value()
{
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const posit<N, ES, WT>& tile<N, ES, WT>::value() const
{
    return *this;
}

} // namespace aarith
