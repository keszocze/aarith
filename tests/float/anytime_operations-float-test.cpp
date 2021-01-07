#include "../test-signature-ranges.hpp"
#include <aarith/float.hpp>
#include <aarith/float/approx_operations.hpp>
#include <catch.hpp>

using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Anytime addition", "[floating_point][arithmetic][constexpr]",
                       AARITH_FLOAT_TEST_SIGNATURE_WITH_NATIVE_TYPE,
                       AARITH_FLOAT_TEMPLATE_NATIVE_RANGE_WITH_TYPE)
{
    using F = floating_point<E, M>;

    GIVEN("Float/Double numbers")
    {

        Native a_ = GENERATE(take(15, random<Native>(std::numeric_limits<Native>::min(),
                                                     std::numeric_limits<Native>::max())));
        Native b_ = GENERATE(take(15, random<Native>(std::numeric_limits<Native>::min(),
                                                     std::numeric_limits<Native>::max())));

        F a{a_};
        F b{b_};

        WHEN("Computing all mantissa bits correctly for the addition")
        {
            F res = anytime_add(a, b, M + 1);
            THEN("The result should match the usual floating_point result")
            {
                F res_exact = add(a, b);
                if (!equal_except_rounding(res, res_exact))
                {
                    F res_dbg = anytime_add(a, b, M + 1);
                    if (equal_except_rounding(res_dbg, res_exact))
                    {
                    }
                    std::cout << to_binary(a) << " +\n" << to_binary(b) << "\n";
                    std::cout << to_binary(res) << " (anytime)\n";
                    std::cout << to_binary(res_exact) << " (exact)\n";
                }
                REQUIRE(equal_except_rounding(res, res_exact));
            }
        }

        WHEN("Computing all mantissa bits correctly for the subtraction")
        {
            F res = anytime_sub(a, b, M + 1);
            THEN("The result should match the usual floating_point result")
            {
                F res_exact = sub(a, b);
                if (!equal_except_rounding(res, res_exact))
                {
                    F res_dbg = anytime_sub(a, b, M + 1);
                    if (equal_except_rounding(res_dbg, res_exact))
                    {
                    }
                    std::cout << to_binary(a) << " -\n" << to_binary(b) << "\n";
                    std::cout << to_binary(res) << " (anytime)\n";
                    std::cout << to_binary(res_exact) << " (exact)\n";
                }
                REQUIRE(equal_except_rounding(res, res_exact));
            }
        }

        WHEN("Computing all mantissa bits correctly for the multiplication")
        {
            F res = anytime_mul(a, b, M + 1);
            THEN("The result should match the usual floating_point result")
            {
                F res_exact = mul(a, b);
                if (!equal_except_rounding(res, res_exact))
                {
                    F res_dbg = anytime_mul(a, b, M + 1);
                    if (equal_except_rounding(res_dbg, res_exact))
                    {
                    }
                    std::cout << to_binary(a) << " *\n" << to_binary(b) << "\n";
                    std::cout << to_binary(res) << " (anytime)\n";
                    std::cout << to_binary(res_exact) << " (exact)\n";
                }
                REQUIRE(equal_except_rounding(res, res_exact));
            }
        }

        WHEN("Computing all mantissa bits correctly for the division")
        {
            F res = anytime_div(a, b, M + 1);
            THEN("The result should match the usual floating_point result")
            {
                F res_exact = div(a, b);
                if (!equal_except_rounding(res, res_exact))
                {
                    F res_dbg = anytime_div(a, b, M + 1);
                    if (equal_except_rounding(res_dbg, res_exact))
                    {
                    }
                    std::cout << to_binary(a) << " /\n" << to_binary(b) << "\n";
                    std::cout << to_binary(res) << " (anytime)\n";
                    std::cout << to_binary(res_exact) << " (exact)\n";
                }
                REQUIRE(equal_except_rounding(res, res_exact));
            }
        }
    }
}
