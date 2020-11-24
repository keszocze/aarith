#include "gen_float.hpp"
#include <aarith/float.hpp>
#include <bitset>
#include <catch.hpp>
using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Total ordering with a single NaN value",
                       "[normalized_float][comparison][utility]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    using F = normalized_float<E, M>;
    constexpr F pqNaN = F::qNaN();
    constexpr F psNaN = F::sNaN();

    F nqNaN = negate(F::qNaN());
    F nsNaN = negate(F::sNaN());

    GIVEN("A random, non NaN, non infinity floating-point number x")
    {
        F x = GENERATE(take(100, random_float<E, M, FloatGenerationModes::NonSpecial>()));

        WHEN("totally odering the number x with NaN")
        {
            AND_WHEN("the NaN is 'positive'") {
                THEN("x < NaN") {
                    CHECK(totalOrder(x, pqNaN));
                    REQUIRE(totalOrder(x,psNaN));
                }
                THEN("!(NaN < x)") {
                    CHECK_FALSE(totalOrder(pqNaN,x));
                    REQUIRE_FALSE(totalOrder(psNaN,x));
                }
            }
            AND_WHEN("the NaN is 'negative'") {
                THEN("NaN < x") {
                    CHECK(totalOrder(nqNaN, x));
                    REQUIRE(totalOrder(nsNaN,x));
                }
                THEN("!(x < NaN)") {
                    CHECK_FALSE(totalOrder(x, nqNaN));
                    REQUIRE_FALSE(totalOrder(x,nsNaN));
                }
            }
        }
    }
    GIVEN("+/- infinity") {
        constexpr F pos_inf= F::pos_infinity();
        constexpr F neg_inf= F::neg_infinity();
        WHEN("totally odering the infinities with NaN")
        {
            AND_WHEN("the NaN is 'positive'") {
                THEN("+/- inf < NaN") {
                    CHECK(totalOrder(pos_inf, pqNaN));
                    REQUIRE(totalOrder(pos_inf,psNaN));
                    CHECK(totalOrder(neg_inf, pqNaN));
                    REQUIRE(totalOrder(neg_inf,psNaN));
                }
                THEN("!(NaN < +/- inf)") {
                    CHECK_FALSE(totalOrder(pqNaN,pos_inf));
                    REQUIRE_FALSE(totalOrder(psNaN,pos_inf));
                    CHECK_FALSE(totalOrder(pqNaN,neg_inf));
                    REQUIRE_FALSE(totalOrder(psNaN,neg_inf));
                }
            }
            AND_WHEN("the NaN is 'negative'") {
                THEN("NaN < +/- inf") {
                    CHECK(totalOrder(nqNaN, pos_inf));
                    REQUIRE(totalOrder(nsNaN,pos_inf));
                    CHECK(totalOrder(nqNaN, neg_inf));
                    REQUIRE(totalOrder(nsNaN,neg_inf));
                }
                THEN("!(+/- inf < NaN)") {
                    CHECK_FALSE(totalOrder(pos_inf, nqNaN));
                    REQUIRE_FALSE(totalOrder(pos_inf,nsNaN));
                    CHECK_FALSE(totalOrder(neg_inf, nqNaN));
                    REQUIRE_FALSE(totalOrder(neg_inf,nsNaN));
                }
            }
        }
    }
}