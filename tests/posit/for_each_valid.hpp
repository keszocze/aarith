#pragma once

#include <aarith/posit.hpp>
#include <functional>

#include "for_each_posit.hpp"

template <typename ValidType>
static void for_each_valid(const std::function<void(const ValidType&)>& operation)
{
    using namespace aarith;

    using PositType = typename ValidType::posit_type;

    constexpr auto open = interval_bound::OPEN;
    constexpr auto closed = interval_bound::CLOSED;

    for_each_posit<PositType>([&](const PositType& p) {
        for_each_posit<PositType>([&](const PositType& q) {
            {
                const ValidType oo = ValidType::from(p, open, q, open);
                operation(oo);
            }

            {
                const ValidType oc = ValidType::from(p, open, q, closed);
                operation(oc);
            }

            {
                const ValidType co = ValidType::from(p, closed, q, open);
                operation(co);
            }

            {
                const ValidType cc = ValidType::from(p, closed, q, closed);
                operation(cc);
            }
        });
    });
}

template <typename ValidType>
static void for_each_regular_valid(const std::function<void(const ValidType&)>& operation)
{
    for_each_valid<ValidType>([&](const ValidType& v) {
        if (v.is_regular())
        {
            operation(v);
        }
    });
}
