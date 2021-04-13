#pragma once

#include <cassert>

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const valid<N, ES, WT>& v)
{
    using posit_type = typename valid<N, ES, WT>::posit_type;

    const posit_type& start_value = v.get_start_value();
    const interval_bound& start_bound = v.get_start_bound();

    const posit_type& end_value = v.get_end_value();
    const interval_bound& end_bound = v.get_end_bound();

    //
    // Handle special cases that are not printed like intervals.
    //

    if (v.is_nar())
    {
        return os << "NaR";
    }

    if (v.is_empty())
    {
        return os << "∅";
    }

    if (v.is_all_reals())
    {
        return os << "ℝ";
    }

    if (v.is_full())
    {
        return os << "◯";
    }

    //
    // Handle exact values [p, p] = p.
    //

    if (v.is_exact_real())
    {
        return os << v.as_exact_real();
    }

    //
    // Handle intervals {p, q}.
    //

    {
        //
        // Handle left bound.
        //

        if (is_open(start_bound))
        {
            os << "(";
        }
        else
        {
            assert(is_closed(start_bound));
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

        if (is_open(end_bound))
        {
            os << ")";
        }
        else
        {
            assert(is_closed(end_bound));
            os << "]";
        }

        //
        // Done!
        //

        return os;
    }
}

} // namespace aarith
