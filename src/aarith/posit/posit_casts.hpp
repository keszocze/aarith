#pragma once

#include <aarith/core/word_array_cast_operations.hpp>
#include <aarith/posit_no_operators.hpp>

namespace aarith {

template <typename DestinationPosit, size_t SourceWidth, size_t SourceExponentSize,
          typename SourceWordType>
constexpr DestinationPosit
width_cast(const posit<SourceWidth, SourceExponentSize, SourceWordType>& p)
{
    // extract parameters from DestinationPosit

    constexpr size_t DestinationWidth = DestinationPosit::width();
    constexpr size_t DestinationExponentSize = DestinationPosit::exponent_size();

    // assert that we are upcasting, not downcasting

    static_assert(DestinationExponentSize == SourceExponentSize, "exponent sizes have to match");

    static_assert(
        DestinationWidth >= SourceWidth,
        "width of returned posit needs to be at least as big as the posit to be converted");

    // extend bits and construct new posit

    const auto bits = p.get_bits();
    const auto extended = width_cast<DestinationWidth>(bits);

    constexpr size_t shift = DestinationWidth - SourceWidth;
    const auto shifted = extended << shift;

    return DestinationPosit::from(shifted);
}

} // namespace aarith
