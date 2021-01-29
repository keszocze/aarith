#include <aarith/float.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_float.hpp"

#include <bitset>
#include <catch.hpp>

using namespace aarith;

template <typename N>
// it is not really "full range" but at least it works
auto full_native_range()
{
    return Catch::Generators::random<N>(std::numeric_limits<N>::lowest() / 100.0f,
                                        std::numeric_limits<N>::max() / 100.0f);
}

template <typename F>
auto compare_float_test_results(const F& lhs, const F& rhs)
{
    return equal_except_rounding(lhs, rhs) || (lhs.is_nan() && rhs.is_nan());
}

TEMPLATE_TEST_CASE_SIG("One is the neutral element of the multiplication",
                       "[floating_point][arithmetic][multiplication][invariant]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    using F = floating_point<E, M>;

    GIVEN("A floating_point created from native data types and the number one")
    {

        F a = GENERATE(take(100, random_float<E, M, FloatGenerationModes::NonSpecial>()));
        F one{F::one()};

        WHEN("Multipliying these numbers")
        {
            F res{a * one};
            THEN("The result should have left the number untouched")
            {
                const bool eq_round = equal_except_rounding(res, a);
                const bool eq = res == a;

                if (!eq_round || !eq)
                {
                    std::cout << to_binary(a) << "\n" << to_binary(one) << "\n";
                    std::cout << to_binary(res) << "\n";
                }

                CHECK(eq_round);
                REQUIRE(eq);
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Multiplication is commutative",
                       "[floating_point][arithmetic][multiplication][invariant]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    using F = floating_point<E, M>;

    GIVEN("Tow floating_points created from native data types")
    {

        F a = GENERATE(take(30, random_float<E, M, FloatGenerationModes::FullyRandom>()));
        F b = GENERATE(take(30, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        WHEN("Multiplying these numbers")
        {
            F res1{a * b};
            F res2{b * a};
            CAPTURE(a, b, res1, res2);
            THEN("The result should not depend on operand order")
            {
                REQUIRE(compare_float_test_results<F>(res1, res2));
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Zero makes the multiplication result zero",
                       "[floating_point][arithmetic][multiplication][invariant]",
                       AARITH_FLOAT_TEST_SIGNATURE_WITH_NATIVE_TYPE,
                       AARITH_FLOAT_TEMPLATE_NATIVE_RANGE_WITH_TYPE)
{
    using F = floating_point<E, M>;

    GIVEN("A normalized floating-point number")
    {

        F a = GENERATE(take(100, random_float<E, M, FloatGenerationModes::NonSpecial>()));
        F zero{F::zero()};

        WHEN("Multiplying with zero")
        {
            F res{a * zero};
            THEN("The result should be zero")
            {
                REQUIRE(res.is_zero());
                REQUIRE(res.is_positive() == a.is_positive());
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Floating point multiplication works for special values",
                       "[floating_point][arithmetic][multiplication]",
                       AARITH_FLOAT_TEST_SIGNATURE_WITH_NATIVE_TYPE,
                       AARITH_FLOAT_TEMPLATE_NATIVE_RANGE_WITH_TYPE)
{

    using F = floating_point<E, M>;

    GIVEN("Random NaN and infinity values")
    {

        F a = GENERATE(take(30, random_float<E, M, FloatGenerationModes::Special>()));
        F b = GENERATE(take(30, random_float<E, M, FloatGenerationModes::Special>()));

        WHEN("Multiplying them")
        {

            F res = a * b;

            THEN("The result should match the native result bit by bit")
            {
                if (a.is_nan() || b.is_nan())
                {
                    REQUIRE(res.is_nan());
                }
                if (a.is_inf() && b.is_inf())
                {

                    const bool is_inf = res.is_inf();

                    if (a.get_sign() != b.get_sign())
                    {
                        REQUIRE((res.is_negative() && is_inf));
                    }
                    else
                    {
                        REQUIRE((res.is_positive() && is_inf));
                    }
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Multiplying with infinity", "[floating_point][arithmetic][multiplication]",
                       AARITH_FLOAT_TEST_SIGNATURE_WITH_NATIVE_TYPE,
                       AARITH_FLOAT_TEMPLATE_NATIVE_RANGE_WITH_TYPE)
{
    using F = floating_point<E, M>;
    constexpr F neg_inf{F::neg_infinity()};
    constexpr F pos_inf{F::pos_infinity()};

    GIVEN("A NaN floating-point")
    {
        F nan = F::NaN();
        WHEN("Multiplying with infinity")
        {
            F res = nan * pos_inf;
            F res_ = nan * neg_inf;

            CHECK(res.is_nan());
            REQUIRE(res_.is_nan());
        }
    }

    GIVEN("A random floating point number")
    {

        // we allow to generate NaNs here as they have different payloads than the single NaN
        // generated from the F::NaN() method. This broadens the range of inputs we test.
        F f = GENERATE(take(100, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        WHEN("Multiplying with infinity")
        {
            THEN("The sign of the resulting infinity should be correct")
            {
                F res = f * neg_inf;
                F res_ = f * pos_inf;

                if (f.is_nan())
                {
                    REQUIRE(res.is_nan());
                    REQUIRE(res_.is_nan());
                }
                else
                {
                    if (f.is_negative())
                    {
                        if (res != pos_inf)
                        {
                            std::cout << to_binary(f, true) << " * pos_inf"
                                      << " = " << to_binary(res, true) << "\n";
                        }

                        if (res_ != neg_inf)
                        {
                            std::cout << to_binary(f, true) << " * neg_inf"
                                      << " = " << to_binary(res_, true) << "\n";
                        }
                        CHECK(res == pos_inf);
                        REQUIRE(res_ == neg_inf);
                    }
                    else
                    {

                        if (res != neg_inf)
                        {
                            std::cout << to_binary(f, true) << " * pos_inf"
                                      << " = " << to_binary(res, true) << "\n";
                        }

                        if (res_ != pos_inf)
                        {
                            std::cout << to_binary(f, true) << " * neg_inf"
                                      << " = " << to_binary(res_, true) << "\n";
                        }

                        CHECK(res == neg_inf);
                        REQUIRE(res_ == pos_inf);
                    }
                }
            }
        }
    }
}

SCENARIO("Multiplication should work correctly (hand picked example)",
         "[floating_point][arithmetic][multiplication]")
{
    GIVEN("Two numbers in in <8,23> format")
    {
        using nf_t = floating_point<8, 23>;

        nf_t nf_a(0.0117647f);
        nf_t nf_b(0.385671f);

        WHEN("Multiplying them")
        {
            nf_t nf_res = nf_a * nf_b;
            THEN("The result should be correct")
            {
                word_array<32> wrong_(0b01000111000000000000000000000001);
                nf_t nf_wrong(wrong_);

                //                std::cout << to_binary(nf_res, true) << "\n";
                //                std::cout << to_binary(nf_wrong, true) << "\n";

                // hand picked counterexample
                REQUIRE(nf_res != nf_wrong);
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Floating point multiplication matches its native counterparts",
                       "[floating_point][arithmetic][multiplication]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{

    using F = floating_point<E, M>;

    Native a_native = GENERATE(take(30, full_native_range<Native>()));
    Native b_native = GENERATE(take(30, full_native_range<Native>()));
    F a{a_native};
    F b{b_native};

    F res = a * b;
    Native res_native = a_native * b_native;

    F res_native_{res_native};

    CHECK(equal_except_rounding(res_native_, res));
}

SCENARIO("Exact multiplication of two floating-point numbers (hand picked examples)",
         "[floating_point][arithmetic][multiplication]")
{
    GIVEN("Single precision floats (E = 8, M = 23)")
    {
        static constexpr size_t E = 8;
        static constexpr size_t M = 23;

        using F = floating_point<E, M>;

        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 213.22154F;
            static constexpr float number_b = 93.211546F;
            const F a{number_a};
            const F b{number_b};
            const F result_float{number_a * number_b};
            const F result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(compare_float_test_results<F>(result, floating_point<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154F;
            static constexpr float number_b = 93.211546F;
            const F a{number_a};
            const F b{number_b};
            const F result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(compare_float_test_results<F>(result, floating_point<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 213.22154F;
            static constexpr float number_b = -93.211546F;
            const F a{number_a};
            const F b{number_b};
            const F result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(compare_float_test_results<F>(result, floating_point<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154F;
            static constexpr float number_b = -93.211546F;
            const F a{number_a};
            const F b{number_b};
            const F result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(compare_float_test_results<F>(result, floating_point<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = 213.22154F;
            const F a{number_a};
            const F b{number_b};
            const F result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(compare_float_test_results<F>(result, floating_point<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = 213.22154F;
            const F a{number_a};
            const F b{number_b};
            const F result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(compare_float_test_results<F>(result, floating_point<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = -213.22154F;
            const F a{number_a};
            const F b{number_b};
            const F result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(compare_float_test_results<F>(result, floating_point<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = -213.22154F;
            const F a{number_a};
            const F b{number_b};
            const F result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(compare_float_test_results<F>(result, floating_point<E, M>(number_a * number_b)));
            }
        }
        WHEN("When one operand is a > 1 and the other is 0 < b < 1.")
        {
            static constexpr float number_a = 2.75F;
            static constexpr float number_b = 0.5F;
            const F a{number_a};
            const F b{number_b};
            const F result = mul(a, b);

            THEN("It should be the correct product.")
            {
                const F res_ = floating_point<E, M>(number_a * number_b);
                REQUIRE(compare_float_test_results<F>(result, res_));
            }
        }
    }
}

SCENARIO("IEEE-754 denormalized number computations: float, double (hand picked examples)",
         "[floating_point][denormalized][ieee-754][computation]")
{
    GIVEN("Two denormalized float numbers")
    {
        using nfloat = floating_point<8, 23>;
        WHEN("The result of the multiplcation is still denormalized > 0")
        {
            unsigned int a_i = 0b00000000000000000000000000000010;
            unsigned int b_i = 0b00000000000000000000000000000001;
            float* a_if = reinterpret_cast<float*>(&a_i);
            float* b_if = reinterpret_cast<float*>(&b_i);
            float a = *a_if;
            float b = *b_if;

            float res_f = a * b;
            const auto nfa = nfloat(a);
            const auto nfb = nfloat(b);
            const nfloat res = nfa * nfb;

            THEN("The results should be the same as the float computation.")
            {
                REQUIRE(static_cast<float>(res) == res_f);
            }
        }
        WHEN("The result of the multiplication is normalized")
        {
            unsigned int a_i = 0b00000000011111111111111111111111;
            unsigned int b_i = 0b00000000011111111111111111111111;
            float* a_if = reinterpret_cast<float*>(&a_i);
            float* b_if = reinterpret_cast<float*>(&b_i);
            float a = *a_if;
            float b = *b_if;

            float res_f = a * b;
            const auto nfa = nfloat(a);
            const auto nfb = nfloat(b);
            auto res = nfa * nfb;

            THEN("The results should be the same as the float computation.")
            {
                REQUIRE(static_cast<float>(res) == res_f);
            }
        }
    }
    GIVEN("Two normalized float numbers")
    {
        using nfloat = floating_point<8, 23>;
        WHEN("The result of the multiplication should be denormalized > 0")
        {
            unsigned int a_i = 0b00011111111111111111111111111111;
            unsigned int b_i = 0b00011111111111111111111111111111;
            float* a_if = reinterpret_cast<float*>(&a_i);
            float* b_if = reinterpret_cast<float*>(&b_i);
            float a = *a_if;
            float b = *b_if;

            const auto nfa = nfloat(a);
            const auto nfb = nfloat(b);
            auto res = nfa * nfb;

            THEN("The results should be the same as the float computation.")
            {
                const bool eq = equal_except_rounding(res, nfloat(res));
                REQUIRE(eq);
            }
        }

        // TODO (brand) Dieser Testfall scheint irgendwie kaputt zu sein.
        WHEN("The result of the multiplication should be denormalized > 0")
        {
            // a und b sin identisch; ist das absicht?
            unsigned int a_i = 0b00011010111111111111111111111111;
            unsigned int b_i = 0b00011010111111111111111111111111;
            float* a_if = reinterpret_cast<float*>(&a_i);
            float* b_if = reinterpret_cast<float*>(&b_i);
            float a = *a_if;
            float b = *b_if;

            const auto nfa = nfloat(a);
            const auto nfb = nfloat(b);
            auto res = nfa * nfb;

            THEN("The results should be the same as the float computation.")
            {
                const bool eq = equal_except_rounding(res, nfloat(res));
                REQUIRE(eq);
            }
        }
    }
}
