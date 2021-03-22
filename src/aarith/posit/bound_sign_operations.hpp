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

} // namespace aarith
