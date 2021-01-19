#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr std::vector<tile<N, ES, WT>> all_values_in(const valid<N, ES, WT>& v)
{
    // TODO (Schärtl): Switch to generator-style function

    using Valid = valid<N, ES, WT>;
    using Tile = typename Valid::tile_type;

    const Tile& start = v.get_start();
    const Tile& end = v.get_end();

    // Handle the particular case where both tiles are identical.

    if (start == end)
    {
        if (v.is_empty())
        {
            return {};
        }
        else
        {
            return {start};
        }
    }

    // Handle regular cases.

    std::vector<Tile> ret;
    Tile current = start;

    while (current != end.incremented())
    {
        ret.push_back(current);
        current = current.incremented();
    }

    return ret;
}

} // namespace aarith
