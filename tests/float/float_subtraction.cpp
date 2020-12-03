#include <aarith/float.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_float.hpp"
#include <aarith/core/bit_cast.hpp>

#include <bitset>
#include <catch.hpp>

using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Floating point subtraction matches its native counterparts",
                       "[normalized_float][arithmetic][subtraction]",
                       AARITH_FLOAT_TEST_SIGNATURE_WITH_NATIVE_TYPE,
                       AARIHT_FLOAT_TEMPLATE_NATIVE_RANGE_WITH_TYPE)
{

    using F = normalized_float<E, M>;

    F a = GENERATE(take(100, random_float<E, M, FloatGenerationModes::NonSpecial>()));
    F b = GENERATE(take(100, random_float<E, M, FloatGenerationModes::NonSpecial>()));
    Native a_native = static_cast<Native>(a);
    Native b_native = static_cast<Native>(b);

    F res = a - b;
    Native res_native = a_native - b_native;

    F res_native_{res_native};
    Native res_ = static_cast<Native>(res);

//    if (!equal_except_rounding(res_native_, res))
//    {
//        std::cout << "a - b\n"
//                  << to_binary(a) << " - \n"
//                  << to_binary(b) << "\n"
//                  << to_binary(res) << "(normalized_float) !=\n"
//                  << to_binary(res_native_) << "(native)\n\n";
//
//        std::cout << a << " - " << b << " = " << res << "\n";
//        std::cout << a_native << " - " << b_native << " = " << res_native << "\n";
//        std::cout << to_binary(F{a_native}, true) << " - " << to_binary(F{b_native}, true) << "\n";
//        std::cout << to_binary(a, true) << " - " << to_binary(b, true) << "\n";
//
//        std::bitset<32> bs_a = bit_cast<uint32_t>(a_native);
//        std::bitset<32> bs_b = bit_cast<uint32_t>(b_native);
//        std::cout << bs_a << " - " << bs_b << "\n";
//
//        std::cout << "\n";
//        std::bitset<32> bs_res_native = bit_cast<uint32_t>(res_native);
//        std::bitset<32> bs_res = bit_cast<uint32_t>(res_);
//        std::cout << bs_res_native << " != \n" << bs_res << "\n";
//        exit(0);
//    }

    CHECK(equal_except_rounding(res_native_, res));
    REQUIRE(equal_except_rounding(F{res_}, F{res_native}));
}

TEMPLATE_TEST_CASE_SIG("Floating point subtraction involving zero",
                       "[normalized_float][arithmetic][subtraction]",
                       AARITH_FLOAT_TEST_SIGNATURE,
                       AARIHT_FLOAT_TEMPLATE_RANGE)
{

    using F = normalized_float<E, M>;

    constexpr F zero = F::zero();
    constexpr F neg_zero = F::neg_zero();

    GIVEN("A random floating-point number")
    {
        F a = GENERATE(take(100, random_float<E, M, FloatGenerationModes::NonSpecial>()));

        WHEN("Subtracting zero")
        {
            const F res_p = a - zero;
            const F res_n = a - neg_zero;

            THEN("The result is unchanged")
            {
                CHECK(res_p == a);
                CHECK(res_n == a);
                REQUIRE(res_p == res_n);
            }
        }

        WHEN("Subtracting from zero")
        {
            const F res1 = zero - a;
            const F res2 = neg_zero - a;

            CHECK(res1 == -a);
            CHECK(res2 == -a);
            REQUIRE(res1 == res2);
        }
    }
}

TEMPLATE_TEST_CASE_SIG(
    "Floating point subtraction matches its native counterparts (special values only)",
    "[normalized_float][arithmetic][subtraction]", AARITH_FLOAT_TEST_SIGNATURE_WITH_NATIVE_TYPE,
    AARIHT_FLOAT_TEMPLATE_NATIVE_RANGE_WITH_TYPE)
{

    using F = normalized_float<E, M>;

    GIVEN("Random NaN and infinity values")
    {

        F a = GENERATE(take(30, random_float<E, M, FloatGenerationModes::Special>()));
        F b = GENERATE(take(30, random_float<E, M, FloatGenerationModes::Special>()));

        Native a_native = static_cast<Native>(a);
        Native b_native = static_cast<Native>(b);

        WHEN("Adding them")
        {

            F res = a - b;
            Native res_native = a_native - b_native;

            F res_native_{res_native};
            //            Native res_ = static_cast<Native>(res);

            THEN("The result should match the native result bit by bit")
            {
                REQUIRE(bit_equal(res, res_native_));
            }
        }
    }
}

SCENARIO("Subtracting two floating-point numbers (hand picked examples)",
         "[normalized_float][arithmetic][subtraction]")
{
    GIVEN("Single precision floats (E = 8, M = 23)")
    {
        static constexpr size_t E = 8;
        static constexpr size_t M = 23;

        WHEN("Both operands are the same.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = number_a;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("The result should be 0")
            {
                REQUIRE(result == normalized_float<E, M>(0.f));
            }
        }
        WHEN("The second operand is 0.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = 0;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("The result should be a")
            {
                REQUIRE(result == a);
            }
        }
        WHEN("The first operand is negative, and the second operand is 0.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = 0;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("The result should be a")
            {
                REQUIRE(result == a);
            }
        }
        WHEN("The first operand is 0.")
        {
            static constexpr float number_a = 0;
            static constexpr float number_b = 93.211546F;
            static constexpr float res = -number_b;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("The result should be -b")
            {
                REQUIRE(result == normalized_float<E, M>(res));
            }
        }
        WHEN("The first operand is 0, and the second operand is negative.")
        {
            static constexpr float number_a = 0;
            static constexpr float number_b = -93.211546F;
            static constexpr float res = -number_b;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("The result should be -b")
            {
                REQUIRE(result == normalized_float<E, M>(res));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 213.22154F;
            static constexpr float number_b = 93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154F;
            static constexpr float number_b = 93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 213.22154F;
            static constexpr float number_b = -93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154F;
            static constexpr float number_b = -93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = 213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = 213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = -213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = -213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
    }
}