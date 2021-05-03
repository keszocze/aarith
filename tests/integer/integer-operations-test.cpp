#include "gen_integer.hpp"
#include <aarith/integer.hpp>
#include <aarith/integer/integer_random_generation.hpp>
#include <catch.hpp>
#include <limits>

#include "../test-signature-ranges.hpp"

using namespace aarith; // NOLINT

TEMPLATE_TEST_CASE_SIG("Pre/postfix operator++", "[integer][arithmetic][foo]",
                       AARITH_INT_EXTENDED_TEST_SIGNATURE,
                       AARITH_INT_EXTENDED_TEST_TEMPLATE_NATIVE_SIZES_PARAM_RANGE)
{
    using I = Type<W, WordType>;
    GIVEN("A random integer and a random uint8_t")
    {
        I a = GENERATE(take(10, random_integer<W, WordType>()));
        I b{a};

        uint8_t iters = GENERATE(take(10, random<uint8_t>(std::numeric_limits<uint8_t>::min(),
                                                          std::numeric_limits<uint8_t>::max())));

        WHEN("Calling the prefix/postfix -- operator multiple times")
        {
            THEN("It should match the subtraction operation")
            {
                I a_sub{sub(a, I{iters})};
                for (size_t i = 0; i < iters; ++i)
                {
                    CHECK(--a == sub(b--, I::one()));
                    CHECK(a == b);
                }
                CHECK(a == a_sub);
                REQUIRE(b == a_sub);
            }
        }
    }

    GIVEN("A random integer and a random uint8_t")
    {
        I a = GENERATE(take(10, random_integer<W, WordType>()));
        I b{a};

        uint8_t iters = GENERATE(take(10, random<uint8_t>(std::numeric_limits<uint8_t>::min(),
                                                          std::numeric_limits<uint8_t>::max())));

        WHEN("Calling the prefix/postfix ++ operator multiple times")
        {
            THEN("It should match the addition operation")
            {
                I a_add{add(a, I{iters})};
                for (size_t i = 0; i < iters; ++i)
                {
                    CHECK(++a == add(b++, I::one()));
                    CHECK(a == b);
                }
                CHECK(a == a_add);
                REQUIRE(b == a_add);
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Arithmetic should be constexpr", "[integer][signed][arithmetic][constexpr]",
                       AARITH_INT_TEST_SIGNATURE, AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;
    GIVEN("Two integers")
    {
        constexpr I a{4};
        constexpr I b{8};

        WHEN("Performing addition")
        {

            THEN("The operation is constexpr")
            {
                constexpr I expected{12};

                constexpr auto result_expanded = expanding_add(a, b);
                constexpr I result = add(a, b);

                REQUIRE(result_expanded == expected);
                REQUIRE(result == expected);
            }
        }
        WHEN("Performing subtraction")
        {
            THEN("The operation is constexpr")
            {
                constexpr I expected{4};

                constexpr I result = sub(b, a);
                //                constexpr integer<32> result = expanding_sub(b, a);

                REQUIRE(result == expected);
            }
        }
        WHEN("Performing multiplication")
        {
            THEN("The operation is constexpr")
            {
                constexpr I expected{32};

                constexpr I result = mul(b, a);

                REQUIRE(result == expected);
            }
        }
        WHEN("Performing Division/Modulo")
        {
            THEN("The operation is constexpr")
            {
                constexpr I expected{2};

                constexpr I result = div(b, a);

                REQUIRE(result == expected);
            }
        }
    }
}

SCENARIO("Adding two positive integers", "[integer][signed][arithmetic][addition]")
{
    GIVEN("Two positive integer<N> a and b with N <= word_width")
    {
        static constexpr size_t TestWidth = 16;
        static_assert(1 == integer<TestWidth>::word_count());

        WHEN("The result a+b still fits into N bits")
        {
            static constexpr uint8_t number_a = 32;
            static constexpr uint8_t number_b = 16;
            const integer<TestWidth> a{number_a};
            const integer<TestWidth> b{number_b};
            const integer<TestWidth> result = add(a, b);
            const integer<TestWidth> result_fun = fun_add(a, b);

            THEN("It should be the correct sum")
            {
                CHECK(result_fun == result);
                REQUIRE(result.word(0) == number_a + number_b);
            }
        }
        WHEN("The result a+b does not fit into N bits")
        {
            static constexpr uint16_t number_a = (1 << TestWidth) - 1;
            static constexpr uint16_t number_b = 1;
            constexpr integer<TestWidth> a{number_a};
            constexpr integer<TestWidth> b{number_b};
            auto const result = add(a, b);
            const integer<TestWidth> result_fun = fun_add(a, b);

            THEN("It should be the masked to fit")
            {
                CHECK(result_fun == result);
                REQUIRE(result.word(0) == 0);
            }
        }
    }
    GIVEN("Two integers with different word_count")
    {
        constexpr integer<64> a{16};
        constexpr integer<128> b{32, 8};
        constexpr integer<128> expected{32, 24};
        WHEN("Adding the integers")
        {
            THEN("All words need to be respected in the operation")
            {
                const auto result = expanding_add(b, a);
                REQUIRE(result == expected);
            }
        }
    }
    GIVEN("Two positive integer<N> a and b with N > word_width")
    {
        static constexpr size_t TestWidth = 128;
        static_assert(integer<TestWidth>::word_count() > 1);

        WHEN("There is a carry into the next word")
        {
            static constexpr uint64_t number_a = 1ULL << 63;
            static constexpr uint64_t number_b = 1ULL << 63;
            constexpr integer<TestWidth> a{number_a};
            constexpr integer<TestWidth> b{number_b};
            auto const result = add(a, b);
            const integer<TestWidth> result_fun = fun_add(a, b);

            THEN("It is added to the next word")
            {
                CHECK(result_fun == result);
                REQUIRE(result.word(1) == 1);
            }
        }

        WHEN("There is no carry into the next word")
        {
            static constexpr uint64_t number_a = 1ULL << 63;
            static constexpr uint64_t number_b = 0;
            constexpr integer<TestWidth> a{number_a};
            constexpr integer<TestWidth> b{number_b};
            auto const result = add(a, b);
            const integer<TestWidth> result_fun = fun_add(a, b);

            THEN("The next word is unchanged")
            {
                REQUIRE(result_fun == result);
                REQUIRE(result.word(1) == 0);
            }
        }
    }
    GIVEN("An integer and its negative")
    {
        constexpr integer<16> a(15);
        constexpr integer<16> a_(-15);
        constexpr integer<16> zero16(0);
        const integer<16> sum16 = add(a, a_);
        const integer<16> sum16_fun = fun_add(a, a_);

        constexpr integer<64> b(150);
        constexpr integer<64> b_(-150);
        constexpr integer<64> zero64(0);
        const integer<64> sum64 = add(b, b_);
        const integer<64> sum64_fun = fun_add(b, b_);

        constexpr integer<150> c(1337);
        constexpr integer<150> c_(-1337);
        constexpr integer<150> zero150(0);
        const integer<150> sum150_fun = fun_add(c, c_);
        const integer<150> sum150 = add(c, c_);

        THEN("The sum should be zero")
        {

            CHECK(sum16_fun == sum16);
            REQUIRE(sum16 == zero16);
            REQUIRE(sum16.is_zero());
            REQUIRE(sum16 == add(a_, a));

            CHECK(sum64_fun == sum64);
            REQUIRE(sum64 == zero64);
            REQUIRE(sum64.is_zero());
            REQUIRE(sum64 == add(b_, b));

            CHECK(sum150_fun == sum150);
            REQUIRE(sum150 == zero150);
            REQUIRE(sum150.is_zero());
            REQUIRE(sum150 == add(c_, c));
        }

        THEN("The sum should be negative")
        {
            REQUIRE_FALSE(sum16.is_negative());
            REQUIRE_FALSE(sum64.is_negative());
            REQUIRE_FALSE(sum150.is_negative());
        }
    }

    GIVEN("The number zero")
    {
        constexpr integer<16> z16;
        constexpr integer<63> z63;
        constexpr integer<150> z150;
        WHEN("Adding a negative number")
        {

            constexpr integer<16> neg16{-5};
            constexpr integer<63> neg63{-5};
            constexpr integer<150> neg150{-5};

            THEN("The result should be the negative number")
            {
                CHECK(add(z16, neg16) == neg16);
                CHECK(add(z63, neg63) == neg63);
                CHECK(add(z150, neg150) == neg150);
            }
        }
    }

    GIVEN("A positive integer and a negative integer with larger absolute value")
    {
        constexpr integer<16> a(15);
        constexpr integer<16> a_(-16);
        const integer<16> sum16 = add(a, a_);

        constexpr integer<64> b(150);
        constexpr integer<64> b_(-235);
        const integer<64> sum64 = add(b, b_);

        constexpr integer<150> c(1337);
        constexpr integer<150> c_(-5000);
        const integer<150> sum150 = add(c, c_);

        THEN("The sum should be negative")
        {
            REQUIRE(sum16.is_negative());
            REQUIRE(sum64.is_negative());
            REQUIRE(sum150.is_negative());
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Signed integer addition is commutative",
                       "[integer][signed][arithmetic][addition]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;
    using R = integer<2 * W, WordType>;

    I a = GENERATE(take(10, random_integer<W, WordType>()));
    I b = GENERATE(take(10, random_integer<W, WordType>()));

    WHEN("Computing with extended bit widths")
    {
        R res_a_b{expanding_add(a, b)};
        R res_b_a{expanding_add(b, a)};
        THEN("The results should match")
        {
            REQUIRE(res_a_b == res_b_a);
        }
    }
    WHEN("Computing with truncation")
    {
        I res_a_b{add(a, b)};
        I res_b_a{add(a, b)};
        THEN("The results should match")
        {
            REQUIRE(res_a_b == res_b_a);
        }
    }
}

TEMPLATE_TEST_CASE_SIG(
    "Signed integer addition is commutative for addends with different bit widths",
    "[integer][signed][arithmetic][addition]", AARITH_INT_TEST_SIGNATURE,
    AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;
    using ISmall = integer<W - 2, WordType>;
    using ILarge = integer<W + I::word_width() + 3, WordType>;

    I a = GENERATE(take(5, random_integer<W, WordType>()));
    ISmall b = GENERATE(take(5, random_integer<W - 2, WordType>()));
    ILarge c = GENERATE(take(5, random_integer<W + I::word_width() + 3, WordType>()));

    WHEN("Using different bit-widths")
    {

        const auto res_a_b{expanding_add(a, b)};
        const auto res_b_a{expanding_add(a, b)};

        const auto res_a_c{expanding_add(a, c)};
        const auto res_c_a{expanding_add(c, a)};
        THEN("The results should match")
        {
            REQUIRE(res_a_b == res_b_a);
            REQUIRE(res_a_c == res_c_a);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Zero is the neutral element of the addition",
                       "[integer][signed][arithmetic][addition]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;
    I a = GENERATE(take(100, random_integer<W, WordType>()));
    REQUIRE(add(a, I::zero()) == a);
    REQUIRE(expanding_add(a, I::zero()) == a);
}

TEMPLATE_TEST_CASE_SIG("Addition wraps around correctly", "[integer][signed][arithmetic][addition]",
                       AARITH_INT_TEST_SIGNATURE, AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;

    static constexpr I one = I::one();
    static constexpr I max_val = I::max();
    static constexpr I min_val = I::min();

    WHEN("Adding one to max")
    {
        THEN("The result should be min")
        {
            REQUIRE(add(max_val, one) == min_val);
        }
    }
    WHEN("Adding max to max")
    {
        THEN("The result should be minus two")
        {
            const I minus_two = sub(I::minus_one(), I::one());
            REQUIRE(add(max_val, max_val) == minus_two);
        }
    }
    WHEN("Adding min to max")
    {
        THEN("The result should be minus one")
        {
            REQUIRE(add(max_val, min_val) == I::minus_one());
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Zero is the neutral element of the subtraction",
                       "[integer][unsigned][arithmetic][subtraction]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;
    I a = GENERATE(take(100, random_integer<W, WordType>()));
    REQUIRE(sub(a, I::zero()) == a);
    REQUIRE(expanding_sub(a, I::zero()) == a);
}

TEMPLATE_TEST_CASE_SIG("Expanding subtraction wraps around correctly for different bit-widths",
                       "[integer][signed][arithmetic][subtraction]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;
    using LargeI = integer<2 * W + 6, WordType>;

    GIVEN("A n-bit zero and a m-bit (m>n)  max")
    {
        static constexpr LargeI max_val = LargeI::max();
        static constexpr LargeI min_val = LargeI::min();
        static constexpr LargeI expected = LargeI::one();

        THEN("Subtracting max from min should give one")
        {
            auto constexpr result = expanding_sub(min_val, max_val);
            REQUIRE(result == expected);
        }
    }

    GIVEN("A n-bit zero and a m-bit (m<n) max")
    {
        static constexpr LargeI zero = LargeI::zero();
        static constexpr I large = I::max();
        static constexpr LargeI expected = sub(LargeI::one(), add(LargeI{I::max()}, LargeI::one()));

        THEN("Subtracting max from zero should give 1-(small::max+1)")
        {
            auto constexpr result = expanding_sub(zero, large);
            REQUIRE(result == expected);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Subtraction wraps around correctly",
                       "[integer][signed][arithmetic][subtraction]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;

    static constexpr I one = I::one();
    static constexpr I minus_one = I::minus_one();
    static constexpr I max_val = I::max();
    static constexpr I min_val = I::min();

    WHEN("Subtracting one from min")
    {
        THEN("The result should be max")
        {
            auto constexpr result = expanding_sub(min_val, one);
            REQUIRE(result == max_val);
        }
    }

    WHEN("Subtracting minus one from max")
    {
        THEN("The result should be min")
        {
            auto constexpr result = expanding_sub(max_val, minus_one);
            REQUIRE(result == min_val);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Unsigned integer multiplication is commutative",
                       "[integer][signed][arithmetic][multiplication]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;
    using R = integer<2 * W, WordType>;

    I a = GENERATE(take(10, random_integer<W, WordType>()));
    I b = GENERATE(take(10, random_integer<W, WordType>()));

    // the most negtive number behaves strangely
    if (a != I::min() && b != I::min())
    {

        WHEN("Computing with extended bit widths")
        {
            R res_a_b_naive{naive_expanding_mul(a, b)};
            R res_b_a_naive{naive_expanding_mul(b, a)};
            R res_a_b_booth{booth_expanding_mul(a, b)};
            R res_b_a_booth{booth_expanding_mul(b, a)};
            THEN("The the flipped results should match")
            {
                REQUIRE(res_a_b_naive == res_b_a_naive);
                REQUIRE(res_a_b_booth == res_b_a_booth);
            }
            AND_THEN("The results of the different methods should match")
            {
                REQUIRE(res_a_b_naive == res_a_b_booth);
            }
        }

        WHEN("Computing with truncation")
        {
            I res_a_b_naive{naive_mul(a, b)};
            I res_b_a_naive{naive_mul(b, a)};
            I res_a_b_booth{booth_mul(a, b)};
            I res_b_a_booth{booth_mul(b, a)};
            THEN("The the flipped results should match")
            {
                REQUIRE(res_a_b_naive == res_b_a_naive);
                REQUIRE(res_a_b_booth == res_b_a_booth);
            }
            AND_THEN("The results of the different methods should match")
            {
                REQUIRE(res_a_b_naive == res_a_b_booth);
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("One is the neutral element of the multiplication",
                       "[integer][signed][arithmetic][multiplication]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;
    I a = GENERATE(take(50, random_integer<W, WordType>()));
    REQUIRE(naive_mul(a, I::one()) == a);
    REQUIRE(naive_expanding_mul(a, I::one()) == a);
    REQUIRE(booth_mul(a, I::one()) == a);
    REQUIRE(booth_expanding_mul(a, I::one()) == a);
}

TEMPLATE_TEST_CASE_SIG("Multiplying with minus one only changes the sign ",
                       "[integer][signed][arithmetic][multiplication]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    GIVEN("No the most negative number")
    {
        using I = integer<W, WordType>;
        I a = GENERATE(take(50, random_integer<W, WordType>()));
        if (a != I::min())
        {
            REQUIRE(naive_mul(a, I::minus_one()) == -a);
            REQUIRE(naive_expanding_mul(a, I::minus_one()) == -a);
            REQUIRE(booth_mul(a, I::minus_one()) == -a);
            REQUIRE(booth_expanding_mul(a, I::minus_one()) == -a);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Zero makes the multiplication result zero",
                       "[integer][signed][arithmetic][multiplication]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;
    using R = integer<2 * W, WordType>;
    I a = GENERATE(take(50, random_integer<W, WordType>()));
    REQUIRE(naive_mul(a, I::zero()) == I::zero());
    REQUIRE(naive_expanding_mul(a, I::zero()) == R::zero());
    REQUIRE(booth_mul(a, I::zero()) == I::zero());
    REQUIRE(booth_expanding_mul(a, I::zero()) == R::zero());
}

TEMPLATE_TEST_CASE_SIG("Multiplying the max value with itself and truncation yields 1",
                       "[integer][signed][arithmetic][multiplication]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;
    I a{I::max()};
    REQUIRE(naive_mul(a, a) == I::one());
    REQUIRE(booth_mul(a, a) == I::one());
}

TEMPLATE_TEST_CASE_SIG("Testing the most negative number", "[integer][signed][arithmetic]",
                       AARITH_INT_TEST_SIGNATURE, AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;
    GIVEN("The most negative number")
    {
        constexpr I mnn = I::min();
        WHEN("Multyplying with -1")
        {
            THEN("The value should not change")
            {
                CHECK(naive_mul(mnn, I::minus_one()) == mnn);
                REQUIRE(booth_mul(mnn, I::minus_one()) == mnn);
            }
        }
        WHEN("Performing unary negation")
        {
            THEN("The value should not change")
            {
                REQUIRE(-mnn == mnn);
            }
        }
        WHEN("Computing the absolute value")
        {
            THEN("The value should not change")
            {
                REQUIRE(abs(mnn) == mnn);
            }
        }
    }
}

SCENARIO("Multiplying larger integers using the various implementations works",
         "[integer][signed][arithmetic][multiplication]")
{
    GIVEN("Some large integers")
    {
        const integer<192> a = integer<192>::from_words(1U, 0U, 0U);
        const integer<192> b = integer<192>::from_words(1U, 1U, 0U);
        const integer<192> c = integer<192>{10};
        const integer<192> one = integer<192>::one();
        const integer<192> ones = integer<192>::all_ones();
        const integer<192> zero = integer<192>::zero();

        std::vector<integer<192>> numbers{a, b, c, one, ones, zero};

        WHEN("Multiplying by zero")
        {
            THEN("The result is zero")
            {

                for (const auto& num : numbers)
                {
                    REQUIRE(booth_mul(num, zero) == zero);
                    REQUIRE(naive_mul(num, zero) == zero);
                    REQUIRE(booth_inplace_mul(num, zero) == zero);
                }
            }
        }

        WHEN("Multiplying by one")
        {
            THEN("The result is unchanged")
            {
                for (const auto& num : numbers)
                {
                    REQUIRE(naive_mul(num, one) == num);
                    REQUIRE(booth_mul(num, one) == num);
                    REQUIRE(booth_inplace_mul(num, one) == num);
                }
            }
        }

        WHEN("Comparing all multiplication variants")
        {
            THEN("The results should match")
            {
                for (const auto& n : numbers)
                {
                    for (const auto& m : numbers)
                    {
                        const auto res_normal = booth_mul(n, m);
                        const auto res_naive = naive_mul(n, m);
                        const auto res_inplace = booth_inplace_mul(n, m);

                        REQUIRE(res_normal == res_naive);
                        REQUIRE(res_naive == res_inplace);
                        REQUIRE(res_normal == res_inplace);
                    }
                }
            }
        }
    }
}

SCENARIO("Division of signed integers", "[integer][signed][arithmetic][division]")
{

    GIVEN("The number 1 << 65")
    {
        constexpr integer<70> m = (integer<70>::one() << 65);

        constexpr integer<70> two{2};

        WHEN("Repeatedly dividing by two")
        {
            THEN("The division should work correctly over word boundaries")
            {
                const auto div1 = restoring_division(m, two);

                CHECK(div1.first.word(1) == 1U);
                CHECK(div1.first.word(0) == 0U);
                CHECK(div1.second == integer<70>::zero());

                const auto div2 = restoring_division(div1.first, two);

                CHECK(div2.first.word(1) == 0U);
                CHECK(div2.first.word(0) == (int64_t(1) << int64_t(63)));
                CHECK(div2.second == integer<70>::zero());
            }
        }
    }

    GIVEN("A signed integer<64> n != INT_MIN")
    {

        const int64_t n_ = GENERATE(take(
            100, random(std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::max())));
        const integer<64> n{n_};

        WHEN("Dividing zero by this number")
        {
            THEN("The result should be zero")
            {
                if (!n.is_zero())
                {
                    const auto result = restoring_division(integer<64>::zero(), n);
                    CHECK(result.second == integer<64>::zero());
                    REQUIRE(result.first == integer<64>::zero());
                }
                else
                {
                    REQUIRE_THROWS_AS(restoring_division(n, n), std::runtime_error);
                }
            }
        }

        WHEN("Dividing by itself")
        {
            AND_WHEN("n is not equal to zero")
            {
                THEN("The result should be one without remainder")
                {
                    if (!n.is_zero())
                    {
                        const auto result = restoring_division(n, n);
                        CHECK(result.second == integer<64>::zero());
                        REQUIRE(result.first == integer<61>::one());
                    }
                    else
                    {
                        REQUIRE_THROWS_AS(restoring_division(n, n), std::runtime_error);
                    }
                }
            }
        }

        WHEN("Dividing by zero")
        {
            THEN("An exception should be raised")
            {
                REQUIRE_THROWS_AS(restoring_division(n, integer<64>::zero()), std::runtime_error);
            }
        }

        WHEN("Dividing by one")
        {
            THEN("The result should be the number itself")
            {

                const auto result = restoring_division(n, integer<64>::one());
                CHECK(result.second == integer<64>::zero());
                REQUIRE(result.first == n);
            }
        }

        WHEN("Dividing by minus one")
        {
            THEN("The result should flip its sign")
            {

                const auto result = restoring_division(n, integer<64>::minus_one());
                CHECK(result.second == integer<64>::zero());
                REQUIRE(result.first == -n);
            }
        }

        AND_GIVEN("Another signed integer<64> m != INT_MIN")
        {
            const int64_t m_ = GENERATE(take(100, random(std::numeric_limits<int64_t>::max(),
                                                         std::numeric_limits<int64_t>::max())));
            const integer<64> m{m_};
            WHEN("Dividing the numbers")
            {
                THEN("The behaviour should match its int64_t counterpart")
                {
                    const auto result64_div = n_ / m_;
                    const auto result64_rem = n_ % m_;
                    const auto result = restoring_division(n, m);
                    CHECK(result.first.word(0) ==
                          static_cast<integer<64>::word_type>(result64_div));
                    CHECK(result.second.word(0) ==
                          static_cast<integer<64>::word_type>(result64_rem));
                }
            }
        }
    }

    GIVEN("INT_MIN")
    {
        AND_GIVEN("Minus one")
        {
            WHEN("Dividing INT_MIN by minus one")
            {
                THEN("The result should be INT_MIN")
                {
                    const auto result =
                        restoring_division(integer<64>::min(), integer<64>::minus_one());
                    CHECK(result.first == integer<64>::min());
                    CHECK(result.second == integer<64>::zero());
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Invariants for the signed integer division",
                       "[integer][signed][arithmetic][division]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;

    GIVEN("A non-zero number")
    {

        const I a = GENERATE(take(100, random_integer<W, WordType>()));

        WHEN("Dividing the number by itself")
        {
            THEN("The result should be 1")
            {
                if (a != I::zero())
                {
                    REQUIRE(div(a, a) == I::one());
                }
            }
        }
        WHEN("Dividing the number by one")
        {
            THEN("The result should be the number itself")
            {
                REQUIRE(div(a, I::one()) == a);
            }
        }
        WHEN("Dividing the number by a larger number (in absolute terms)")
        {
            THEN("The result should be zero")
            {

                if (a.is_negative())
                {
                    if (a != I::min())
                    {
                        const I c = sub(a, I::one());
                        if (div(a, c) != I::zero())
                        {
                            std::cout << a << "/" << c << " == " << div(a, c) << " should be 1"
                                      << "\n";
                        }
                        REQUIRE(div(a, c) == I::zero());
                    }
                }
                else
                {
                    if (a != I::max())
                    {
                        const I c = add(a, I::one());
                        if (c != I::one())
                        {
                            REQUIRE(div(a, c) == I::zero());
                        }
                    }
                }
            }
        }
        WHEN("Dividing the number by zero")
        {
            THEN("A runtime exception should be thrown")
            {
                REQUIRE_THROWS_AS(div(a, I::zero()), std::runtime_error);
            }
        }
    }
}

SCENARIO("Multiplying signed integers using Booth's algorithm",
         "[integer][signed][arithmetic][multiplication]")
{
    GIVEN("Two signed integers m and r")
    {

        WHEN("The numbers are the example from Wikipedia")
        {
            AND_WHEN("It is the 'normal' example")
            {
                THEN("The result should be correct")
                {
                    using T = integer<4>;
                    using R = integer<8>;
                    constexpr T m{3U};
                    constexpr T r{-4};

                    static constexpr auto res = booth_expanding_mul(m, r);
                    static constexpr auto res_naive = naive_expanding_mul(m, r);
                    static constexpr auto res_inplace = booth_inplace_expanding_mul(m, r);

                    static constexpr R expected{-12};

                    CHECK(res == expected);
                    CHECK(res == res_inplace);
                    REQUIRE(res == res_naive);
                }
            }
            AND_WHEN("The example involves the most negative number")
            {
                THEN("The result should still be correct")
                {
                    using T = integer<4>;
                    using R = integer<8>;
                    constexpr T m{-8};
                    constexpr T r{2U};

                    static constexpr auto res = booth_expanding_mul(m, r);
                    static constexpr auto res_naive = naive_expanding_mul(m, r);
                    static constexpr auto res_inplace = booth_inplace_expanding_mul(m, r);

                    static constexpr R expected{-16};
                    CHECK(res == expected);
                    CHECK(res == res_inplace);
                    REQUIRE(res == res_naive);
                }
            }
        }
        WHEN("m is the most negative number")
        {
            THEN("The algorithm should still work")
            {
                constexpr integer<8> m{-16};
                constexpr integer<8> r{2};

                const integer<8> res = booth_mul(m, r);
                const integer<8> res_naive = naive_mul(m, r);
                const integer<8> res_inplace = booth_inplace_mul(m, r);

                int8_t mi = -16;
                int8_t ri = 2;

                int8_t resi = mi * ri;
                CHECK(res == res_naive);
                CHECK(res == res_inplace);
                CHECK((uint8_t)res.word(0) == (uint8_t)resi);
            }
        }
    }
}

SCENARIO("Multiplying two integers exactly", "[integer][signed][arithmetic][multiplication]")
{

    GIVEN("Two integer<N> a and b with N <= 32")
    {

        int32_t val_a =
            GENERATE(0, 1, 56567, 23, static_cast<int32_t>(-4366), static_cast<int32_t>(-15654),
                     std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max());
        int32_t val_b = GENERATE(1, 56567, 23, 234, 76856, 2342353456, static_cast<int32_t>(-4366),
                                 static_cast<int32_t>(-1457));

        const integer<32> a = integer<32>::from_words(val_a);
        const integer<32> b = integer<32>::from_words(val_b);

        THEN("Multiplication should be commutative")
        {
            CHECK(mul(a, b) == mul(b, a));
        }

        THEN("Multiplication by 1 should not change the other multiplicand")
        {
            const integer<32> one{1U};
            CHECK(naive_mul(a, one) == a);
            CHECK(naive_mul(one, a) == a);
            CHECK(naive_mul(b, one) == b);
            CHECK(naive_mul(one, b) == b);

            CHECK(booth_mul(a, one) == a);
            CHECK(booth_mul(one, a) == a);
            CHECK(booth_mul(b, one) == b);
            CHECK(booth_mul(one, b) == b);

            CHECK(booth_inplace_mul(a, one) == a);
            CHECK(booth_inplace_mul(one, a) == a);
            CHECK(booth_inplace_mul(b, one) == b);
            CHECK(booth_inplace_mul(one, b) == b);
        }
        THEN("Multiplication by 0 should result in 0")
        {
            const integer<32> zero{0U};
            CHECK(naive_mul(a, zero) == zero);
            CHECK(naive_mul(zero, a) == zero);
            CHECK(naive_mul(b, zero) == zero);
            CHECK(naive_mul(zero, b) == zero);

            CHECK(booth_mul(a, zero) == zero);
            CHECK(booth_mul(zero, a) == zero);
            CHECK(booth_mul(b, zero) == zero);
            CHECK(booth_mul(zero, b) == zero);

            CHECK(booth_inplace_mul(a, zero) == zero);
            CHECK(booth_inplace_mul(zero, a) == zero);
            CHECK(booth_inplace_mul(b, zero) == zero);
            CHECK(booth_inplace_mul(zero, b) == zero);
        }

        THEN("Multiplication by -1 should negate the sign")
        {
            const integer<32> minus_one = integer<32>::minus_one();

            const integer<32> mul_res1 = naive_mul(b, minus_one);
            const integer<32> mul_res2 = naive_mul(minus_one, b);

            const integer<32> mul_res1b = booth_mul(b, minus_one);
            const integer<32> mul_res2b = booth_mul(minus_one, b);

            const integer<32> mul_res1i = booth_inplace_mul(b, minus_one);
            const integer<32> mul_res2i = booth_inplace_mul(minus_one, b);

            CHECK(mul_res1.is_negative() == !b.is_negative());
            CHECK(mul_res2.is_negative() == !b.is_negative());
            CHECK(mul_res1 == mul_res2);
            CHECK(mul_res1 == -b);

            CHECK(mul_res1b.is_negative() == !b.is_negative());
            CHECK(mul_res2b.is_negative() == !b.is_negative());
            CHECK(mul_res1b == mul_res2b);
            CHECK(mul_res1b == -b);

            CHECK(mul_res1i.is_negative() == !b.is_negative());
            CHECK(mul_res2i.is_negative() == !b.is_negative());
            CHECK(mul_res1i == mul_res2i);
            CHECK(mul_res1i == -b);
        }
    }

    GIVEN("Two integer<N> a and b to be multiplied")
    {
        constexpr int64_t val = (static_cast<int64_t>(1) << 35);
        integer<128> constexpr a = integer<128>::from_words(1, val);
        integer<128> constexpr c = integer<128>::from_words(13435, 345897);
        integer<128> constexpr d =
            integer<128>::from_words(static_cast<typename integer<128>::word_type>(-1),
                                     static_cast<typename integer<128>::word_type>(-1));
        integer<128> constexpr zero = integer<128>::from_words(0, 0);
        integer<128> constexpr one = integer<128>::from_words(0, 1);

        const std::vector<integer<128>> numbers{a, c, d, one, zero};

        THEN("The operation should be commutative")
        {
            for (const integer<128>& num_a : numbers)
            {
                for (const integer<128>& num_b : numbers)
                {
                    CHECK(naive_mul(num_a, num_b) == naive_mul(num_b, num_a));
                    CHECK(booth_mul(num_a, num_b) == booth_mul(num_b, num_a));
                    CHECK(booth_inplace_mul(num_a, num_b) == booth_inplace_mul(num_b, num_a));
                }
            }
        }

        WHEN("One multiplicant is zero")
        {

            THEN("The result should be zero")
            {
                for (const integer<128>& num : numbers)
                {
                    CHECK(naive_mul(num, zero) == zero);
                    CHECK(naive_mul(zero, num) == zero);

                    CHECK(booth_mul(num, zero) == zero);
                    CHECK(booth_mul(zero, num) == zero);

                    CHECK(booth_inplace_mul(num, zero) == zero);
                    CHECK(booth_inplace_mul(zero, num) == zero);
                }
            }
        }
        WHEN("One multiplicand is one")
        {
            THEN("Multiplication does not do much")
            {

                for (const integer<128>& num : numbers)
                {
                    CHECK(naive_mul(num, one) == num);
                    CHECK(naive_mul(one, num) == num);

                    CHECK(booth_mul(num, one) == num);
                    CHECK(booth_mul(one, num) == num);

                    CHECK(booth_inplace_mul(num, one) == num);
                    CHECK(booth_inplace_mul(one, num) == num);
                }
            }
        }
        WHEN("Both multiplicands are maximum")
        {
            THEN("The product is 1 for the truncating multiplication")
            {
                REQUIRE(naive_mul(d, d) == one);
                REQUIRE(booth_mul(d, d) == one);
                REQUIRE(booth_inplace_mul(d, d) == one);
            }
        }
    }
}

SCENARIO("Multiplication of numbers fitting in a uint64_t",
         "[integer][signed][arithmetic][multiplication]")
{
    GIVEN("A random number a")
    {
        int64_t val_a = GENERATE(take(
            100, random(std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max())));
        integer<64> a{val_a};
        AND_GIVEN("A random number b")
        {
            int64_t val_b = GENERATE(take(100, random(std::numeric_limits<int64_t>::min(),
                                                      std::numeric_limits<int64_t>::max())));
            integer<64> b{val_b};

            THEN("The multiplication should match its uint64_t counterpart")
            {
                int64_t expected = val_a * val_b;

                integer<64> result = naive_mul(a, b);
                integer<64> resultb = booth_mul(a, b);
                integer<64> resulti = booth_inplace_mul(a, b);

                integer<64> expected_integer{expected};

                CHECK(expected == result.word(0));
                REQUIRE(expected_integer == result);

                CHECK(expected == resultb.word(0));
                REQUIRE(expected_integer == resultb);

                CHECK(expected == resulti.word(0));
                REQUIRE(expected_integer == resulti);
            }
        }
    }
}

SCENARIO("Multiplication of larger numbers", "[integer][signed][arithmetic][multiplication]")
{

    GIVEN("Two large integer numbers")
    {
        using aint = integer<1024>;
        using aintR = integer<2048>;

        using I = integer<128>;
        using IR = integer<256>;

        I a_ = I::one();
        I b_ = I::one();

        a_ = a_ << 126;
        b_ = b_ << 63;
        static constexpr IR expected128{IR::one() << (126 + 63)};

        aint a = aint::one();
        aint b = aint::one();
        a = a << 1022;
        b = b << 511;
        static constexpr aintR expected1024{aintR::one() << (1022 + 511)};

        THEN("The product should be correct")
        {
            const auto result_booth = booth_expanding_mul(a, b);
            const auto result_naive = naive_expanding_mul(a, b);
            const auto result_inplace = booth_inplace_expanding_mul(a, b);

            const auto result_booth_ = booth_expanding_mul(a_, b_);
            const auto result_naive_ = naive_expanding_mul(a_, b_);
            const auto result_inplace_ = booth_inplace_expanding_mul(a_, b_);

            CHECK(result_booth == result_naive);
            CHECK(result_naive == result_inplace);
            CHECK(result_booth == result_inplace);
            CHECK(result_booth == expected1024);

            CHECK(result_booth_ == result_naive_);
            CHECK(result_booth_ == result_inplace_);
            CHECK(result_booth_ == expected128);

            REQUIRE(result_booth_ != I::zero());   // test case coming from a found bug
            REQUIRE(result_booth != aint::zero()); // test case coming from a found bug
        }
    }
}

SCENARIO("Absolute value computation", "[integer][signed][operations][utility]")
{
    GIVEN("The smallest possible value")
    {
        constexpr integer<150> min = std::numeric_limits<integer<150>>::min();
        THEN("The absolute value of that value is the value again")
        {
            const integer<150> absolute = abs(min);
            REQUIRE(absolute == min);
        }

        THEN("The 'real' absolute value is 2^(W-1)")
        {
            const uinteger<150> abs = expanding_abs(min);
            CHECK(abs.word(0) == 0U);
            CHECK(abs.word(1) == 0U);
            REQUIRE(abs.word(2) == (1U << 21U));
        }
    }

    GIVEN("Any non-smallest value")
    {
        using sint = integer<64>;
        const int32_t val_32 =
            GENERATE(take(50, random(std::numeric_limits<int32_t>::min() + 1, -1)));
        const int64_t val_64 = GENERATE(
            take(50, random(std::numeric_limits<int64_t>::min() + 1, static_cast<int64_t>(-1))));
        const sint a{val_32};
        const sint b{val_64};

        THEN("Computing abs is idempotent")
        {
            REQUIRE(abs(abs(a)) == abs(a));
            REQUIRE(abs(abs(b)) == abs(b));

            // not "really" applying it twice but close enough
            REQUIRE(abs(sint(expanding_abs(a))) == sint(expanding_abs(a)));
            REQUIRE(abs(sint(expanding_abs(b))) == sint(expanding_abs(b)));
        }

        // does not work for int64 as, for some reason, there is no matching abs function for that
        THEN("Computing abs should match its int32 type counterpart")
        {
            REQUIRE(abs(a).word(0) == abs(val_32));
            REQUIRE(expanding_abs(a).word(0) == abs(val_32));
        }
    }
}

SCENARIO("Expanding subtraction works correctly", "[integer][signed][arithmetic][subtraction]")
{
    GIVEN("A n-bit min and a m-bit (m>n)  max")
    {
        static constexpr integer<4> min4 = integer<4>::min();
        static const integer<8> max8 = integer<8>::max();
        static constexpr integer<8> expected = integer<8>{-135};

        THEN("Subtracting max from min should correctly yield (min-max) as it now fits the width")
        {
            const auto result = expanding_sub(min4, max8);
            REQUIRE(result == expected);
        }
    }

    GIVEN("A n-bit min and a m-bit (m<n) max")
    {
        static constexpr integer<8> min8 = integer<8>::min();
        static const integer<4> max4 = integer<4>::max();
        static const integer<8> expected =
            add(sub(integer<8>::max(), integer<8>(integer<4>::max())), integer<8>{1U});

        THEN("Subtracting max from min should give (max-min)+1")
        {
            auto const result = expanding_sub(min8, max4);
            REQUIRE(result == expected);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Unary minus operation", "[integer][signed][arithmetic][utility]",
                       AARITH_INT_TEST_SIGNATURE, AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;
    GIVEN("An integer")
    {
        const I a = GENERATE(take(20, random_integer<W, WordType>()));
        THEN("Unary minus is self-inverse")
        {
            if (a != I::min())
            {
                REQUIRE(-(-a) == a);
            }
        }
        THEN("Unary minus changes the sign")
        {
            if (a != I::zero() && a != I::min())
            {
                REQUIRE(signum(-a) == -signum(a));
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("MIN/MAX Values behave as expected", "[integer][signed][operation][utility]",
                       AARITH_INT_TEST_SIGNATURE, AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;
    GIVEN("The min and max value")
    {
        constexpr I min = I::min();
        constexpr I max = I::max();
        constexpr I one{I::one()};

        THEN("Adding/subtracting one should wrap around")
        {
            const auto sum = add(max, one);
            REQUIRE(sum == min);
            REQUIRE(sum == fun_add(max, one));

            REQUIRE(sub(min, one) == max);
        }

        THEN("Taking the absolute value of the min value yields the same value")
        {
            const auto abs_min = abs(min);
            REQUIRE(abs_min == min);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Computing the signum of an non-zero integer",
                       "[integer][signed][operation][utility]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = integer<W, WordType>;
    GIVEN("The number  zero")
    {
        THEN("The signum should be zero")
        {
            REQUIRE(signum(I::zero()) == 0);
        }
    }
}

SCENARIO("Computing the signum of an non-zero integer", "[integer][signed][operation][utility]")
{

    GIVEN("A non-negative number")
    {
        THEN("The signum should be one")
        {
            int8_t val_a =
                GENERATE(take(100, random(int8_t(1), std::numeric_limits<int8_t>::max())));

            REQUIRE(signum(integer<8>{val_a}) == 1);
            REQUIRE(signum(integer<16>{val_a}) == 1);
            REQUIRE(signum(integer<17>{val_a}) == 1);
            REQUIRE(signum(integer<32>{val_a}) == 1);
            REQUIRE(signum(integer<64>{val_a}) == 1);
            REQUIRE(signum(integer<128>{val_a}) == 1);
            REQUIRE(signum(integer<256>{val_a, val_a}) == 1);
        }
    }
    GIVEN("A negative number")
    {
        THEN("The signum should be one")
        {
            int8_t val_a =
                GENERATE(take(100, random(std::numeric_limits<int8_t>::min(), int8_t(-1))));

            REQUIRE(signum(integer<8>{val_a}) == -1);
            REQUIRE(signum(integer<16>{val_a}) == -1);
            REQUIRE(signum(integer<17>{val_a}) == -1);
            REQUIRE(signum(integer<32>{val_a}) == -1);
            REQUIRE(signum(integer<64>{val_a}) == -1);
            REQUIRE(signum(integer<128>{val_a}) == -1);
        }
    }
}

SCENARIO("Computing the powers of integers", "[integer][signed][operation]")
{

    GIVEN("Base and exponent values")
    {
        THEN("Compute the correct power")
        {
            using Integer = integer<32>;

            CHECK(pow(Integer(2), Integer(0)) == Integer(1));
            CHECK(pow(Integer(2), Integer(1)) == Integer(2));
            CHECK(pow(Integer(2), Integer(2)) == Integer(4));
            CHECK(pow(Integer(2), Integer(3)) == Integer(8));
            CHECK(pow(Integer(2), Integer(4)) == Integer(16));
        }
    }
}

SCENARIO("Computing the integer logarithm of integers", "[integer][signed][operation]")
{
    GIVEN("Some integers")
    {
        THEN("Compute the integer base 2 logarithm")
        {
            using Integer = integer<32>;

            constexpr auto base = Integer(2);

            CHECK(ilog(Integer(2LL), base) == Integer(1));
            CHECK(ilog(Integer(3LL), base) == Integer(1));
            CHECK(ilog(Integer(4LL), base) == Integer(2));
            CHECK(ilog(Integer(5LL), base) == Integer(2));
            CHECK(ilog(Integer(8LL), base) == Integer(3));
            CHECK(ilog(Integer(9LL), base) == Integer(3));
            CHECK(ilog(Integer(16LL), base) == Integer(4));
            CHECK(ilog(Integer(17LL), base) == Integer(4));
            CHECK(ilog(Integer(32LL), base) == Integer(5));
            CHECK(ilog(Integer(45LL), base) == Integer(5));
            CHECK(ilog(Integer(64LL), base) == Integer(6));
            CHECK(ilog(Integer(100LL), base) == Integer(6));
            CHECK(ilog(Integer(128LL), base) == Integer(7));
            CHECK(ilog(Integer(200LL), base) == Integer(7));
            CHECK(ilog(Integer(256LL), base) == Integer(8));
            CHECK(ilog(Integer(300LL), base) == Integer(8));
        }

        THEN("Compute the integer base 10 logarithm")
        {
            using Integer = integer<32>;

            constexpr auto base = Integer(10);

            CHECK(ilog(Integer(10LL), base) == Integer(1));
            CHECK(ilog(Integer(11LL), base) == Integer(1));
            CHECK(ilog(Integer(100LL), base) == Integer(2));
            CHECK(ilog(Integer(120LL), base) == Integer(2));
            CHECK(ilog(Integer(1000LL), base) == Integer(3));
            CHECK(ilog(Integer(1300LL), base) == Integer(3));
            CHECK(ilog(Integer(10000LL), base) == Integer(4));
            CHECK(ilog(Integer(14000LL), base) == Integer(4));
            CHECK(ilog(Integer(100000LL), base) == Integer(5));
            CHECK(ilog(Integer(150000LL), base) == Integer(5));
            CHECK(ilog(Integer(1000000LL), base) == Integer(6));
            CHECK(ilog(Integer(1600000LL), base) == Integer(6));
            CHECK(ilog(Integer(10000000LL), base) == Integer(7));
            CHECK(ilog(Integer(17000000LL), base) == Integer(7));
        }
    }
}

SCENARIO("Computing the greatest common divisor of integers", "[integer][signed][operation]")
{
    GIVEN("Some integers")
    {
        THEN("Compute their GCD correctly")
        {
            using Integer = integer<32>;

            CHECK(gcd(Integer(12), Integer(15)) == Integer(3));
            CHECK(gcd(Integer(9), Integer(10)) == Integer(1));
            CHECK(gcd(Integer(9), Integer(21)) == Integer(3));
            CHECK(gcd(Integer(1701), Integer(3768)) == Integer(3));
            CHECK(gcd(Integer(12345678), Integer(87654321)) == Integer(9));
        }
    }
}
