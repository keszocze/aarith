#pragma once

#include <aarith/integer.hpp>
#include <aarith/posit.hpp>
#include <functional>

/**
 * @brief Run function on each posit for a given type.
 *
 * Runs function operation on every posit of PositType in some unspecified
 * order.
 */
template <typename PositType>
static void for_each_posit(const std::function<void(const PositType&)>& operation)
{
    using namespace aarith;
    typename PositType::storage_type bits;

    do {
        const auto p = PositType::from(bits);
        operation(p);
    } while (++bits);
}
