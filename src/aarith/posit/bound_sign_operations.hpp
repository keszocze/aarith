#pragma once

#include <aarith/posit.hpp>

namespace aarith {

[[nodiscard]] constexpr bool is_exact(bound_sign bs)
{
    return bs == bound_sign::EXACT;
}

[[nodiscard]] constexpr bool is_plus_eps(bound_sign bs)
{
    return bs == bound_sign::PLUS_EPS;
}

[[nodiscard]] constexpr bool is_minus_eps(bound_sign bs)
{
    return bs == bound_sign::MINUS_EPS;
}

[[nodiscard]] constexpr bool is_unsure(bound_sign bs)
{
    return bs == bound_sign::UNSURE;
}

[[nodiscard]] constexpr bound_sign negate(bound_sign bs)
{
    switch (bs)
    {
    case bound_sign::PLUS_EPS: return bound_sign::MINUS_EPS; break;
    case bound_sign::MINUS_EPS: return bound_sign::PLUS_EPS; break;
    default: return bs;
    };
}

} // namespace aarith
