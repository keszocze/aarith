#include <aarith/float.hpp>
#include <bitset>
#include <catch.hpp>
#include <cmath>
using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Constructing larger normalized floats from smaller ones",
                       "[normalized_float][casting][constructor]",
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

            AND_THEN("Constructing a larger normalized float from it")
            {

                G g{f};
                H h{f};
                J j{f};

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

TEMPLATE_TEST_CASE_SIG("Width-casting special values into larger normalized floats",
                       "[normalized_float][casting][constructor]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, float), (11, 52, double))
{
    using F = normalized_float<E, M>;
    using G = normalized_float<E + 80, M>;
    using H = normalized_float<M, M + 80>;
    using J = normalized_float<E + 80, M + 80>;

    GIVEN("Infinity")
    {
        Native inf_ = std::numeric_limits<Native>::infinity();

        WHEN("Casting it to a normalized_float<8,23>")
        {
            F inf{inf_};

            AND_THEN("Constructing a larger normalized float from it")
            {

                G g{inf};
                H h{inf};
                J j{inf};

                THEN("The value should not have changed")
                {
                    CHECK(inf == g);
                    CHECK(inf == h);
                    CHECK(g == inf);
                    CHECK(h == inf);
                    CHECK(j == inf);
                    REQUIRE(inf == j);
                }
                AND_THEN("It should be infinity")
                {
                    CHECK(inf.is_inf());
                    CHECK(g.is_inf());
                    CHECK(h.is_inf());
                    REQUIRE(j.is_inf());
                }
            }
        }
    }
    GIVEN("NaN")
    {
        WHEN("Casting it to a normalized_float<8,23>")
        {
            F nan{std::nanf("")};

            AND_THEN("Constructing a larger normalized float from it")
            {

                G g{nan};
                H h{nan};
                J j{nan};

                THEN("It should still be NaN")
                {
                    CHECK(nan.is_nan());
                    CHECK(g.is_nan());
                    CHECK(h.is_nan());
                    REQUIRE(j.is_nan());
                }
            }
        }
    }
}