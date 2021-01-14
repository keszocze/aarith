#pragma once

#include <aarith/posit.hpp>
#include <sstream>

namespace aarith {

template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const tile<N, ES, WT>& t)
{
    std::stringstream ss;

    if (t.is_uncertain())
    {
        // We are dealing with an uncertain tile, that is we are looking at
        // some open interval. First find out the bounds.

        posit<N, ES, WT> lower, upper;

        if (t.is_negative())
        {
            lower = t.incremented();
            upper = t;
        }
        else
        {
            lower = t;
            upper = t.incremented();
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
        ss << t.value() << "↓";
    }

    return os << ss.str();
}

} // namespace aarith
