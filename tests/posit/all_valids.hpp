#pragma once

#include <aarith/posit.hpp>

template <size_t N, size_t ES, typename WT>
inline std::vector<aarith::valid<N, ES, WT>> all_valids(const aarith::posit<N, ES, WT>& start,
                                                        const aarith::posit<N, ES, WT>& end)
{
    using namespace aarith;

    using Valid = valid<N, ES, WT>;

    constexpr auto open = interval_bound::OPEN;
    constexpr auto closed = interval_bound::CLOSED;

    return {Valid::from(start, open, end, open), Valid::from(start, open, end, closed),
            Valid::from(start, closed, end, open), Valid::from(start, closed, end, closed)};
}
