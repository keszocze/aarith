#pragma once

namespace aarith {

template <typename FixedA, typename FixedB,
          typename = std::enable_if_t<is_fixed_point_v<FixedA> && is_fixed_point_v<FixedB>>>
constexpr bool operator==(const FixedA& a, const FixedB& b)
{
    constexpr size_t fracA = FixedA::frac_width;
    constexpr size_t fracB = FixedB::frac_width;

    if constexpr (fracA == fracB)
    {
        return (a.bits() == b.bits());
    }
    else
    {
        if constexpr (fracA > fracB)
        {
            return a.bits() == width_cast<FixedB::int_width, fracA>(b).bits();
        }
        else
        {
            return width_cast<FixedA::int_width, fracB>(a).bits() == b.bits();
        }
    }
}

template <typename FixedA, typename FixedB,
          typename = std::enable_if_t<is_fixed_point_v<FixedA> && is_fixed_point_v<FixedB>>>
constexpr bool operator!=(const FixedA& a, const FixedB& b)
{
    return !(a == b);
}

template <typename FixedA, typename FixedB,
          typename = std::enable_if_t<is_fixed_point_v<FixedA> && is_fixed_point_v<FixedB> &&
                                      same_signedness<FixedA, FixedB>>>
constexpr bool operator<(const FixedA& a, const FixedB& b)
{
    constexpr size_t fracA = FixedA::frac_width;
    constexpr size_t fracB = FixedB::frac_width;

    if constexpr (fracA == fracB)
    {
        return (a.bits() < b.bits());
    }
    else
    {
        if constexpr (fracA > fracB)
        {
            return a.bits() < width_cast<FixedB::int_width, fracA>(b).bits();
        }
        else
        {
            return width_cast<FixedA::int_width, fracB>(a).bits() == b.bits();
        }
    }
}

template <typename FixedA, typename FixedB,
          typename = std::enable_if_t<is_fixed_point_v<FixedA> && is_fixed_point_v<FixedB> &&
                                      same_signedness<FixedA, FixedB>>>
constexpr bool operator<=(const FixedA& a, const FixedB& b)
{
    return (a < b) || (a == b);
}

template <typename FixedA, typename FixedB,
          typename = std::enable_if_t<is_fixed_point_v<FixedA> && is_fixed_point_v<FixedB> &&
                                      same_signedness<FixedA, FixedB>>>
constexpr bool operator>=(const FixedA& a, const FixedB& b)
{
    return b <= a;
}

template <typename FixedA, typename FixedB,
          typename = std::enable_if_t<is_fixed_point_v<FixedA> && is_fixed_point_v<FixedB> &&
                                      same_signedness<FixedA, FixedB>>>
constexpr bool operator>(const FixedA& a, const FixedB& b)
{
    return (b < a);
}

} // namespace aarith