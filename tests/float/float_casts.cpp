#include <aarith/float.hpp>
#include <aarith/integer.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_float.hpp"

#include <bitset>
#include <catch.hpp>
using namespace aarith;

// TODO (keszocze) make it generic
SCENARIO("IEEE-754 arithmetic conversion: float, double",
         "[normalized_float][conversion][ieee-754][casting]")
{

    using F = normalized_float<8, 23>;

    GIVEN("A random number in the native format")
    {
        float a = GENERATE(
            take(50, random(std::numeric_limits<float>::min(), std::numeric_limits<float>::max())));

        WHEN("Creating an aarith floating-point from that number")
        {

            const F a_cast{a};

            AND_WHEN("Converting back to the native data format")
            {

                const float a_cast_back = static_cast<float>(a_cast);

                THEN("The number should not have changed")
                {
                    REQUIRE(a == a_cast_back);
                }
            }
        }
    }

    GIVEN("A random number in the aarith format")
    {
        F b = GENERATE(take(50, random_float<8, 23, FloatGenerationModes::NonSpecial>()));

        std::cout << to_binary(b, true) << "\n";

        WHEN("Casting the value to the native format")
        {
            const float b_cast = static_cast<float>(b);

            AND_WHEN("Calling the aarith floating-point constructor with the cast value")
            {
                const F b_cast_back{b_cast};
                THEN("The number should not have changed")
                {
                    if (b != b_cast_back)
                    {
                        std::cout << "considered unequal: "
                                  << "\n";
                        std::cout << to_binary(b, true) << "\n"
                                  << to_binary(b_cast_back, true) << "\n";
                        std::cout << b << "\n" << b_cast_back << "\n";
                        exit(0);
                    }
                    REQUIRE(b == b_cast_back);
                }
            }
        }
    }

    GIVEN("A float number")
    {
        WHEN("The number is 0.")
        {
            float a = 0.F;

            THEN("The normalized_float should convert back to the original number.")
            {
                REQUIRE(static_cast<float>(normalized_float<8, 23>(a)) == a);
            }
        }
    }
    GIVEN("A double number")
    {
        WHEN("The number is 0.")
        {
            double a = 0.;

            THEN("The normalized_float should convert back to the original number.")
            {
                REQUIRE(static_cast<double>(normalized_float<11, 52>(a)) == a);
            }
        }
    }
    GIVEN("A random float number")
    {
        float a = GENERATE(take(10, random(float(1.0), std::numeric_limits<float>::max())));

        THEN("The normalized_float should convert back to the original number.")
        {
            REQUIRE(static_cast<float>(normalized_float<8, 23>(a)) == a);
        }
    }
    GIVEN("A random double number")
    {
        double a = GENERATE(take(10, random(double(1.0), std::numeric_limits<double>::max())));

        THEN("The normalized_float should convert back to the original number.")
        {
            REQUIRE(static_cast<double>(normalized_float<11, 52>(a)) == a);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Casting from and to the native data types should be lossless",
                       "[normalized_float][casting][utility]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
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
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, float), (11, 52, double))
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
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
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