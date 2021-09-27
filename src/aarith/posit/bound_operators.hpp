#pragma once

#include <aarith/posit_no_operators.hpp>
#include <sstream>
#include <string>

namespace aarith {

template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const bound<N, ES, WT>& b)
{
    const auto& value = b.get_value();
    const auto& sign = b.get_sign();
    const auto& is_left = b.get_is_left();

    // First, get a string representation of the value itself.

    std::string svalue;

    {
        std::stringstream ss;

        if (value.is_nar() && is_left)
        {
            ss << "-";
        }

        ss << value;

        svalue = ss.str();
    }

    // Now look at eps.

    switch (sign)
    {
    case bound_sign::EXACT: return os << svalue; break;
    case bound_sign::PLUS_EPS: return os << "(" << svalue << " + ε)"; break;
    case bound_sign::MINUS_EPS: return os << "(" << svalue << " - ε)"; break;
    case bound_sign::UNSURE: return os << "(" << svalue << " ± ε)"; break;
    default: throw std::invalid_argument("unknown value for bound_sign enum");
    };
}

} // namespace aarith
