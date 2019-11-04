#include "aarith/operations/exact_operations.hpp"
#include "aarith/operations/approx_operations.hpp"
#include "aarith/types/integer.hpp"
#include <aarith/operations/comparisons.hpp>
#include <catch.hpp>
#include <sstream>

using namespace aarith;

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


// for static_assert tests:
// https://stackoverflow.com/questions/30155619/expected-build-failure-tests-in-cmake
