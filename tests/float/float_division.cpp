#include <aarith/float.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_float.hpp"

#include <bitset>
#include <catch.hpp>

using namespace aarith;

TEMPLATE_TEST_CASE_SIG(
    "Floating point division matches its native counterparts (special values only)",
    "[floating_point][arithmetic][division]", AARITH_FLOAT_TEST_SIGNATURE_WITH_NATIVE_TYPE,
    AARIHT_FLOAT_TEMPLATE_NATIVE_RANGE_WITH_TYPE)
{

    using F = floating_point<E, M>;

    GIVEN("Random NaN and infinity values")
    {

        F a = GENERATE(take(30, random_float<E, M, FloatGenerationModes::Special>()));
        F b = GENERATE(take(30, random_float<E, M, FloatGenerationModes::Special>()));

        Native a_native = static_cast<Native>(a);
        Native b_native = static_cast<Native>(b);

        WHEN("Adding them")
        {

            F res = a / b;
            Native res_native = a_native / b_native;

            F res_native_{res_native};
            //            Native res_ = static_cast<Native>(res);

            THEN("The result should match the native result bit by bit")
            {
                REQUIRE(bit_equal(res, res_native_));
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Dividing by one", "[floating_point][arithmetic][division]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARIHT_FLOAT_TEMPLATE_RANGE)
{
    GIVEN("A random floating point number")
    {

        using F = floating_point<E, M>;

        F f = GENERATE(take(100, random_float<E, M, FloatGenerationModes::NonSpecial>()));

        WHEN("Dividing the number by 1")
        {
            const F res = f / F::one();

            THEN("The result should be unchanged")
            {
                REQUIRE(res == f);
            }
        }

        WHEN("Dividing the number by -1")
        {
            const F res = f / F::neg_one();

            THEN("The number should have switched its sign")
            {
                REQUIRE(res == -f);
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Dividing by infinity", "[floating_point][arithmetic][division]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARIHT_FLOAT_TEMPLATE_RANGE)
{
    // I haven't found easily readable official documents that motivate these test cases. They are
    // based on the following web sites:
    // https://wiki.analytica.com/index.php/INF,_Nan,_Zero_and_IEEE/SANE_arithmetic
    // https://stackoverflow.com/questions/42926763/the-behaviour-of-floating-point-division-by-zero
    //      and the reference to Annex F

    GIVEN("A random floating point number")
    {

        using F = floating_point<E, M>;

        F f = GENERATE(take(100, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        constexpr F neg_inf{F::neg_infinity()};
        constexpr F pos_inf{F::pos_infinity()};
        constexpr F pos_zero{F::zero()};
        constexpr F neg_zero{F::neg_zero()};

        WHEN("Dividing by infinity")
        {
            THEN("The sign of the resulting zero should be correct")
            {
                if (!f.is_zero() && !f.is_nan() && !f.is_inf())
                {
                    F res = f / neg_inf;
                    F res_ = f / pos_inf;

                    if (f.is_negative())
                    {
                        if (res != pos_zero || res_ != neg_zero)
                        {
                            std::cout << "f:    " << to_binary(f) << "(" << f.is_inf() << ")"
                                      << "\n"
                                      << "res:  " << to_binary(res) << "\n"
                                      << "res_: " << to_binary(F(res_)) << "\n";
                        }

                        CHECK(res == pos_zero);
                        REQUIRE(res_ == neg_zero);
                    }
                    else
                    {
                        if (res != neg_zero || res_ != pos_zero)
                        {
                            std::cout << "f:    " << to_binary(f) << "(" << f.is_inf() << ")"
                                      << "\n"
                                      << "res:  " << to_binary(res) << "\n"
                                      << "res_: " << to_binary(F(res_)) << "\n";
                        }

                        CHECK(res == neg_zero);
                        REQUIRE(res_ == pos_zero);
                    }
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Dividing by zero", "[floating_point][arithmetic][division]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARIHT_FLOAT_TEMPLATE_RANGE)
{
    // I haven't found easily readable official documents that motivate these test cases. They are
    // based on the following web sites:
    // https://wiki.analytica.com/index.php/INF,_Nan,_Zero_and_IEEE/SANE_arithmetic
    // https://stackoverflow.com/questions/42926763/the-behaviour-of-floating-point-division-by-zero
    //      and the reference to Annex F

    GIVEN("A random floating point number")
    {

        using F = floating_point<E, M>;

        F f = GENERATE(take(100, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        constexpr F neg_inf{F::neg_infinity()};
        constexpr F pos_inf{F::pos_infinity()};
        constexpr F pos_zero{F::zero()};
        constexpr F neg_zero{F::neg_zero()};
        WHEN("Dividing by infinity")
        {
            THEN("The sign of the resulting infinity should be correct")
            {

                if (!f.is_zero() && !f.is_nan())
                {

                    F res = f / neg_zero;
                    F res_ = f / pos_zero;

                    if (f.is_negative())
                    {
                        if (res != pos_inf || res_ != neg_inf)
                        {
                            std::cout << "f:    " << to_binary(f) << "(" << f.is_inf() << ")"
                                      << "\n"
                                      << "res:  " << to_binary(res) << "\n"
                                      << "res_: " << to_binary(F(res_)) << "\n"
                                      << "pos_inf: " << to_binary(pos_inf) << "\n"
                                      << "neg_inf: " << to_binary(neg_inf) << "\n";
                        }

                        CHECK(res == pos_inf);
                        REQUIRE(res_ == neg_inf);
                    }
                    else
                    {
                        if (res != neg_inf || res_ != pos_inf)
                        {
                            std::cout << "f:    " << to_binary(f) << "(" << f.is_inf() << ")"
                                      << "\n"
                                      << "res:  " << to_binary(res) << "\n"
                                      << "res_: " << to_binary(F(res_)) << "\n";
                        }

                        CHECK(res == neg_inf);
                        REQUIRE(res_ == pos_inf);
                    }
                }
            }
        }
    }
}

SCENARIO("Hand-picked division examples", "[floating_point][arithmetic][division][current]")
{

    GIVEN("Two fixed single-precision floating-point numbers a and b")
    {

        static constexpr size_t E_ = 8;
        static constexpr size_t M_ = 23;
        using F = floating_point<E_, M_>;
        using E = uinteger<E_>;
        using M = uinteger<M_>;

        constexpr F a{false, E{0b00101101}, M{0b11000010001001010011000}};
        constexpr F b{true, E{0b11000011}, M{0b10011001000111101011101}};

        const float a_float = static_cast<float>(a);
        const float b_float = static_cast<float>(b);

        WHEN("Computing a/b")
        {
            const F result_aarith = a / b;

            const float result_float = a_float / b_float;
            THEN("The result should match the native data type's result")
            {
                const bool eql = equal_except_rounding(result_aarith, F{result_float});

                if (!eql)
                {
                    std::cout << "a (aarith): " << to_binary(a) << "\n"
                              << "a (native): " << to_binary(F(a_float)) << "\n"
                              << "b (aarith): " << to_binary(b) << "\n"
                              << "b (native): " << to_binary(F(b_float)) << "\n"
                              << "res (aarith): " << to_binary(result_aarith) << "\n"
                              << "res (native): " << to_binary(F(result_float)) << "\n";
                }

                REQUIRE(eql);
            }
        }
    }
    GIVEN("Two fixed single-precision floating-point numbers a and b")
    {

        static constexpr size_t E_ = 11;
        static constexpr size_t M_ = 52;
        using F = floating_point<E_, M_>;
        using E = uinteger<E_>;
        using M = uinteger<M_>;

        constexpr F a{false, E{0b00101011111},
                      M{0b0100010100101001010001101111000011011110111100011111}};
        constexpr F b{true, E{0b11010011100},
                      M{0b0001001011101011000101111101101001100001011100101110}};

        const double a_float = static_cast<double>(a);
        const double b_float = static_cast<double>(b);

        std::bitset<64> bs_a = bit_cast<uint64_t>(a_float);
        std::bitset<64> bs_b = bit_cast<uint64_t>(b_float);

        WHEN("Computing a/b")
        {
            const F result_aarith = a / b;
            const double result_float = a_float / b_float;

            std::bitset<64> bs_res = bit_cast<uint64_t>(result_float);
            THEN("The result should match the native data type's result")
            {
                const bool eql = equal_except_rounding(result_aarith, F{result_float});

                if (!eql)
                {
                    std::cout << "a (aarith): " << to_binary(a) << "\n"
                              << "a (native): " << bs_a << "\n"
                              << "b (aarith): " << to_binary(b) << "\n"
                              << "b (native): " << bs_b << "\n"
                              << "res (aarith): " << to_binary(result_aarith) << "\n"
                              << "res (native): " << to_binary(F{result_float}) << "\n"
                              << "res (bs): " << bs_res << "\n"
                              << (a_float / b_float) << "\n";
                }

                REQUIRE(eql);
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Floating point division matches its native counterparts",
                       "[floating_point][arithmetic][division]",
                       AARITH_FLOAT_TEST_SIGNATURE_WITH_NATIVE_TYPE,
                       AARIHT_FLOAT_TEMPLATE_NATIVE_RANGE_WITH_TYPE)
{

    using F = floating_point<E, M>;

    F a = GENERATE(take(100, random_float<E, M, FloatGenerationModes::FullyRandom>()));
    F b = GENERATE(take(100, random_float<E, M, FloatGenerationModes::FullyRandom>()));

    Native a_native = static_cast<Native>(a);
    Native b_native = static_cast<Native>(b);

    F res = a / b;
    Native res_native = a_native / b_native;

    F res_native_{res_native};
    Native res_ = static_cast<Native>(res);

    if (!equal_except_rounding(res_native_, res) && !bit_equal(res_native_, res))
    {
        std::cout << "a (aarith): " << to_binary(a) << "\n"
                  << "a (native): " << to_binary(F(a)) << "\n"
                  << "b (aarith): " << to_binary(b) << "\n"
                  << "b (native): " << to_binary(F(b)) << "\n"
                  << "res (aarith): " << to_binary(res) << "\n"
                  << "res (native): " << to_binary(F(res_native)) << "\n"
                  << "res (cast):   " << to_binary(F(res_)) << "\n";

        F res2 = a / b;
        Native res_native2 = a_native / b_native;

        F res_native2_{res_native2};
        Native res2_ = static_cast<Native>(res2);

        std::cout << "a (aarith): " << to_binary(a) << "\n"
                  << "a (native): " << to_binary(F(a)) << "\n"
                  << "b (aarith): " << to_binary(b) << "\n"
                  << "b (native): " << to_binary(F(b)) << "\n"
                  << "res (aarith): " << to_binary(res2) << "\n"
                  << "res (native): " << to_binary(F(res_native2)) << "\n"
                  << "res (cast):   " << to_binary(F(res2_)) << "\n";
    }

    if (a.is_nan() || b.is_nan())
    {
        REQUIRE(bit_equal(res_native_, res));
    }
    else
    {
        CHECK(equal_except_rounding(res_native_, res));
        REQUIRE(equal_except_rounding(F{res_}, F{res_native}));
    }
}

SCENARIO("Manual test case for floating point division",
         "[floating_point][arithmetic][division][bar]")
{
    using F = floating_point<8, 23>;
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
            //            std::cout << to_binary(smallest) << "\t" << to_binary(smallest, true) <<
            //            "\t"
            //                      << smallest << "\n";
            //            std::cout << to_binary(res) << "\t" << to_binary(res, true) << "\t" << res
            //            << "\n";

            WHEN("Dividing one by the smallest denormalized number")
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
    }
}

SCENARIO("Division of two floating-point numbers (hand picked examples)",
         "[floating_point][arithmetic][division]")
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
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154F;
            static constexpr float number_b = 93.211546F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 213.22154F;
            static constexpr float number_b = -93.211546F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154F;
            static constexpr float number_b = -93.211546F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = 213.22154F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = 213.22154F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = -213.22154F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = -213.22154F;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a / number_b)));
            }
        }
    }
}
