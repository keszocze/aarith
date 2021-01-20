#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const valid<N, ES, WT>& v)
{
    return os << "start=" << v.get_start() << " end=" << v.get_end();
}

} // namespace aarith
