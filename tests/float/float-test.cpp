#include "../test-signature-ranges.hpp"

#include <aarith/float.hpp>
#include <bitset>
#include <catch.hpp>
#include <cmath>
using namespace aarith; // NO-LINT

template <typename N>
// it is not really "full range" but at least it works
auto full_native_range()
{
    return Catch::Generators::random<N>(std::numeric_limits<N>::lowest() / 100.0F,
                                        std::numeric_limits<N>::max() / 100.0F);
}

TEMPLATE_TEST_CASE_SIG("Constructing larger normalized floats from smaller ones",
                       "[floating_point][casting][constructor][foo]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, float), (11, 52, double))
{
    static constexpr uint delta = 80;
    using F = floating_point<E, M>;
    using G = floating_point<E + delta, M>;
    using H = floating_point<M, M + delta>;
    using J = floating_point<E + delta, M + delta>;

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
                    CAPTURE(f, to_binary(f), g, to_binary(g), h, to_binary(h), j, to_binary(j));
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

TEMPLATE_TEST_CASE_SIG("Width-casting special values into larger floats",
                       "[floating_point][casting][constructor]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, float), (11, 52, double))
{

    static constexpr uint delta = 80;
    using F = floating_point<E, M>;
    using G = floating_point<E + delta, M>;
    using H = floating_point<M, M + delta>;
    using J = floating_point<E + delta, M + delta>;

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

                    CAPTURE(nan, g, h, j);

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
                       AARITH_FLOAT_TEMPLATE_RANGE)
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

SCENARIO("Creating a floating point value from a bitstring")
{
    using namespace std::string_literals;
    // to have the s suffix for strings available
    GIVEN("a random single precision number x")
    {
        float x = GENERATE(
            take(50, random(std::numeric_limits<float>::min(), std::numeric_limits<float>::max())));
        WHEN("Creating an aarith floating point value from the bit representation")
        {

            static constexpr uint float_width = 32;
            std::string xstring =
                std::bitset<float_width>(aarith::bit_cast<uint32_t>(x)).to_string(); // NO-LINT
            //            std::cout << "xstring= " << xstring << "\n";
            aarith::single_precision fpx{x};
            aarith::single_precision fpb{xstring};

            //            std::cout << "fpx= " << aarith::to_binary(fpx) << "\n";
            //            std::cout << "fpb= " << aarith::to_binary(fpb) << "\n";

            THEN("The value should match the one from the constructor taking a float value")
            {

                REQUIRE(fpx == fpb);
            }
        }
    }

    GIVEN("A hand-written test case")
    {
        THEN("Everything should work smoothly")
        {

            static const std::string s = "1"s + "01111111"s + "11000000000000000000000"s;

            static const aarith::single_precision a{s};
            static constexpr float val = -1.75;
            REQUIRE(a == aarith::single_precision{val});
        }
    }

    GIVEN("An invalid bit string")
    {

        WHEN("The string just doesn't make sense")
        {
            static const std::string s = "A"s + "01111111"s + "1100000000000000asdasda0000000"s;
            THEN("An exception should be thrown")
            {
                REQUIRE_THROWS(aarith::single_precision{s});
            }
        }
        WHEN("The string is too long or to short ")
        {
            THEN("The corresponding exception should be thrown")
            {
                static const std::string s1 = "A"s + "01111111"s + "11short"s;
                static const std::string s2 =
                    "A"s + "01111111"s + "110verymuchtooolongtoactuallycontainafloatingpointvalue"s;
                REQUIRE_THROWS_WITH(
                    aarith::single_precision{s1},
                    Catch::Matchers::Contains(
                        "oes not match the number of bits in the aarith::floating_point"));
                REQUIRE_THROWS_WITH(
                    aarith::single_precision{s2},
                    Catch::Matchers::Contains(
                        "oes not match the number of bits in the aarith::floating_point"));
            }
        }

        WHEN("The sign bit makes no sense")
        {
            static const std::string s = "A"s + "01111111"s + "11000000000000000000000"s;
            THEN("A corresponding exception should be thrown")
            {
                REQUIRE_THROWS_WITH(
                    aarith::single_precision{s},
                    Catch::Matchers::Contains("Unexpected character at sign position: "));
            }
        }
        WHEN("The exponent bits are invalid")
        {
            static const std::string s = "1"s + "0NOPE111"s + "11000000000000000000000"s;
            THEN("A corresponding exception should be thrown")
            {
                REQUIRE_THROWS_WITH(aarith::single_precision{s},
                                    Catch::Matchers::Contains("Unexpected character in exponent:"));
            }
        }

        WHEN("The mantissa bits are invalid")
        {
            static const std::string s = "1"s + "01111111"s + "110000000000NOPE0000001"s;
            THEN("A corresponding exception should be thrown")
            {
                REQUIRE_THROWS_WITH(aarith::single_precision{s},
                                    Catch::Matchers::Contains("Unexpected character in mantissa:"));
            }
        }
    }

    // TODO Add testcase for constexpr'nes
}