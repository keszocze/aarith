#pragma once

#include <aarith/posit.hpp>
#include <ostream>

namespace aarith {

template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const posit<N, ES, WT>& p)
{
    return os << quire(p);
}

} // namespace aarith
