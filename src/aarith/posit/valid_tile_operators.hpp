#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const valid_tile<N, ES, WT>& t)
{
    os << t.value();

    // U-Bit Notation inspired by "The End Of Error", Gustafson, 2015.

    if (t.is_uncertain())
    {
        os << "···";
    }
    else
    {
        os << "↓";
    }

    return os;
}

} // namespace aarith
