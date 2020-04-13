#include <aarith/integer.hpp>
#include <catch.hpp>

using namespace aarith;

SCENARIO("Comparing two positive sintegers", "[uinteger][utility]")
{
    GIVEN("Two integer<N> a and b with a < b")
    {
        WHEN("N <= word_width")
        {
            constexpr size_t TestWidth = 16;
            static constexpr uint16_t number_a = 7;
            static constexpr uint16_t number_b = 23;
            constexpr integer<TestWidth> a{number_a};
            constexpr integer<TestWidth> b{number_b};

            THEN("operator< returns true")
            {
                // enforce constexpr context
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
            const size_t TestWidth = 80;
            integer<TestWidth> constexpr a{7, 0};
            integer<TestWidth> constexpr b{23, 0};

            integer<TestWidth> constexpr c{7, 0};
            integer<TestWidth> constexpr d{0, 23};

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
SCENARIO("Comparing two positive sintegers with different bit widths", "[uinteger][utility]")
{
    GIVEN("Two integer<N> a and b with a < b with different bit widths")
    {
        WHEN("N <= word_width")
        {
            const size_t TestWidth = 16;
            static constexpr uint16_t number_a = 7;
            static constexpr uint16_t number_b = 23;
            constexpr integer<TestWidth> a{number_a};
            constexpr integer<TestWidth + 10> b{number_b};

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
    GIVEN("Two integer<N> a and b with a < b with different bit widths")
    {

        const constexpr size_t small = 32;
        const constexpr size_t big = 150;
        static constexpr uint16_t number_a = 7;
        static constexpr uint16_t number_b = 23;
        constexpr integer<small> a{number_a};
        constexpr integer<small> a_neg{-number_a};
        constexpr integer<big> b = integer<big>::from_words(number_b, 0U, 0U);

        constexpr integer<big> c_neg{-number_b};

        THEN("operator< returns true")
        {
            REQUIRE(a_neg < b);
            REQUIRE(a < b);
        }
        THEN("operator<= returns true")
        {
            REQUIRE(a_neg <= b);
            REQUIRE(a <= b);
        }
        THEN("operator> returns false")
        {
            REQUIRE_FALSE(a_neg > b);
            REQUIRE_FALSE(a > b);
        }
        THEN("operator>= returns false")
        {
            REQUIRE_FALSE(a_neg >= b);
            REQUIRE_FALSE(a >= b);
        }
        THEN("operator== returns false")
        {
            REQUIRE_FALSE(a_neg == b);
            REQUIRE_FALSE(a == b);
        }
        THEN("operator!= returns true")
        {
            REQUIRE(a_neg != b);
            REQUIRE(a != b);
        }
    }
}

SCENARIO("Determining strict (in)equality")
{
    GIVEN("Two  integers storing the same value")
    {

        using T = integer<8>;
        using I = integer<9>;

        WHEN("The bit widths are identical")
        {

            THEN("They should be strictly equal")
            {
                for (int i = -10; i < 11; ++i)
                {
                    T a{i};
                    T b{i};

                    const bool strictly_equal = strict_eq(a, b);

                    REQUIRE(strictly_equal);
                }
            }
        }
        WHEN("The bit widths are not identical")
        {
            THEN("They should not be strictly equal")
            {
                for (int i = -10; i < 11; ++i)
                {
                    T a{i};
                    I b{i};

                    const bool strictly_equal = strict_eq(a, b);

                    REQUIRE_FALSE(strictly_equal);
                }
            }
        }
    }

    GIVEN("Two unsigned integers storing the same value")
    {
        using T = uinteger<8>;
        using I = uinteger<9>;

        WHEN("The bit widths are identical")
        {

            THEN("They should be strictly equal")
            {
                for (size_t i = 0; i < 23; ++i)
                {
                    T a{i};
                    T b{i};

                    const bool strictly_equal = strict_eq(a, b);

                    REQUIRE(strictly_equal);
                }
            }
        }
        WHEN("The bit widths are not identical")
        {
            THEN("They should not be strictly equal")
            {
                for (size_t i = 0; i < 23; ++i)
                {
                    T a{i};
                    I b{i};

                    const bool strictly_equal = strict_eq(a, b);

                    REQUIRE_FALSE(strictly_equal);
                }
            }
        }
    }
}