#pragma once

#include <aarith/posit/binprod.hpp>
#include <aarith/posit/posit.hpp>
#include <aarith/posit/posit_operations.hpp>
#include <iostream>
#include <ostream>

namespace aarith {

template <size_t N, size_t ES, typename WT>
std::ostream& operator<<(std::ostream& os, const posit<N, ES, WT>& p)
{
    // TODO(Schärtl): actually print decimal representation; to do this
    // however we need to have arithmetic operators implemented first

    return os << to_binary(p.get_bits());
}

} // namespace aarith
