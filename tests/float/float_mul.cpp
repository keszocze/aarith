
#include <aarith/float.hpp>
#include <bitset>
#include <catch.hpp>

#include "../test-signature-ranges.hpp"
using namespace aarith;

template <typename N>
// it is not really "full range" but at least it works
auto full_native_range()
{
    return Catch::Generators::random<N>(std::numeric_limits<N>::lowest() / 100.0f,
                                        std::numeric_limits<N>::max() / 100.0f);
}


TEMPLATE_TEST_CASE_SIG("One is the neutral element of the multiplication",
                       "[normalized_float][arithmetic][multiplication][invariant]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    using F = normalized_float<E, M>;

    GIVEN("A normalized_float created from native data types and the number one")
    {

        Native a_native = GENERATE(take(15, full_native_range<Native>()));
        F a{a_native};
        F one{F::one()};

        WHEN("Adding these numbers")
        {
            F res{a * one};
            THEN("The result should have left the number untouched")
            {
                REQUIRE(res == a);
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Multiplication is commutative",
                       "[normalized_float][arithmetic][multiplication][invariant]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    using F = normalized_float<E, M>;

    GIVEN("Tow normalized_floats created from native data types")
    {

        Native a_native = GENERATE(take(15, full_native_range<Native>()));
        Native b_native = GENERATE(take(15, full_native_range<Native>()));

        F a{a_native};
        F b{b_native};

        WHEN("Multiplying these numbers")
        {
            F res1{a * b};
            F res2{b * a};
            THEN("The result should not depend on operand order")
            {
                REQUIRE(res1 == res2);
            }
        }
    }
}


TEMPLATE_TEST_CASE_SIG("Zero makes the multiplication result zero",
                       "[normalized_float][arithmetic][multiplication][invariant]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    using F = normalized_float<E, M>;

    GIVEN("A normalized_float  created from native data types and the number zero")
    {

        Native a_native = GENERATE(take(15, full_native_range<Native>()));
        F a{a_native};
        F zero{F::zero()};

        WHEN("Adding these numbers")
        {
            F res{a * zero};
            THEN("The result should have left the number untouched")
            {
                if (res != zero)
                {
                    std::cout << to_binary(a) << "\n";
                    std::cout << to_binary(res) << "\n";
                    std::cout << to_binary(zero) << "\n";
                }
                REQUIRE(res == zero);
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Multiplying with infinity",
                       "[normalized_float][arithmetic][multiplication]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    // I haven't found easily readable official documents that motivate these test cases. They are
    // based on the following web sites:
    // https://wiki.analytica.com/index.php/INF,_Nan,_Zero_and_IEEE/SANE_arithmetic
    // https://stackoverflow.com/questions/42926763/the-behaviour-of-floating-point-division-by-zero
    //      and the reference to Annex F
    GIVEN("A random floating point number")
    {

        using F = normalized_float<E, M>;

        Native f_ = GENERATE(take(15, full_native_range<Native>()));
        F f{f_};

        constexpr F neg_inf{F::neg_infinity()};
        constexpr F pos_inf{F::pos_infinity()};

        WHEN("Multiplying with infinity")
        {
            THEN("The sign of the resulting infinity should be correct")
            {
                F res = f * neg_inf;
                F res_ = f * pos_inf;

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


SCENARIO("Multiplication should work correctly", "[normalized_float][arithmetic][multiplication]")
{
    GIVEN("Two numbers in in <8,23> format")
    {
        using nf_t = normalized_float<8, 23>;

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
                       "[normalized_float][arithmetic][multiplication]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{

    using F = normalized_float<E, M>;

    Native a_native = GENERATE(take(15, full_native_range<Native>()));
    Native b_native = GENERATE(take(15, full_native_range<Native>()));
    F a{a_native};
    F b{b_native};

    F res = a * b;
    Native res_native = a_native * b_native;

    F res_native_{res_native};
    Native res_ = static_cast<Native>(res);

    if (!equal_except_rounding(res_native_, res))
    {
        F res_dbg = a * b;
        if (equal_except_rounding(res_native_, res_dbg))
        {
        }

        std::cout << "a * b\n"
                  << to_binary(a) << " * \n"
                  << to_binary(b) << "\n"
                  << to_binary(res) << "(normalized_float) !=\n"
                  << to_binary(res_native_) << "(float)\n\n";
    }

    CHECK(equal_except_rounding(res_native_, res));
    REQUIRE(equal_except_rounding(F{res_}, F{res_native}));
}


SCENARIO("Exact multiplication of two floating-point numbers (hand picked examples)",
         "[normalized_float][arithmetic][multiplication]")
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
            const normalized_float<E, M> result_float{number_a * number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154F;
            static constexpr float number_b = 93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 213.22154F;
            static constexpr float number_b = -93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154F;
            static constexpr float number_b = -93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = 213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = 213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = -213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = -213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
        WHEN("When one operand is a > 1 and the other is 0 < b < 1.")
        {
            static constexpr float number_a = 2.75F;
            static constexpr float number_b = 0.5F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
    }
}

SCENARIO("IEEE-754 denormalized number computations: float, double",
         "[normalized_float][denormalized][ieee-754][computation]")
{
    GIVEN("Two denormalized float numbers")
    {
        using nfloat = normalized_float<8, 23>;
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
        using nfloat = normalized_float<8, 23>;
        WHEN("The result of the multiplication should be denormalized > 0")
        {
            unsigned int a_i = 0b00011111111111111111111111111111;
            unsigned int b_i = 0b00011111111111111111111111111111;
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
                REQUIRE(equal_except_rounding(res, nfloat(res_f)));
                // REQUIRE(static_cast<float>(res) == res_f);
            }
        }

        WHEN("The result of the multiplication should be denormalized > 0")
        {
            unsigned int a_i = 0b00011010111111111111111111111111;
            unsigned int b_i = 0b00011010111111111111111111111111;
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
                REQUIRE(equal_except_rounding(res, nfloat(res_f)));
                // REQUIRE(static_cast<float>(res) == res_f);
            }
        }
    }
}
