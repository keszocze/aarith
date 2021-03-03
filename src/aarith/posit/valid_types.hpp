#pragma once

#include <aarith/posit.hpp>
#include <cstdint>

namespace aarith {

/**
 * Standard valid with 8 bit posits and 2 exponent bits.
 */
using valid8 = ivalid<8, 2, uint64_t>;

/**
 * Standard valid with 16 bit posits and 2 exponent bits.
 */
using valid16 = ivalid<16, 2, uint64_t>;

/**
 * Standard valid with 32 bit posits and 2 exponent bits.
 */
using valid32 = ivalid<32, 2, uint64_t>;

/**
 * Standard valid with 64 bit posits and 2 exponent bits.
 */
using valid64 = ivalid<64, 2, uint64_t>;

} // namespace aarith
