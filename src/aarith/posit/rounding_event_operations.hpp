#pragma once

#include <aarith/posit_no_operators.hpp>

namespace aarith {

[[nodiscard]] constexpr bool is_not_rounded(rounding_event r)
{
    return r == rounding_event::NOT_ROUNDED;
}

[[nodiscard]] constexpr bool is_rounded_down(rounding_event r)
{
    return r == rounding_event::ROUNDED_DOWN;
}

[[nodiscard]] constexpr bool is_rounded_up(rounding_event r)
{
    return r == rounding_event::ROUNDED_UP;
}

} // namespace aarith
