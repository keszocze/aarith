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

        WHEN("Casting it to a normalized_float<8,23>")
        {
            F f_cast{f};

            AND_THEN("Casting it to a double")
            {
                double d = static_cast<double>(f_cast);

                THEN("The value should not have changed")
                {
                    REQUIRE(f == d);
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