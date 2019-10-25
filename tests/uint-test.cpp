#include <catch.hpp>
#include <iostream>
#include <operations/exact_operations.hpp>
#include <types/integer.hpp>

using namespace aarith;

SCENARIO("Exact sum of unsigned integers", "[uinteger]")
{
    GIVEN("Two uintegers with width < word_width")
    {
        static constexpr size_t TestWidth = 16;
        static_assert(uinteger<TestWidth>::word_count() == 1);

        WHEN("The result still fits the width")
        {
            static constexpr uint16_t number_a = 32;
            static constexpr uint16_t number_b = 16;
            uinteger<TestWidth> a{number_a};
            uinteger<TestWidth> b{number_b};
            auto result = exact_uint_add(a, b);

            THEN("It should be the correct sum")
            {
                REQUIRE(result.word(0) == number_a + number_b);
            }
        }
        WHEN("The result does not fit the width")
        {
            static constexpr uint16_t number_a = 65535;
            static constexpr uint16_t number_b = 1;
            uinteger<TestWidth> a{number_a};
            uinteger<TestWidth> b{number_b};
            auto result = exact_uint_add(a, b);

            THEN("It should be the masked to fit")
            {
                REQUIRE(result.word(0) == 0);
            }
        }
    }
}

// for static_assert tests:
// https://stackoverflow.com/questions/30155619/expected-build-failure-tests-in-cmake