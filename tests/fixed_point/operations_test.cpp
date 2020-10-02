#include <catch.hpp>

#include "gen_fixed_point.hpp"
#include <aarith/fixed_point.hpp>

#include "../test-signature-ranges.hpp"

using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Fixed-point addition is commutative",
                       "[fixed_point][signed][unsigned][arithmetic][addition]",
                       AARITH_FIXED_TEST_EXTENDED_SIGNATURE, AARITH_FIXED_TEST_EXTENDED_PARAM_RANGE)
{
    GIVEN("Two fixed point numbers")
    {
        using Fixed = fixed<I, F, BaseInt, WordType>;

        Fixed a = GENERATE(take(10, random_fixed_point<I, F, BaseInt, WordType>()));

        WHEN("Both numbers have the same widths")
        {
            Fixed b = GENERATE(take(10, random_fixed_point<I, F, BaseInt, WordType>()));

            AND_WHEN("Adding the numbers with width expansion")
            {
                auto res1 = expanding_add(a, b);
                auto res2 = expanding_add(b, a);
                THEN("The results should be identical")
                {
                    REQUIRE(res1 == res2);
                }
            }
            AND_WHEN("Adding the numbers without width expansion")
            {
                Fixed res1 = add(a, b);
                Fixed res2 = add(b, a);
                //                std::cout << a << "\n" << b << "\n" << res1 << "\n\n";
                THEN("The results should be identical")
                {
                    REQUIRE(res1 == res2);
                }
            }
        }
        WHEN("The numbers are of different widths")
        {
            AND_WHEN("Adding the numbers with width expansion")
            {
                THEN("The addition should be commutative")
                {
                    auto b = GENERATE(take(5, random_fixed_point<I, F + 8, BaseInt, WordType>()));
                    auto c =
                        GENERATE(take(5, random_fixed_point<I + 2, F + 8, BaseInt, WordType>()));
                    auto d =
                        GENERATE(take(5, random_fixed_point<I + 2, F - 1, BaseInt, WordType>()));

                    CHECK(expanding_add(a, b) == expanding_add(b, a));
                    CHECK(expanding_add(a, c) == expanding_add(c, a));
                    CHECK(expanding_add(a, d) == expanding_add(d, a));
                    CHECK(expanding_add(b, d) == expanding_add(d, b));
                    CHECK(expanding_add(c, d) == expanding_add(d, c));
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Adding zero does not do anything",
                       "[fixed_point][signed][unsigned][arithmetic][addition]",
                       AARITH_FIXED_TEST_EXTENDED_SIGNATURE, AARITH_FIXED_TEST_EXTENDED_PARAM_RANGE)
{
    GIVEN("Two fixed point numbers")
    {

        using Fixed = fixed<I, F, BaseInt, WordType>;

        Fixed a = GENERATE(take(20, random_fixed_point<I, F, BaseInt, WordType>()));

        WHEN("Adding the numbers with width expansion")
        {
            auto res = expanding_add(a, Fixed::zero());
            auto other_zero = fixed<I - 1, F + 923, BaseInt, WordType>::zero();
            auto res_other = expanding_add(a, other_zero);
            THEN("The results should be identical")
            {
                CHECK(res_other == a);
                REQUIRE(res == a);
            }
        }
        WHEN("Adding the numbers with width expansion")
        {
            auto res = add(a, Fixed::zero());
            THEN("The results should be identical")
            {
                REQUIRE(res == a);
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Addition wraps around correctly for unsigned fixed points",
                       "[fixed_point][unsigned][arithmetic][addition]", AARITH_FIXED_TEST_SIGNATURE,
                       AARITH_FIXED_TEST_PARAM_RANGE)
{
    using Fixed = fixed<I, F, uinteger, WordType>;

    static constexpr Fixed zero = Fixed::zero();
    static constexpr Fixed ulp = Fixed::ulp();
    static constexpr Fixed max_val = Fixed::max();

    WHEN("Adding one to max")
    {
        THEN("The result should be zero")
        {
            auto sum = add(max_val, ulp);
            REQUIRE(sum == zero);
        }
    }
        WHEN("Adding max to max")
        {
            THEN("The result should be max - 1")
            {
                REQUIRE(add(max_val, max_val) == sub(max_val, ulp));
            }
        }
}

TEMPLATE_TEST_CASE_SIG("Addition wraps around correctly for signed fixed points",
                       "[fixed_point][signed][arithmetic][addition]", AARITH_FIXED_TEST_SIGNATURE,
                       AARITH_FIXED_TEST_PARAM_RANGE)
{
    using Fixed = fixed<I, F, integer, WordType>;

    static constexpr Fixed ulp = Fixed::ulp();
    static constexpr Fixed max_val = Fixed::max();
    static constexpr Fixed min_val = Fixed::min();

    WHEN("Adding one to max")
    {
        THEN("The result should be min")
        {
            auto sum = add(max_val, ulp);
            REQUIRE(sum == min_val);
        }
    }
    WHEN("Adding min to min")
    {
        THEN("The result should be zero")
        {
            auto sum = add(min_val, min_val);
            REQUIRE(sum == Fixed::zero());
        }
    }
        WHEN("Adding max to max")
        {
            THEN("The result should be max - 1")
            {
                REQUIRE(add(max_val, max_val) == sub(max_val, ulp));
            }
        }
}

SCENARIO("Addition of fixed point numbers", "[fixed point][signed][arithmetic][addition]")
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