#include "gen_uinteger.hpp"
#include "test_util.hpp"
#include <aarith/integer_no_operators.hpp>
#include <catch.hpp>

using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Comparisons for certain values should be fixed and constexpr",
                       "[integer][unsigned][utility][comparison][constexpr]",
                       AARITH_INT_TEST_SIGNATURE, AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = uinteger<W, WordType>;

    GIVEN("Two uinteger<N> a and b with a < b")
    {
        constexpr I zero{I::zero()};
        constexpr I one{I::one()};
        constexpr I max{I::max()};

        THEN("operator< returns true")
        {
            // the extra variable enforces the constexpr context
            constexpr bool comp = zero < one;
            constexpr bool comp2 = zero < max;
            constexpr bool comp3 = one < max;
            REQUIRE(comp);
            REQUIRE(comp2);
            REQUIRE(comp3);
        }
        THEN("operator<= returns true")
        {
            constexpr bool comp = zero <= one;
            constexpr bool comp2 = zero <= max;
            constexpr bool comp3 = one <= max;
            REQUIRE(comp);
            REQUIRE(comp2);
            REQUIRE(comp3);
        }
        THEN("operator> returns false")
        {
            constexpr bool comp = zero > one;
            constexpr bool comp2 = zero > max;
            constexpr bool comp3 = one > max;
            REQUIRE_FALSE(comp);
            REQUIRE_FALSE(comp2);
            REQUIRE_FALSE(comp3);
        }
        THEN("operator>= returns false")
        {
            constexpr bool comp = zero >= one;
            constexpr bool comp2 = zero >= max;
            constexpr bool comp3 = one >= max;
            REQUIRE_FALSE(comp);
            REQUIRE_FALSE(comp2);
            REQUIRE_FALSE(comp3);
        }
        THEN("operator== returns false")
        {
            constexpr bool comp = zero == one;
            constexpr bool comp2 = zero == max;
            constexpr bool comp3 = one == max;
            REQUIRE_FALSE(comp);
            REQUIRE_FALSE(comp2);
            REQUIRE_FALSE(comp3);
        }
        THEN("operator!= returns true")
        {
            constexpr bool comp = zero != one;
            constexpr bool comp2 = zero != max;
            constexpr bool comp3 = one != max;
            REQUIRE(comp);
            REQUIRE(comp2);
            REQUIRE(comp3);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Comparing two unsigned integers of identical bit-width",
                       "[integer][unsigned][utility][comparison]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{

    using I = uinteger<W, WordType>;

    GIVEN("Two uinteger<N> a and b with a < b")
    {

        I a = GENERATE(take(10, random_uinteger<W, WordType>(I::zero(), div(I::max(), I{2U}))));
        I b = GENERATE(
            take(10, random_uinteger<W, WordType>(add(div(I::max(), I{2U}), I::one()), I::max())));

        THEN("operator< returns true")
        {
            bool comp = a < b;
            REQUIRE(comp);
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

TEMPLATE_TEST_CASE_SIG("Comparing two unsigned integers with different bit-widths",
                       "[integer][unsigned][utility][comparison]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{

    using I = uinteger<W, WordType>;
    using S = uinteger<2*W+1, WordType>;

    GIVEN("Two a and b with a < b")
    {

        I a = GENERATE(take(10, random_uinteger<W, WordType>(I::zero(), div(I::max(), I{2U}))));
        S b = GENERATE(
            take(10, random_uinteger<2*W+1, WordType>(expanding_add(div(I::max(), I{2U}), S::one()), S::max())));

        THEN("operator< returns true")
        {
            bool comp = a < b;
            REQUIRE(comp);
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
