#include <aarith/float.hpp>

#include "gen_float.hpp"
#include "../test-signature-ranges.hpp"

#include <bitset>
#include <catch.hpp>


using namespace aarith;


TEMPLATE_TEST_CASE_SIG("Dividing by infinity", "[normalized_float][arithmetic][division]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARIHT_FLOAT_TEMPLATE_RANGE)
{
    // I haven't found easily readable official documents that motivate these test cases. They are
    // based on the following web sites:
    // https://wiki.analytica.com/index.php/INF,_Nan,_Zero_and_IEEE/SANE_arithmetic
    // https://stackoverflow.com/questions/42926763/the-behaviour-of-floating-point-division-by-zero
    //      and the reference to Annex F

    GIVEN("A random floating point number")
    {

        using F = normalized_float<E, M>;

        F f = GENERATE(take(100, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        constexpr F neg_inf{F::neg_infinity()};
        constexpr F pos_inf{F::pos_infinity()};
        constexpr F pos_zero{F::zero()};
        constexpr F neg_zero{F::neg_zero()};

        WHEN("Dividing by infinity")
        {
            THEN("The sign of the resulting zero should be correct")
            {
                F res = f / neg_inf;
                F res_ = f / pos_inf;

                if (f.is_negative())
                {
                    CHECK(res == pos_zero);
                    REQUIRE(res_ == neg_zero);
                }
                else
                {
                    CHECK(res == neg_zero);
                    REQUIRE(res_ == pos_zero);
                }
            }
        }
    }
}


TEMPLATE_TEST_CASE_SIG("Dividing by zero", "[normalized_float][arithmetic][division]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARIHT_FLOAT_TEMPLATE_RANGE)
{
    // I haven't found easily readable official documents that motivate these test cases. They are
    // based on the following web sites:
    // https://wiki.analytica.com/index.php/INF,_Nan,_Zero_and_IEEE/SANE_arithmetic
    // https://stackoverflow.com/questions/42926763/the-behaviour-of-floating-point-division-by-zero
    //      and the reference to Annex F

    GIVEN("A random floating point number")
    {

        using F = normalized_float<E, M>;

        F f = GENERATE(take(100, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        constexpr F neg_inf{F::neg_infinity()};
        constexpr F pos_inf{F::pos_infinity()};
        constexpr F pos_zero{F::zero()};
        constexpr F neg_zero{F::neg_zero()};
        WHEN("Dividing by infinity")
        {
            THEN("The sign of the resulting zero should be correct")
            {

                if (!f.is_zero())
                {

                    F res = f / neg_zero;
                    F res_ = f / pos_zero;

                    if (f.is_negative())
                    {
                        CHECK(res == pos_inf);
                        REQUIRE(res_ == neg_inf);
                    }
                    else
                    {
                        CHECK(res == neg_inf);
                        REQUIRE(res_ == pos_inf);
                    }
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Floating point division matches its native counterparts",
                       "[normalized_float][arithmetic][division]",
                       AARITH_FLOAT_TEST_SIGNATURE_WITH_NATIVE_TYPE, AARIHT_FLOAT_TEMPLATE_NATIVE_RANGE_WITH_TYPE)
{

    using F = normalized_float<E, M>;

    F a = GENERATE(take(100, random_float<E, M, FloatGenerationModes::FullyRandom>()));
    F b = GENERATE(take(100, random_float<E, M, FloatGenerationModes::FullyRandom>()));


    Native a_native = static_cast<Native>(a);
    Native b_native = static_cast<Native>(b);

    F res = a / b;
    Native res_native = a_native / b_native;

    F res_native_{res_native};
    Native res_ = static_cast<Native>(res);

    if (!equal_except_rounding(res_native_, res))
    {
        std::cout
            << "a (aarith): " << to_binary(a) << "\n"
            << "a (native): " << to_binary(F(a)) << "\n"
            << "b (aarith): " << to_binary(b) << "\n"
            << "b (native): " << to_binary(F(b)) << "\n"
            << "res (aarith): " << to_binary(res) << "\n"
            << "res (native): " << to_binary(F(res_native)) << "\n";
            
        F res2 = a / b;
        Native res_native2 = a_native / b_native;

        F res_native2_ {res_native2};
        //Native res2_ = static_cast<Native>(res2);

        std::cout
            << "a (aarith): " << to_binary(a) << "\n"
            << "a (native): " << to_binary(F(a)) << "\n"
            << "b (aarith): " << to_binary(b) << "\n"
            << "b (native): " << to_binary(F(b)) << "\n"
            << "res (aarith): " << to_binary(res2) << "\n"
            << "res (native): " << to_binary(F(res_native2)) << "\n";
    }

    CHECK(equal_except_rounding(res_native_, res));
    REQUIRE(equal_except_rounding(F{res_}, F{res_native}));
}

SCENARIO("Manual test case for  floating point division",
         "[normalized_float][arithmetic][division][bar]")
{
    using F = normalized_float<8, 23>;
    GIVEN("The number one and the smallest normalised number")
    {
        F one{F::one()};
        AND_GIVEN("The smallest normalised number sn")
        {
            F smallest{F::smallest_normalized()};

            WHEN("Dividing one by sn")
            {
                THEN("It should not throw an exception")
                {
                    REQUIRE_NOTHROW(div(one, smallest));
                }
                THEN("The resulting number should be larger than one")
                {
                    REQUIRE(F::one() < div(one, smallest));
                }
            }
        }
        AND_GIVEN("The smallest denormalized number")
        {
            F smallest{F::smallest_denormalized()};
//            F res = div(F::one(), smallest);
//            std::cout << to_binary(smallest) << "\t" << to_binary(smallest, true) << "\t"
//                      << smallest << "\n";
//            std::cout << to_binary(res) << "\t" << to_binary(res, true) << "\t" << res << "\n";

            WHEN("Dividing one by the smallest denormalized number")
            {
                THEN("It should not throw an exception")
                {
                    REQUIRE_NOTHROW(div(one, smallest));
                }
                THEN("The resulting number should be larger than one")
                {
                    // TODO turn this test back on
                    REQUIRE(F::one() < div(one, smallest));
                }
            }
        }
    }
}


SCENARIO("Division of two floating-point numbers (hand picked examples)", "[normalized_float][arithmetic][division]")
{
    GIVEN("Single precision floats (E = 8, M = 23)")
    {
        static constexpr size_t E = 8;
        static constexpr size_t M = 23;

        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 213.22154F;
            static constexpr float number_b = 93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154F;
            static constexpr float number_b = 93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 213.22154F;
            static constexpr float number_b = -93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154F;
            static constexpr float number_b = -93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = 213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = 213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = -213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = -213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a / number_b)));
            }
        }
    }
}
