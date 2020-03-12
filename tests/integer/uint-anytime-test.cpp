#include "aarith/operations/approx_operations.hpp"
#include <aarith/integer.hpp>

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

SCENARIO("Approximate sum of unsigned integers with anytime bitmasking",
         "[uinteger][arithmetic][anytime]")
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
            auto const result_pre = approx_add_pre_masking(a, b, 1U);
            auto const result_post = approx_add_post_masking(a, b, 1U);

            THEN("It should be number_a")
            {
                REQUIRE(result_pre.word(0) == number_a);
            }
            AND_THEN("Pre and post masking should be identical")
            {
                REQUIRE(result_pre == result_post);
            }
        }
        WHEN("Eight bits are calculated")
        {
            static constexpr uint16_t number_a = 32768U;
            static constexpr uint16_t number_b = 32767U;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};
            auto const result_pre = approx_add_pre_masking(a, b, 8U);
            auto const result_post = approx_add_post_masking(a, b, 8U);
            auto const result_8bits = 65280U;

            THEN("It should be number_a")
            {
                REQUIRE(result_pre.word(0) == result_8bits);
            }
            AND_THEN("Pre and post masking should be identical")
            {
                REQUIRE(result_pre == result_post);
            }
        }
        WHEN("All bits are calculated")
        {
            static constexpr uint16_t number_a = 32768U;
            static constexpr uint16_t number_b = 32767U;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};
            auto const result_pre = approx_add_pre_masking(a, b, 16U);
            auto const result_post = approx_add_post_masking(a, b, 16U);
            auto const result_16bits = 65535U;

            THEN("It should be number_a")
            {
                REQUIRE(result_pre.word(0) == result_16bits);
            }
            AND_THEN("Pre and post masking should be identical")
            {
                REQUIRE(result_pre == result_post);
            }
        }
    }
}

SCENARIO("Approximate difference of unsigned integers with anytime bitmasking",
         "[uinteger][arithmetic][anytime]")
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
            auto const result_pre = approx_sub_pre_masking(a, b, 1U);
            auto const result_post = approx_sub_post_masking(a, b, 1U);

            THEN("It should be one")
            {
                REQUIRE(result_pre.word(0) == number_a);
            }
            AND_THEN("Pre and post masking should be identical")
            {
                REQUIRE(result_pre == result_post);
            }
        }
        WHEN("Eight bits are calculated")
        {
            static constexpr uint16_t number_a = 32768U;
            static constexpr uint16_t number_b = 32767U;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};
            auto const result_pre = approx_add_pre_masking(a, b, 8U);
            auto const result_post = approx_add_post_masking(a, b, 8U);
            auto const result_8bits = 65280U;

            THEN("It should be number_a")
            {
                REQUIRE(result_pre.word(0) == result_8bits);
            }
            AND_THEN("Pre and post masking should be identical")
            {
                REQUIRE(result_pre == result_post);
            }
        }
        WHEN("All bits are calculated")
        {
            static constexpr uint16_t number_a = 32768U;
            static constexpr uint16_t number_b = 32767U;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};
            auto const result_pre = approx_add_pre_masking(a, b, 16U);
            auto const result_post = approx_add_post_masking(a, b, 16U);
            auto const result_16bits = 65535U;

            THEN("It should be number_a")
            {
                REQUIRE(result_pre.word(0) == result_16bits);
            }
            AND_THEN("Pre and post masking should be identical")
            {
                REQUIRE(result_pre == result_post);
            }
        }
    }
}

// SCENARIO("Approximate product of unsigned integers with anytime bitmasking",
//         "[uinteger][arithmetic][anytime]")
//{
//    GIVEN("Two uintegers with width < word_width")
//    {
//        static constexpr size_t TestWidth = 8;
//        static_assert(uinteger<TestWidth>::word_count() == 1);
//        static constexpr uint8_t number_a = 170U;
//        static constexpr uint8_t number_b = 255U;
//        const uinteger<TestWidth> a{number_a};
//        const uinteger<TestWidth> b{number_b};
//
//        WHEN("One bit is calculated")
//        {
//            auto const result = approx_uint_bitmasking_mul(a, b, 1U);
//            auto const result_pre = approx_mul_pre_masking(a, b, 1U);
//            auto const result_post = approx_mul_post_masking(a, b, 1U);
//
//            CHECK(result == result_pre);
//            CHECK(result == result_post);
//            REQUIRE(result.word(0) == 0);
//        }
//        WHEN("Two bits are calculated")
//        {
//            auto const result = approx_uint_bitmasking_mul(a, b, 2U);
//            auto const result_2bits = 16384U;
//            auto const result_pre = approx_mul_pre_masking(a, b, 1U);
//            auto const result_post = approx_mul_post_masking(a, b, 1U);
//
//            CHECK(result == result_pre);
//            CHECK(result == result_post);
//            REQUIRE(result.word(0) == result_2bits);
//        }
//        WHEN("Two bits are calculated and a and b are switched")
//        {
//            auto const result = approx_uint_bitmasking_mul(b, a, 2U);
//            auto const result_2bits = 16384U;
//            auto const result_pre = approx_mul_pre_masking(a, b, 1U);
//            auto const result_post = approx_mul_post_masking(a, b, 1U);
//
//            CHECK(result == result_pre);
//            CHECK(result == result_post);
//            REQUIRE(result.word(0) == result_2bits);
//        }
//        WHEN("Eight bits are calculated")
//        {
//            auto const result = approx_uint_bitmasking_mul(a, b, 8U);
//            auto const result_8bits = 42496U;
//            auto const result_pre = approx_mul_pre_masking(a, b, 1U);
//            auto const result_post = approx_mul_post_masking(a, b, 1U);
//
//            CHECK(result == result_pre);
//            CHECK(result == result_post);
//            REQUIRE(result.word(0) == result_8bits);
//        }
//        WHEN("Eight bits are calculated and a and b are switched")
//        {
//            auto const result = approx_uint_bitmasking_mul(b, a, 8U);
//            auto const result_8bits = 42496U;
//            auto const result_pre = approx_mul_pre_masking(a, b, 1U);
//            auto const result_post = approx_mul_post_masking(a, b, 1U);
//
//            CHECK(result == result_pre);
//            CHECK(result == result_post);
//            REQUIRE(result.word(0) == result_8bits);
//        }
//        WHEN("All bits are calculated")
//        {
//            auto const result = approx_uint_bitmasking_mul(a, b, 16U);
//            auto const result_16bits = 43350U;
//            auto const result_pre = approx_mul_pre_masking(a, b, 1U);
//            auto const result_post = approx_mul_post_masking(a, b, 1U);
//
//            CHECK(result == result_pre);
//            CHECK(result == result_post);
//            REQUIRE(result.word(0) == result_16bits);
//        }
//        WHEN("All bits are calculated and a and b are switched")
//        {
//            auto const result = approx_uint_bitmasking_mul(b, a, 16U);
//            auto const result_16bits = 43350U;
//            auto const result_pre = approx_mul_pre_masking(a, b, 1U);
//            auto const result_post = approx_mul_post_masking(a, b, 1U);
//
//            CHECK(result == result_pre);
//            CHECK(result == result_post);
//            REQUIRE(result.word(0) == result_16bits);
//        }
//    }
//}

// for static_assert tests:
// https://stackoverflow.com/questions/30155619/expected-build-failure-tests-in-cmake
