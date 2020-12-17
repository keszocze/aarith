#include "../test-signature-ranges.hpp"
#include "gen_float.hpp"
#include <aarith/float.hpp>
#include <bitset>
#include <catch.hpp>
using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Setting a quiet NaN payload +/- zero", "[normalized_float][NaN][utility]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = normalized_float<E, M>;

    GIVEN("A random floating-point number")
    {
        F x = GENERATE(take(100, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        WHEN("Setting this number as a payload")
        {
            F n = setPayload(x);
            THEN("The result is a NaN of correct type")
            {
                if (!x.is_negative() && !x.is_nan())
                {
                    CHECK(n.is_nan());
                    CHECK(n.is_qNaN());
                    REQUIRE_FALSE(n.is_sNaN());
                }
            }

            THEN("The payload should be the correct value")
            {

                if (x.is_negative() || x.is_nan())
                {
                    REQUIRE(bitwise_equality(n, F::zero()));
                }
                else
                {
                    auto input_mantissa = x.get_mantissa();
                    input_mantissa.set_msb(true);
                    REQUIRE(input_mantissa == n.get_mantissa());
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Setting a signalling NaN payload +/- zero",
                       "[normalized_float][NaN][utility]", AARITH_FLOAT_TEST_SIGNATURE,
                       AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = normalized_float<E, M>;

    GIVEN("A random floating-point number")
    {
        F x = GENERATE(take(100, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        WHEN("Setting this number as a payload")
        {
            F n = setPayloadSignaling(x);
            THEN("The result is a NaN of correct type")
            {
                if (!(x.is_negative() || x.is_nan()))
                {
                    auto input_mantissa = x.get_mantissa();
                    input_mantissa.set_msb(false);

                    if (!input_mantissa.is_zero())
                    {
                        CHECK(n.is_nan());
                        CHECK_FALSE(n.is_qNaN());
                        CHECK(n.is_sNaN());
                    }
                }
            }
            THEN("The payload should be the correct value")
            {
                if (x.is_negative() || x.is_nan())
                {
                    REQUIRE(bitwise_equality(n, F::zero()));
                }
                else
                {
                    auto input_mantissa = x.get_mantissa();
                    input_mantissa.set_msb(false);
                    REQUIRE(input_mantissa == n.get_mantissa());
                }
            }
        }
    }
}