#include "aarith/types/uinteger.hpp"
#include <aarith/operations/comparisons.hpp>
#include <aarith/utilities/string_utils.hpp>
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
            uinteger<TestWidth> const a{7,0};
            uinteger<TestWidth> const b{23,0};


           uinteger<TestWidth> const c{7,0};
           uinteger<TestWidth> const d{0,23};

            THEN("operator< returns true")
            {
                REQUIRE(a < b);
                REQUIRE_FALSE(c < d);
            }
            THEN("operator<= returns true")
            {
                REQUIRE(a <= b);
                REQUIRE_FALSE(c <= d);
            }
            THEN("operator> returns false")
            {
                REQUIRE_FALSE(a > b);
                REQUIRE(c > d);
            }
            THEN("operator>= returns false")
            {
                REQUIRE_FALSE(a >= b);
                REQUIRE(c >= d);
            }
            THEN("operator== returns false")
            {
                REQUIRE_FALSE(a == b);
                REQUIRE_FALSE(c == d);
            }
            THEN("operator!= returns true")
            {
                REQUIRE(a != b);
                REQUIRE(c != d);
            }
        }
    }
}
SCENARIO("Comparing two uintegers with different bit widths", "[uinteger][utility]")
{
    GIVEN("Two uinteger<N> a and b with a < b with different bit widths")
    {
        WHEN("N <= word_width")
        {
            const size_t TestWidth = 16;
            static constexpr uint16_t number_a = 7;
            static constexpr uint16_t number_b = 23;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth + 10> b{number_b};

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
    GIVEN("Two uinteger<N> a and b with a < b with different bit widths")
    {
        const size_t small = 32;
        const size_t big = 150;
        static constexpr uint16_t number_a = 7;
        static constexpr uint16_t number_c = 4;
        static constexpr uint16_t number_b = 23;
        const uinteger<small> a{number_a};
        const uinteger<big> b = uinteger<big>::from_words(number_b, 0U, number_c);

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