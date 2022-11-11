#include <stdexcept>

#include <aarith/posit_no_operators.hpp>

namespace aarith {

inline std::ostream& operator<<(std::ostream& os, const interval_bound& u)
{
    switch (u)
    {
    case interval_bound::OPEN: return os << "OPEN"; break;
    case interval_bound::CLOSED: return os << "CLOSED"; break;
    default: throw std::invalid_argument("unknown value for interval_bound enum");
    };
}

} // namespace aarith
