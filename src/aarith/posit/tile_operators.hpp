#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const tile<N, ES, WT>& t)
{
    const posit<N, ES, WT>& value = t.value();

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

        os << "(";

        if (t == t.min())
        {
            os << "-∞";
        }
        else
        {
            os << lower;
        }

        os << ", ";

        // Print the right bound.

        if (t == t.max())
        {
            os << "+∞";
        }
        else
        {
            os << upper;
        }

        os << ")";
    }
    else
    {
        os << value << "↓";
    }

    return os;
}

} // namespace aarith
