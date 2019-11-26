#include "aarith/types/integer.hpp"
#include <aarith/operations/comparisons.hpp>
#include <catch.hpp>

using namespace aarith;

SCENARIO("Comparing two uintegers", "[uinteger][utility]")
{
    GIVEN("Two uinteger<N> a and b with a < b")
    {
        WHEN("N <= word_width")
        {
            const size_t TestWidth = 16;
            static constexpr uint16_t number_a = 7;
            static constexpr uint16_t number_b = 23;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};

            THEN("operator< returns true")
            {
                REQUIRE(a < b);
            }
            THEN("operator<= returns true")
            {
                REQUIRE(a <= b);
            }
            THEN("operator> returns false")
            {
                REQUIRE_FALSE(a > b);
            }
            THEN("operator>= returns false")
            {
                REQUIRE_FALSE(a >= b);
            }
            THEN("operator== returns false")
            {
                REQUIRE_FALSE(a == b);
            }
            THEN("operator!= returns true")
            {
                REQUIRE(a != b);
            }
        }
        WHEN("N > word_width")
        {
            const size_t TestWidth = 80;
            auto const a = uinteger<TestWidth>::from_words(7, 0);
            auto const b = uinteger<TestWidth>::from_words(23, 0);

            THEN("operator< returns true")
            {
                REQUIRE(a < b);
            }
            THEN("operator<= returns true")
            {
                REQUIRE(a <= b);
            }
            THEN("operator> returns false")
            {
                REQUIRE_FALSE(a > b);
            }
            THEN("operator>= returns false")
            {
                REQUIRE_FALSE(a >= b);
            }
            THEN("operator== returns false")
            {
                REQUIRE_FALSE(a == b);
            }
            THEN("operator!= returns true")
            {
                REQUIRE(a != b);
            }
        }
    }
}
