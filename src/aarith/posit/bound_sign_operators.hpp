#pragma once

#include <aarith/posit.hpp>

namespace aarith {

std::ostream& operator<<(std::ostream& os, const bound_sign& bs)
{
    switch (bs)
    {
    case bound_sign::EXACT: return os << "EXACT"; break;
    case bound_sign::PLUS_EPS: return os << "PLUS_EPS"; break;
    case bound_sign::MINUS_EPS: return os << "MINUS_EPS"; break;
    case bound_sign::UNSURE: return os << "UNSURE"; break;
    default: throw std::invalid_argument("unknown value for bound_sign enum");
    };
}

} // namespace aarith
