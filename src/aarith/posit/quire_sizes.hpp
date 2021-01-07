#pragma once

#include <aarith/posit.hpp>

namespace aarith {

[[nodiscard]] constexpr size_t quire_integer_width(size_t posit_width, size_t posit_exponent_size)
{
    return (1 << posit_exponent_size) * (posit_width - 2);
}

[[nodiscard]] constexpr size_t quire_fraction_width(size_t posit_width, size_t posit_exponent_size)
{
    // integer and fraction have the same width in the most recent draft (as
    // of 2021/01/07) so we just return the integer size
    return quire_integer_width(posit_width, posit_exponent_size);
}

[[nodiscard]] constexpr size_t quire_carry_width(size_t posit_width, size_t posit_exponent_size)
{
    const size_t ntotal = quire_width(posit_width, posit_exponent_size);

    const size_t nsign = 1;
    const size_t ninteger = quire_integer_width(posit_width, posit_exponent_size);
    const size_t nfrac = quire_fraction_width(posit_width, posit_exponent_size);

    return ntotal - nsign - ninteger - nfrac;
}

[[nodiscard]] constexpr size_t quire_width(size_t posit_width, size_t posit_exponent_size)
{
    const size_t nsign = 1;
    const size_t min_quire_width = 30;
    const size_t ninteger = quire_integer_width(posit_width, posit_exponent_size);
    const size_t nfrac = quire_fraction_width(posit_width, posit_exponent_size);

    const size_t min_width = nsign + min_quire_width + ninteger + nfrac;

    // We now need to round min_width to the next power of two. To do that, we
    // effectively compute width_power := floor(ld min_width) and then return
    // 2^width_power.

    size_t actual = min_width;
    size_t width_power = 0;
    size_t truncated_ones = 0;

    while (actual)
    {
        if (actual & 1)
        {
            truncated_ones += 1;
        }

        actual <<= 1;

        width_power += 1;
    }

    if (truncated_ones > 0)
    {
        return 1 << width_power;
    }
    else
    {
        // min_width already is a power of two
        return min_width;
    }
}

} // namespace aarith
