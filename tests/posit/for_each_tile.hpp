#pragma once

#include <aarith/posit.hpp>
#include <functional>

#include "for_each_posit.hpp"

/**
 * @brief Run function on each tile for a given type.
 *
 * Runs function operation on every tile of TileType in some unspecified
 * order.
 */
template <typename TileType>
static void for_each_tile(const std::function<void(const TileType&)>& operation)
{
    using namespace aarith;

    using PositType = typename TileType::posit_type;

    for_each_posit<PositType>([&](const PositType& p) {
        const TileType certain = TileType::from(p, false);
        const TileType uncertain = TileType::from(p, true);

        operation(certain);
        operation(uncertain);
    });
}
