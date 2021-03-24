#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bound<N, ES, WT> abs(const bound<N, ES, WT>& b)
{
    auto value = b.get_value();
    auto sign = b.get_sign();

    if (b.value.is_negative())
    {
        value = abs(value);
        sign = negate(sign);
    }

    return bound<N, ES, WT>(value, sign);
}

} // namespace aarith
