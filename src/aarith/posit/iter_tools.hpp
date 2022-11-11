#pragma once

#include <aarith/posit_no_operators.hpp>

namespace aarith {

template <typename PositType>
inline void for_each_posit(const std::function<void(const PositType&)>& operation)
{
    PositType current = PositType::min();

    do
    {
        operation(current);
        current = current.incremented();
    } while (current != PositType::min());
}

template <typename PositType>
inline void for_each_regular_posit(const std::function<void(const PositType&)>& operation)
{
    for_each_posit<PositType>([&](const PositType& p) {
        if (!p.is_nar())
        {
            operation(p);
        }
    });
}

template <typename ValidType>
inline void for_each_valid(const std::function<void(const ValidType&)>& operation)
{
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
inline void for_each_regular_valid(const std::function<void(const ValidType&)>& operation)
{
    for_each_valid<ValidType>([&](const ValidType& v) {
        if (v.is_regular())
        {
            operation(v);
        }
    });
}

} // namespace aarith
