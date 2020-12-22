#include "../test-signature-ranges.hpp"

#include <aarith/float.hpp>
#include <bitset>
#include <catch.hpp>
#include <cmath>
using namespace aarith;

template <typename N>
// it is not really "full range" but at least it works
auto full_native_range()
{
    return Catch::Generators::random<N>(std::numeric_limits<N>::lowest() / 100.0f,
                                        std::numeric_limits<N>::max() / 100.0f);
}

TEMPLATE_TEST_CASE_SIG("Constructing larger normalized floats from smaller ones",
                       "[floating_point][casting][constructor]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, float), (11, 52, double))
{
    using F = floating_point<E, M>;
    using G = floating_point<E + 80, M>;
    using H = floating_point<M, M + 80>;
    using J = floating_point<E + 80, M + 80>;

    GIVEN("A random floating point value")
    {
        Native f_ = GENERATE(take(50, full_native_range<Native>()));

        WHEN("Casting it to a floating_point<8,23>")
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

TEMPLATE_TEST_CASE_SIG("Creating constant values is a constexpr",
                       "[floating_point][constant][constexpr]", ((size_t E, size_t M), E, M),
                       (8, 23), (11, 52), (3, 3), (80, 80))
{
    using F = floating_point<E, M>;

    WHEN("Creating some constant values")
    {
        THEN("This should be possible as constexpr")
        {

            constexpr F zero{F::zero()};
            constexpr F pos_inf{F::pos_infinity()};
            constexpr F neg_inf{F::neg_infinity()};
            constexpr F nan{F::NaN()};
            constexpr F one{F::one()};

            // the tests are only there to ensure that the compiler does not complain about
            // unused variables
            CHECK(zero != pos_inf);
            CHECK(zero != neg_inf);
            CHECK(zero != nan);
            CHECK(zero < one);
            REQUIRE(nan.is_nan());
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Width-casting special values into larger normalized floats",
                       "[floating_point][casting][constructor]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, float), (11, 52, double))
{
    using F = floating_point<E, M>;
    using G = floating_point<E + 80, M>;
    using H = floating_point<M, M + 80>;
    using J = floating_point<E + 80, M + 80>;

    GIVEN("Infinity")
    {
        Native inf_ = std::numeric_limits<Native>::infinity();

        WHEN("Casting it to a floating_point<8,23>")
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
        WHEN("Casting it to a floating_point<8,23>")
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

TEMPLATE_TEST_CASE_SIG("Investigating the minimal exponent",
                       "[floating_point][invariant][constructor]", AARITH_FLOAT_TEST_SIGNATURE,
                       AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = floating_point<E, M>;
    using Exp = typename F::IntegerUnbiasedExp;

    GIVEN("A floating-point number")
    {
        THEN("Its minimal exponent is larger than -bias")
        {

            Exp b{F::bias};
            b = negate(b);
            constexpr Exp min_exp{F::min_exp};

            REQUIRE(min_exp > b);
        }
    }
}