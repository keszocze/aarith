#include "aarith/types/integer.hpp"
#include <catch.hpp>
#include <sstream>

using namespace aarith;

SCENARIO("Outputting uintegers", "[uinteger]")
{
    WHEN("Writing a uinteger into a stream")
    {
        const uinteger<16> uint{static_cast<uint16_t>(0b11001100)};
        std::stringstream ss;
        ss << uint;

        THEN("Its bit representation is output")
        {
            REQUIRE(ss.str() == "0000000011001100");
        }
    }
}

SCENARIO("Casting uintegers into different width", "[uinteger]")
{
    GIVEN("width_cast is called")
    {
        static constexpr uint16_t test_value = 123;
        static constexpr size_t SourceWidth = 16;
        const uinteger<SourceWidth> uint{test_value};

        WHEN("The source width <= destination width")
        {
            static constexpr size_t DestinationWidth = 32;
            auto const result = width_cast<DestinationWidth>(uint);

            THEN("The result has the destination width")
            {
                REQUIRE(result.width() == DestinationWidth);
            }
            AND_THEN("The result is not cut off")
            {
                REQUIRE(result.word(0) == test_value);
            }
        }
        WHEN("The source width > destination width")
        {
            static constexpr size_t DestinationWidth = 8;
            auto const result = width_cast<DestinationWidth>(uint);

            THEN("The result has the destination width")
            {
                REQUIRE(result.width() == DestinationWidth);
            }
            AND_THEN("The result is cut off")
            {
                REQUIRE(result.word(0) == (test_value & 0xff));
            }
        }
    }
}

SCENARIO("Calculating the word_masks of uintegers", "[uinteger][utility]")
{
    // The tests all assume that uinteger uses 64-bit words.
    static_assert(uinteger<64>::word_width() == 64);

    GIVEN("A uinteger<N> where N < word_width")
    {
        uinteger<32> uint;
        THEN("The mask is correct")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffff);
        }
    }
    GIVEN("A uinteger<N> where N == word_width")
    {
        uinteger<64> uint;
        THEN("The mask is all 1s")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffffffffffff);
        }
    }
    GIVEN("A uinteger<N> where N > word_width and N % word_width != 0")
    {
        uinteger<96> uint;
        THEN("All masks except the last are all 1s")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffffffffffff);
        }
        THEN("The last mask is all ones up to the correct bit")
        {
            REQUIRE(uint.word_mask(1) == 0xffffffff);
        }
    }
    GIVEN("A uinteger<N> where N > word_width and N % word_width == 0")
    {
        uinteger<128> uint;
        THEN("All masks are all 1s")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffffffffffff);
            REQUIRE(uint.word_mask(1) == 0xffffffffffffffff);
        }
    }
}

// for static_assert tests:
// https://stackoverflow.com/questions/30155619/expected-build-failure-tests-in-cmake