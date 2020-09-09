#include "aarith/integer/approx_operations.hpp"
#include <aarith/integer_no_operators.hpp>

#include <catch.hpp>
#include <iostream>

using namespace aarith;

SCENARIO("Generate a bitmask for a certain number of bits", "[uinteger][utlitiy]")
{
    GIVEN("A uinteger and the length of a bitmask")
    {
        WHEN("The bitmask spans over only one word")
        {
            static constexpr size_t uinteger_width = 16;
            static constexpr auto mask_width = 8;

            const auto result = generate_bitmask<uinteger<uinteger_width>>(mask_width);

            THEN("The mask should be 8 bits long")
            {
                REQUIRE(to_binary(result) == "1111111100000000");
            }
        }
        WHEN("The bitmask spans over several words")
        {
            static constexpr size_t uinteger_width = 192;
            static constexpr auto mask_width = 190;

            const auto result = generate_bitmask<uinteger<uinteger_width>>(mask_width);
            const auto result_ref_0 = 0xFFFFFFFFFFFFFFFC;
            const auto result_ref_1 = 0xFFFFFFFFFFFFFFFF;
            const auto result_ref_2 = 0xFFFFFFFFFFFFFFFF;

            THEN("The mask should be 190 bits long")
            {
                CHECK(result.word(0) == result_ref_0);
                CHECK(result.word(1) == result_ref_1);
                CHECK(result.word(2) == result_ref_2);
            }
        }
        WHEN("The bitmask is as long as the complete uinteger and fills all words completely")
        {
            static constexpr size_t uinteger_width = 192;
            static constexpr auto mask_width = 192;

            const auto result = generate_bitmask<uinteger<uinteger_width>>(mask_width);
            const auto result_ref_0 = 0xFFFFFFFFFFFFFFFF;
            const auto result_ref_1 = 0xFFFFFFFFFFFFFFFF;
            const auto result_ref_2 = 0xFFFFFFFFFFFFFFFF;

            THEN("The mask should be 192 bits long")
            {

                CHECK(result.word(0) == result_ref_0);
                CHECK(result.word(1) == result_ref_1);
                CHECK(result.word(2) == result_ref_2);
            }
        }
    }
}

SCENARIO("Approximate product of unsigned integers with anytime bitmasking",
         "[uinteger][arithmetic]")
{
    GIVEN("Two uintegers with width < word_width")
    {
        static constexpr size_t TestWidth = 8;
        static_assert(uinteger<TestWidth>::word_count() == 1);
        static constexpr uint8_t number_a = 170U;
        static constexpr uint8_t number_b = 255U;
        // 170*255 = 43350 = 0b1010100101010110
        const uinteger<TestWidth> a{number_a};
        const uinteger<TestWidth> b{number_b};

        WHEN("One bit is calculated")
        {
            auto const result = approx_expanding_mul_post_masking(a, b, 1U);

            REQUIRE(result.word(0) == 0b1000000000000000);
        }
        WHEN("Two bits are calculated")
        {
            auto const result = approx_expanding_mul_post_masking(a, b, 2U);
            auto const result_2bits = 0b1000000000000000;

            REQUIRE(result.word(0) == result_2bits);
        }
        WHEN("Two bits are calculated and a and b are switched")
        {
            auto const result = approx_expanding_mul_post_masking(b, a, 2U);
            auto const result_2bits = 0b1000000000000000;

            REQUIRE(result.word(0) == result_2bits);
        }
        WHEN("Eight bits are calculated")
        {
            auto const result = approx_expanding_mul_post_masking(a, b, 8U);
            auto const result_8bits = 0b1010100100000000;

            REQUIRE(result.word(0) == result_8bits);
        }
        WHEN("Eight bits are calculated and a and b are switched")
        {
            auto const result = approx_expanding_mul_post_masking(b, a, 8U);
            auto const result_8bits = 0b1010100100000000;

            REQUIRE(result.word(0) == result_8bits);
        }
        WHEN("All bits are calculated")
        {
            auto const result = approx_expanding_mul_post_masking(a, b, 16U);
            auto const result_16bits = 43350U;

            REQUIRE(result.word(0) == result_16bits);
        }
        WHEN("All bits are calculated and a and b are switched")
        {
            auto const result = approx_expanding_mul_post_masking(b, a, 16U);
            auto const result_16bits = 43350U;

            REQUIRE(result.word(0) == result_16bits);
        }
    }
}

// for static_assert tests:
// https://stackoverflow.com/questions/30155619/expected-build-failure-tests-in-cmake
