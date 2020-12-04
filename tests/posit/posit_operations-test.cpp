#include <catch.hpp>
#include <iostream>

#include "../test-signature-ranges.hpp"
#include <aarith/integer.hpp>
#include <aarith/posit.hpp>

using namespace aarith;

constexpr size_t NBits = 16;
constexpr size_t ExponentSize = 8;
using WordType = unsigned int;

using P16 = aarith::posit<NBits, ExponentSize, WordType>;
using RegimeType = decltype(get_regime(P16()));

static void require_regime(const P16& p, const uint16_t& expected, size_t expected_size)
{
    const size_t nregime = get_num_regime_bits(p);
    REQUIRE(nregime == expected_size);

    const RegimeType regime = get_regime(p);
    REQUIRE(regime == RegimeType(expected));
}

SCENARIO("Extract the regime bits")
{
    GIVEN("Some arbitrary posit values")
    {
        THEN("Assert that the returned regime is correct.")
        {
            require_regime(P16(0b01010101010101010U), 0b00000000000000000U, 0);
            require_regime(P16(0b10101010101010101U), 0b00000000000000000U, 0);

            require_regime(P16(0b01111101010101010U), 0b00000000000001111U, 4);
            require_regime(P16(0b10000010101010101U), 0b00000000000000000U, 4);

            require_regime(P16(0b01111111111111110U), 0b00011111111111111U, 14);
            require_regime(P16(0b10000000000000001U), 0b00000000000000000U, 14);
        }
    }
}
