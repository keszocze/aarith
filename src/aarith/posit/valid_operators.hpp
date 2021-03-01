#pragma once

#include <cassert>

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const valid<N, ES, WT>& v)
{
    using posit_type = typename valid<N, ES, WT>::posit_type;
    const auto& start = v.get_start();
    const auto& end = v.get_end();

    // Handle special cases close to ±∞.

    if (v == v.max())
    {
        return os << "(" << posit_type::max() << ", +∞)";
    }

    if (v == v.min())
    {
        return os << "(-∞, " << posit_type::min() << ")";
    }

    if (v == v.nar())
    {
        return os << "±∞";
    }

    // Handle exact values.

    if (start == end && !start.is_uncertain())
    {
        assert(!end.is_uncertain());
        return os << start.value();
    }

    // Handle intervals.

    {
        // Handle left bound.

        if (start.is_uncertain())
        {
            if (start == start.min())
            {
                os << "(-∞";
            }
            else
            {
                os << "(" << start.as_start_value();
            }
        }
        else
        {
            os << "[" << start.as_start_value();
        }

        os << ", ";

        // Handle right bound.

        if (end.is_uncertain())
        {
            if (end == end.max())
            {
                os << "∞)";
            }
            else
            {
                os << end.as_end_value() << ")";
            }
        }
        else
        {
            os << end.as_end_value() << "]";
        }

        return os;
    }
}

} // namespace aarith
