#pragma once

#include <cassert>

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const ivalid<N, ES, WT>& v)
{
    using posit_type = typename ivalid<N, ES, WT>::posit_type;

    const posit_type& start_value = v.get_start_value();
    const interval_bound& start_bound = v.get_start_bound();

    const posit_type& end_value = v.get_end_value();
    const interval_bound& end_bound = v.get_end_bound();

    constexpr interval_bound open = interval_bound::OPEN;
    constexpr interval_bound closed = interval_bound::CLOSED;

    //
    // Handle special cases (1) empty set and (2) NaR. These need special
    // treatment as we do not print them as regular values.
    //

    if (v.is_empty())
    {
        return os << "∅";
    }

    if (v.is_nar())
    {
        return os << "NaR";
    }

    //
    // Handle exact values [p, p] = p.
    //

    if (start_bound == closed && end_bound == closed && start_value == end_value)
    {
        return os << start_value;
    }

    //
    // Handle intervals {p, q}.
    //

    {
        //
        // Handle left bound.
        //

        if (start_bound == open)
        {
            os << "(";
        }
        else
        {
            assert(start_bound == closed);
            os << "[";
        }

        if (start_value.is_nar())
        {
            os << "-∞";
        }
        else
        {
            os << start_value;
        }

        //
        // Print seperator.
        //

        os << ", ";

        //
        // Handle right bound.
        //

        if (end_value.is_nar())
        {
            os << "∞";
        }
        else
        {
            os << end_value;
        }

        if (end_bound == open)
        {
            os << ")";
        }
        else
        {
            assert(end_bound == closed);
            os << "]";
        }

        //
        // Done!
        //

        return os;
    }
}

} // namespace aarith
