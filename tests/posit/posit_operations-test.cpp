#include <catch.hpp>
#include <iostream>

#include "../test-signature-ranges.hpp"
#include <aarith/integer.hpp>
#include <aarith/posit.hpp>
#include <iostream>

using namespace aarith;

constexpr size_t NBits = 6;
constexpr size_t ExponentSize = 2;
using WordType = uint16_t;

using P8 = aarith::posit<NBits, ExponentSize, WordType>;

static void require_regime(const P8& p, size_t expected_nregime, int16_t expected_regime_value)
{
    const size_t nregime = get_num_regime_bits(p);
    REQUIRE(nregime == expected_nregime);

    const auto regime_value = get_regime_value(p);
    using RegimeValType = decltype(regime_value);
    REQUIRE(regime_value == RegimeValType(expected_regime_value));
}

SCENARIO("Extract the regime bits")
{
    GIVEN("Some arbitrary posit values")
    {
        THEN("Assert that the returned regime is correct.")
        {
            // Based on Posit Arithmetic, Gustafson, October 2017, p. 15.

            require_regime(P8(0b100000U), 6, 5);
            require_regime(P8(0b100001U), 5, 4);
            require_regime(P8(0b100010U), 4, 3);
            require_regime(P8(0b100011U), 3, 2);
            require_regime(P8(0b100100U), 3, 2);
            require_regime(P8(0b100101U), 2, 1);
            require_regime(P8(0b100110U), 2, 1);
            require_regime(P8(0b100111U), 2, 1);
            require_regime(P8(0b101000U), 2, 1);
            require_regime(P8(0b101011U), 1, 0);
            require_regime(P8(0b101001U), 1, 0);
            require_regime(P8(0b101010U), 1, 0);
            require_regime(P8(0b101100U), 1, 0);
            require_regime(P8(0b101101U), 1, 0);
            require_regime(P8(0b101110U), 1, 0);
            require_regime(P8(0b101111U), 1, 0);
            require_regime(P8(0b110000U), 1, 0);
            require_regime(P8(0b110001U), 1, 0);
            require_regime(P8(0b110010U), 1, 0);
            require_regime(P8(0b110011U), 1, 0);
            require_regime(P8(0b110100U), 1, 0);
            require_regime(P8(0b110101U), 1, 0);
            require_regime(P8(0b110110U), 1, 0);
            require_regime(P8(0b110111U), 1, 0);
            require_regime(P8(0b111000U), 1, 0);
            require_regime(P8(0b111001U), 2, 1);
            require_regime(P8(0b111010U), 2, 1);
            require_regime(P8(0b111011U), 2, 1);
            require_regime(P8(0b111100U), 2, 1);
            require_regime(P8(0b111101U), 3, 2);
            require_regime(P8(0b111110U), 3, 2);
            require_regime(P8(0b111111U), 4, 3);

            require_regime(P8(0b000000U), 5, -5);
            require_regime(P8(0b000001U), 4, -4);
            require_regime(P8(0b000010U), 3, -3);
            require_regime(P8(0b000011U), 3, -3);
            require_regime(P8(0b000100U), 2, -2);
            require_regime(P8(0b000101U), 2, -2);
            require_regime(P8(0b000110U), 2, -2);
            require_regime(P8(0b000111U), 2, -2);
            require_regime(P8(0b001000U), 1, -1);
            require_regime(P8(0b001001U), 1, -1);
            require_regime(P8(0b001010U), 1, -1);
            require_regime(P8(0b001011U), 1, -1);
            require_regime(P8(0b001100U), 1, -1);
            require_regime(P8(0b001101U), 1, -1);
            require_regime(P8(0b001110U), 1, -1);
            require_regime(P8(0b001111U), 1, -1);
            require_regime(P8(0b010000U), 1, -1);
            require_regime(P8(0b010001U), 1, -1);
            require_regime(P8(0b010010U), 1, -1);
            require_regime(P8(0b010011U), 1, -1);
            require_regime(P8(0b010100U), 1, -1);
            require_regime(P8(0b010101U), 1, -1);
            require_regime(P8(0b010110U), 1, -1);
            require_regime(P8(0b010111U), 1, -1);
            require_regime(P8(0b011000U), 2, -2);
            require_regime(P8(0b011001U), 2, -2);
            require_regime(P8(0b011010U), 2, -2);
            require_regime(P8(0b011011U), 2, -2);
            require_regime(P8(0b011100U), 3, -3);
            require_regime(P8(0b011101U), 3, -3);
            require_regime(P8(0b011110U), 4, -4);
            require_regime(P8(0b011111U), 5, -5);

            // Based on Posit Arithmetic, Gustafson, October 2017, p. 12
            require_regime(P8(0b000011U), 3, -3);
            require_regime(P8(0b100001U), 5, 4);
        }
    }
}
