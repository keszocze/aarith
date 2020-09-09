#include <aarith/integer_no_operators.hpp>
#include <catch.hpp>

using namespace aarith;

SCENARIO("Comparing two uintegers", "[integer][unsigned][utility][comparison]")
{
    GIVEN("Two uinteger<N> a and b with a < b")
    {
        WHEN("N <= word_width")
        {
            const size_t TestWidth = 16;
            static constexpr uint16_t number_a = 7;
            static constexpr uint16_t number_b = 23;
            constexpr uinteger<TestWidth> a{number_a};
            constexpr uinteger<TestWidth> b{number_b};

            THEN("operator< returns true")
            {
                // the extra variable enforces the constexpr context
                constexpr bool comp = a < b;
                REQUIRE(comp);
            }
            THEN("operator<= returns true")
            {
                constexpr bool comp = a <= b;
                REQUIRE(comp);
            }
            THEN("operator> returns false")
            {
                constexpr bool comp = a > b;
                REQUIRE_FALSE(comp);
            }
            THEN("operator>= returns false")
            {
                constexpr bool comp = a >= b;
                REQUIRE_FALSE(comp);
            }
            THEN("operator== returns false")
            {
                constexpr bool comp = a == b;
                REQUIRE_FALSE(comp);
            }
            THEN("operator!= returns true")
            {
                constexpr bool comp = a != b;
                REQUIRE(comp);
            }
        }
        WHEN("N > word_width")
        {
            constexpr size_t TestWidth = 80;
            const uinteger<TestWidth> a{7, 0};
            const uinteger<TestWidth> b{23, 0};

            const uinteger<TestWidth> c{7, 0};
            const uinteger<TestWidth> d{0, 23};

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
SCENARIO("Comparing two uintegers with different bit widths", "[integer][unsigned][utility][comparison]")
{
    GIVEN("Two uinteger<N> a and b with a < b with different bit widths")
    {
        WHEN("N <= word_width")
        {
            const size_t TestWidth = 16;
            static constexpr uint16_t number_a = 7;
            static constexpr uint16_t number_b = 23;
            constexpr uinteger<TestWidth> a{number_a};
            constexpr uinteger<TestWidth + 10> b{number_b};

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
        constexpr size_t small = 32;
        constexpr size_t big = 150;
        static constexpr uint16_t number_a = 7;
        static constexpr uint16_t number_c = 4;
        static constexpr uint16_t number_b = 23;
        constexpr uinteger<small> a{number_a};
        constexpr uinteger<big> b = uinteger<big>::from_words(number_b, 0U, number_c);

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