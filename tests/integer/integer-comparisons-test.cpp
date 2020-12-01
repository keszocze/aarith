#include "gen_integer.hpp"
#include <aarith/integer/integer_random_generation.hpp>
#include <aarith/integer_no_operators.hpp>
#include <catch.hpp>

#include "../test-signature-ranges.hpp"

using namespace aarith; // NOLINT

TEMPLATE_TEST_CASE_SIG("Comparing two signed integers of same bit width",
                       "[integer][signed][utility][comparison]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;

    GIVEN("a and b with a < b")
    {
        I a = GENERATE(take(30, random_integer<W, WordType>()));

        if (a != I::max())
        {
            I b = add(a, I::one());

            THEN("operator< returns true")
            {
                bool comp = a < b;
                REQUIRE(comp); //
            }
            THEN("operator<= returns true")
            {
                bool comp = a <= b;
                REQUIRE(comp);
            }
            THEN("operator> returns false")
            {
                bool comp = a > b;
                REQUIRE_FALSE(comp);
            }
            THEN("operator>= returns false")
            {
                bool comp = a >= b;
                REQUIRE_FALSE(comp);
            }
            THEN("operator== returns false")
            {
                bool comp = a == b;
                REQUIRE_FALSE(comp);
            }
            THEN("operator!= returns true")
            {
                bool comp = a != b;
                REQUIRE(comp);
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Comparing two signed integers of different bit width",
                       "[integer][signed][utility][comparison]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;
    using V = integer<W + 4, WordType>;

    GIVEN("a and b with a < b")
    {
        I a = GENERATE(take(300, random_integer<W, WordType>()));

        V b{a};

        b = add(b, V::one());

        THEN("operator< returns true")
        {
            bool comp = a < b;
            REQUIRE(comp); //
        }
        THEN("operator<= returns true")
        {
            bool comp = a <= b;
            REQUIRE(comp);
        }
        THEN("operator> returns false")
        {
            bool comp = a > b;
            if (comp) {
                std::cout << "a > b failed for\n";
                std::cout << "a: " << a << "\t" << to_binary(a) << "\n";
                std::cout << "b: " << b << "\t" << to_binary(b) << "\n";
            }
            REQUIRE_FALSE(comp);
        }
        THEN("operator>= returns false")
        {
            bool comp = a >= b;
            REQUIRE_FALSE(comp);
        }
        THEN("operator== returns false")
        {
            bool comp = a == b;
            REQUIRE_FALSE(comp);
        }
        THEN("operator!= returns true")
        {
            bool comp = a != b;
            REQUIRE(comp);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Comparing two positive signed integers of same bit width",
                       "[integer][signed][utility][comparison]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;

    GIVEN("Two z=0 and m=I::max()")
    {
        I z{I::zero()};
        I m{I::max()};

        THEN("operator< returns true")
        {
            bool comp = z < m;
            REQUIRE(comp); //
        }
        THEN("operator<= returns true")
        {
            bool comp = z <= m;
            REQUIRE(comp);
        }
        THEN("operator> returns false")
        {
            bool comp = z > m;
            REQUIRE_FALSE(comp);
        }
        THEN("operator>= returns false")
        {
            bool comp = z >= m;
            REQUIRE_FALSE(comp);
        }
        THEN("operator== returns false")
        {
            bool comp = z == m;
            REQUIRE_FALSE(comp);
        }
        THEN("operator!= returns true")
        {
            bool comp = z != m;
            REQUIRE(comp);
        }
    }
}

SCENARIO("Comparing two positive integers with different bit widths",
         "[integer][signed][utility][comparison]")
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

TEMPLATE_TEST_CASE_SIG("Investigating the comparison of max and min values",
                       "[integer][signed][utility][comparison]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using T = integer<W, WordType>;
    T min = T::min();
    T max = T::max();

    T min_from_limits = std::numeric_limits<T>::min();
    T max_from_limits = std::numeric_limits<T>::max();

    WHEN("Comparing the integer::max/min and numeric_limits::max/min")
    {
        THEN("The values should be identical")
        {
            REQUIRE(min == min_from_limits);
            REQUIRE(max == max_from_limits);

            REQUIRE(!(min != min_from_limits));
            REQUIRE(!(max != max_from_limits));
        }
        THEN("The minimum should be negative")
        {
            REQUIRE(min.is_negative());
            REQUIRE(min_from_limits.is_negative());
        }
    }

    WHEN("Constructing min and max value into a larger integer")
    {
        integer<W + 1, WordType> min_{T::min()};
        integer<W + 1, WordType> max_ = T::max();

        THEN("min should be negative")
        {
            CHECK(min_.is_negative());
            REQUIRE(min.is_negative());
        }
        AND_THEN("the values should match the values from the integers with the smaller width")
        {

            CHECK(min == min_);
            CHECK(max == max_);

            CHECK(min_ == T::min());
            CHECK(max_ == T::max());

            CHECK(min == T::min());
            REQUIRE(max == T::max());
        }
        AND_THEN("min should be smaller than max")
        {
            CHECK(min_ < max);
            REQUIRE(min < max);
        }
    }

    WHEN("Comparing min to itself")
    {
        THEN("The result should make sense")
        {
            REQUIRE_FALSE(min < min);
            REQUIRE_FALSE(min_from_limits < min_from_limits);

            REQUIRE(min <= min);
            REQUIRE(min_from_limits <= min_from_limits);

            REQUIRE_FALSE(min > min);
            REQUIRE_FALSE(min_from_limits > min_from_limits);

            REQUIRE(min >= min);
            REQUIRE(min_from_limits >= min_from_limits);

            REQUIRE_FALSE(min != min);
            REQUIRE_FALSE(min_from_limits != min_from_limits);

            REQUIRE(min == min);
            REQUIRE(min_from_limits == min_from_limits);
        }
    }

    WHEN("Comparing max to itself")
    {
        THEN("The result should make sense")
        {
            REQUIRE_FALSE(max < max);
            REQUIRE_FALSE(max_from_limits < max_from_limits);

            REQUIRE(max <= max);
            REQUIRE(max_from_limits <= max_from_limits);

            REQUIRE_FALSE(max > max);
            REQUIRE_FALSE(max_from_limits > max_from_limits);

            REQUIRE(max >= max);
            REQUIRE(max_from_limits >= max_from_limits);

            REQUIRE_FALSE(max != max);
            REQUIRE_FALSE(max_from_limits != max_from_limits);

            REQUIRE(max == max);
            REQUIRE(max_from_limits == max_from_limits);
        }
    }

    WHEN("Comparing these values")
    {
        THEN("The result should make sense")
        {
            REQUIRE(min < max);
            REQUIRE(min_from_limits < max_from_limits);

            REQUIRE(min <= max);
            REQUIRE(min_from_limits <= max_from_limits);

            REQUIRE_FALSE(min > max);
            REQUIRE_FALSE(min_from_limits > max_from_limits);

            REQUIRE_FALSE(min >= max);
            REQUIRE_FALSE(min_from_limits >= max_from_limits);

            REQUIRE(min != max);
            REQUIRE(min_from_limits != max_from_limits);

            REQUIRE_FALSE(min == max);
            REQUIRE_FALSE(min_from_limits == max_from_limits);
        }
    }
}

SCENARIO("Handpicked test cases", "[integer][signed][utility][comparison]")
{
    GIVEN("Two negative numbers a,b with a < b")
    {
        constexpr size_t width = 4;
        static constexpr integer<width> a{-8};
        static constexpr integer<width> b{-7};

        WHEN("Computing b > a")
        {
            constexpr bool res1 = (a < b);
            constexpr bool res2 = (b > a);
            THEN("The result should be true")
            {
                REQUIRE(res1);
                REQUIRE(res2);
            }
        }
    }
}