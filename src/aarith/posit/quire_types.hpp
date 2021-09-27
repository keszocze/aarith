#pragma once

#include <aarith/posit_no_operators.hpp>
#include <cstdint>

namespace aarith {

/**
 * Standard quire for 8 bit posits with 2 exponent bits.
 */
using quire8 = quire<8, 2, uint64_t>;

/**
 * Standard quire for 16 bit posits with 2 exponent bits.
 */
using quire16 = quire<16, 2, uint64_t>;

/**
 * Standard quire for 32 bit posits with 2 exponent bits.
 */
using quire32 = quire<32, 2, uint64_t>;

/**
 * Standard quire for 64 bit posits with 2 exponent bits.
 */
using quire64 = quire<64, 2, uint64_t>;

} // namespace aarith
