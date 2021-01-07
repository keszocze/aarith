#pragma once

#include <aarith/posit.hpp>
#include <cstdint>

namespace aarith {

/**
 * Standard 8 bit quire with up to two exponent bits.
 */
using quire8 = quire<8, 2, uint64_t>;

/**
 * Standard 16 bit quire with up to two exponent bits.
 */
using quire16 = quire<16, 2, uint64_t>;

/**
 * Standard 32 bit quire with up to two exponent bits.
 */
using quire32 = quire<32, 2, uint64_t>;

/**
 * Standard 64 bit quire with up to two exponent bits.
 */
using quire64 = quire<64, 2, uint64_t>;

} // namespace aarith
