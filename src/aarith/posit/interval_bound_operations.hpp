#include <stdexcept>

#include <aarith/posit.hpp>

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

} // namespace aarith
