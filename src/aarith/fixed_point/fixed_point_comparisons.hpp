#pragma once

namespace aarith {

template <typename FixedA, typename FixedB>
constexpr bool operator==(const FixedA& a, const FixedB& b)
{
    static_assert(::aarith::is_fixed_point_v<FixedA>,
                  "First parameter must be an aarith fixed point number");
    static_assert(::aarith::is_fixed_point_v<FixedB>,
                  "Second parameter must be an aarith fixed point number");
    return (a.bits() == b.bits());
}

template <typename FixedA, typename FixedB>
constexpr bool operator<(const FixedA& a, const FixedB& b)
{
    static_assert(::aarith::is_fixed_point_v<FixedA>,
                  "First parameter must be an aarith fixed point number");
    static_assert(::aarith::is_fixed_point_v<FixedB>,
                  "Second parameter must be an aarith fixed point number");
    static_assert(::aarith::same_signedness<FixedA, FixedB>,
                  "The fixed point numbers need to have the same signedness");

    return (a.bits() < b.bits());
}

template <typename FixedA, typename FixedB,
          typename = std::enable_if_t<
              ::aarith::is_fixed_point_v<FixedA>&& ::aarith::is_fixed_point_v<FixedB>>>
constexpr bool operator<=(const FixedA& a, const FixedB& b)
{
    return (a < b) || (a == b);
}

template <typename FixedA, typename FixedB,
          typename = std::enable_if_t<
              ::aarith::is_fixed_point_v<FixedA>&& ::aarith::is_fixed_point_v<FixedB>>>
constexpr bool operator>=(const FixedA& a, const FixedB& b)
{
    return b <= a;
}

template <typename FixedA, typename FixedB,
          typename = std::enable_if_t<
              ::aarith::is_fixed_point_v<FixedA>&& ::aarith::is_fixed_point_v<FixedB>>>
constexpr bool operator>(const FixedA& a, const FixedB& b)
{
    return (b < a);
}

//template <typename FixedA, typename FixedB> constexpr auto min(const FixedA& a, const FixedB& b)
//{
//    return a;
//}

} // namespace aarith