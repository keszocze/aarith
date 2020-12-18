#include "../test-signature-ranges.hpp"
#include "gen_float.hpp"
#include <aarith/float.hpp>
#include <bitset>
#include <catch.hpp>
using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Total ordering +/- zero", "[normalized_float][comparison][utility]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = normalized_float<E, M>;

    GIVEN("+/- zero")
    {

        constexpr F pos_zero{F::zero()};
        constexpr F neg_zero{F::neg_zero()};

        THEN("The ordering behaves as defined in the standard")
        {
            constexpr bool v = totalOrder(neg_zero, pos_zero);
            REQUIRE(v);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Total ordering with itself", "[normalized_float][comparison][utility]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = normalized_float<E, M>;

    GIVEN("A random, non NaN, non infinity floating-point number x")
    {
        F x = GENERATE(take(100, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        if (!x.is_nan())
        {
            WHEN("ordering the number with itself")
            {
                THEN("The result should be true")
                {
                    const bool b = totalOrder(x, x);
                    REQUIRE(b);
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Total ordering with a single NaN value",
                       "[normalized_float][comparison][utility]", AARITH_FLOAT_TEST_SIGNATURE,
                       AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = normalized_float<E, M>;
    constexpr F pqNaN = F::qNaN();
    constexpr F psNaN = F::sNaN();

    constexpr F nqNaN = negate(F::qNaN());
    constexpr F nsNaN = negate(F::sNaN());

    GIVEN("A random, non NaN, non infinity floating-point number x")
    {
        F x = GENERATE(take(100, random_float<E, M, FloatGenerationModes::NonSpecial>()));

        WHEN("totally ordering the number x with NaN")
        {
            AND_WHEN("the NaN is 'positive'")
            {
                THEN("x < NaN")
                {
                    CHECK(totalOrder(x, pqNaN));
                    REQUIRE(totalOrder(x, psNaN));
                }
                THEN("!(NaN < x)")
                {
                    CHECK_FALSE(totalOrder(pqNaN, x));
                    REQUIRE_FALSE(totalOrder(psNaN, x));
                }
            }
            AND_WHEN("the NaN is 'negative'")
            {
                THEN("NaN < x")
                {
                    CHECK(totalOrder(nqNaN, x));
                    REQUIRE(totalOrder(nsNaN, x));
                }
                THEN("!(x < NaN)")
                {
                    CHECK_FALSE(totalOrder(x, nqNaN));
                    REQUIRE_FALSE(totalOrder(x, nsNaN));
                }
            }
        }
    }
    GIVEN("+/- infinity")
    {
        constexpr F pos_inf = F::pos_infinity();
        constexpr F neg_inf = F::neg_infinity();
        WHEN("totally odering the infinities with NaN")
        {
            AND_WHEN("the NaN is 'positive'")
            {
                THEN("+/- inf < NaN")
                {
                    CHECK(totalOrder(pos_inf, pqNaN));
                    REQUIRE(totalOrder(pos_inf, psNaN));
                    CHECK(totalOrder(neg_inf, pqNaN));
                    REQUIRE(totalOrder(neg_inf, psNaN));
                }
                THEN("!(NaN < +/- inf)")
                {
                    CHECK_FALSE(totalOrder(pqNaN, pos_inf));
                    REQUIRE_FALSE(totalOrder(psNaN, pos_inf));
                    CHECK_FALSE(totalOrder(pqNaN, neg_inf));
                    REQUIRE_FALSE(totalOrder(psNaN, neg_inf));
                }
            }
            AND_WHEN("the NaN is 'negative'")
            {
                THEN("NaN < +/- inf")
                {
                    CHECK(totalOrder(nqNaN, pos_inf));
                    REQUIRE(totalOrder(nsNaN, pos_inf));
                    CHECK(totalOrder(nqNaN, neg_inf));
                    REQUIRE(totalOrder(nsNaN, neg_inf));
                }
                THEN("!(+/- inf < NaN)")
                {
                    CHECK_FALSE(totalOrder(pos_inf, nqNaN));
                    REQUIRE_FALSE(totalOrder(pos_inf, nsNaN));
                    CHECK_FALSE(totalOrder(neg_inf, nqNaN));
                    REQUIRE_FALSE(totalOrder(neg_inf, nsNaN));
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Total ordering with a two NaN values",
                       "[normalized_float][comparison][utility]", AARITH_FLOAT_TEST_SIGNATURE,
                       AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = normalized_float<E, M>;
    constexpr F pqNaN = F::qNaN();
    constexpr F psNaN = F::sNaN();

    constexpr F nqNaN = negate(F::qNaN());
    constexpr F nsNaN = negate(F::sNaN());

    GIVEN("All four types of NaNs (ignoring any payloads)")
    {
        WHEN("Totally ordering them")
        {
            THEN("i) negative sign orders below positive sign")
            {
                CHECK(totalOrder(nqNaN, pqNaN));
                CHECK(totalOrder(nqNaN, psNaN));
                CHECK(totalOrder(nsNaN, psNaN));
                CHECK(totalOrder(nsNaN, pqNaN));
                CHECK_FALSE(totalOrder(pqNaN, nqNaN));
                CHECK_FALSE(totalOrder(pqNaN, nsNaN));
                CHECK_FALSE(totalOrder(psNaN, nsNaN));
                CHECK_FALSE(totalOrder(psNaN, nqNaN));
            }
            THEN("ii) signaling orders below quiet for +NaN, reverse for −NaN")
            {
                CHECK(totalOrder(psNaN, pqNaN));
                CHECK_FALSE(totalOrder(pqNaN, psNaN));

                CHECK(totalOrder(nqNaN, nsNaN));
                CHECK_FALSE(totalOrder(nsNaN, nqNaN));
            }
            THEN("iii) otherwise, the order of NaNs is implementation-defined. (false in our case)") {
                CHECK_FALSE(totalOrder(psNaN, psNaN));
                CHECK_FALSE(totalOrder(nsNaN, nsNaN));
                CHECK_FALSE(totalOrder(nqNaN, nqNaN));
                REQUIRE_FALSE(totalOrder(pqNaN, pqNaN));
            }
        }
    }
}







TEMPLATE_TEST_CASE_SIG("Total ordering mag +/- zero", "[normalized_float][comparison][utility]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = normalized_float<E, M>;

    GIVEN("+/- zero")
    {

        constexpr F pos_zero{F::zero()};
        constexpr F neg_zero{F::neg_zero()};

        THEN("The ordering behaves as defined in the standard")
        {
            constexpr bool v = totalOrderMag(neg_zero, pos_zero);
            REQUIRE(v);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Total ordering mag with itself", "[normalized_float][comparison][utility]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = normalized_float<E, M>;

    GIVEN("A random, non NaN, non infinity floating-point number x")
    {
        F x = GENERATE(take(100, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        if (!x.is_nan())
        {
            WHEN("ordering the number with itself")
            {
                THEN("The result should be true")
                {
                    const bool b = totalOrderMag(x, x);
                    REQUIRE(b);
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Total ordering mag with a single NaN value",
                       "[normalized_float][comparison][utility]", AARITH_FLOAT_TEST_SIGNATURE,
                       AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = normalized_float<E, M>;
    constexpr F pqNaN = F::qNaN();
    constexpr F psNaN = F::sNaN();

    constexpr F nqNaN = negate(F::qNaN());
    constexpr F nsNaN = negate(F::sNaN());

    GIVEN("A random, non NaN, non infinity floating-point number x")
    {
        F x = GENERATE(take(100, random_float<E, M, FloatGenerationModes::NonSpecial>()));

        WHEN("totally ordering the number x with NaN")
        {
            AND_WHEN("the NaN is 'positive'")
            {
                THEN("x < NaN")
                {
                    CHECK(totalOrderMag(x, pqNaN));
                    REQUIRE(totalOrderMag(x, psNaN));
                }
                THEN("!(NaN < x)")
                {
                    CHECK_FALSE(totalOrderMag(pqNaN, x));
                    REQUIRE_FALSE(totalOrderMag(psNaN, x));
                }
            }
            AND_WHEN("the NaN is 'negative'")
            {
                THEN("!(abs(NaN) < abs(x))")
                {
                    CHECK_FALSE(totalOrderMag(nqNaN, x));
                    REQUIRE_FALSE(totalOrderMag(nsNaN, x));
                }
                THEN("(abs(x) < abs(NaN))")
                {
                    CHECK(totalOrderMag(x, nqNaN));
                    REQUIRE(totalOrderMag(x, nsNaN));
                }
            }
        }
    }
    GIVEN("+/- infinity")
    {
        constexpr F pos_inf = F::pos_infinity();
        constexpr F neg_inf = F::neg_infinity();
        WHEN("totally odering the infinities with NaN")
        {
            AND_WHEN("the NaN is 'positive'")
            {
                THEN("+/- inf < NaN")
                {
                    CHECK(totalOrderMag(pos_inf, pqNaN));
                    REQUIRE(totalOrderMag(pos_inf, psNaN));
                    CHECK(totalOrderMag(neg_inf, pqNaN));
                    REQUIRE(totalOrderMag(neg_inf, psNaN));
                }
                THEN("!(NaN < +/- inf)")
                {
                    CHECK_FALSE(totalOrderMag(pqNaN, pos_inf));
                    REQUIRE_FALSE(totalOrderMag(psNaN, pos_inf));
                    CHECK_FALSE(totalOrderMag(pqNaN, neg_inf));
                    REQUIRE_FALSE(totalOrderMag(psNaN, neg_inf));
                }
            }
            AND_WHEN("the NaN is 'negative'")
            {
                THEN("!(abs(NaN) < abs(+/- inf))")
                {
                    CHECK_FALSE(totalOrderMag(nqNaN, pos_inf));
                    CHECK_FALSE(totalOrderMag(nsNaN, pos_inf));
                    CHECK_FALSE(totalOrderMag(nqNaN, neg_inf));
                    REQUIRE_FALSE(totalOrderMag(nsNaN, neg_inf));
                }
                THEN("abs(+/- inf) < abs(NaN)")
                {
                    CHECK(totalOrderMag(pos_inf, nqNaN));
                    REQUIRE(totalOrderMag(pos_inf, nsNaN));
                    CHECK(totalOrderMag(neg_inf, nqNaN));
                    REQUIRE(totalOrderMag(neg_inf, nsNaN));
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Total ordering mag with a two NaN values",
                       "[normalized_float][comparison][utility]", AARITH_FLOAT_TEST_SIGNATURE,
                       AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = normalized_float<E, M>;
    constexpr F pqNaN = F::qNaN();
    constexpr F psNaN = F::sNaN();

    constexpr F nqNaN = negate(F::qNaN());
    constexpr F nsNaN = negate(F::sNaN());

    GIVEN("All four types of NaNs (ignoring any payloads)")
    {
        WHEN("Totally ordering them")
        {
            THEN("i) negative sign orders below positive sign")
            {
                CHECK_FALSE(totalOrderMag(nqNaN, pqNaN));
                CHECK_FALSE(totalOrderMag(nqNaN, psNaN));
                CHECK_FALSE(totalOrderMag(nsNaN, psNaN));
                CHECK(totalOrderMag(nsNaN, pqNaN));
                CHECK_FALSE(totalOrderMag(pqNaN, nqNaN));
                CHECK_FALSE(totalOrderMag(pqNaN, nsNaN));
                CHECK_FALSE(totalOrderMag(psNaN, nsNaN));
                CHECK(totalOrderMag(psNaN, nqNaN));
            }
            THEN("ii) signaling orders below quiet for +NaN, reverse for −NaN")
            {
                CHECK(totalOrderMag(psNaN, pqNaN));
                CHECK_FALSE(totalOrderMag(pqNaN, psNaN));

                CHECK_FALSE(totalOrderMag(nqNaN, nsNaN));
                CHECK(totalOrderMag(nsNaN, nqNaN));
            }
        }
    }
}