#pragma once

#include <aarith/posit_no_operators.hpp>
#include <sstream>

namespace aarith {

template <size_t N, size_t ES, typename WT> std::string to_binary(const valid<N, ES, WT>& v)
{
    std::stringstream ss;

    const auto& start_value = v.get_start_value();
    const auto& start_bound = v.get_start_bound();
    const auto& end_value = v.get_end_value();
    const auto& end_bound = v.get_end_bound();

    static const auto bound_to_binary = [](const interval_bound u) -> const char* {
        return is_open(u) ? "1" : "0";
    };

    ss << to_binary(start_value);
    ss << bound_to_binary(start_bound);
    ss << to_binary(end_value);
    ss << bound_to_binary(end_bound);

    return ss.str();
}

} // namespace aarith
