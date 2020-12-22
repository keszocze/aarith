#include "../test-signature-ranges.hpp"
#include "gen_float.hpp"
#include <aarith/float.hpp>
#include <bitset>
#include <catch.hpp>
#include <cmath>

using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Negation works as intended",
                       "[floating_point][negate][sig_bit_operation][invariant]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = floating_point<E, M>;

    GIVEN("A random floating-point number")
    {

        F a = GENERATE(take(100, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        WHEN("Negating the number")
        {
            F b = negate(a);
            THEN("Negating the number again should yield the original number")
            {
                // we need to use this equality as NaNs do not compare equal otherwise
                REQUIRE(bitwise_equality(negate(b), a));
                REQUIRE(bitwise_equality(negate(a), b));
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Copying a floating-point number works",
                       "[floating_point][copy][sig_bit_operation][invariant]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = floating_point<E, M>;

    GIVEN("A random floating-point number")
    {

        F a = GENERATE(take(100, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        WHEN("Copying the number")
        {
            F b = copy(a);

            THEN("The result compares equal")
            {
                // we need to use this equality as NaNs do not compare equal otherwise
                REQUIRE(bitwise_equality(a, b));
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Copying a floating-point number with the sign of another number works",
                       "[floating_point][copySign][sig_bit_operation][invariant]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = floating_point<E, M>;

    GIVEN("A random floating-point number")
    {

        F a = GENERATE(take(100, random_float<E, M, FloatGenerationModes::FullyRandom>()));
        F b = GENERATE(take(100, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        WHEN("Copying the number with the sign of another number")
        {
            F c = copySign(a, b);

            THEN("The result compares equal")
            {
                REQUIRE(c.get_sign() == b.get_sign());
                REQUIRE(bitwise_equality(abs(c), abs(a)));
            }
        }
    }
}