#include <aarith/float.hpp>
#include <aarith/integer.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_float.hpp"

#include <bitset>
#include <catch.hpp>
using namespace aarith;

TEMPLATE_TEST_CASE_SIG("IEEE-754 arithmetic conversion: float, double",
                       "[normalized_float][conversion][ieee-754][casting]",
                       AARITH_FLOAT_TEST_SIGNATURE_WITH_NATIVE_TYPE,
                       AARIHT_FLOAT_TEMPLATE_NATIVE_RANGE_WITH_TYPE)
{

    using F = normalized_float<E, M>;

    GIVEN("A random number in the native format")
    {
        Native a = GENERATE(take(
            50, random(std::numeric_limits<Native>::min(), std::numeric_limits<Native>::max())));

        WHEN("Creating an aarith floating-point from that number")
        {

            const F a_cast{a};

            AND_WHEN("Converting back to the native data format")
            {

                const Native a_cast_back = static_cast<Native>(a_cast);

                THEN("The number should not have changed")
                {
                    REQUIRE(a == a_cast_back);
                }
            }
        }
    }

    GIVEN("A random number in the aarith format")
    {
        F b = GENERATE(take(50, random_float<E, M, FloatGenerationModes::NonSpecial>()));

        WHEN("Casting the value to the native format")
        {
            const Native b_cast = static_cast<Native>(b);

            AND_WHEN("Calling the aarith floating-point constructor with the cast value")
            {
                const F b_cast_back{b_cast};
                THEN("The number should not have changed")
                {
                    REQUIRE(b == b_cast_back);
                }
            }
        }
    }
}


SCENARIO("Creating aarith floats from native +/- infinity")
{
    using Single = normalized_float<8, 23>;
    using Double = normalized_float<11, 52>;
    using Large = normalized_float<20, 70>;
    using Small = normalized_float<3, 10>;

    GIVEN("The infinities in IEEE 754 single precision")
    {
        constexpr float native_pos_inf = std::numeric_limits<float>::infinity();
        constexpr float native_neg_inf = -std::numeric_limits<float>::infinity();

        WHEN("Creating aarith foating-point numbers with enough bits from the infinities")
        {
            const Single sposinf(native_pos_inf);
            const Double dposinf(native_pos_inf);
            const Large lposinf(native_pos_inf);

            const Single sneginf(native_neg_inf);
            const Double dneginf(native_neg_inf);
            const Large lneginf(native_neg_inf);

            THEN("The result should be the correct infinity")
            {
                REQUIRE(sposinf.is_positive());
                REQUIRE(sposinf.is_pos_inf());
                REQUIRE(sposinf.is_inf());

                REQUIRE(sneginf.is_negative());
                REQUIRE(sneginf.is_neg_inf());
                REQUIRE(sneginf.is_inf());

                REQUIRE(dposinf.is_positive());
                REQUIRE(dposinf.is_pos_inf());
                REQUIRE(dposinf.is_inf());

                REQUIRE(dneginf.is_negative());
                REQUIRE(dneginf.is_neg_inf());
                REQUIRE(dneginf.is_inf());

                REQUIRE(lposinf.is_positive());
                REQUIRE(lposinf.is_pos_inf());
                REQUIRE(lposinf.is_inf());

                REQUIRE(lneginf.is_negative());
                REQUIRE(lneginf.is_neg_inf());
                REQUIRE(lneginf.is_inf());


            }
        }
        WHEN("Creating aarith foating-point numbers with enough bits from the infinities")
        {
            THEN("The result should still be the correct infinity")
            {
                const Small smallposinf(native_pos_inf);
                const Small smallneginf(native_neg_inf);

                REQUIRE(smallposinf.is_positive());
                REQUIRE(smallposinf.is_pos_inf());
                REQUIRE(smallposinf.is_inf());

                REQUIRE(smallneginf.is_negative());
                REQUIRE(smallneginf.is_neg_inf());
                REQUIRE(smallneginf.is_inf());
            }
        }
    }
    GIVEN("The inifities in IEEE 754 double precision")
    {
        constexpr double native_pos_inf = std::numeric_limits<double>::infinity();
        constexpr double native_neg_inf = -std::numeric_limits<double>::infinity();

        WHEN("Creating aarith foating-point numbers with enough bits from the zero")
        {

            const Double dposinf(native_pos_inf);
            const Large lposinf(native_pos_inf);


            const Double dneginf(native_neg_inf);
            const Large lneginf(native_neg_inf);

            THEN("The result should be the correct infinity")
            {
                REQUIRE(dposinf.is_positive());
                REQUIRE(dposinf.is_pos_inf());
                REQUIRE(dposinf.is_inf());

                REQUIRE(dneginf.is_negative());
                REQUIRE(dneginf.is_neg_inf());
                REQUIRE(dneginf.is_inf());

                REQUIRE(lposinf.is_positive());
                REQUIRE(lposinf.is_pos_inf());
                REQUIRE(lposinf.is_inf());

                REQUIRE(lneginf.is_negative());
                REQUIRE(lneginf.is_neg_inf());
                REQUIRE(lneginf.is_inf());
            }
        }
        WHEN("Creating aarith foating-point numbers with enough bits from the zero")
        {
            THEN("The result should still be zero")
            {
                const Small smallposinf(native_pos_inf);
                const Single sposinf(native_pos_inf);


                const Small smallneginf(native_neg_inf);
                const Single sneginf(native_neg_inf);

                REQUIRE(smallposinf.is_positive());
                REQUIRE(smallposinf.is_pos_inf());
                REQUIRE(smallposinf.is_inf());

                REQUIRE(smallneginf.is_negative());
                REQUIRE(smallneginf.is_neg_inf());
                REQUIRE(smallneginf.is_inf());

                REQUIRE(sposinf.is_positive());
                REQUIRE(sposinf.is_pos_inf());
                REQUIRE(sposinf.is_inf());

                REQUIRE(sneginf.is_negative());
                REQUIRE(sneginf.is_neg_inf());
                REQUIRE(sneginf.is_inf());
            }
        }
    }
}

SCENARIO("Creating aarith floats from native negative zeros")
{
    using Single = normalized_float<8, 23>;
    using Double = normalized_float<11, 52>;
    using Large = normalized_float<20, 70>;
    using Small = normalized_float<3, 10>;

    GIVEN("Negative zero in IEEE 754 single precision")
    {
        constexpr float fnegzero = -0.0F;

        WHEN("Creating aarith foating-point numbers with enough bits from the zero")
        {
            const Single snegzero(fnegzero);
            const Double dnegzero(fnegzero);
            const Large lnegzero(fnegzero);
            THEN("The result should be negative zero")
            {
                REQUIRE(snegzero.is_negative());
                REQUIRE(snegzero.is_zero());

                REQUIRE(dnegzero.is_negative());
                REQUIRE(dnegzero.is_zero());

                REQUIRE(lnegzero.is_negative());
                REQUIRE(lnegzero.is_zero());
            }
        }
        WHEN("Creating aarith foating-point numbers with enough bits from the zero")
        {
            THEN("The result should still be zero")
            {
                const Small smallnegzero(fnegzero);

                REQUIRE(smallnegzero.is_negative());
                REQUIRE(smallnegzero.is_zero());
            }
        }
    }
    GIVEN("Negative zero in IEEE 754 double precision")
    {
        constexpr double dnegzero = -0.0;
        WHEN("Creating aarith foating-point numbers with enough bits from the zero")
        {
            const Double negzero(dnegzero);
            const Large lnegzero(dnegzero);

            THEN("The result should be negative zero")
            {
                REQUIRE(negzero.is_negative());
                REQUIRE(negzero.is_zero());

                REQUIRE(lnegzero.is_negative());
                REQUIRE(lnegzero.is_zero());
            }
        }
        WHEN("Creating aarith foating-point numbers with enough bits from the zero")
        {
            THEN("The result should still be zero")
            {
                const Small smallnegzero(dnegzero);
                const Single snegzero(dnegzero);

                REQUIRE(smallnegzero.is_negative());
                REQUIRE(smallnegzero.is_zero());

                REQUIRE(snegzero.is_negative());
                REQUIRE(snegzero.is_zero());
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Casting from and to the native data types should be lossless",
                       "[normalized_float][casting][utility]",
                       AARITH_FLOAT_TEST_SIGNATURE_WITH_NATIVE_TYPE,
                       AARIHT_FLOAT_TEMPLATE_NATIVE_RANGE_WITH_TYPE)
{
    using F = normalized_float<E, M>;

    Native native = GENERATE(take(30, random<Native>(std::numeric_limits<Native>::min(),
                                                     std::numeric_limits<Native>::max())));

    F from_native{native};

    Native casted_back = static_cast<Native>(from_native);

    REQUIRE(casted_back == native);
}

SCENARIO("Casting to a larger native data type does not change the value",
         "[normalized_float][casting][utility]")
{
    using F = normalized_float<8, 23>;
    GIVEN("A random floating point value")
    {

        float f = GENERATE(take(50, random<float>(std::numeric_limits<float>::min(),
                                                  std::numeric_limits<float>::max())));

        float neg_f = -f;

        WHEN("Casting it to a normalized_float<8,23>")
        {
            F f_cast{f};
            F f_neg_cast{neg_f};

            AND_THEN("Casting it to a double")
            {
                float d = static_cast<float>(f_cast);
                float neg_d = static_cast<float>(f_neg_cast);

                THEN("The value should not have changed")
                {
                    REQUIRE(f == d);
                    REQUIRE(neg_f == neg_d);
                    REQUIRE(-f == neg_d);
                    REQUIRE(-d == neg_d);
                    REQUIRE(-f == neg_f);
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Casting to a larger data type does not change the value",
                       "[normalized_float][casting][utility]",
                       AARITH_FLOAT_TEST_SIGNATURE_WITH_NATIVE_TYPE,
                       AARIHT_FLOAT_TEMPLATE_NATIVE_RANGE_WITH_TYPE)
{
    using F = normalized_float<E, M>;
    using G = normalized_float<E + 80, M>;
    using H = normalized_float<M, M + 80>;
    using J = normalized_float<E + 80, M + 80>;

    GIVEN("A random floating point value")
    {
        Native f_ = GENERATE(take(50, random<float>(std::numeric_limits<Native>::min(),
                                                    std::numeric_limits<Native>::max())));

        WHEN("Casting it to a normalized_float<8,23>")
        {
            F f{f_};

            AND_THEN("Casting it to a larger normalized float")
            {

                G g = static_cast<G>(f);
                H h = static_cast<H>(f);
                J j = static_cast<J>(f);

                THEN("The value should not have changed")
                {
                    CHECK(f == g);
                    CHECK(f == h);
                    CHECK(g == f);
                    CHECK(h == f);
                    CHECK(j == f);
                    REQUIRE(f == j);
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Infinity and NaNs are created correctly",
                       "[normalized_float][casting][utility]",
                       AARITH_FLOAT_TEST_SIGNATURE_WITH_NATIVE_TYPE,
                       AARIHT_FLOAT_TEMPLATE_NATIVE_RANGE_WITH_TYPE)
{
    using F = normalized_float<E, M>;

    Native nan_native = std::numeric_limits<Native>::quiet_NaN();
    Native pos_inf_native = std::numeric_limits<Native>::infinity();
    F nan{F::NaN()};
    F nan_from_native{nan_native};
    F pos_inf{F::pos_infinity()};

    Native pos_inf_casted = static_cast<Native>(pos_inf);

    REQUIRE(pos_inf_casted == pos_inf_native);

    REQUIRE(nan.is_nan());
    REQUIRE(nan_from_native.is_nan());
}