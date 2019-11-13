#include "aarith/operations/comparisons.hpp"
#include "aarith/operations/exact_operations.hpp"
#include "aarith/types/integer.hpp"
#include <catch.hpp>

using namespace aarith;

SCENARIO("Adding two uintegers exactly", "[uinteger][arithmetic]")
{
    GIVEN("Two uinteger<N> a and b with N <= word_width")
    {
        static constexpr size_t TestWidth = 16;
        static_assert(uinteger<TestWidth>::word_count() == 1);

        WHEN("The result a+b still fits into N bits")
        {
            static constexpr uint8_t number_a = 32;
            static constexpr uint8_t number_b = 16;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};
            auto const result = exact_uint_add(a, b);

            THEN("It should be the correct sum")
            {
                REQUIRE(result.word(0) == number_a + number_b);
            }
        }
        WHEN("The result a+b does not fit into N bits")
        {
            static constexpr uint16_t number_a = (1 << TestWidth) - 1;
            static constexpr uint16_t number_b = 1;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};
            auto const result = exact_uint_add(a, b);

            THEN("It should be the masked to fit")
            {
                REQUIRE(result.word(0) == 0);
            }
        }
    }

    GIVEN("Two uinteger<N> a and b with N > word_width")
    {
        static constexpr size_t TestWidth = 128;
        static_assert(uinteger<TestWidth>::word_count() > 1);

        WHEN("There is a carry into the next word")
        {
            static constexpr uint64_t number_a = 1ULL << 63;
            static constexpr uint64_t number_b = 1ULL << 63;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};
            auto const result = exact_uint_add(a, b);

            THEN("It is added to the next word")
            {
                REQUIRE(result.word(1) == 1);
            }
        }
        
        WHEN("There is no carry into the next word")
        {
            static constexpr uint64_t number_a = 1ULL << 63;
            static constexpr uint64_t number_b = 0;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};
            auto const result = exact_uint_add(a, b);

            THEN("The next word is unchanged")
            {
                REQUIRE(result.word(1) == 0);
            }
        }
    }
}

SCENARIO("Subtracting two uintegers exactly", "[uinteger][arithmetic]")
{
    GIVEN("Two uinteger<N> a and b are subtracted")
    {
        WHEN("N <= word_width")
        {
            AND_WHEN("a >= b")
            {
                const uinteger<32> a{15u};
                const uinteger<32> b{7u};
                const uinteger<32> expected{static_cast<uint32_t>(15 - 7)};
                auto const result = exact_uint_sub(a, b);

                THEN("The result is a - b")
                {
                    REQUIRE(result == expected);
                }
            }
            AND_WHEN("a < b")
            {
                const uinteger<32> a{7u};
                const uinteger<32> b{15u};
                const uinteger<32> expected{static_cast<uint32_t>(7 - 15)};
                auto const result = exact_uint_sub(a, b);

                THEN("The result wraps around")
                {
                    REQUIRE(result == expected);
                }
            }
        }
    }
    GIVEN("Two uinteger<N> a and b are subtracted")
    {
        WHEN("N > word_width")
        {
            AND_WHEN("There is a borrow between words")
            {
                auto const a = uinteger<128>::from_words(1, 0);
                auto const b = uinteger<128>::from_words(0, 1);
                auto const expected = uinteger<128>::from_words(0, static_cast<uint64_t>(-1));
                auto const result = exact_uint_sub(a, b);

                THEN("The result borrow is subtracted from the next word")
                {
                    REQUIRE(result == expected);
                }
            }
            AND_WHEN("There is no borrow between words")
            {
                auto const a = uinteger<128>::from_words(1, 1);
                auto const b = uinteger<128>::from_words(0, 1);
                auto const expected = uinteger<128>::from_words(1, 0);
                auto const result = exact_uint_sub(a, b);

                THEN("No borrow is subtracted from the next word")
                {
                    REQUIRE(result == expected);
                }
            }
        }
    }
}
