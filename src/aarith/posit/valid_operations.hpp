#pragma once

#include <aarith/posit.hpp>
#include <stdexcept>

namespace aarith {

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr valid<N, ES, WT> sqrt(const valid<N, ES, WT>& v)
{
    using Valid = valid<N, ES, WT>;
    constexpr auto open = interval_bound::OPEN;

    if (v.is_irregular())
    {
        throw std::logic_error("sqrt not defined on irregular arguments");
    }

    const auto& start = v.get_start_value();
    const auto& end = v.get_end_value();

    const auto left = sqrt(start).decremented();
    const auto right = sqrt(end).incremented();

    return Valid::from(left, open, right, open);
}

} // namespace aarith
