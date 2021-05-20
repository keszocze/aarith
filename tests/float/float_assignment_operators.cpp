#include "../test-signature-ranges.hpp"
#include "gen_float.hpp"
#include <aarith/float.hpp>
#include <catch.hpp>

using namespace aarith;

/**
 * Return whether lhs and rhs are equal. In particular, returns
 * true when both arguments are NaN.
 */
template <size_t E, size_t M, typename WordType>
static bool eq(const floating_point<E, M, WordType>& lhs, const floating_point<E, M, WordType>& rhs)
{
    return (lhs.is_nan() && rhs.is_nan()) || (lhs == rhs);
}

TEMPLATE_TEST_CASE_SIG("Assignment operators behave like normal operators",
                       "[floating_point][arithmetic]", AARITH_FLOAT_TEST_SIGNATURE,
                       AARITH_FLOAT_TEMPLATE_RANGE)
{
    using F = floating_point<E, M>;

    GIVEN("Two random arguments x and y")
    {
        F x = GENERATE(take(200, random_float<E, M, FloatGenerationModes::FullyRandom>()));
        F y = GENERATE(take(200, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        WHEN("Doing arithmetic on x and y")
        {
            THEN("The results of infix operators and assignment operators match")
            {

                {
                    const F infix = x + y;

                    F assign = x;
                    assign += y;

                    REQUIRE(eq(infix, assign));
                }

                {
                    const F infix = x - y;

                    F assign = x;
                    assign -= y;

                    REQUIRE(eq(infix, assign));
                }

                {
                    const F infix = x * y;

                    F assign = x;
                    assign *= y;

                    REQUIRE(eq(infix, assign));
                }

                {
                    const F infix = x / y;

                    F assign = x;
                    assign /= y;

                    REQUIRE(eq(infix, assign));
                }
            }
        }
    }
}
