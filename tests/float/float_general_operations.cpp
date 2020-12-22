#include <aarith/float.hpp>
#include <bitset>
#include <catch.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_float.hpp"
using namespace aarith;

template <typename N>
// it is not really "full range" but at least it works
auto full_native_range()
{
    return Catch::Generators::random<N>(std::numeric_limits<N>::lowest() / 100.0f,
                                        std::numeric_limits<N>::max() / 100.0f);
}

TEMPLATE_TEST_CASE_SIG("Generating NaN as a result", "[floating_point][arithmetic][addition]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARIHT_FLOAT_TEMPLATE_RANGE)
{
    // these tests are modeled after the information presented on
    // https://www.doc.ic.ac.uk/~eedwards/compsys/float/nan.html

    using F = floating_point<E, M>;

    GIVEN("NaN and +/- infinity")
    {
        const F neg_inf{F::neg_infinity()};
        const F pos_inf{F::pos_infinity()};
        WHEN("Adding/subtracting")
        {
            F res_add1 = neg_inf + pos_inf;
            F res_add2 = pos_inf + neg_inf;
            F res_sub1 = pos_inf - pos_inf;
            F res_sub2 = neg_inf - neg_inf;

            CHECK(res_add1.is_nan());
            CHECK(res_add2.is_nan());
            REQUIRE(res_sub1.is_nan());
            REQUIRE(res_sub2.is_nan());
        }
        WHEN("Multiplying")
        {
            F res_mul1 = F::zero() * pos_inf;
            F res_mul2 = pos_inf * F::zero();
            F res_mul3 = neg_inf * F::zero();
            F res_mul4 = neg_inf * F::neg_zero();
            F res_mul5 = pos_inf * F::neg_zero();

            REQUIRE(res_mul1.is_nan());
            REQUIRE(res_mul2.is_nan());
            REQUIRE(res_mul3.is_nan());
            REQUIRE(res_mul4.is_nan());
            REQUIRE(res_mul5.is_nan());
        }
        WHEN("Dividing/computing modulus")
        {
            F res_div_zero = F::zero() / F::zero();
            F res_div_inf1 = pos_inf / pos_inf;
            F res_div_inf2 = pos_inf / neg_inf;
            F res_div_inf3 = neg_inf / pos_inf;
            F res_div_inf4 = neg_inf / neg_inf;
            CHECK(res_div_zero.is_nan());
            REQUIRE(res_div_inf1.is_nan());
            REQUIRE(res_div_inf2.is_nan());
            REQUIRE(res_div_inf3.is_nan());
            REQUIRE(res_div_inf4.is_nan());
        }
    }
}
