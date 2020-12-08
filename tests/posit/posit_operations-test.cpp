#include <catch.hpp>
#include <iostream>

#include "../test-signature-ranges.hpp"
#include <aarith/integer.hpp>
#include <aarith/posit.hpp>
#include <iostream>

using namespace aarith;

using P6 = aarith::posit<6, 2>;
using P8 = aarith::posit<8, 2>;

/*

static void require_regime(const P6& p, size_t expected_nregime, int16_t expected_regime_value)
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

            require_regime(P6(0b100000U), 6, 5);
            require_regime(P6(0b100001U), 5, 4);
            require_regime(P6(0b100010U), 4, 3);
            require_regime(P6(0b100011U), 3, 2);
            require_regime(P6(0b100100U), 3, 2);
            require_regime(P6(0b100101U), 2, 1);
            require_regime(P6(0b100110U), 2, 1);
            require_regime(P6(0b100111U), 2, 1);
            require_regime(P6(0b101000U), 2, 1);
            require_regime(P6(0b101011U), 1, 0);
            require_regime(P6(0b101001U), 1, 0);
            require_regime(P6(0b101010U), 1, 0);
            require_regime(P6(0b101100U), 1, 0);
            require_regime(P6(0b101101U), 1, 0);
            require_regime(P6(0b101110U), 1, 0);
            require_regime(P6(0b101111U), 1, 0);
            require_regime(P6(0b110000U), 1, 0);
            require_regime(P6(0b110001U), 1, 0);
            require_regime(P6(0b110010U), 1, 0);
            require_regime(P6(0b110011U), 1, 0);
            require_regime(P6(0b110100U), 1, 0);
            require_regime(P6(0b110101U), 1, 0);
            require_regime(P6(0b110110U), 1, 0);
            require_regime(P6(0b110111U), 1, 0);
            require_regime(P6(0b111000U), 1, 0);
            require_regime(P6(0b111001U), 2, 1);
            require_regime(P6(0b111010U), 2, 1);
            require_regime(P6(0b111011U), 2, 1);
            require_regime(P6(0b111100U), 2, 1);
            require_regime(P6(0b111101U), 3, 2);
            require_regime(P6(0b111110U), 3, 2);
            require_regime(P6(0b111111U), 4, 3);

            require_regime(P6(0b000000U), 5, -5);
            require_regime(P6(0b000001U), 4, -4);
            require_regime(P6(0b000010U), 3, -3);
            require_regime(P6(0b000011U), 3, -3);
            require_regime(P6(0b000100U), 2, -2);
            require_regime(P6(0b000101U), 2, -2);
            require_regime(P6(0b000110U), 2, -2);
            require_regime(P6(0b000111U), 2, -2);
            require_regime(P6(0b001000U), 1, -1);
            require_regime(P6(0b001001U), 1, -1);
            require_regime(P6(0b001010U), 1, -1);
            require_regime(P6(0b001011U), 1, -1);
            require_regime(P6(0b001100U), 1, -1);
            require_regime(P6(0b001101U), 1, -1);
            require_regime(P6(0b001110U), 1, -1);
            require_regime(P6(0b001111U), 1, -1);
            require_regime(P6(0b010000U), 1, -1);
            require_regime(P6(0b010001U), 1, -1);
            require_regime(P6(0b010010U), 1, -1);
            require_regime(P6(0b010011U), 1, -1);
            require_regime(P6(0b010100U), 1, -1);
            require_regime(P6(0b010101U), 1, -1);
            require_regime(P6(0b010110U), 1, -1);
            require_regime(P6(0b010111U), 1, -1);
            require_regime(P6(0b011000U), 2, -2);
            require_regime(P6(0b011001U), 2, -2);
            require_regime(P6(0b011010U), 2, -2);
            require_regime(P6(0b011011U), 2, -2);
            require_regime(P6(0b011100U), 3, -3);
            require_regime(P6(0b011101U), 3, -3);
            require_regime(P6(0b011110U), 4, -4);
            require_regime(P6(0b011111U), 5, -5);

            // Based on Posit Arithmetic, Gustafson, October 2017, p. 12
            require_regime(P6(0b000011U), 3, -3);
            require_regime(P6(0b100001U), 5, 4);
        }
    }
}

static void require_exponent(const P6& p, size_t expected_nexp, uint16_t expected_exponent_value)
{
    const size_t nexp = get_num_exponent_bits<NBits, ExponentSize, WordType>(p);
    REQUIRE(nexp == expected_nexp);

    const auto exp_value = get_exponent_value(p);
    using ExponentValType = decltype(exp_value);
    REQUIRE(exp_value == ExponentValType(expected_exponent_value));
}

SCENARIO("Find the exponent bits")
{
    GIVEN("Some arbitrary posit values")
    {
        THEN("Assert that the returned exponent values are correct")
        {
            // Based on Posit Arithmetic, Gustafson, October 2017, p. 15.

            require_exponent(P6(0b100000U), 0, 0);
            require_exponent(P6(0b100001U), 0, 0);
            require_exponent(P6(0b100010U), 0, 0);
            require_exponent(P6(0b100011U), 1, 1);
            require_exponent(P6(0b100100U), 1, 0);
            require_exponent(P6(0b100101U), 2, 3);
            require_exponent(P6(0b100110U), 2, 2);
            require_exponent(P6(0b100111U), 2, 1);
            require_exponent(P6(0b101000U), 2, 0);
            require_exponent(P6(0b101001U), 2, 3);
            require_exponent(P6(0b101010U), 2, 3);
            require_exponent(P6(0b101011U), 2, 2);
            require_exponent(P6(0b101100U), 2, 2);
            require_exponent(P6(0b101101U), 2, 1);
            require_exponent(P6(0b101110U), 2, 1);
            require_exponent(P6(0b101111U), 2, 0);
            require_exponent(P6(0b110000U), 2, 0);
            require_exponent(P6(0b110001U), 2, 3);
            require_exponent(P6(0b110010U), 2, 3);
            require_exponent(P6(0b110011U), 2, 2);
            require_exponent(P6(0b110100U), 2, 2);
            require_exponent(P6(0b110101U), 2, 1);
            require_exponent(P6(0b110110U), 2, 1);
            require_exponent(P6(0b110111U), 2, 0);
            require_exponent(P6(0b111000U), 2, 0);
            require_exponent(P6(0b111001U), 2, 3);
            require_exponent(P6(0b111010U), 2, 2);
            require_exponent(P6(0b111011U), 2, 1);
            require_exponent(P6(0b111100U), 2, 0);
            require_exponent(P6(0b111101U), 1, 1);
            require_exponent(P6(0b111110U), 1, 0);
            require_exponent(P6(0b111111U), 0, 0);

            require_exponent(P6(0b000000U), 0, 0);
            require_exponent(P6(0b000001U), 0, 0);
            require_exponent(P6(0b000010U), 1, 0);
            require_exponent(P6(0b000011U), 1, 1);
            require_exponent(P6(0b000100U), 2, 0);
            require_exponent(P6(0b000101U), 2, 1);
            require_exponent(P6(0b000110U), 2, 2);
            require_exponent(P6(0b000111U), 2, 3);
            require_exponent(P6(0b001000U), 2, 0);
            require_exponent(P6(0b001001U), 2, 0);
            require_exponent(P6(0b001010U), 2, 1);
            require_exponent(P6(0b001011U), 2, 1);
            require_exponent(P6(0b001100U), 2, 2);
            require_exponent(P6(0b001101U), 2, 2);
            require_exponent(P6(0b001110U), 2, 3);
            require_exponent(P6(0b001111U), 2, 3);
            require_exponent(P6(0b010000U), 2, 0);
            require_exponent(P6(0b010001U), 2, 0);
            require_exponent(P6(0b010010U), 2, 1);
            require_exponent(P6(0b010011U), 2, 1);
            require_exponent(P6(0b010100U), 2, 2);
            require_exponent(P6(0b010101U), 2, 2);
            require_exponent(P6(0b010110U), 2, 3);
            require_exponent(P6(0b010111U), 2, 3);
            require_exponent(P6(0b011000U), 2, 0);
            require_exponent(P6(0b011001U), 2, 1);
            require_exponent(P6(0b011010U), 2, 2);
            require_exponent(P6(0b011011U), 2, 3);
            require_exponent(P6(0b011100U), 1, 0);
            require_exponent(P6(0b011101U), 1, 1);
            require_exponent(P6(0b011110U), 0, 0);
            require_exponent(P6(0b011111U), 0, 0);
        }
    }
}

static void require_fraction(const P6& p, size_t expected_nfrac, uint16_t expected_fraction_value)
{
    const size_t nfrac = get_num_fraction_bits(p);
    REQUIRE(nfrac == expected_nfrac);

    const auto frac_value = get_fraction_value(p);
    using FracValType = decltype(frac_value);
    REQUIRE(frac_value == FracValType(expected_fraction_value));
}

SCENARIO("Find the fraction bits")
{
    GIVEN("Some arbitrary posit values")
    {
        THEN("Assert that the returned fraction values are correct")
        {
            // Based on Posit Arithmetic, Gustafson, October 2017, p. 15.

            require_fraction(P6(0b100000U), 0, 0);
            require_fraction(P6(0b100001U), 0, 0);
            require_fraction(P6(0b100010U), 0, 0);
            require_fraction(P6(0b100011U), 0, 0);
            require_fraction(P6(0b100100U), 0, 0);
            require_fraction(P6(0b100101U), 0, 0);
            require_fraction(P6(0b100110U), 0, 0);
            require_fraction(P6(0b100111U), 0, 0);
            require_fraction(P6(0b101000U), 0, 0);
            require_fraction(P6(0b101001U), 1, 1);
            require_fraction(P6(0b101010U), 1, 0);
            require_fraction(P6(0b101011U), 1, 1);
            require_fraction(P6(0b101100U), 1, 0);
            require_fraction(P6(0b101101U), 1, 1);
            require_fraction(P6(0b101110U), 1, 0);
            require_fraction(P6(0b101111U), 1, 1);
            require_fraction(P6(0b110000U), 1, 0);
            require_fraction(P6(0b110001U), 1, 1);
            require_fraction(P6(0b110010U), 1, 0);
            require_fraction(P6(0b110011U), 1, 1);
            require_fraction(P6(0b110100U), 1, 0);
            require_fraction(P6(0b110101U), 1, 1);
            require_fraction(P6(0b110110U), 1, 0);
            require_fraction(P6(0b110111U), 1, 1);
            require_fraction(P6(0b111000U), 1, 0);
            require_fraction(P6(0b111001U), 0, 0);
            require_fraction(P6(0b111010U), 0, 0);
            require_fraction(P6(0b111011U), 0, 0);
            require_fraction(P6(0b111100U), 0, 0);
            require_fraction(P6(0b111101U), 0, 0);
            require_fraction(P6(0b111110U), 0, 0);
            require_fraction(P6(0b111111U), 0, 0);

            require_fraction(P6(0b000000U), 0, 0);
            require_fraction(P6(0b000001U), 0, 0);
            require_fraction(P6(0b000010U), 0, 0);
            require_fraction(P6(0b000011U), 0, 0);
            require_fraction(P6(0b000100U), 0, 0);
            require_fraction(P6(0b000101U), 0, 0);
            require_fraction(P6(0b000110U), 0, 0);
            require_fraction(P6(0b000111U), 0, 0);
            require_fraction(P6(0b001000U), 1, 0);
            require_fraction(P6(0b001001U), 1, 1);
            require_fraction(P6(0b001010U), 1, 0);
            require_fraction(P6(0b001011U), 1, 1);
            require_fraction(P6(0b001100U), 1, 0);
            require_fraction(P6(0b001101U), 1, 1);
            require_fraction(P6(0b001110U), 1, 0);
            require_fraction(P6(0b001111U), 1, 1);
            require_fraction(P6(0b010000U), 1, 0);
            require_fraction(P6(0b010001U), 1, 1);
            require_fraction(P6(0b010010U), 1, 0);
            require_fraction(P6(0b010011U), 1, 1);
            require_fraction(P6(0b010100U), 1, 0);
            require_fraction(P6(0b010101U), 1, 1);
            require_fraction(P6(0b010110U), 1, 0);
            require_fraction(P6(0b010111U), 1, 1);
            require_fraction(P6(0b011000U), 0, 0);
            require_fraction(P6(0b011001U), 0, 0);
            require_fraction(P6(0b011010U), 0, 0);
            require_fraction(P6(0b011011U), 0, 0);
            require_fraction(P6(0b011100U), 0, 0);
            require_fraction(P6(0b011101U), 0, 0);
            require_fraction(P6(0b011110U), 0, 0);
            require_fraction(P6(0b011111U), 0, 0);
        }
    }
}

*/

static void check_bitstring(const P8& p, const char* expected)
{
    const std::string actual = to_binary(p, " ");
    REQUIRE(std::string(expected) == actual);
}

SCENARIO("Print the bitstrings")
{
    GIVEN("All posit<8, 2> values")
    {
        THEN("Assert that the bitstring prints correctly")
        {
            // Test based on output by Juli library "SoftPosit",
            // https://github.com/milankl/SoftPosit.jl

            check_bitstring(P8(0b00000000U), "0 0000000");
            check_bitstring(P8(0b00000001U), "0 0000001");
            check_bitstring(P8(0b00000010U), "0 000001 0");
            check_bitstring(P8(0b00000011U), "0 000001 1");
            check_bitstring(P8(0b00000100U), "0 00001 00");
            check_bitstring(P8(0b00000101U), "0 00001 01");
            check_bitstring(P8(0b00000110U), "0 00001 10");
            check_bitstring(P8(0b00000111U), "0 00001 11");
            check_bitstring(P8(0b00001000U), "0 0001 00 0");
            check_bitstring(P8(0b00001001U), "0 0001 00 1");
            check_bitstring(P8(0b00001010U), "0 0001 01 0");
            check_bitstring(P8(0b00001011U), "0 0001 01 1");
            check_bitstring(P8(0b00001100U), "0 0001 10 0");
            check_bitstring(P8(0b00001101U), "0 0001 10 1");
            check_bitstring(P8(0b00001110U), "0 0001 11 0");
            check_bitstring(P8(0b00001111U), "0 0001 11 1");
            check_bitstring(P8(0b00010000U), "0 001 00 00");
            check_bitstring(P8(0b00010001U), "0 001 00 01");
            check_bitstring(P8(0b00010010U), "0 001 00 10");
            check_bitstring(P8(0b00010011U), "0 001 00 11");
            check_bitstring(P8(0b00010100U), "0 001 01 00");
            check_bitstring(P8(0b00010101U), "0 001 01 01");
            check_bitstring(P8(0b00010110U), "0 001 01 10");
            check_bitstring(P8(0b00010111U), "0 001 01 11");
            check_bitstring(P8(0b00011000U), "0 001 10 00");
            check_bitstring(P8(0b00011001U), "0 001 10 01");
            check_bitstring(P8(0b00011010U), "0 001 10 10");
            check_bitstring(P8(0b00011011U), "0 001 10 11");
            check_bitstring(P8(0b00011100U), "0 001 11 00");
            check_bitstring(P8(0b00011101U), "0 001 11 01");
            check_bitstring(P8(0b00011110U), "0 001 11 10");
            check_bitstring(P8(0b00011111U), "0 001 11 11");
            check_bitstring(P8(0b00100000U), "0 01 00 000");
            check_bitstring(P8(0b00100001U), "0 01 00 001");
            check_bitstring(P8(0b00100010U), "0 01 00 010");
            check_bitstring(P8(0b00100011U), "0 01 00 011");
            check_bitstring(P8(0b00100100U), "0 01 00 100");
            check_bitstring(P8(0b00100101U), "0 01 00 101");
            check_bitstring(P8(0b00100110U), "0 01 00 110");
            check_bitstring(P8(0b00100111U), "0 01 00 111");
            check_bitstring(P8(0b00101000U), "0 01 01 000");
            check_bitstring(P8(0b00101001U), "0 01 01 001");
            check_bitstring(P8(0b00101010U), "0 01 01 010");
            check_bitstring(P8(0b00101011U), "0 01 01 011");
            check_bitstring(P8(0b00101100U), "0 01 01 100");
            check_bitstring(P8(0b00101101U), "0 01 01 101");
            check_bitstring(P8(0b00101110U), "0 01 01 110");
            check_bitstring(P8(0b00101111U), "0 01 01 111");
            check_bitstring(P8(0b00110000U), "0 01 10 000");
            check_bitstring(P8(0b00110001U), "0 01 10 001");
            check_bitstring(P8(0b00110010U), "0 01 10 010");
            check_bitstring(P8(0b00110011U), "0 01 10 011");
            check_bitstring(P8(0b00110100U), "0 01 10 100");
            check_bitstring(P8(0b00110101U), "0 01 10 101");
            check_bitstring(P8(0b00110110U), "0 01 10 110");
            check_bitstring(P8(0b00110111U), "0 01 10 111");
            check_bitstring(P8(0b00111000U), "0 01 11 000");
            check_bitstring(P8(0b00111001U), "0 01 11 001");
            check_bitstring(P8(0b00111010U), "0 01 11 010");
            check_bitstring(P8(0b00111011U), "0 01 11 011");
            check_bitstring(P8(0b00111100U), "0 01 11 100");
            check_bitstring(P8(0b00111101U), "0 01 11 101");
            check_bitstring(P8(0b00111110U), "0 01 11 110");
            check_bitstring(P8(0b00111111U), "0 01 11 111");
            check_bitstring(P8(0b01000000U), "0 10 00 000");
            check_bitstring(P8(0b01000001U), "0 10 00 001");
            check_bitstring(P8(0b01000010U), "0 10 00 010");
            check_bitstring(P8(0b01000011U), "0 10 00 011");
            check_bitstring(P8(0b01000100U), "0 10 00 100");
            check_bitstring(P8(0b01000101U), "0 10 00 101");
            check_bitstring(P8(0b01000110U), "0 10 00 110");
            check_bitstring(P8(0b01000111U), "0 10 00 111");
            check_bitstring(P8(0b01001000U), "0 10 01 000");
            check_bitstring(P8(0b01001001U), "0 10 01 001");
            check_bitstring(P8(0b01001010U), "0 10 01 010");
            check_bitstring(P8(0b01001011U), "0 10 01 011");
            check_bitstring(P8(0b01001100U), "0 10 01 100");
            check_bitstring(P8(0b01001101U), "0 10 01 101");
            check_bitstring(P8(0b01001110U), "0 10 01 110");
            check_bitstring(P8(0b01001111U), "0 10 01 111");
            check_bitstring(P8(0b01010000U), "0 10 10 000");
            check_bitstring(P8(0b01010001U), "0 10 10 001");
            check_bitstring(P8(0b01010010U), "0 10 10 010");
            check_bitstring(P8(0b01010011U), "0 10 10 011");
            check_bitstring(P8(0b01010100U), "0 10 10 100");
            check_bitstring(P8(0b01010101U), "0 10 10 101");
            check_bitstring(P8(0b01010110U), "0 10 10 110");
            check_bitstring(P8(0b01010111U), "0 10 10 111");
            check_bitstring(P8(0b01011000U), "0 10 11 000");
            check_bitstring(P8(0b01011001U), "0 10 11 001");
            check_bitstring(P8(0b01011010U), "0 10 11 010");
            check_bitstring(P8(0b01011011U), "0 10 11 011");
            check_bitstring(P8(0b01011100U), "0 10 11 100");
            check_bitstring(P8(0b01011101U), "0 10 11 101");
            check_bitstring(P8(0b01011110U), "0 10 11 110");
            check_bitstring(P8(0b01011111U), "0 10 11 111");
            check_bitstring(P8(0b01100000U), "0 110 00 00");
            check_bitstring(P8(0b01100001U), "0 110 00 01");
            check_bitstring(P8(0b01100010U), "0 110 00 10");
            check_bitstring(P8(0b01100011U), "0 110 00 11");
            check_bitstring(P8(0b01100100U), "0 110 01 00");
            check_bitstring(P8(0b01100101U), "0 110 01 01");
            check_bitstring(P8(0b01100110U), "0 110 01 10");
            check_bitstring(P8(0b01100111U), "0 110 01 11");
            check_bitstring(P8(0b01101000U), "0 110 10 00");
            check_bitstring(P8(0b01101001U), "0 110 10 01");
            check_bitstring(P8(0b01101010U), "0 110 10 10");
            check_bitstring(P8(0b01101011U), "0 110 10 11");
            check_bitstring(P8(0b01101100U), "0 110 11 00");
            check_bitstring(P8(0b01101101U), "0 110 11 01");
            check_bitstring(P8(0b01101110U), "0 110 11 10");
            check_bitstring(P8(0b01101111U), "0 110 11 11");
            check_bitstring(P8(0b01110000U), "0 1110 00 0");
            check_bitstring(P8(0b01110001U), "0 1110 00 1");
            check_bitstring(P8(0b01110010U), "0 1110 01 0");
            check_bitstring(P8(0b01110011U), "0 1110 01 1");
            check_bitstring(P8(0b01110100U), "0 1110 10 0");
            check_bitstring(P8(0b01110101U), "0 1110 10 1");
            check_bitstring(P8(0b01110110U), "0 1110 11 0");
            check_bitstring(P8(0b01110111U), "0 1110 11 1");
            check_bitstring(P8(0b01111000U), "0 11110 00");
            check_bitstring(P8(0b01111001U), "0 11110 01");
            check_bitstring(P8(0b01111010U), "0 11110 10");
            check_bitstring(P8(0b01111011U), "0 11110 11");
            check_bitstring(P8(0b01111100U), "0 111110 0");
            check_bitstring(P8(0b01111101U), "0 111110 1");
            check_bitstring(P8(0b01111110U), "0 1111110");
            check_bitstring(P8(0b01111111U), "0 1111111");
            check_bitstring(P8(0b10000000U), "1 0000000");
            check_bitstring(P8(0b10000001U), "1 0000001");
            check_bitstring(P8(0b10000010U), "1 000001 0");
            check_bitstring(P8(0b10000011U), "1 000001 1");
            check_bitstring(P8(0b10000100U), "1 00001 00");
            check_bitstring(P8(0b10000101U), "1 00001 01");
            check_bitstring(P8(0b10000110U), "1 00001 10");
            check_bitstring(P8(0b10000111U), "1 00001 11");
            check_bitstring(P8(0b10001000U), "1 0001 00 0");
            check_bitstring(P8(0b10001001U), "1 0001 00 1");
            check_bitstring(P8(0b10001010U), "1 0001 01 0");
            check_bitstring(P8(0b10001011U), "1 0001 01 1");
            check_bitstring(P8(0b10001100U), "1 0001 10 0");
            check_bitstring(P8(0b10001101U), "1 0001 10 1");
            check_bitstring(P8(0b10001110U), "1 0001 11 0");
            check_bitstring(P8(0b10001111U), "1 0001 11 1");
            check_bitstring(P8(0b10010000U), "1 001 00 00");
            check_bitstring(P8(0b10010001U), "1 001 00 01");
            check_bitstring(P8(0b10010010U), "1 001 00 10");
            check_bitstring(P8(0b10010011U), "1 001 00 11");
            check_bitstring(P8(0b10010100U), "1 001 01 00");
            check_bitstring(P8(0b10010101U), "1 001 01 01");
            check_bitstring(P8(0b10010110U), "1 001 01 10");
            check_bitstring(P8(0b10010111U), "1 001 01 11");
            check_bitstring(P8(0b10011000U), "1 001 10 00");
            check_bitstring(P8(0b10011001U), "1 001 10 01");
            check_bitstring(P8(0b10011010U), "1 001 10 10");
            check_bitstring(P8(0b10011011U), "1 001 10 11");
            check_bitstring(P8(0b10011100U), "1 001 11 00");
            check_bitstring(P8(0b10011101U), "1 001 11 01");
            check_bitstring(P8(0b10011110U), "1 001 11 10");
            check_bitstring(P8(0b10011111U), "1 001 11 11");
            check_bitstring(P8(0b10100000U), "1 01 00 000");
            check_bitstring(P8(0b10100001U), "1 01 00 001");
            check_bitstring(P8(0b10100010U), "1 01 00 010");
            check_bitstring(P8(0b10100011U), "1 01 00 011");
            check_bitstring(P8(0b10100100U), "1 01 00 100");
            check_bitstring(P8(0b10100101U), "1 01 00 101");
            check_bitstring(P8(0b10100110U), "1 01 00 110");
            check_bitstring(P8(0b10100111U), "1 01 00 111");
            check_bitstring(P8(0b10101000U), "1 01 01 000");
            check_bitstring(P8(0b10101001U), "1 01 01 001");
            check_bitstring(P8(0b10101010U), "1 01 01 010");
            check_bitstring(P8(0b10101011U), "1 01 01 011");
            check_bitstring(P8(0b10101100U), "1 01 01 100");
            check_bitstring(P8(0b10101101U), "1 01 01 101");
            check_bitstring(P8(0b10101110U), "1 01 01 110");
            check_bitstring(P8(0b10101111U), "1 01 01 111");
            check_bitstring(P8(0b10110000U), "1 01 10 000");
            check_bitstring(P8(0b10110001U), "1 01 10 001");
            check_bitstring(P8(0b10110010U), "1 01 10 010");
            check_bitstring(P8(0b10110011U), "1 01 10 011");
            check_bitstring(P8(0b10110100U), "1 01 10 100");
            check_bitstring(P8(0b10110101U), "1 01 10 101");
            check_bitstring(P8(0b10110110U), "1 01 10 110");
            check_bitstring(P8(0b10110111U), "1 01 10 111");
            check_bitstring(P8(0b10111000U), "1 01 11 000");
            check_bitstring(P8(0b10111001U), "1 01 11 001");
            check_bitstring(P8(0b10111010U), "1 01 11 010");
            check_bitstring(P8(0b10111011U), "1 01 11 011");
            check_bitstring(P8(0b10111100U), "1 01 11 100");
            check_bitstring(P8(0b10111101U), "1 01 11 101");
            check_bitstring(P8(0b10111110U), "1 01 11 110");
            check_bitstring(P8(0b10111111U), "1 01 11 111");
            check_bitstring(P8(0b11000000U), "1 10 00 000");
            check_bitstring(P8(0b11000001U), "1 10 00 001");
            check_bitstring(P8(0b11000010U), "1 10 00 010");
            check_bitstring(P8(0b11000011U), "1 10 00 011");
            check_bitstring(P8(0b11000100U), "1 10 00 100");
            check_bitstring(P8(0b11000101U), "1 10 00 101");
            check_bitstring(P8(0b11000110U), "1 10 00 110");
            check_bitstring(P8(0b11000111U), "1 10 00 111");
            check_bitstring(P8(0b11001000U), "1 10 01 000");
            check_bitstring(P8(0b11001001U), "1 10 01 001");
            check_bitstring(P8(0b11001010U), "1 10 01 010");
            check_bitstring(P8(0b11001011U), "1 10 01 011");
            check_bitstring(P8(0b11001100U), "1 10 01 100");
            check_bitstring(P8(0b11001101U), "1 10 01 101");
            check_bitstring(P8(0b11001110U), "1 10 01 110");
            check_bitstring(P8(0b11001111U), "1 10 01 111");
            check_bitstring(P8(0b11010000U), "1 10 10 000");
            check_bitstring(P8(0b11010001U), "1 10 10 001");
            check_bitstring(P8(0b11010010U), "1 10 10 010");
            check_bitstring(P8(0b11010011U), "1 10 10 011");
            check_bitstring(P8(0b11010100U), "1 10 10 100");
            check_bitstring(P8(0b11010101U), "1 10 10 101");
            check_bitstring(P8(0b11010110U), "1 10 10 110");
            check_bitstring(P8(0b11010111U), "1 10 10 111");
            check_bitstring(P8(0b11011000U), "1 10 11 000");
            check_bitstring(P8(0b11011001U), "1 10 11 001");
            check_bitstring(P8(0b11011010U), "1 10 11 010");
            check_bitstring(P8(0b11011011U), "1 10 11 011");
            check_bitstring(P8(0b11011100U), "1 10 11 100");
            check_bitstring(P8(0b11011101U), "1 10 11 101");
            check_bitstring(P8(0b11011110U), "1 10 11 110");
            check_bitstring(P8(0b11011111U), "1 10 11 111");
            check_bitstring(P8(0b11100000U), "1 110 00 00");
            check_bitstring(P8(0b11100001U), "1 110 00 01");
            check_bitstring(P8(0b11100010U), "1 110 00 10");
            check_bitstring(P8(0b11100011U), "1 110 00 11");
            check_bitstring(P8(0b11100100U), "1 110 01 00");
            check_bitstring(P8(0b11100101U), "1 110 01 01");
            check_bitstring(P8(0b11100110U), "1 110 01 10");
            check_bitstring(P8(0b11100111U), "1 110 01 11");
            check_bitstring(P8(0b11101000U), "1 110 10 00");
            check_bitstring(P8(0b11101001U), "1 110 10 01");
            check_bitstring(P8(0b11101010U), "1 110 10 10");
            check_bitstring(P8(0b11101011U), "1 110 10 11");
            check_bitstring(P8(0b11101100U), "1 110 11 00");
            check_bitstring(P8(0b11101101U), "1 110 11 01");
            check_bitstring(P8(0b11101110U), "1 110 11 10");
            check_bitstring(P8(0b11101111U), "1 110 11 11");
            check_bitstring(P8(0b11110000U), "1 1110 00 0");
            check_bitstring(P8(0b11110001U), "1 1110 00 1");
            check_bitstring(P8(0b11110010U), "1 1110 01 0");
            check_bitstring(P8(0b11110011U), "1 1110 01 1");
            check_bitstring(P8(0b11110100U), "1 1110 10 0");
            check_bitstring(P8(0b11110101U), "1 1110 10 1");
            check_bitstring(P8(0b11110110U), "1 1110 11 0");
            check_bitstring(P8(0b11110111U), "1 1110 11 1");
            check_bitstring(P8(0b11111000U), "1 11110 00");
            check_bitstring(P8(0b11111001U), "1 11110 01");
            check_bitstring(P8(0b11111010U), "1 11110 10");
            check_bitstring(P8(0b11111011U), "1 11110 11");
            check_bitstring(P8(0b11111100U), "1 111110 0");
            check_bitstring(P8(0b11111101U), "1 111110 1");
            check_bitstring(P8(0b11111110U), "1 1111110");
            check_bitstring(P8(0b11111111U), "1 1111111");
        }
    }
}
