#include <aarith/float.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_float.hpp"

#include <bitset>
#include <catch.hpp>

using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Addition is commutative",
                       "[floating_point][arithmetic][addition][invariant]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARITH_FLOAT_TEMPLATE_RANGE)
{
    using F = floating_point<E, M>;

    GIVEN("Two random floating-point numbers")
    {

        F a = GENERATE(take(80, random_float<E, M, FloatGenerationModes::FullyRandom>()));
        F b = GENERATE(take(80, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        WHEN("Adding these numbers")
        {
            F res1{a + b};
            F res2{b + a};
            THEN("The result should not depend on operand order")
            {
                // NaNs should simply propagate (see below)
                if (!a.is_nan() && !b.is_nan())
                {

                    // Adding infinities of different sign is another special case
                    if (a.is_inf() && b.is_inf() && (a.is_negative() != b.is_negative()))
                    {
                        CHECK(bit_equal(res1, res2));
                        CHECK(res1.is_nan());
                        REQUIRE(res2.is_nan());
                    }
                    else
                    {
                        const bool pass_test = res1 == res2 || (res1.is_inf() && res2.is_inf()) ||
                                               bit_equal(res1, res2);
                        if (!pass_test)
                        {
                            std::cout << to_binary(a) << " + \n"
                                      << to_binary(b) << " = \n"
                                      << "res1: " << to_binary(res1) << "\n"
                                      << "res2: " << to_binary(res2) << "\n";
                            auto dbg1 = a + b;
                            auto dbg2 = b + a;
                            std::cout << "dbg1: " << to_binary(dbg1) << "\n"
                                      << "dbg2: " << to_binary(dbg2) << "\n";
                        }
                        REQUIRE(pass_test);
                    }
                }
                else
                {
                    CHECK(res1.is_nan());
                    REQUIRE(res2.is_nan());
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Zero is the neutral element of the addition",
                       "[floating_point][arithmetic][addition][invariant]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARITH_FLOAT_TEMPLATE_RANGE)
{
    using F = floating_point<E, M>;

    GIVEN("A floating_point  created from native data types and the number zero")
    {

        F a = GENERATE(take(15, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        WHEN("Adding these numbers")
        {
            F res{a + F::zero()};
            THEN("The result should have left the number untouched")
            {
                if (!a.is_nan())
                {
                    if (res != a)
                    {
                        std::cout << "a:   " << to_binary(a) << "\n"
                                  << "res: " << to_binary(res) << "\n";
                    }
                    REQUIRE(res == a);
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Floating point addition works for special values",
                       "[floating_point][arithmetic][addition]",
                       AARITH_FLOAT_TEST_SIGNATURE_WITH_NATIVE_TYPE,
                       AARITH_FLOAT_TEMPLATE_NATIVE_RANGE_WITH_TYPE)
{

    using F = floating_point<E, M>;

    GIVEN("Random NaN and infinity values")
    {

        F a = GENERATE(take(30, random_float<E, M, FloatGenerationModes::Special>()));
        F b = GENERATE(take(30, random_float<E, M, FloatGenerationModes::Special>()));

        WHEN("Adding them")
        {

            F res = a + b;

            THEN("The special cases should be respected")
            {

                if (a.is_nan() || b.is_nan())
                {
                    REQUIRE(res.is_nan());
                }
                if (a.is_inf() && b.is_inf())
                {
                    if (a.get_sign() != b.get_sign())
                    {
                        REQUIRE(res.is_nan());
                    }
                    else
                    {
                        const bool same_sign = res.get_sign() == a.get_sign();
                        const bool result = res.is_inf() && same_sign;
                        REQUIRE(result);
                    }
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Floating point addition matches its native counterparts",
                       "[floating_point][arithmetic][addition][foo]",
                       AARITH_FLOAT_TEST_SIGNATURE_WITH_NATIVE_TYPE,
                       AARITH_FLOAT_TEMPLATE_NATIVE_RANGE_WITH_TYPE)
{

    using F = floating_point<E, M>;

    F a = GENERATE(take(150, random_float<E, M, FloatGenerationModes::FullyRandom>()));
    F b = GENERATE(take(150, random_float<E, M, FloatGenerationModes::FullyRandom>()));

    Native a_native = static_cast<Native>(a);
    Native b_native = static_cast<Native>(b);

    F res = a + b;
    Native res_native = a_native + b_native;

    F res_native_{res_native};
    Native res_ = static_cast<Native>(res);

    if (a.is_nan() || b.is_nan())
    {
        CAPTURE(res_, res_native, F{res_}, F{res_native}, res.is_nan(), res_native_.is_nan());
        CHECK(res.is_nan());
        REQUIRE(res_native_.is_nan());
    }
    else
    {
        CAPTURE(a, to_binary(a), a_native, b, to_binary(b), b_native, a.is_nan(), b.is_nan(),
                a.is_inf(), b.is_inf(), res, res_native, res_);
        CHECK(equal_except_rounding(res_native_, res));
        REQUIRE(equal_except_rounding(F{res_}, F{res_native}));
    }
}

TEMPLATE_TEST_CASE_SIG("Adding to infinity", "[floating_point][arithmetic][addition]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARITH_FLOAT_TEMPLATE_RANGE)
{
    using F = floating_point<E, M>;
    constexpr F neg_inf{F::neg_infinity()};
    constexpr F pos_inf{F::pos_infinity()};

    GIVEN("Positive infinity")
    {
        WHEN("Adding infinity to infinity")
        {
            F res = pos_inf + pos_inf;
            THEN("The result should still be infinity")
            {
                CHECK(res == pos_inf);
                CHECK(res.is_pos_inf());
                REQUIRE(res.is_inf());
            }
        }
    }
    GIVEN("Negative infinity")
    {
        WHEN("Adding negative infinity")
        {
            F res = neg_inf + neg_inf;
            THEN("The result should still be negative infinity")
            {
                CHECK(res == neg_inf);
                CHECK(res.is_neg_inf());
                REQUIRE(res.is_inf());
            }
        }
    }
    GIVEN("Both infinities")
    {
        WHEN("Adding them")
        {
            F res1 = pos_inf + neg_inf;
            F res2 = neg_inf + pos_inf;

            THEN("The result should be NaN")
            {
                CHECK(res1.is_nan());
                REQUIRE(res2.is_nan());
            }
        }
    }
    AND_GIVEN("A random floating point number")
    {
        F f = GENERATE(take(100, random_float<E, M, FloatGenerationModes::NonSpecial>()));

        WHEN("Adding that number to infinity")
        {
            F res = pos_inf + f;
            F res_neg = neg_inf + f;
            THEN("The value should still be infinity")
            {
                CHECK(res_neg == neg_inf);
                CHECK(res_neg.is_inf());
                CHECK(res_neg.is_neg_inf());
                CHECK(res == pos_inf);
                CHECK(res.is_pos_inf());
                REQUIRE(res.is_inf());
            }
        }
    }
}

SCENARIO("Adding two floating-point numbers", "[floating_point][arithmetic][addition]")
{
    GIVEN("Single precision floats (E = 8, M = 23)")
    {
        static constexpr size_t E = 8;
        static constexpr size_t M = 23;

        WHEN("The second operand is -a.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = -number_a;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("The result should be 0")
            {
                REQUIRE(result == floating_point<E, M>(0.f));
            }
        }
        WHEN("The second operand is 0.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = 0;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("The result should be a")
            {
                REQUIRE(result == a);
            }
        }
        WHEN("The first operand is negative, and the second operand is 0.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = 0;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("The result should be a")
            {
                REQUIRE(result == a);
            }
        }
        WHEN("The first operand is 0.")
        {
            static constexpr float number_a = 0;
            static constexpr float number_b = 93.211546F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("The result should be b")
            {
                REQUIRE(result == b);
            }
        }
        WHEN("The first operand is 0, and the second operand is negative.")
        {
            static constexpr float number_a = 0;
            static constexpr float number_b = -93.211546F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("The result should be b")
            {
                REQUIRE(result == b);
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 213.22154F;
            static constexpr float number_b = 93.211546F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154F;
            static constexpr float number_b = 93.211546F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 213.22154F;
            static constexpr float number_b = -93.211546F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154F;
            static constexpr float number_b = -93.211546F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = 213.22154F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = 213.22154F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = -213.22154F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = -213.22154F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
        WHEN("When one operand is a > 1 and the other is 0 < b < 1.")
        {
            static constexpr float number_a = 2.75F;
            static constexpr float number_b = 0.5F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
    }
}

SCENARIO("IEEE-754 denormalized number computations: float, double",
         "[floating_point][denormalized][ieee-754][computation]")
{
    GIVEN("Two denormalized float numbers")
    {
        using nfloat = floating_point<8, 23>;
        WHEN("The result of the addition is still denormalized > 0")
        {
            unsigned int a_i = 0b00000000000000000000000000000010;
            unsigned int b_i = 0b00000000000000000000000000000001;
            float* a_if = reinterpret_cast<float*>(&a_i);
            float* b_if = reinterpret_cast<float*>(&b_i);
            float a = *a_if;
            float b = *b_if;

            float res_f = a + b;
            const auto nfa = nfloat(a);
            const auto nfb = nfloat(b);
            auto res = nfa + nfb;

            THEN("The results should be the same as the float computation.")
            {
                REQUIRE(static_cast<float>(res) == res_f);
            }
        }
        WHEN("The result of the addition is normalized")
        {
            unsigned int a_i = 0b00000000011111111111111111111111;
            unsigned int b_i = 0b00000000011111111111111111111111;
            float* a_if = reinterpret_cast<float*>(&a_i);
            float* b_if = reinterpret_cast<float*>(&b_i);
            float a = *a_if;
            float b = *b_if;
            auto const a_nf = nfloat(a);
            auto const b_nf = nfloat(b);

            float res_f = a + b;
            auto res = a_nf + b_nf;

            THEN("The results should be the same as the float computation.")
            {
                REQUIRE(static_cast<float>(res) == res_f);
            }
        }
    }
}
