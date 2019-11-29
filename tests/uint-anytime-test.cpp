#include "aarith/operations/approx_operations.hpp"
#include "aarith/operations/exact_operations.hpp"
#include "aarith/types/integer.hpp"
#include <aarith/operations/comparisons.hpp>
#include <catch.hpp>
#include <sstream>

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
            const auto result_ref = 0xFF;

            THEN("The mask should be 8 bits long")
            {
                REQUIRE(result.word(0) == result_ref);
            }
        }
        WHEN("The bitmask spans over several words")
        {
            static constexpr size_t uinteger_width = 192;
            static constexpr auto mask_width = 190;

            const auto result = generate_bitmask<uinteger<uinteger_width>>(mask_width);
            const auto result_ref_0 = 0xFFFFFFFFFFFFFFFF;
            const auto result_ref_1 = 0xFFFFFFFFFFFFFFFF;
            const auto result_ref_2 = 0x3FFFFFFFFFFFFFFF;

            THEN("The mask should be 190 bits long")
            {
                REQUIRE(result.word(0) == result_ref_0);
                REQUIRE(result.word(1) == result_ref_1);
                REQUIRE(result.word(2) == result_ref_2);
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
                REQUIRE(result.word(0) == result_ref_0);
                REQUIRE(result.word(1) == result_ref_1);
                REQUIRE(result.word(2) == result_ref_2);
            }
        }
    }
}

SCENARIO("Approximate sum of unsigned integers with anytime bitmasking", "[uinteger][arithmetic]")
{
    GIVEN("Two uintegers with width < word_width")
    {
        static constexpr size_t TestWidth = 16;
        static_assert(uinteger<TestWidth>::word_count() == 1);

        WHEN("One bit is calculated")
        {
            static constexpr uint16_t number_a = 32768U;
            static constexpr uint16_t number_b = 32767U;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};
            auto const result = approx_uint_bitmasking_add(a, b, 1U);

            THEN("It should be number_a")
            {
                REQUIRE(result.word(0) == number_a);
            }
        }
        WHEN("Eight bits are calculated")
        {
            static constexpr uint16_t number_a = 32768U;
            static constexpr uint16_t number_b = 32767U;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};
            auto const result = approx_uint_bitmasking_add(a, b, 8U);
            auto const result_8bits = 65280U;

            THEN("It should be number_a")
            {
                REQUIRE(result.word(0) == result_8bits);
            }
        }
        WHEN("All bits are calculated")
        {
            static constexpr uint16_t number_a = 32768U;
            static constexpr uint16_t number_b = 32767U;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};
            auto const result = approx_uint_bitmasking_add(a, b, 16U);
            auto const result_16bits = 65535U;

            THEN("It should be number_a")
            {
                REQUIRE(result.word(0) == result_16bits);
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
        const uinteger<TestWidth> a{number_a};
        const uinteger<TestWidth> b{number_b};

        WHEN("One bit is calculated")
        {
            auto const result = approx_uint_bitmasking_mul(a, b, 1U);

            REQUIRE(result.word(0) == 0);
        }
        WHEN("Two bits are calculated")
        {
            auto const result = approx_uint_bitmasking_mul(a, b, 2U);
            auto const result_2bits = 16384U;

            REQUIRE(result.word(0) == result_2bits);
        }
        WHEN("Two bits are calculated and a and b are switched")
        {
            auto const result = approx_uint_bitmasking_mul(b, a, 2U);
            auto const result_2bits = 16384U;

            REQUIRE(result.word(0) == result_2bits);
        }
        WHEN("Eight bits are calculated")
        {
            auto const result = approx_uint_bitmasking_mul(a, b, 8U);
            auto const result_8bits = 42496U;

            REQUIRE(result.word(0) == result_8bits);
        }
        WHEN("Eight bits are calculated and a and b are switched")
        {
            auto const result = approx_uint_bitmasking_mul(b, a, 8U);
            auto const result_8bits = 42496U;

            REQUIRE(result.word(0) == result_8bits);
        }
        WHEN("All bits are calculated")
        {
            auto const result = approx_uint_bitmasking_mul(a, b, 16U);
            auto const result_16bits = 43350U;

            REQUIRE(result.word(0) == result_16bits);
        }
        WHEN("All bits are calculated and a and b are switched")
        {
            auto const result = approx_uint_bitmasking_mul(b, a, 16U);
            auto const result_16bits = 43350U;

            REQUIRE(result.word(0) == result_16bits);
        }
    }
}

// for static_assert tests:
// https://stackoverflow.com/questions/30155619/expected-build-failure-tests-in-cmake