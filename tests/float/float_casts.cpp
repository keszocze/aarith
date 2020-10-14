#include <aarith/float.hpp>
#include <bitset>
#include <catch.hpp>
using namespace aarith;

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