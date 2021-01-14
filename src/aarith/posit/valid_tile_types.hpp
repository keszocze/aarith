#pragma once

#include <aarith/posit.hpp>
#include <cstdint>

namespace aarith {

/**
 * Standard tile with 8 bit posits and 2 exponent bits.
 */
using valid_tile8 = valid_tile<8, 2, uint64_t>;

/**
 * Standard tile with 16 bit posits and 2 exponent bits.
 */
using valid_tile16 = valid_tile<16, 2, uint64_t>;

/**
 * Standard tile with 32 bit posits and 2 exponent bits.
 */
using valid_tile32 = valid_tile<32, 2, uint64_t>;

/**
 * Standard tile with 64 bit posits and 2 exponent bits.
 */
using valid_tile64 = valid_tile<64, 2, uint64_t>;

} // namespace aarith
