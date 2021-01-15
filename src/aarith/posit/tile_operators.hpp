#pragma once

#include <aarith/posit.hpp>
#include <sstream>

namespace aarith {

template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const tile<N, ES, WT>& t)
{
    const posit<N, ES, WT>& value = t.value();

    std::stringstream ss;

    if (t.is_uncertain())
    {
        // We are dealing with an uncertain tile, that is we are looking at
        // some open interval. First find out the bounds.

        posit<N, ES, WT> lower, upper;

        if (value.is_negative())
        {
            lower = value.incremented();
            upper = value;
        }
        else
        {
            lower = value;
            upper = value.incremented();
        }

        // Print the left bound.

        ss << "(";

        if (t == t.min())
        {
            ss << "-∞";
        }
        else
        {
            ss << lower;
        }

        ss << ", ";

        // Print the right bound.

        if (t == t.max())
        {
            ss << "+∞";
        }
        else
        {
            ss << upper;
        }

        ss << ")";
    }
    else
    {
        ss << value << "↓";
    }

    return os << ss.str();
}

} // namespace aarith
