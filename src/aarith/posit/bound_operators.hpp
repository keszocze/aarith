#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const bound<N, ES, WT>& b)
{
    const auto& value = b.get_value();
    const auto& sign = b.get_sign();

    switch (sign)
    {
    case bound_sign::EXACT: return os << value; break;
    case bound_sign::PLUS_EPS: return os << "(" << value << " + ε)"; break;
    case bound_sign::MINUS_EPS: return os << "(" << value << " - ε)"; break;
    case bound_sign::UNSURE: return os << "(" << value << " ± ε)"; break;
    default: throw std::invalid_argument("unknown value for bound_sign enum");
    };
}

} // namespace aarith
