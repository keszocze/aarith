#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const valid<N, ES, WT>& v)
{
    const auto& start = v.get_start();
    const auto& end = v.get_end();

    if (start.is_uncertain())
    {
        if (start == start.min())
        {
            os << "(-∞";
        }
        else if (start == start.max())
        {
            os << "(+∞";
        }
        else
        {
            os << "(" << start.value();
        }
    }
    else
    {
        os << "[" << start.value();
    }

    os << ", ";

    if (end.is_uncertain())
    {
        if (end == end.min())
        {
            os << "-∞)";
        }
        else if (end == end.max())
        {
            os << "+∞)";
        }
        else
        {
            os << end.value().incremented() << ")";
        }
    }
    else
    {
        os << end.value() << "]";
    }

    return os;
}

} // namespace aarith
