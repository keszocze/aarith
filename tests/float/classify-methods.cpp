#include "../test-signature-ranges.hpp"
#include "gen_float.hpp"
#include <aarith/float.hpp>
#include <bitset>
#include <catch.hpp>
using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Testing the classification methods",
                       "[floating_point][classify][utility]", AARITH_FLOAT_TEST_SIGNATURE,
                       AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = floating_point<E, M>;

    GIVEN("A random subnormal number")
    {
        F x = GENERATE(take(50, random_float<E, M, FloatGenerationModes::DenormalizedOnly>()));

        WHEN("Checking the is<SomeClass> methods")
        {
            THEN("It should determine the correct value")
            {
                CHECK_FALSE(isNormal(x));

                CHECK(isFinite(x));

                // for whatever reason, zero actually is *not* normal
                if (x.is_zero())
                {
                    CHECK(isZero(x));
                }
                else
                {
                    CHECK(isSubnormal(x));
                }

                CHECK_FALSE(isInfinite(x));

                CHECK_FALSE(isNaN(x));

                CHECK_FALSE(isSignaling(x));

                CHECK_FALSE(isQuiet(x));

                REQUIRE(isCanonical(x));
            }
        }
    }

    GIVEN("A random normal number")
    {
        F x = GENERATE(take(50, random_float<E, M, FloatGenerationModes::NormalizedOnly>()));

        WHEN("Checking the is<SomeClass> methods")
        {
            THEN("It should determine the correct value")
            {
                CHECK(isNormal(x));

                CHECK(isFinite(x));

                // for whatever reason, zero actually is *not* normal
                CHECK_FALSE(isZero(x));
                CHECK_FALSE(isSubnormal(x));

                CHECK_FALSE(isInfinite(x));

                CHECK_FALSE(isNaN(x));

                CHECK_FALSE(isSignaling(x));

                CHECK_FALSE(isQuiet(x));

                REQUIRE(isCanonical(x));
            }
        }
    }

    GIVEN("+/- zero")
    {

        F pZero = F::zero();
        F nZero = F::neg_zero();

        WHEN("Checking the is<SomeClass> methods")
        {
            THEN("It should determine the correct value")
            {
                CHECK_FALSE(isSignMinus(pZero));
                CHECK(isSignMinus(nZero));

                // for whatever reason, zero actually is *not* normal
                CHECK_FALSE(isNormal(pZero));
                CHECK_FALSE(isNormal(nZero));

                CHECK(isFinite(pZero));
                CHECK(isFinite(nZero));

                CHECK(isZero(pZero));
                CHECK(isZero(nZero));

                CHECK_FALSE(isSubnormal(pZero));
                CHECK_FALSE(isSubnormal(nZero));

                CHECK_FALSE(isInfinite(pZero));
                CHECK_FALSE(isInfinite(nZero));

                CHECK_FALSE(isNaN(pZero));
                CHECK_FALSE(isNaN(nZero));

                CHECK_FALSE(isSignaling(pZero));
                CHECK_FALSE(isSignaling(nZero));

                CHECK_FALSE(isQuiet(pZero));
                CHECK_FALSE(isQuiet(nZero));

                CHECK(isCanonical(pZero));
                REQUIRE(isCanonical(nZero));
            }
        }
    }

    GIVEN("+/- infinity")
    {

        F pInf = F::pos_infinity();
        F nInf = F::neg_infinity();

        WHEN("Checking the is<SomeClass> methods")
        {
            THEN("It should determine the correct value")
            {
                CHECK_FALSE(isSignMinus(pInf));
                CHECK(isSignMinus(nInf));

                CHECK_FALSE(isNormal(pInf));
                CHECK_FALSE(isNormal(nInf));

                CHECK_FALSE(isFinite(pInf));
                CHECK_FALSE(isFinite(nInf));

                CHECK_FALSE(isZero(pInf));
                CHECK_FALSE(isZero(nInf));

                CHECK_FALSE(isSubnormal(pInf));
                CHECK_FALSE(isSubnormal(nInf));

                CHECK(isInfinite(pInf));
                CHECK(isInfinite(nInf));

                CHECK_FALSE(isNaN(pInf));
                CHECK_FALSE(isNaN(nInf));

                CHECK_FALSE(isSignaling(pInf));
                CHECK_FALSE(isSignaling(nInf));

                CHECK_FALSE(isQuiet(pInf));
                CHECK_FALSE(isQuiet(nInf));

                CHECK(isCanonical(pInf));
                REQUIRE(isCanonical(nInf));
            }
        }
    }

    GIVEN("+/- signaling/quiet NaN")
    {

        F pqNaN = F::qNaN();
        F nqNaN = -pqNaN;

        F psNaN = F::sNaN();
        F nsNaN = -psNaN;

        WHEN("Checking the is<SomeClass> methods")
        {
            THEN("It should determine the correct value")
            {
                CHECK(isSignMinus(nqNaN));
                CHECK(isSignMinus(nsNaN));
                CHECK_FALSE(isSignMinus(pqNaN));
                CHECK_FALSE(isSignMinus(psNaN));

                CHECK_FALSE(isNormal(nqNaN));
                CHECK_FALSE(isNormal(nsNaN));
                CHECK_FALSE(isNormal(pqNaN));
                CHECK_FALSE(isNormal(psNaN));

                CHECK_FALSE(isFinite(nqNaN));
                CHECK_FALSE(isFinite(nsNaN));
                CHECK_FALSE(isFinite(psNaN));
                CHECK_FALSE(isFinite(pqNaN));

                CHECK_FALSE(isZero(nqNaN));
                CHECK_FALSE(isZero(nsNaN));
                CHECK_FALSE(isZero(psNaN));
                CHECK_FALSE(isZero(pqNaN));

                CHECK_FALSE(isSubnormal(nqNaN));
                CHECK_FALSE(isSubnormal(nsNaN));
                CHECK_FALSE(isSubnormal(psNaN));
                CHECK_FALSE(isSubnormal(pqNaN));

                CHECK_FALSE(isInfinite(nqNaN));
                CHECK_FALSE(isInfinite(nsNaN));
                CHECK_FALSE(isInfinite(psNaN));
                CHECK_FALSE(isInfinite(pqNaN));

                CHECK(isNaN(nqNaN));
                CHECK(isNaN(nsNaN));
                CHECK(isNaN(psNaN));
                CHECK(isNaN(pqNaN));

                CHECK_FALSE(isSignaling(nqNaN));
                CHECK(isSignaling(nsNaN));
                CHECK(isSignaling(psNaN));
                CHECK_FALSE(isSignaling(pqNaN));

                CHECK(isQuiet(nqNaN));
                CHECK_FALSE(isQuiet(nsNaN));
                CHECK_FALSE(isQuiet(psNaN));
                CHECK(isQuiet(pqNaN));

                CHECK(isCanonical(nqNaN));
                CHECK(isCanonical(nsNaN));
                CHECK(isCanonical(psNaN));
                CHECK(isCanonical(pqNaN));
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Testing the fp_class method", "[floating_point][classify][utility]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = floating_point<E, M>;

    GIVEN("A random subnormal number")
    {
        F x = GENERATE(take(50, random_float<E, M, FloatGenerationModes::DenormalizedOnly>()));

        WHEN("Computing the class of the number")
        {
            THEN("It should determine the correct value")
            {
                if (x.is_negative())
                {
                    if (x.is_zero())
                    {
                        REQUIRE(fp_class(x) == IEEEClass::negativeZero);
                    }
                    else
                    {
                        REQUIRE(fp_class(x) == IEEEClass::negativeSubnormal);
                    }
                }
                else
                {
                    if (x.is_zero())
                    {
                        REQUIRE(fp_class(x) == IEEEClass::positiveZero);
                    }
                    else
                    {
                        REQUIRE(fp_class(x) == IEEEClass::positiveSubnormal);
                    }
                }
            }
        }
    }

    GIVEN("A random normal number")
    {
        F x = GENERATE(take(50, random_float<E, M, FloatGenerationModes::NormalizedOnly>()));

        WHEN("Computing the class of the number")
        {
            THEN("It should determine the correct value")
            {
                if (x.is_negative())
                {
                    REQUIRE(fp_class(x) == IEEEClass::negativeNormal);
                }
                else
                {
                    REQUIRE(fp_class(x) == IEEEClass::positiveNormal);
                }
            }
        }
    }

    GIVEN("+/- zero")
    {

        F pZero = F::zero();
        F nZero = F::neg_zero();

        WHEN("Computing the class of the number")
        {
            THEN("It should determine the correct value")
            {
                CHECK(fp_class(pZero) == IEEEClass::positiveZero);
                REQUIRE(fp_class(nZero) == IEEEClass::negativeZero);
            }
        }
    }

    GIVEN("+/- infinity")
    {

        F pInf = F::pos_infinity();
        F nInf = F::neg_infinity();

        WHEN("Computing the class of the number")
        {
            THEN("It should determine the correct value")
            {
                CHECK(fp_class(pInf) == IEEEClass::positiveInfinity);
                REQUIRE(fp_class(nInf) == IEEEClass::negativeInfinity);
            }
        }
    }

    GIVEN("+/- signaling/quiet NaN")
    {

        F pqNaN = F::qNaN();
        F nqNaN = -pqNaN;

        F psNaN = F::sNaN();
        F nsNaN = -psNaN;

        WHEN("Computing the class of the number")
        {
            THEN("It should determine the correct value")
            {
                CHECK(fp_class(pqNaN) == IEEEClass::quietNaN);
                CHECK(fp_class(nqNaN) == IEEEClass::quietNaN);
                CHECK(fp_class(psNaN) == IEEEClass::signalingNaN);
                REQUIRE(fp_class(nsNaN) == IEEEClass::signalingNaN);
            }
        }
    }
}