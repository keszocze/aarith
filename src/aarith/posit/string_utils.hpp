#pragma once

#include <aarith/integer.hpp>

namespace aarith {

template <size_t N, size_t ES, class WT>
[[nodiscard]] std::string to_binary(const posit<N, ES, WT>& value)
{
    return to_binary(value.get_bits());
}

} // namespace aarith
