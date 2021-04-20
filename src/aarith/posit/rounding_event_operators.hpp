#include <aarith/posit.hpp>

namespace aarith {

inline std::ostream& operator<<(std::ostream& os, const rounding_event& r)
{
    switch (r)
    {
    case rounding_event::NOT_ROUNDED: os << "NOT_ROUNDED"; break;
    case rounding_event::ROUNDED_DOWN: os << "ROUNDED_DOWN"; break;
    case rounding_event::ROUNDED_UP: os << "ROUNDED_UP"; break;
    };

    return os;
}

} // namespace aarith
