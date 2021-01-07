#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT> std::string to_binary(const quire<N, ES, WT>& q)
{
    const auto bits = q.get_bits();
    return to_binary(bits);
}

} // namespace aarith
