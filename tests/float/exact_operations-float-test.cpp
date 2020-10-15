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

TEMPLATE_TEST_CASE_SIG("Addition is commutative",
                       "[normalized_float][arithmetic][addition][invariant]",
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

        WHEN("Adding these numbers")
        {
            F res1{a + b};
            F res2{b + a};
            THEN("The result should not depend on operand order")
            {
                REQUIRE(res1 == res2);
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

TEMPLATE_TEST_CASE_SIG("Zero is the neutral element of the addition",
                       "[normalized_float][arithmetic][addition][invariant]",
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
            F res{a + zero};
            THEN("The result should have left the number untouched")
            {
                REQUIRE(res == a);
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

TEMPLATE_TEST_CASE_SIG("Floating point addition matches its native counterparts",
                       "[normalized_float][arithmetic][addition]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{

    using F = normalized_float<E, M>;

    Native a_native = GENERATE(take(15, full_native_range<Native>()));
    Native b_native = GENERATE(take(15, full_native_range<Native>()));
    F a{a_native};
    F b{b_native};

    F res = a + b;
    Native res_native = a_native + b_native;

    F res_native_{res_native};
    Native res_ = static_cast<Native>(res);

    if (!equal_except_rounding(res_native_, res))
    {
        F res_ = a + b;
        if (!equal_except_rounding(res_native_, res_))
        {
        }
        std::cout << a << " + " << b << " = " << res_ << "\n";
        std::cout << to_binary(a) << " + \n" << to_binary(b) << " = \n" << to_binary(res) << "\n";
        // std::cout << to_compute_string(a) << "\t+\t" << to_compute_string(b) << " = "
        //          << to_compute_string(res) << "\n";

        std::cout << a_native << " + " << b_native << " = " << res_native << "\n";
        std::cout << to_binary(res) << "\n" << to_binary(res_native_) << "\n";
    }

    CHECK(equal_except_rounding(res_native_, res));
    REQUIRE(equal_except_rounding(F{res_}, F{res_native}));
}

TEMPLATE_TEST_CASE_SIG("Adding to infinity", "[normalized_float][arithmetic][addition]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    using F = normalized_float<E, M>;
    constexpr F neg_inf{F::neg_infinity()};
    constexpr F pos_inf{F::pos_infinity()};

    GIVEN("Infinity")
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
    AND_GIVEN("A random floating point number")
    {

        Native f_ = GENERATE(take(50, full_native_range<Native>()));

        F f{f_};
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

TEMPLATE_TEST_CASE_SIG("Dividing by infinity", "[normalized_float][arithmetic][division]",
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

TEMPLATE_TEST_CASE_SIG("Generating NaN as a result", "[normalized_float][arithmetic][addition]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    // these tests are modeled after the information presented on
    // https://www.doc.ic.ac.uk/~eedwards/compsys/float/nan.html

    using F = normalized_float<E, M>;

    GIVEN("NaN and +/- infinity")
    {
        const F nan{F::NaN()};
        const F neg_inf{F::neg_infinity()};
        const F pos_inf{F::pos_infinity()};
        WHEN("Adding/subtracting")
        {
            F res_add1 = neg_inf + pos_inf;
            F res_add2 = pos_inf + neg_inf;
            F res_sub1 = pos_inf - pos_inf;
            F res_sub2 = neg_inf - neg_inf;
            //            std::cout << to_binary(neg_inf) << "\n";
            //            std::cout << to_binary(pos_inf) << "\n";
            //            std::cout << to_binary(res_add) << "\n";

            CHECK(res_add1.is_nan());
            CHECK(res_add2.is_nan());
            REQUIRE(res_sub1.is_nan());
            REQUIRE(res_sub2.is_nan());
        }
        WHEN("Multiplying")
        {
            F res_mul1 = F::zero() * pos_inf;
            F res_mul2 = pos_inf * F::zero();
            F res_mul3 = neg_inf * F::zero();
            F res_mul4 = neg_inf * F::neg_zero();
            F res_mul5 = pos_inf * F::neg_zero();

            REQUIRE(res_mul1.is_nan());
            REQUIRE(res_mul2.is_nan());
            REQUIRE(res_mul3.is_nan());
            REQUIRE(res_mul4.is_nan());
            REQUIRE(res_mul5.is_nan());
        }
        WHEN("Dividing/computing modulus")
        {
            F res_div_zero = F::zero() / F::zero();
            F res_div_inf1 = pos_inf / pos_inf;
            F res_div_inf2 = pos_inf / neg_inf;
            F res_div_inf3 = neg_inf / pos_inf;
            F res_div_inf4 = neg_inf / neg_inf;
            CHECK(res_div_zero.is_nan());
            REQUIRE(res_div_inf1.is_nan());
            REQUIRE(res_div_inf2.is_nan());
            REQUIRE(res_div_inf3.is_nan());
            REQUIRE(res_div_inf4.is_nan());
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Floating point subtraction matches its native counterparts",
                       "[normalized_float][arithmetic][subtraction]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{

    using F = normalized_float<E, M>;

    Native a_native = GENERATE(take(15, full_native_range<Native>()));
    Native b_native = GENERATE(take(15, full_native_range<Native>()));
    F a{a_native};
    F b{b_native};

    F res = a - b;
    Native res_native = a_native - b_native;

    F res_native_{res_native};
    Native res_ = static_cast<Native>(res);

    if (!equal_except_rounding(res_native_, res))
    {
        F res_dbg = a - b;
        if (equal_except_rounding(res_native_, res_dbg))
        {
        }

        std::cout << "a - b\n"
                  << to_binary(F(a)) << " - \n"
                  << to_binary(F(b)) << "\n"
                  << to_binary(res) << "(normalized_float) !=\n"
                  << to_binary(F(res_native_)) << "(float)\n\n";
    }

    CHECK(equal_except_rounding(res_native_, res));
    REQUIRE(equal_except_rounding(F{res_}, F{res_native}));
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

TEMPLATE_TEST_CASE_SIG("Floating point division matches its native counterparts",
                       "[normalized_float][arithmetic][division]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{

    using F = normalized_float<E, M>;

    Native a_native = GENERATE(take(15, full_native_range<Native>()));
    Native b_native = GENERATE(take(15, full_native_range<Native>()));
    F a{a_native};
    F b{b_native};

    F res = a / b;
    Native res_native = a_native / b_native;

    F res_native_{res_native};
    Native res_ = static_cast<Native>(res);

    CHECK(equal_except_rounding(res_native_, res));
    REQUIRE(equal_except_rounding(F{res_}, F{res_native}));
}



// TODO create this test case the moment the random float branch is merged into development
//TEMPLATE_TEST_CASE_SIG("Dividing by denormalised numbers returns a larger number",
//                       "[normalized_float][arithmetic][division]", ((size_t E, size_t M), E, M),
//                       (8, 23), (11, 52), (15, 15), (3, 2))
//{
//
//    using F = normalized_float<E,M>;
//
//    GIVEN("A random normalized floating point number") {
//        F = GENERATE(take(15, full_native_range<Native>()));
//    }
//}

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
        AND_GIVEN("The smallest denormalised number sd")
        {
            F smallest{F::smallest_denormalized()};
            F res = div(F::one(), smallest);
            std::cout << to_binary(smallest) << "\t" << to_binary(smallest, true) << "\t"
                      << smallest << "\n";
            std::cout << to_binary(res) << "\t" << to_binary(res, true) << "\t" << res << "\n";

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
    }
}

SCENARIO("Adding two floating-point numbers exactly", "[normalized_float][arithmetic][addition]")
{
    GIVEN("Single precision floats (E = 8, M = 23)")
    {
        static constexpr size_t E = 8;
        static constexpr size_t M = 23;

        WHEN("The second operand is -a.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = -number_a;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

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
            const normalized_float<E, M> result = add(a, b);

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
            const normalized_float<E, M> result = add(a, b);

            THEN("The result should be a")
            {
                REQUIRE(result == a);
            }
        }
        WHEN("The first operand is 0.")
        {
            static constexpr float number_a = 0;
            static constexpr float number_b = 93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("The result should be b")
            {
                REQUIRE(result == b);
            }
        }
        WHEN("The first operand is 0, and the second operand is negative.")
        {
            static constexpr float number_a = 0;
            static constexpr float number_b = -93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

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
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154F;
            static constexpr float number_b = 93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 213.22154F;
            static constexpr float number_b = -93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154F;
            static constexpr float number_b = -93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = 213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = 213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = -213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = -213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a + number_b)));
            }
        }
        WHEN("When one operand is a > 1 and the other is 0 < b < 1.")
        {
            static constexpr float number_a = 2.75F;
            static constexpr float number_b = 0.5F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a + number_b)));
            }
        }
    }
}

SCENARIO("Subtracting two floating-point numbers exactly",
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

SCENARIO("Exact multiplication of two floating-point numbers",
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

SCENARIO("Exact division of two floating-point numbers", "[normalized_float][arithmetic][division]")
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

SCENARIO("IEEE-754 arithmetic conversion: float, double",
         "[normalized_float][conversion][ieee-754][casting]")
{
    GIVEN("A float number")
    {
        WHEN("The number is 0.")
        {
            float a = 0.F;

            THEN("The normalized_float should convert back to the original number.")
            {
                REQUIRE(static_cast<float>(normalized_float<8, 23>(a)) == a);
            }
        }
    }
    GIVEN("A double number")
    {
        WHEN("The number is 0.")
        {
            double a = 0.;

            THEN("The normalized_float should convert back to the original number.")
            {
                REQUIRE(static_cast<double>(normalized_float<11, 52>(a)) == a);
            }
        }
    }
    GIVEN("A random float number")
    {
        float a = GENERATE(take(10, random(float(1.0), std::numeric_limits<float>::max())));

        THEN("The normalized_float should convert back to the original number.")
        {
            REQUIRE(static_cast<float>(normalized_float<8, 23>(a)) == a);
        }
    }
    GIVEN("A random double number")
    {
        double a = GENERATE(take(10, random(double(1.0), std::numeric_limits<double>::max())));

        THEN("The normalized_float should convert back to the original number.")
        {
            REQUIRE(static_cast<double>(normalized_float<11, 52>(a)) == a);
        }
    }
}

SCENARIO("IEEE-754 denormalized number computations: float, double",
         "[normalized_float][denormalized][ieee-754][computation]")
{
    GIVEN("Two denormalized float numbers")
    {
        using nfloat = normalized_float<8, 23>;
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
            auto res = nfa * nfb;

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
