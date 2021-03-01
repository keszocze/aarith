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

    // Handle special cases close to \pm\infty.

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
        if (start.is_uncertain())
        {
            os << "(" << start.as_start_value();
        }
        else
        {
            os << "[" << start.as_start_value();
        }

        os << ", ";

        // Handle right bounds.

        if (end.is_uncertain())
        {
            os << end.as_end_value() << ")";
        }
        else
        {
            os << end.as_end_value() << "]";
        }

        return os;
    }
}

} // namespace aarith
