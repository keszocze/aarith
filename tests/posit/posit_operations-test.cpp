#include <catch.hpp>
#include <iostream>

#include "../test-signature-ranges.hpp"
#include <aarith/integer.hpp>
#include <aarith/posit.hpp>
#include <cmath>
#include <iostream>

using namespace aarith;

template <size_t N, size_t ES, class WT>
static void check_bitstring(const aarith::posit<N, ES, WT>& p, const char* expected)
{
    const std::string actual = to_binary(p, " ");
    REQUIRE(std::string(expected) == actual);
}

SCENARIO("Print the bitstrings with seperator")
{
    GIVEN("All posit<8, 2> values")
    {
        THEN("Assert that the bitstring prints correctly")
        {
            using namespace aarith;

            using P8 = aarith::posit<8, 2>;

            // Test based on the very helpful "edu_tables" program provided by
            // https://github.com/stillwater-sc/universal

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
            check_bitstring(P8(0b10000010U), "1 0000010");
            check_bitstring(P8(0b10000011U), "1 000001 1");
            check_bitstring(P8(0b10000100U), "1 000010 0");
            check_bitstring(P8(0b10000101U), "1 00001 01");
            check_bitstring(P8(0b10000110U), "1 00001 10");
            check_bitstring(P8(0b10000111U), "1 00001 11");
            check_bitstring(P8(0b10001000U), "1 00010 00");
            check_bitstring(P8(0b10001001U), "1 0001 00 1");
            check_bitstring(P8(0b10001010U), "1 0001 01 0");
            check_bitstring(P8(0b10001011U), "1 0001 01 1");
            check_bitstring(P8(0b10001100U), "1 0001 10 0");
            check_bitstring(P8(0b10001101U), "1 0001 10 1");
            check_bitstring(P8(0b10001110U), "1 0001 11 0");
            check_bitstring(P8(0b10001111U), "1 0001 11 1");
            check_bitstring(P8(0b10010000U), "1 0010 00 0");
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
            check_bitstring(P8(0b10100000U), "1 010 00 00");
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
            check_bitstring(P8(0b11100000U), "1 11 00 000");
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
            check_bitstring(P8(0b11110000U), "1 111 00 00");
            check_bitstring(P8(0b11110001U), "1 1110 00 1");
            check_bitstring(P8(0b11110010U), "1 1110 01 0");
            check_bitstring(P8(0b11110011U), "1 1110 01 1");
            check_bitstring(P8(0b11110100U), "1 1110 10 0");
            check_bitstring(P8(0b11110101U), "1 1110 10 1");
            check_bitstring(P8(0b11110110U), "1 1110 11 0");
            check_bitstring(P8(0b11110111U), "1 1110 11 1");
            check_bitstring(P8(0b11111000U), "1 1111 00 0");
            check_bitstring(P8(0b11111001U), "1 11110 01");
            check_bitstring(P8(0b11111010U), "1 11110 10");
            check_bitstring(P8(0b11111011U), "1 11110 11");
            check_bitstring(P8(0b11111100U), "1 11111 00");
            check_bitstring(P8(0b11111101U), "1 111110 1");
            check_bitstring(P8(0b11111110U), "1 111111 0");
            check_bitstring(P8(0b11111111U), "1 1111111");
        }
    }
}

template <size_t N, size_t ES, class WT>
static void require_sizes(const posit<N, ES, WT>& p, size_t expected_nregime = 0,
                          size_t expected_nexp = 0, size_t expected_nfrac = 0)
{
    const size_t nregime = get_num_regime_bits(p);
    const size_t nexp = get_num_exponent_bits(p);
    const size_t nfrac = get_num_fraction_bits(p);

    REQUIRE(nregime == expected_nregime);
    REQUIRE(nexp == expected_nexp);
    REQUIRE(nfrac == expected_nfrac);
}

SCENARIO("Check width of dynamic fields")
{
    GIVEN("All posit<6, 2> values")
    {
        THEN("Assert that the different components have the correct length")
        {
            using namespace aarith;

            using P6 = aarith::posit<6, 2>;

            // Test based on the very helpful "edu_tables" program provided by
            // https://github.com/stillwater-sc/universal

            require_sizes(P6(0b000000U), 5);
            require_sizes(P6(0b000001U), 5);
            require_sizes(P6(0b000010U), 4, 1);
            require_sizes(P6(0b000011U), 4, 1);
            require_sizes(P6(0b000100U), 3, 2);
            require_sizes(P6(0b000101U), 3, 2);
            require_sizes(P6(0b000110U), 3, 2);
            require_sizes(P6(0b000111U), 3, 2);
            require_sizes(P6(0b001000U), 2, 2, 1);
            require_sizes(P6(0b001001U), 2, 2, 1);
            require_sizes(P6(0b001010U), 2, 2, 1);
            require_sizes(P6(0b001011U), 2, 2, 1);
            require_sizes(P6(0b001100U), 2, 2, 1);
            require_sizes(P6(0b001101U), 2, 2, 1);
            require_sizes(P6(0b001110U), 2, 2, 1);
            require_sizes(P6(0b001111U), 2, 2, 1);
            require_sizes(P6(0b010000U), 2, 2, 1);
            require_sizes(P6(0b010001U), 2, 2, 1);
            require_sizes(P6(0b010010U), 2, 2, 1);
            require_sizes(P6(0b010011U), 2, 2, 1);
            require_sizes(P6(0b010100U), 2, 2, 1);
            require_sizes(P6(0b010101U), 2, 2, 1);
            require_sizes(P6(0b010110U), 2, 2, 1);
            require_sizes(P6(0b010111U), 2, 2, 1);
            require_sizes(P6(0b011000U), 3, 2);
            require_sizes(P6(0b011001U), 3, 2);
            require_sizes(P6(0b011010U), 3, 2);
            require_sizes(P6(0b011011U), 3, 2);
            require_sizes(P6(0b011100U), 4, 1);
            require_sizes(P6(0b011101U), 4, 1);
            require_sizes(P6(0b011110U), 5);
            require_sizes(P6(0b011111U), 5);
            require_sizes(P6(0b100000U), 5);
            require_sizes(P6(0b100001U), 5);
            require_sizes(P6(0b100010U), 5);
            require_sizes(P6(0b100011U), 4, 1);
            require_sizes(P6(0b100100U), 4, 1);
            require_sizes(P6(0b100101U), 3, 2);
            require_sizes(P6(0b100110U), 3, 2);
            require_sizes(P6(0b100111U), 3, 2);
            require_sizes(P6(0b101000U), 3, 2);
            require_sizes(P6(0b101001U), 2, 2, 1);
            require_sizes(P6(0b101010U), 2, 2, 1);
            require_sizes(P6(0b101011U), 2, 2, 1);
            require_sizes(P6(0b101100U), 2, 2, 1);
            require_sizes(P6(0b101101U), 2, 2, 1);
            require_sizes(P6(0b101110U), 2, 2, 1);
            require_sizes(P6(0b101111U), 2, 2, 1);
            require_sizes(P6(0b110000U), 2, 2, 1);
            require_sizes(P6(0b110001U), 2, 2, 1);
            require_sizes(P6(0b110010U), 2, 2, 1);
            require_sizes(P6(0b110011U), 2, 2, 1);
            require_sizes(P6(0b110100U), 2, 2, 1);
            require_sizes(P6(0b110101U), 2, 2, 1);
            require_sizes(P6(0b110110U), 2, 2, 1);
            require_sizes(P6(0b110111U), 2, 2, 1);
            require_sizes(P6(0b111000U), 2, 2, 1);
            require_sizes(P6(0b111001U), 3, 2);
            require_sizes(P6(0b111010U), 3, 2);
            require_sizes(P6(0b111011U), 3, 2);
            require_sizes(P6(0b111100U), 3, 2);
            require_sizes(P6(0b111101U), 4, 1);
            require_sizes(P6(0b111110U), 4, 1);
            require_sizes(P6(0b111111U), 5);
        }
    }

    GIVEN("All posit<8, 0> values")
    {
        THEN("Assert that the different components have the correct length")
        {
            using namespace aarith;

            using P80 = aarith::posit<8, 0>;

            // Test based on the very helpful "edu_tables" program provided by
            // https://github.com/stillwater-sc/universal

            require_sizes(P80(0b00000000U), 7, 0);
            require_sizes(P80(0b00000001U), 7, 0);
            require_sizes(P80(0b00000010U), 6, 0, 1);
            require_sizes(P80(0b00000011U), 6, 0, 1);
            require_sizes(P80(0b00000100U), 5, 0, 2);
            require_sizes(P80(0b00000101U), 5, 0, 2);
            require_sizes(P80(0b00000110U), 5, 0, 2);
            require_sizes(P80(0b00000111U), 5, 0, 2);
            require_sizes(P80(0b00001000U), 4, 0, 3);
            require_sizes(P80(0b00001001U), 4, 0, 3);
            require_sizes(P80(0b00001010U), 4, 0, 3);
            require_sizes(P80(0b00001011U), 4, 0, 3);
            require_sizes(P80(0b00001100U), 4, 0, 3);
            require_sizes(P80(0b00001101U), 4, 0, 3);
            require_sizes(P80(0b00001110U), 4, 0, 3);
            require_sizes(P80(0b00001111U), 4, 0, 3);
            require_sizes(P80(0b00010000U), 3, 0, 4);
            require_sizes(P80(0b00010001U), 3, 0, 4);
            require_sizes(P80(0b00010010U), 3, 0, 4);
            require_sizes(P80(0b00010011U), 3, 0, 4);
            require_sizes(P80(0b00010100U), 3, 0, 4);
            require_sizes(P80(0b00010101U), 3, 0, 4);
            require_sizes(P80(0b00010110U), 3, 0, 4);
            require_sizes(P80(0b00010111U), 3, 0, 4);
            require_sizes(P80(0b00011000U), 3, 0, 4);
            require_sizes(P80(0b00011001U), 3, 0, 4);
            require_sizes(P80(0b00011010U), 3, 0, 4);
            require_sizes(P80(0b00011011U), 3, 0, 4);
            require_sizes(P80(0b00011100U), 3, 0, 4);
            require_sizes(P80(0b00011101U), 3, 0, 4);
            require_sizes(P80(0b00011110U), 3, 0, 4);
            require_sizes(P80(0b00011111U), 3, 0, 4);
            require_sizes(P80(0b00100000U), 2, 0, 5);
            require_sizes(P80(0b00100001U), 2, 0, 5);
            require_sizes(P80(0b00100010U), 2, 0, 5);
            require_sizes(P80(0b00100011U), 2, 0, 5);
            require_sizes(P80(0b00100100U), 2, 0, 5);
            require_sizes(P80(0b00100101U), 2, 0, 5);
            require_sizes(P80(0b00100110U), 2, 0, 5);
            require_sizes(P80(0b00100111U), 2, 0, 5);
            require_sizes(P80(0b00101000U), 2, 0, 5);
            require_sizes(P80(0b00101001U), 2, 0, 5);
            require_sizes(P80(0b00101010U), 2, 0, 5);
            require_sizes(P80(0b00101011U), 2, 0, 5);
            require_sizes(P80(0b00101100U), 2, 0, 5);
            require_sizes(P80(0b00101101U), 2, 0, 5);
            require_sizes(P80(0b00101110U), 2, 0, 5);
            require_sizes(P80(0b00101111U), 2, 0, 5);
            require_sizes(P80(0b00110000U), 2, 0, 5);
            require_sizes(P80(0b00110001U), 2, 0, 5);
            require_sizes(P80(0b00110010U), 2, 0, 5);
            require_sizes(P80(0b00110011U), 2, 0, 5);
            require_sizes(P80(0b00110100U), 2, 0, 5);
            require_sizes(P80(0b00110101U), 2, 0, 5);
            require_sizes(P80(0b00110110U), 2, 0, 5);
            require_sizes(P80(0b00110111U), 2, 0, 5);
            require_sizes(P80(0b00111000U), 2, 0, 5);
            require_sizes(P80(0b00111001U), 2, 0, 5);
            require_sizes(P80(0b00111010U), 2, 0, 5);
            require_sizes(P80(0b00111011U), 2, 0, 5);
            require_sizes(P80(0b00111100U), 2, 0, 5);
            require_sizes(P80(0b00111101U), 2, 0, 5);
            require_sizes(P80(0b00111110U), 2, 0, 5);
            require_sizes(P80(0b00111111U), 2, 0, 5);
            require_sizes(P80(0b01000000U), 2, 0, 5);
            require_sizes(P80(0b01000001U), 2, 0, 5);
            require_sizes(P80(0b01000010U), 2, 0, 5);
            require_sizes(P80(0b01000011U), 2, 0, 5);
            require_sizes(P80(0b01000100U), 2, 0, 5);
            require_sizes(P80(0b01000101U), 2, 0, 5);
            require_sizes(P80(0b01000110U), 2, 0, 5);
            require_sizes(P80(0b01000111U), 2, 0, 5);
            require_sizes(P80(0b01001000U), 2, 0, 5);
            require_sizes(P80(0b01001001U), 2, 0, 5);
            require_sizes(P80(0b01001010U), 2, 0, 5);
            require_sizes(P80(0b01001011U), 2, 0, 5);
            require_sizes(P80(0b01001100U), 2, 0, 5);
            require_sizes(P80(0b01001101U), 2, 0, 5);
            require_sizes(P80(0b01001110U), 2, 0, 5);
            require_sizes(P80(0b01001111U), 2, 0, 5);
            require_sizes(P80(0b01010000U), 2, 0, 5);
            require_sizes(P80(0b01010001U), 2, 0, 5);
            require_sizes(P80(0b01010010U), 2, 0, 5);
            require_sizes(P80(0b01010011U), 2, 0, 5);
            require_sizes(P80(0b01010100U), 2, 0, 5);
            require_sizes(P80(0b01010101U), 2, 0, 5);
            require_sizes(P80(0b01010110U), 2, 0, 5);
            require_sizes(P80(0b01010111U), 2, 0, 5);
            require_sizes(P80(0b01011000U), 2, 0, 5);
            require_sizes(P80(0b01011001U), 2, 0, 5);
            require_sizes(P80(0b01011010U), 2, 0, 5);
            require_sizes(P80(0b01011011U), 2, 0, 5);
            require_sizes(P80(0b01011100U), 2, 0, 5);
            require_sizes(P80(0b01011101U), 2, 0, 5);
            require_sizes(P80(0b01011110U), 2, 0, 5);
            require_sizes(P80(0b01011111U), 2, 0, 5);
            require_sizes(P80(0b01100000U), 3, 0, 4);
            require_sizes(P80(0b01100001U), 3, 0, 4);
            require_sizes(P80(0b01100010U), 3, 0, 4);
            require_sizes(P80(0b01100011U), 3, 0, 4);
            require_sizes(P80(0b01100100U), 3, 0, 4);
            require_sizes(P80(0b01100101U), 3, 0, 4);
            require_sizes(P80(0b01100110U), 3, 0, 4);
            require_sizes(P80(0b01100111U), 3, 0, 4);
            require_sizes(P80(0b01101000U), 3, 0, 4);
            require_sizes(P80(0b01101001U), 3, 0, 4);
            require_sizes(P80(0b01101010U), 3, 0, 4);
            require_sizes(P80(0b01101011U), 3, 0, 4);
            require_sizes(P80(0b01101100U), 3, 0, 4);
            require_sizes(P80(0b01101101U), 3, 0, 4);
            require_sizes(P80(0b01101110U), 3, 0, 4);
            require_sizes(P80(0b01101111U), 3, 0, 4);
            require_sizes(P80(0b01110000U), 4, 0, 3);
            require_sizes(P80(0b01110001U), 4, 0, 3);
            require_sizes(P80(0b01110010U), 4, 0, 3);
            require_sizes(P80(0b01110011U), 4, 0, 3);
            require_sizes(P80(0b01110100U), 4, 0, 3);
            require_sizes(P80(0b01110101U), 4, 0, 3);
            require_sizes(P80(0b01110110U), 4, 0, 3);
            require_sizes(P80(0b01110111U), 4, 0, 3);
            require_sizes(P80(0b01111000U), 5, 0, 2);
            require_sizes(P80(0b01111001U), 5, 0, 2);
            require_sizes(P80(0b01111010U), 5, 0, 2);
            require_sizes(P80(0b01111011U), 5, 0, 2);
            require_sizes(P80(0b01111100U), 6, 0, 1);
            require_sizes(P80(0b01111101U), 6, 0, 1);
            require_sizes(P80(0b01111110U), 7, 0);
            require_sizes(P80(0b01111111U), 7, 0);
            require_sizes(P80(0b10000000U), 7, 0);
            require_sizes(P80(0b10000001U), 7, 0);
            require_sizes(P80(0b10000010U), 7, 0);
            require_sizes(P80(0b10000011U), 6, 0, 1);
            require_sizes(P80(0b10000100U), 6, 0, 1);
            require_sizes(P80(0b10000101U), 5, 0, 2);
            require_sizes(P80(0b10000110U), 5, 0, 2);
            require_sizes(P80(0b10000111U), 5, 0, 2);
            require_sizes(P80(0b10001000U), 5, 0, 2);
            require_sizes(P80(0b10001001U), 4, 0, 3);
            require_sizes(P80(0b10001010U), 4, 0, 3);
            require_sizes(P80(0b10001011U), 4, 0, 3);
            require_sizes(P80(0b10001100U), 4, 0, 3);
            require_sizes(P80(0b10001101U), 4, 0, 3);
            require_sizes(P80(0b10001110U), 4, 0, 3);
            require_sizes(P80(0b10001111U), 4, 0, 3);
            require_sizes(P80(0b10010000U), 4, 0, 3);
            require_sizes(P80(0b10010001U), 3, 0, 4);
            require_sizes(P80(0b10010010U), 3, 0, 4);
            require_sizes(P80(0b10010011U), 3, 0, 4);
            require_sizes(P80(0b10010100U), 3, 0, 4);
            require_sizes(P80(0b10010101U), 3, 0, 4);
            require_sizes(P80(0b10010110U), 3, 0, 4);
            require_sizes(P80(0b10010111U), 3, 0, 4);
            require_sizes(P80(0b10011000U), 3, 0, 4);
            require_sizes(P80(0b10011001U), 3, 0, 4);
            require_sizes(P80(0b10011010U), 3, 0, 4);
            require_sizes(P80(0b10011011U), 3, 0, 4);
            require_sizes(P80(0b10011100U), 3, 0, 4);
            require_sizes(P80(0b10011101U), 3, 0, 4);
            require_sizes(P80(0b10011110U), 3, 0, 4);
            require_sizes(P80(0b10011111U), 3, 0, 4);
            require_sizes(P80(0b10100000U), 3, 0, 4);
            require_sizes(P80(0b10100001U), 2, 0, 5);
            require_sizes(P80(0b10100010U), 2, 0, 5);
            require_sizes(P80(0b10100011U), 2, 0, 5);
            require_sizes(P80(0b10100100U), 2, 0, 5);
            require_sizes(P80(0b10100101U), 2, 0, 5);
            require_sizes(P80(0b10100110U), 2, 0, 5);
            require_sizes(P80(0b10100111U), 2, 0, 5);
            require_sizes(P80(0b10101000U), 2, 0, 5);
            require_sizes(P80(0b10101001U), 2, 0, 5);
            require_sizes(P80(0b10101010U), 2, 0, 5);
            require_sizes(P80(0b10101011U), 2, 0, 5);
            require_sizes(P80(0b10101100U), 2, 0, 5);
            require_sizes(P80(0b10101101U), 2, 0, 5);
            require_sizes(P80(0b10101110U), 2, 0, 5);
            require_sizes(P80(0b10101111U), 2, 0, 5);
            require_sizes(P80(0b10110000U), 2, 0, 5);
            require_sizes(P80(0b10110001U), 2, 0, 5);
            require_sizes(P80(0b10110010U), 2, 0, 5);
            require_sizes(P80(0b10110011U), 2, 0, 5);
            require_sizes(P80(0b10110100U), 2, 0, 5);
            require_sizes(P80(0b10110101U), 2, 0, 5);
            require_sizes(P80(0b10110110U), 2, 0, 5);
            require_sizes(P80(0b10110111U), 2, 0, 5);
            require_sizes(P80(0b10111000U), 2, 0, 5);
            require_sizes(P80(0b10111001U), 2, 0, 5);
            require_sizes(P80(0b10111010U), 2, 0, 5);
            require_sizes(P80(0b10111011U), 2, 0, 5);
            require_sizes(P80(0b10111100U), 2, 0, 5);
            require_sizes(P80(0b10111101U), 2, 0, 5);
            require_sizes(P80(0b10111110U), 2, 0, 5);
            require_sizes(P80(0b10111111U), 2, 0, 5);
            require_sizes(P80(0b11000000U), 2, 0, 5);
            require_sizes(P80(0b11000001U), 2, 0, 5);
            require_sizes(P80(0b11000010U), 2, 0, 5);
            require_sizes(P80(0b11000011U), 2, 0, 5);
            require_sizes(P80(0b11000100U), 2, 0, 5);
            require_sizes(P80(0b11000101U), 2, 0, 5);
            require_sizes(P80(0b11000110U), 2, 0, 5);
            require_sizes(P80(0b11000111U), 2, 0, 5);
            require_sizes(P80(0b11001000U), 2, 0, 5);
            require_sizes(P80(0b11001001U), 2, 0, 5);
            require_sizes(P80(0b11001010U), 2, 0, 5);
            require_sizes(P80(0b11001011U), 2, 0, 5);
            require_sizes(P80(0b11001100U), 2, 0, 5);
            require_sizes(P80(0b11001101U), 2, 0, 5);
            require_sizes(P80(0b11001110U), 2, 0, 5);
            require_sizes(P80(0b11001111U), 2, 0, 5);
            require_sizes(P80(0b11010000U), 2, 0, 5);
            require_sizes(P80(0b11010001U), 2, 0, 5);
            require_sizes(P80(0b11010010U), 2, 0, 5);
            require_sizes(P80(0b11010011U), 2, 0, 5);
            require_sizes(P80(0b11010100U), 2, 0, 5);
            require_sizes(P80(0b11010101U), 2, 0, 5);
            require_sizes(P80(0b11010110U), 2, 0, 5);
            require_sizes(P80(0b11010111U), 2, 0, 5);
            require_sizes(P80(0b11011000U), 2, 0, 5);
            require_sizes(P80(0b11011001U), 2, 0, 5);
            require_sizes(P80(0b11011010U), 2, 0, 5);
            require_sizes(P80(0b11011011U), 2, 0, 5);
            require_sizes(P80(0b11011100U), 2, 0, 5);
            require_sizes(P80(0b11011101U), 2, 0, 5);
            require_sizes(P80(0b11011110U), 2, 0, 5);
            require_sizes(P80(0b11011111U), 2, 0, 5);
            require_sizes(P80(0b11100000U), 2, 0, 5);
            require_sizes(P80(0b11100001U), 3, 0, 4);
            require_sizes(P80(0b11100010U), 3, 0, 4);
            require_sizes(P80(0b11100011U), 3, 0, 4);
            require_sizes(P80(0b11100100U), 3, 0, 4);
            require_sizes(P80(0b11100101U), 3, 0, 4);
            require_sizes(P80(0b11100110U), 3, 0, 4);
            require_sizes(P80(0b11100111U), 3, 0, 4);
            require_sizes(P80(0b11101000U), 3, 0, 4);
            require_sizes(P80(0b11101001U), 3, 0, 4);
            require_sizes(P80(0b11101010U), 3, 0, 4);
            require_sizes(P80(0b11101011U), 3, 0, 4);
            require_sizes(P80(0b11101100U), 3, 0, 4);
            require_sizes(P80(0b11101101U), 3, 0, 4);
            require_sizes(P80(0b11101110U), 3, 0, 4);
            require_sizes(P80(0b11101111U), 3, 0, 4);
            require_sizes(P80(0b11110000U), 3, 0, 4);
            require_sizes(P80(0b11110001U), 4, 0, 3);
            require_sizes(P80(0b11110010U), 4, 0, 3);
            require_sizes(P80(0b11110011U), 4, 0, 3);
            require_sizes(P80(0b11110100U), 4, 0, 3);
            require_sizes(P80(0b11110101U), 4, 0, 3);
            require_sizes(P80(0b11110110U), 4, 0, 3);
            require_sizes(P80(0b11110111U), 4, 0, 3);
            require_sizes(P80(0b11111000U), 4, 0, 3);
            require_sizes(P80(0b11111001U), 5, 0, 2);
            require_sizes(P80(0b11111010U), 5, 0, 2);
            require_sizes(P80(0b11111011U), 5, 0, 2);
            require_sizes(P80(0b11111100U), 5, 0, 2);
            require_sizes(P80(0b11111101U), 6, 0, 1);
            require_sizes(P80(0b11111110U), 6, 0, 1);
            require_sizes(P80(0b11111111U), 7, 0);
        }
    }

    GIVEN("All posit<8, 2> values")
    {
        THEN("Assert that the different components have the correct length")
        {
            using namespace aarith;

            using P8 = aarith::posit<8, 2>;

            // Test based on the very helpful "edu_tables" program provided by
            // https://github.com/stillwater-sc/universal

            require_sizes(P8(0b00000000U), 7);
            require_sizes(P8(0b00000001U), 7);
            require_sizes(P8(0b00000010U), 6, 1);
            require_sizes(P8(0b00000011U), 6, 1);
            require_sizes(P8(0b00000100U), 5, 2);
            require_sizes(P8(0b00000101U), 5, 2);
            require_sizes(P8(0b00000110U), 5, 2);
            require_sizes(P8(0b00000111U), 5, 2);
            require_sizes(P8(0b00001000U), 4, 2, 1);
            require_sizes(P8(0b00001001U), 4, 2, 1);
            require_sizes(P8(0b00001010U), 4, 2, 1);
            require_sizes(P8(0b00001011U), 4, 2, 1);
            require_sizes(P8(0b00001100U), 4, 2, 1);
            require_sizes(P8(0b00001101U), 4, 2, 1);
            require_sizes(P8(0b00001110U), 4, 2, 1);
            require_sizes(P8(0b00001111U), 4, 2, 1);
            require_sizes(P8(0b00010000U), 3, 2, 2);
            require_sizes(P8(0b00010001U), 3, 2, 2);
            require_sizes(P8(0b00010010U), 3, 2, 2);
            require_sizes(P8(0b00010011U), 3, 2, 2);
            require_sizes(P8(0b00010100U), 3, 2, 2);
            require_sizes(P8(0b00010101U), 3, 2, 2);
            require_sizes(P8(0b00010110U), 3, 2, 2);
            require_sizes(P8(0b00010111U), 3, 2, 2);
            require_sizes(P8(0b00011000U), 3, 2, 2);
            require_sizes(P8(0b00011001U), 3, 2, 2);
            require_sizes(P8(0b00011010U), 3, 2, 2);
            require_sizes(P8(0b00011011U), 3, 2, 2);
            require_sizes(P8(0b00011100U), 3, 2, 2);
            require_sizes(P8(0b00011101U), 3, 2, 2);
            require_sizes(P8(0b00011110U), 3, 2, 2);
            require_sizes(P8(0b00011111U), 3, 2, 2);
            require_sizes(P8(0b00100000U), 2, 2, 3);
            require_sizes(P8(0b00100001U), 2, 2, 3);
            require_sizes(P8(0b00100010U), 2, 2, 3);
            require_sizes(P8(0b00100011U), 2, 2, 3);
            require_sizes(P8(0b00100100U), 2, 2, 3);
            require_sizes(P8(0b00100101U), 2, 2, 3);
            require_sizes(P8(0b00100110U), 2, 2, 3);
            require_sizes(P8(0b00100111U), 2, 2, 3);
            require_sizes(P8(0b00101000U), 2, 2, 3);
            require_sizes(P8(0b00101001U), 2, 2, 3);
            require_sizes(P8(0b00101010U), 2, 2, 3);
            require_sizes(P8(0b00101011U), 2, 2, 3);
            require_sizes(P8(0b00101100U), 2, 2, 3);
            require_sizes(P8(0b00101101U), 2, 2, 3);
            require_sizes(P8(0b00101110U), 2, 2, 3);
            require_sizes(P8(0b00101111U), 2, 2, 3);
            require_sizes(P8(0b00110000U), 2, 2, 3);
            require_sizes(P8(0b00110001U), 2, 2, 3);
            require_sizes(P8(0b00110010U), 2, 2, 3);
            require_sizes(P8(0b00110011U), 2, 2, 3);
            require_sizes(P8(0b00110100U), 2, 2, 3);
            require_sizes(P8(0b00110101U), 2, 2, 3);
            require_sizes(P8(0b00110110U), 2, 2, 3);
            require_sizes(P8(0b00110111U), 2, 2, 3);
            require_sizes(P8(0b00111000U), 2, 2, 3);
            require_sizes(P8(0b00111001U), 2, 2, 3);
            require_sizes(P8(0b00111010U), 2, 2, 3);
            require_sizes(P8(0b00111011U), 2, 2, 3);
            require_sizes(P8(0b00111100U), 2, 2, 3);
            require_sizes(P8(0b00111101U), 2, 2, 3);
            require_sizes(P8(0b00111110U), 2, 2, 3);
            require_sizes(P8(0b00111111U), 2, 2, 3);
            require_sizes(P8(0b01000000U), 2, 2, 3);
            require_sizes(P8(0b01000001U), 2, 2, 3);
            require_sizes(P8(0b01000010U), 2, 2, 3);
            require_sizes(P8(0b01000011U), 2, 2, 3);
            require_sizes(P8(0b01000100U), 2, 2, 3);
            require_sizes(P8(0b01000101U), 2, 2, 3);
            require_sizes(P8(0b01000110U), 2, 2, 3);
            require_sizes(P8(0b01000111U), 2, 2, 3);
            require_sizes(P8(0b01001000U), 2, 2, 3);
            require_sizes(P8(0b01001001U), 2, 2, 3);
            require_sizes(P8(0b01001010U), 2, 2, 3);
            require_sizes(P8(0b01001011U), 2, 2, 3);
            require_sizes(P8(0b01001100U), 2, 2, 3);
            require_sizes(P8(0b01001101U), 2, 2, 3);
            require_sizes(P8(0b01001110U), 2, 2, 3);
            require_sizes(P8(0b01001111U), 2, 2, 3);
            require_sizes(P8(0b01010000U), 2, 2, 3);
            require_sizes(P8(0b01010001U), 2, 2, 3);
            require_sizes(P8(0b01010010U), 2, 2, 3);
            require_sizes(P8(0b01010011U), 2, 2, 3);
            require_sizes(P8(0b01010100U), 2, 2, 3);
            require_sizes(P8(0b01010101U), 2, 2, 3);
            require_sizes(P8(0b01010110U), 2, 2, 3);
            require_sizes(P8(0b01010111U), 2, 2, 3);
            require_sizes(P8(0b01011000U), 2, 2, 3);
            require_sizes(P8(0b01011001U), 2, 2, 3);
            require_sizes(P8(0b01011010U), 2, 2, 3);
            require_sizes(P8(0b01011011U), 2, 2, 3);
            require_sizes(P8(0b01011100U), 2, 2, 3);
            require_sizes(P8(0b01011101U), 2, 2, 3);
            require_sizes(P8(0b01011110U), 2, 2, 3);
            require_sizes(P8(0b01011111U), 2, 2, 3);
            require_sizes(P8(0b01100000U), 3, 2, 2);
            require_sizes(P8(0b01100001U), 3, 2, 2);
            require_sizes(P8(0b01100010U), 3, 2, 2);
            require_sizes(P8(0b01100011U), 3, 2, 2);
            require_sizes(P8(0b01100100U), 3, 2, 2);
            require_sizes(P8(0b01100101U), 3, 2, 2);
            require_sizes(P8(0b01100110U), 3, 2, 2);
            require_sizes(P8(0b01100111U), 3, 2, 2);
            require_sizes(P8(0b01101000U), 3, 2, 2);
            require_sizes(P8(0b01101001U), 3, 2, 2);
            require_sizes(P8(0b01101010U), 3, 2, 2);
            require_sizes(P8(0b01101011U), 3, 2, 2);
            require_sizes(P8(0b01101100U), 3, 2, 2);
            require_sizes(P8(0b01101101U), 3, 2, 2);
            require_sizes(P8(0b01101110U), 3, 2, 2);
            require_sizes(P8(0b01101111U), 3, 2, 2);
            require_sizes(P8(0b01110000U), 4, 2, 1);
            require_sizes(P8(0b01110001U), 4, 2, 1);
            require_sizes(P8(0b01110010U), 4, 2, 1);
            require_sizes(P8(0b01110011U), 4, 2, 1);
            require_sizes(P8(0b01110100U), 4, 2, 1);
            require_sizes(P8(0b01110101U), 4, 2, 1);
            require_sizes(P8(0b01110110U), 4, 2, 1);
            require_sizes(P8(0b01110111U), 4, 2, 1);
            require_sizes(P8(0b01111000U), 5, 2);
            require_sizes(P8(0b01111001U), 5, 2);
            require_sizes(P8(0b01111010U), 5, 2);
            require_sizes(P8(0b01111011U), 5, 2);
            require_sizes(P8(0b01111100U), 6, 1);
            require_sizes(P8(0b01111101U), 6, 1);
            require_sizes(P8(0b01111110U), 7);
            require_sizes(P8(0b01111111U), 7);
            require_sizes(P8(0b10000000U), 7);
            require_sizes(P8(0b10000001U), 7);
            require_sizes(P8(0b10000010U), 7);
            require_sizes(P8(0b10000011U), 6, 1);
            require_sizes(P8(0b10000100U), 6, 1);
            require_sizes(P8(0b10000101U), 5, 2);
            require_sizes(P8(0b10000110U), 5, 2);
            require_sizes(P8(0b10000111U), 5, 2);
            require_sizes(P8(0b10001000U), 5, 2);
            require_sizes(P8(0b10001001U), 4, 2, 1);
            require_sizes(P8(0b10001010U), 4, 2, 1);
            require_sizes(P8(0b10001011U), 4, 2, 1);
            require_sizes(P8(0b10001100U), 4, 2, 1);
            require_sizes(P8(0b10001101U), 4, 2, 1);
            require_sizes(P8(0b10001110U), 4, 2, 1);
            require_sizes(P8(0b10001111U), 4, 2, 1);
            require_sizes(P8(0b10010000U), 4, 2, 1);
            require_sizes(P8(0b10010001U), 3, 2, 2);
            require_sizes(P8(0b10010010U), 3, 2, 2);
            require_sizes(P8(0b10010011U), 3, 2, 2);
            require_sizes(P8(0b10010100U), 3, 2, 2);
            require_sizes(P8(0b10010101U), 3, 2, 2);
            require_sizes(P8(0b10010110U), 3, 2, 2);
            require_sizes(P8(0b10010111U), 3, 2, 2);
            require_sizes(P8(0b10011000U), 3, 2, 2);
            require_sizes(P8(0b10011001U), 3, 2, 2);
            require_sizes(P8(0b10011010U), 3, 2, 2);
            require_sizes(P8(0b10011011U), 3, 2, 2);
            require_sizes(P8(0b10011100U), 3, 2, 2);
            require_sizes(P8(0b10011101U), 3, 2, 2);
            require_sizes(P8(0b10011110U), 3, 2, 2);
            require_sizes(P8(0b10011111U), 3, 2, 2);
            require_sizes(P8(0b10100000U), 3, 2, 2);
            require_sizes(P8(0b10100001U), 2, 2, 3);
            require_sizes(P8(0b10100010U), 2, 2, 3);
            require_sizes(P8(0b10100011U), 2, 2, 3);
            require_sizes(P8(0b10100100U), 2, 2, 3);
            require_sizes(P8(0b10100101U), 2, 2, 3);
            require_sizes(P8(0b10100110U), 2, 2, 3);
            require_sizes(P8(0b10100111U), 2, 2, 3);
            require_sizes(P8(0b10101000U), 2, 2, 3);
            require_sizes(P8(0b10101001U), 2, 2, 3);
            require_sizes(P8(0b10101010U), 2, 2, 3);
            require_sizes(P8(0b10101011U), 2, 2, 3);
            require_sizes(P8(0b10101100U), 2, 2, 3);
            require_sizes(P8(0b10101101U), 2, 2, 3);
            require_sizes(P8(0b10101110U), 2, 2, 3);
            require_sizes(P8(0b10101111U), 2, 2, 3);
            require_sizes(P8(0b10110000U), 2, 2, 3);
            require_sizes(P8(0b10110001U), 2, 2, 3);
            require_sizes(P8(0b10110010U), 2, 2, 3);
            require_sizes(P8(0b10110011U), 2, 2, 3);
            require_sizes(P8(0b10110100U), 2, 2, 3);
            require_sizes(P8(0b10110101U), 2, 2, 3);
            require_sizes(P8(0b10110110U), 2, 2, 3);
            require_sizes(P8(0b10110111U), 2, 2, 3);
            require_sizes(P8(0b10111000U), 2, 2, 3);
            require_sizes(P8(0b10111001U), 2, 2, 3);
            require_sizes(P8(0b10111010U), 2, 2, 3);
            require_sizes(P8(0b10111011U), 2, 2, 3);
            require_sizes(P8(0b10111100U), 2, 2, 3);
            require_sizes(P8(0b10111101U), 2, 2, 3);
            require_sizes(P8(0b10111110U), 2, 2, 3);
            require_sizes(P8(0b10111111U), 2, 2, 3);
            require_sizes(P8(0b11000000U), 2, 2, 3);
            require_sizes(P8(0b11000001U), 2, 2, 3);
            require_sizes(P8(0b11000010U), 2, 2, 3);
            require_sizes(P8(0b11000011U), 2, 2, 3);
            require_sizes(P8(0b11000100U), 2, 2, 3);
            require_sizes(P8(0b11000101U), 2, 2, 3);
            require_sizes(P8(0b11000110U), 2, 2, 3);
            require_sizes(P8(0b11000111U), 2, 2, 3);
            require_sizes(P8(0b11001000U), 2, 2, 3);
            require_sizes(P8(0b11001001U), 2, 2, 3);
            require_sizes(P8(0b11001010U), 2, 2, 3);
            require_sizes(P8(0b11001011U), 2, 2, 3);
            require_sizes(P8(0b11001100U), 2, 2, 3);
            require_sizes(P8(0b11001101U), 2, 2, 3);
            require_sizes(P8(0b11001110U), 2, 2, 3);
            require_sizes(P8(0b11001111U), 2, 2, 3);
            require_sizes(P8(0b11010000U), 2, 2, 3);
            require_sizes(P8(0b11010001U), 2, 2, 3);
            require_sizes(P8(0b11010010U), 2, 2, 3);
            require_sizes(P8(0b11010011U), 2, 2, 3);
            require_sizes(P8(0b11010100U), 2, 2, 3);
            require_sizes(P8(0b11010101U), 2, 2, 3);
            require_sizes(P8(0b11010110U), 2, 2, 3);
            require_sizes(P8(0b11010111U), 2, 2, 3);
            require_sizes(P8(0b11011000U), 2, 2, 3);
            require_sizes(P8(0b11011001U), 2, 2, 3);
            require_sizes(P8(0b11011010U), 2, 2, 3);
            require_sizes(P8(0b11011011U), 2, 2, 3);
            require_sizes(P8(0b11011100U), 2, 2, 3);
            require_sizes(P8(0b11011101U), 2, 2, 3);
            require_sizes(P8(0b11011110U), 2, 2, 3);
            require_sizes(P8(0b11011111U), 2, 2, 3);
            require_sizes(P8(0b11100000U), 2, 2, 3);
            require_sizes(P8(0b11100001U), 3, 2, 2);
            require_sizes(P8(0b11100010U), 3, 2, 2);
            require_sizes(P8(0b11100011U), 3, 2, 2);
            require_sizes(P8(0b11100100U), 3, 2, 2);
            require_sizes(P8(0b11100101U), 3, 2, 2);
            require_sizes(P8(0b11100110U), 3, 2, 2);
            require_sizes(P8(0b11100111U), 3, 2, 2);
            require_sizes(P8(0b11101000U), 3, 2, 2);
            require_sizes(P8(0b11101001U), 3, 2, 2);
            require_sizes(P8(0b11101010U), 3, 2, 2);
            require_sizes(P8(0b11101011U), 3, 2, 2);
            require_sizes(P8(0b11101100U), 3, 2, 2);
            require_sizes(P8(0b11101101U), 3, 2, 2);
            require_sizes(P8(0b11101110U), 3, 2, 2);
            require_sizes(P8(0b11101111U), 3, 2, 2);
            require_sizes(P8(0b11110000U), 3, 2, 2);
            require_sizes(P8(0b11110001U), 4, 2, 1);
            require_sizes(P8(0b11110010U), 4, 2, 1);
            require_sizes(P8(0b11110011U), 4, 2, 1);
            require_sizes(P8(0b11110100U), 4, 2, 1);
            require_sizes(P8(0b11110101U), 4, 2, 1);
            require_sizes(P8(0b11110110U), 4, 2, 1);
            require_sizes(P8(0b11110111U), 4, 2, 1);
            require_sizes(P8(0b11111000U), 4, 2, 1);
            require_sizes(P8(0b11111001U), 5, 2);
            require_sizes(P8(0b11111010U), 5, 2);
            require_sizes(P8(0b11111011U), 5, 2);
            require_sizes(P8(0b11111100U), 5, 2);
            require_sizes(P8(0b11111101U), 6, 1);
            require_sizes(P8(0b11111110U), 6, 1);
            require_sizes(P8(0b11111111U), 7);
        }
    }
}

SCENARIO("Check conversion to double")
{
    GIVEN("All posit<5, 1> values")
    {
        THEN("Assert that converting to double yields the expected result")
        {
            using namespace aarith;

            using P5 = aarith::posit<5, 1>;

            // Test based on the very helpful "edu_tables" program provided by
            // https://github.com/stillwater-sc/universal

            CHECK(to_double(P5(0b00000U)) == Approx(0));
            CHECK(to_double(P5(0b00001U)) == Approx(0.015625));
            CHECK(to_double(P5(0b00010U)) == Approx(0.0625));
            CHECK(to_double(P5(0b00011U)) == Approx(0.125));
            CHECK(to_double(P5(0b00100U)) == Approx(0.25));
            CHECK(to_double(P5(0b00101U)) == Approx(0.375));
            CHECK(to_double(P5(0b00110U)) == Approx(0.5));
            CHECK(to_double(P5(0b00111U)) == Approx(0.75));
            CHECK(to_double(P5(0b01000U)) == Approx(1));
            CHECK(to_double(P5(0b01001U)) == Approx(1.5));
            CHECK(to_double(P5(0b01010U)) == Approx(2));
            CHECK(to_double(P5(0b01011U)) == Approx(3));
            CHECK(to_double(P5(0b01100U)) == Approx(4));
            CHECK(to_double(P5(0b01101U)) == Approx(8));
            CHECK(to_double(P5(0b01110U)) == Approx(16));
            CHECK(to_double(P5(0b01111U)) == Approx(64));
            CHECK(std::isnan(to_double(P5(0b10000U))));
            CHECK(to_double(P5(0b10001U)) == Approx(-64));
            CHECK(to_double(P5(0b10010U)) == Approx(-16));
            CHECK(to_double(P5(0b10011U)) == Approx(-8));
            CHECK(to_double(P5(0b10100U)) == Approx(-4));
            CHECK(to_double(P5(0b10101U)) == Approx(-3));
            CHECK(to_double(P5(0b10110U)) == Approx(-2));
            CHECK(to_double(P5(0b10111U)) == Approx(-1.5));
            CHECK(to_double(P5(0b11000U)) == Approx(-1));
            CHECK(to_double(P5(0b11001U)) == Approx(-0.75));
            CHECK(to_double(P5(0b11010U)) == Approx(-0.5));
            CHECK(to_double(P5(0b11011U)) == Approx(-0.375));
            CHECK(to_double(P5(0b11100U)) == Approx(-0.25));
            CHECK(to_double(P5(0b11101U)) == Approx(-0.125));
            CHECK(to_double(P5(0b11110U)) == Approx(-0.0625));
            CHECK(to_double(P5(0b11111U)) == Approx(-0.015625));
        }
    }
}

SCENARIO("Check conversion from double")
{
    GIVEN("All posit<6, 1> values as IEEE double")
    {
        THEN("Assert that converting to posit<6,1> yields the expected result")
        {
            using namespace aarith;

            constexpr size_t N = 6;
            constexpr size_t ES = 1;

            CHECK(to_binary(from_double<N, ES>(1.0)) == "010000");
            CHECK(to_binary(from_double<N, ES>(5.0 / 4.0)) == "010001");
            CHECK(to_binary(from_double<N, ES>(7.0 / 4.0)) == "010011");
            CHECK(to_binary(from_double<N, ES>(3.0 / 2.0)) == "010010");
            CHECK(to_binary(from_double<N, ES>(2.0)) == "010100");
            CHECK(to_binary(from_double<N, ES>(3.0)) == "010110");
            CHECK(to_binary(from_double<N, ES>(5.0 / 2.0)) == "010101");
            CHECK(to_binary(from_double<N, ES>(4.0)) == "011000");
            CHECK(to_binary(from_double<N, ES>(7.0 / 2.0)) == "010111");
            CHECK(to_binary(from_double<N, ES>(6.0)) == "011001");
            CHECK(to_binary(from_double<N, ES>(8.0)) == "011010");
            CHECK(to_binary(from_double<N, ES>(12.0)) == "011011");
            CHECK(to_binary(from_double<N, ES>(16.0)) == "011100");
            CHECK(to_binary(from_double<N, ES>(32.0)) == "011101");
            CHECK(to_binary(from_double<N, ES>(64.0)) == "011110");
            CHECK(to_binary(from_double<N, ES>(256.0)) == "011111");
        }
    }

    GIVEN("Values that are way too big for a <6,1> posit")
    {
        THEN("Assert that converting the given double to posit<6,1> returns maxpos")
        {
            using namespace aarith;

            constexpr size_t N = 6;
            constexpr size_t ES = 1;

            CHECK(to_binary(from_double<N, ES>(999.9999)) == "011111");
            CHECK(to_binary(from_double<N, ES>(123456.78901592)) == "011111");
            CHECK(to_binary(from_double<N, ES>(9999999999999.999999999)) == "011111");
        }
    }
}
