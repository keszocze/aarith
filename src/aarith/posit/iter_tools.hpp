#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <typename PositType>
inline void for_each_posit(const std::function<void(const PositType&)>& operation)
{
    typename PositType::storage_type bits;

    do
    {
        const auto p = PositType::from(bits);
        operation(p);
    } while (++bits);
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
