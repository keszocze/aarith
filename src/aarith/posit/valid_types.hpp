#pragma once

#include <aarith/posit_no_operators.hpp>
#include <cstdint>

namespace aarith {

/**
 * Standard valid with 8 bit posits and 2 exponent bits.
 */
using valid8 = valid<8, 2, uint64_t>;

/**
 * Standard valid with 16 bit posits and 2 exponent bits.
 */
using valid16 = valid<16, 2, uint64_t>;

/**
 * Standard valid with 32 bit posits and 2 exponent bits.
 */
using valid32 = valid<32, 2, uint64_t>;

/**
 * Standard valid with 64 bit posits and 2 exponent bits.
 */
using valid64 = valid<64, 2, uint64_t>;

} // namespace aarith
