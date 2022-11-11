#include <catch.hpp>
#include <iostream>
#include <string>

#include "../test-signature-ranges.hpp"
#include "gen_word_array.hpp"
#include <aarith/core.hpp>

using namespace aarith;

SCENARIO("Creating a mask with n bits", "[word_array][utility]")
{
    GIVEN("A a number n of bits to be set")
    {
        THEN("Create a mask with the n least significant bits set to one")
        {
            constexpr size_t Width = 64;
            using WA64 = word_array<Width, uint64_t>;

            for (size_t n = 1; n <= Width; ++n)
            {
                const WA64 word_mask = get_low_mask<Width, uint64_t>(n);
                const std::string word_string = to_binary(word_mask);

                const size_t expected_zeroes = Width - n;
                const size_t expected_ones = n;
                REQUIRE(expected_zeroes + expected_ones == Width);

                // compare strings; it's the easiest

                const std::string expected_string =
                    std::string(expected_zeroes, '0') + std::string(expected_ones, '1');

                REQUIRE(word_string == expected_string);
            }
        }
    }
}
