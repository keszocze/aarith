#pragma once

#include <aarith/posit_no_operators.hpp>
#include <cstdint>

namespace aarith {

/**
 * Standard 8 bit posit with up to two exponent bits.
 */
using posit8 = posit<8, 2, uint64_t>;

/**
 * Standard 16 bit posit with up to two exponent bits.
 */
using posit16 = posit<16, 2, uint64_t>;

/**
 * Standard 32 bit posit with up to two exponent bits.
 */
using posit32 = posit<32, 2, uint64_t>;

/**
 * Standard 64 bit posit with up to two exponent bits.
 */
using posit64 = posit<64, 2, uint64_t>;

} // namespace aarith
