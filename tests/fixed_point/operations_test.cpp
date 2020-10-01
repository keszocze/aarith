#include <catch.hpp>

#include "gen_fixed_point.hpp"
#include <aarith/fixed_point.hpp>

#include "../test-signature-ranges.hpp"

using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Fixed-point addition is commutative",
                       "[fixed_point][signed][unsigned][arithmetic][addition]",
                       AARITH_FIXED_TEST_SIGNATURE, AARITH_FIXED_TEST_PARAM_RANGE)
{
    GIVEN("Two fixed point numbers")
    {

        using Fixed = fixed_point<I, F, WordType>;
        Fixed a = GENERATE(take(10, random_fixed_point<I, F, fixed_point, WordType>()));
        Fixed b = GENERATE(take(10, random_fixed_point<I, F, fixed_point, WordType>()));

        WHEN("Adding the numbers with width expansion")
        {
            auto res1 = expanding_add(a, b);
            auto res2 = expanding_add(b, a);
            THEN("The results should be identical")
            {

                REQUIRE(res1 == res2);
            }
        }
    }
}

SCENARIO("Addition of fixed point numbers", "[fixed point][arithmetic][addition]")
{
    GIVEN("Two fixed point numbers")
    {
        constexpr fixed_point<32, 3> a{8};
        constexpr fixed_point<32, 3> b{4};
        {
            WHEN("Adding these numbers")
            {
                THEN("The result should be correct")
                {

                    constexpr size_t twelve{12};
                    constexpr fixed_point<33, 3> expected =
                        fixed_point<33, 3>::from_integral(twelve);

                    const auto result = expanding_add(a, b);
                    //
                    REQUIRE(result == expected);
                }
            }
        }
    }
}