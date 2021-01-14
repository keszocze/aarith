#pragma once

#include <aarith/posit.hpp>
#include <cstdint>

namespace aarith {

/**
 * Standard tile with 8 bit posits and 2 exponent bits.
 */
using tile8 = tile<8, 2, uint64_t>;

/**
 * Standard tile with 16 bit posits and 2 exponent bits.
 */
using tile16 = tile<16, 2, uint64_t>;

/**
 * Standard tile with 32 bit posits and 2 exponent bits.
 */
using tile32 = tile<32, 2, uint64_t>;

/**
 * Standard tile with 64 bit posits and 2 exponent bits.
 */
using tile64 = tile<64, 2, uint64_t>;

} // namespace aarith
