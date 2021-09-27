#include <stdexcept>

#include <aarith/posit_no_operators.hpp>

namespace aarith {

[[nodiscard]] constexpr interval_bound negate(interval_bound u)
{
    switch (u)
    {
    case interval_bound::OPEN: return interval_bound::CLOSED;
    case interval_bound::CLOSED: return interval_bound::OPEN;
    default: throw std::invalid_argument("unknown value for interval_bound enum");
    };
}

[[nodiscard]] constexpr bool is_open(interval_bound u)
{
    return u == interval_bound::OPEN;
}

[[nodiscard]] constexpr bool is_closed(interval_bound u)
{
    return u == interval_bound::CLOSED;
}

[[nodiscard]] constexpr interval_bound merge(interval_bound u0, interval_bound u1)
{
    if (is_closed(u0) && is_closed(u1))
    {
        return interval_bound::CLOSED;
    }
    else
    {
        return interval_bound::OPEN;
    }
}

} // namespace aarith
