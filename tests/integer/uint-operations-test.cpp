#include "../test-signature-ranges.hpp"
#include "gen_integer.hpp"
#include <aarith/integer_no_operators.hpp>
#include <catch.hpp>

using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Unsigned integer addition is commutative",
                       "[integer][unsigned][arithmetic][addition]", AARITH_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = uinteger<W, WordType>;
    using R = uinteger<2 * W, WordType>;

    I a = GENERATE(take(10, random_uinteger<W, WordType>()));
    I b = GENERATE(take(10, random_uinteger<W, WordType>()));

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
    "Unsigned integer addition is commutative for addends with different bit widths",
    "[integer][unsigned][arithmetic][addition]", AARITH_TEST_SIGNATURE,
    AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = uinteger<W, WordType>;
    using ISmall = uinteger<W - 2, WordType>;
    using ILarge = uinteger<W + I::word_width() + 3, WordType>;

    I a = GENERATE(take(5, random_uinteger<W, WordType>()));
    ISmall b = GENERATE(take(5, random_uinteger<W - 2, WordType>()));
    ILarge c = GENERATE(take(5, random_uinteger<W + I::word_width() + 3, WordType>()));

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
                       "[integer][unsigned][arithmetic][addition]", AARITH_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = uinteger<W, WordType>;
    I a = GENERATE(take(100, random_uinteger<W, WordType>()));
    REQUIRE(add(a, I::zero()) == a);
    REQUIRE(expanding_add(a, I::zero()) == a);
}



TEMPLATE_TEST_CASE_SIG("Addition wraps around correctly",
                       "[integer][unsigned][arithmetic][addition]", AARITH_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = uinteger<W, WordType>;

    static constexpr I zero = I::zero();
    static constexpr I one = I::one();
    static constexpr I max_val = I::max();

    WHEN("Adding one to max")
    {
        THEN("The result should be zero")
        {
            REQUIRE(add(max_val, one) == zero);
        }
    }
    AND_WHEN("Adding another one") {
        THEN("The result should be one") {
            REQUIRE(add(add(max_val, one), one) == one);
        }
    }
    WHEN("Adding max to max") {
        THEN("The result should be max - 1") {
            REQUIRE(add(max_val, max_val) == sub(max_val, one));
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Zero is the neutral element of the subtraction",
                       "[integer][unsigned][arithmetic][subtraction]", AARITH_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = uinteger<W, WordType>;
    I a = GENERATE(take(100, random_uinteger<W, WordType>()));
    REQUIRE(sub(a, I::zero()) == a);
    REQUIRE(expanding_sub(a, I::zero()) == a);
}

TEMPLATE_TEST_CASE_SIG("Expanding subtraction wraps around correctly for different bit-widths",
                       "[integer][unsigned][arithmetic][subtraction]", AARITH_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = uinteger<W, WordType>;
    using LargeI = uinteger<2 * W + 6, WordType>;

    GIVEN("A n-bit zero and a m-bit (m>n)  max")
    {
        static constexpr I zero = I::zero();
        static constexpr LargeI max_val = LargeI::max();
        static constexpr LargeI expected = LargeI::one();

        THEN("Subtracting max from zero should give one")
        {
            auto constexpr result = expanding_sub(zero, max_val);
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
                       "[integer][unsigned][arithmetic][subtraction]", AARITH_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = uinteger<W, WordType>;

    static constexpr I zero = I::zero();
    static constexpr I one = I::one();
    static constexpr I max_val = I::max();

    WHEN("Subtracting max from zero")
    {
        THEN("The result should be one")
        {
            auto constexpr result = expanding_sub(zero, max_val);
            REQUIRE(result == one);
        }
    }

    WHEN("Subtracting one from zero")
    {
        THEN("The result should be max")
        {
            auto constexpr result = expanding_sub(zero, one);
            REQUIRE(result == max_val);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Unsigned integer multiplication is commutative",
                       "[integer][unsigned][arithmetic][multiplication]", AARITH_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = uinteger<W, WordType>;
    using R = uinteger<2 * W, WordType>;

    I a = GENERATE(take(10, random_uinteger<W, WordType>()));
    I b = GENERATE(take(10, random_uinteger<W, WordType>()));

    WHEN("Computing with extended bit widths")
    {
        R res_a_b_school{schoolbook_expanding_mul(a, b)};
        R res_b_a_school{schoolbook_expanding_mul(b, a)};
        R res_a_b_kara{expanding_karazuba(a, b)};
        R res_b_a_kara{expanding_karazuba(b, a)};
        THEN("The the flipped results should match")
        {
            REQUIRE(res_a_b_school == res_b_a_school);
            REQUIRE(res_a_b_kara == res_b_a_kara);
        }
        AND_THEN("The results of the different methods should match")
        {
            REQUIRE(res_a_b_school == res_a_b_kara);
        }
    }

    WHEN("Computing with truncation")
    {
        I res_a_b_school{schoolbook_mul(a, b)};
        I res_b_a_school{schoolbook_mul(b, a)};
        I res_a_b_kara{karazuba(a, b)};
        I res_b_a_kara{karazuba(b, a)};
        THEN("The the flipped results should match")
        {
            REQUIRE(res_a_b_school == res_b_a_school);
            REQUIRE(res_a_b_kara == res_b_a_kara);
        }
        AND_THEN("The results of the different methods should match")
        {
            REQUIRE(res_a_b_school == res_a_b_kara);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("One is the neutral element of the multiplication",
                       "[integer][unsigned][arithmetic][multiplication]", AARITH_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = uinteger<W, WordType>;
    I a = GENERATE(take(50, random_uinteger<W, WordType>()));
    REQUIRE(schoolbook_mul(a, I::one()) == a);
    REQUIRE(schoolbook_expanding_mul(a, I::one()) == a);
    REQUIRE(karazuba(a, I::one()) == a);
    REQUIRE(expanding_karazuba(a, I::one()) == a);
}

TEMPLATE_TEST_CASE_SIG("Zero makes the multiplication result zero",
                       "[integer][unsigned][arithmetic][multiplication]", AARITH_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = uinteger<W, WordType>;
    using R = uinteger<2 * W, WordType>;
    I a = GENERATE(take(50, random_uinteger<W, WordType>()));
    REQUIRE(schoolbook_mul(a, I::zero()) == I::zero());
    REQUIRE(schoolbook_expanding_mul(a, I::zero()) == R::zero());
    REQUIRE(karazuba(a, I::zero()) == I::zero());
    REQUIRE(expanding_karazuba(a, I::zero()) == R::zero());
}

TEMPLATE_TEST_CASE_SIG("Multiplying the max value with itself and truncation yields 1",
                       "[integer][unsigned][arithmetic][multiplication]", AARITH_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = uinteger<W, WordType>;
    I a{I::max()};
    REQUIRE(schoolbook_mul(a, a) == I::one());
    REQUIRE(karazuba(a, a) == I::one());
}

SCENARIO("Adding two unsigned integers exactly", "[integer][unsigned][arithmetic][addition]")
{
    GIVEN("Two uinteger<N> a and b with N <= word_width")
    {
        static constexpr size_t TestWidth = 16;
        static_assert(uinteger<TestWidth>::word_count() == 1);

        WHEN("The result a+b still fits into N bits")
        {
            static constexpr uint8_t number_a = 32;
            static constexpr uint8_t number_b = 16;
            constexpr uinteger<TestWidth> a{number_a};
            constexpr uinteger<TestWidth> b{number_b};
            constexpr uinteger<TestWidth> result = add(a, b);
            auto constexpr result_fun = fun_add(a, b);

            THEN("It should be the correct sum")
            {
                CHECK(result_fun == result);
                REQUIRE(result.word(0) == number_a + number_b);
            }
        }
        WHEN("The result a+b does not fit into N bits")
        {
            static constexpr uint16_t number_a = (1U << TestWidth) - 1U;
            static constexpr uint16_t number_b = 1;
            constexpr uinteger<TestWidth> a{number_a};
            constexpr uinteger<TestWidth> b{number_b};
            auto constexpr result = add(a, b);
            auto constexpr result_fun = fun_add(a, b);

            THEN("It should be the masked to fit")
            {
                CHECK(result == result_fun);
                REQUIRE(result.word(0) == 0);
            }
        }
    }

    GIVEN("Two uinteger<N> a and b with N > word_width")
    {
        static constexpr size_t TestWidth = 128;
        static_assert(uinteger<TestWidth>::word_count() > 1);

        WHEN("There is a carry into the next word")
        {
            static constexpr uint64_t number_a = 1ULL << 63U;
            static constexpr uint64_t number_b = 1ULL << 63U;
            constexpr uinteger<TestWidth> a{number_a};
            constexpr uinteger<TestWidth> b{number_b};
            auto constexpr result = add(a, b);
            auto constexpr result_fun = fun_add(a, b);

            THEN("It is added to the next word")
            {
                CHECK(result_fun == result);
                REQUIRE(result.word(1) == 1);
            }
        }

        WHEN("There is no carry into the next word")
        {
            static constexpr uint64_t number_a = 1ULL << 63U;
            static constexpr uint64_t number_b = 0;
            constexpr uinteger<TestWidth> a{number_a};
            constexpr uinteger<TestWidth> b{number_b};
            auto constexpr result = add(a, b);
            auto constexpr result_fun = fun_add(a, b);

            THEN("The next word is unchanged")
            {
                CHECK(result_fun == result);
                REQUIRE(result.word(1) == 0);
            }
        }
    }

    GIVEN("Two uinteger<N> a and b with N > 2*word_width")
    {
        using auint = uinteger<129>;
        static_assert(auint::word_count() > 2);

        WHEN("There should be a carry into the third word")
        {
            constexpr auto a = auint::from_words(0, 0xffffffffffffffff, 0xffffffffffffffff);
            constexpr auto b = a;
            auto constexpr result = add(a, b);
            auto constexpr result_fun = fun_add(a, b);
            constexpr auto ref = auint::from_words(1, 0xffffffffffffffff, 0xfffffffffffffffe);

            THEN("There is a carry in the third word")
            {
                CHECK(result_fun == result);
                REQUIRE(result == ref);
            }
        }

        WHEN("There should be no carry into the third word")
        {
            constexpr auto a = auint::from_words(0, 0xfffffffffffffffe, 0xffffffffffffffff);
            constexpr auto b = auint(1U);
            auto constexpr result = add(a, b);
            constexpr auto ref = auint::from_words(0, 0xffffffffffffffff, 0);
            auto constexpr result_fun = fun_add(a, b);

            THEN("There is no carry in the third word")
            {
                CHECK(result_fun == result);
                REQUIRE(result == ref);
            }
        }
    }
}

SCENARIO("Bit shifting is possible as constexpr for unsigned integers",
         "[integer][unsigned][utility][bit_logic][constexpr]")
{
    GIVEN("An unsigned integer of width N")
    {
        THEN("Right-shifting should be a constexpr operation")
        {
            constexpr size_t W = 32;
            constexpr uinteger<W> num{4};

            constexpr uinteger<W> shift1{2};
            constexpr uinteger<W> shift2{1};
            constexpr uinteger<W> shift3{0};

            constexpr uinteger<W> shifted1 = (num >> 1);
            constexpr uinteger<W> shifted2 = (num >> 2);
            constexpr uinteger<W> shifted3 = (num >> 3);

            REQUIRE(shift1 == shifted1);
            REQUIRE(shift2 == shifted2);
            REQUIRE(shift3 == shifted3);
        }
        THEN("Left-shifting should be a constexpr operation")
        {
            constexpr size_t W = 32;
            constexpr uinteger<W> num{4};

            constexpr uinteger<W> shift1{8};
            constexpr uinteger<W> shift2{16};
            constexpr uinteger<W> shift3{32};

            constexpr uinteger<W> shifted1 = (num << 1);
            constexpr uinteger<W> shifted2 = (num << 2);
            constexpr uinteger<W> shifted3 = (num << 3);

            REQUIRE(shift1 == shifted1);
            REQUIRE(shift2 == shifted2);
            REQUIRE(shift3 == shifted3);
        }
    }
}

SCENARIO("Adding two unsigned integers of different bit width",
         "[integer][unsigned][arithmetic][addition]")
{
    GIVEN("An uinteger of bit width 128")
    {

        static const uinteger<128> large = uinteger<128>::from_words(1U, 0U);
        static const uint32_t m_ = GENERATE(
            take(100, random(static_cast<uint32_t>(0U), std::numeric_limits<uint32_t>::max())));

        static const uinteger<32> m{m_};

        THEN("Adding a small number of bit width 32 should not change the second word")
        {
            uinteger<129> result = expanding_add(large, m);
            auto const result_fun = fun_add_expand(large, m);
            CHECK(result_fun == result);
            CHECK(result.word(2) == 0U);
            CHECK(result.word(1) == 1U);
            REQUIRE(result.word(0) == m_);
        }

        THEN("The addition with 32 bits should still be commutative")
        {
            uinteger<129> result1 = expanding_add(large, m);
            uinteger<129> result2 = expanding_add(m, large);
            auto const result_fun1 = fun_add_expand(large, m);
            auto const result_fun2 = fun_add_expand(m, large);
            REQUIRE(result_fun1 == result1);
            REQUIRE(result_fun2 == result2);
            REQUIRE(result1 == result2);
        }
    }
    GIVEN("An unsigned integer consisting of zeros only")
    {
        static constexpr uint64_t ones = static_cast<uint64_t>(-1);
        static constexpr uinteger<128> large = uinteger<128>::from_words(ones, ones);

        THEN("Adding a one with few bits should create a correct overflow")
        {

            static constexpr uinteger<32> one{1U};

            constexpr uinteger<129> result = expanding_add(large, one);
            const auto result_fun = fun_add_expand(large, one);
            CHECK(result_fun == result);
            CHECK(result.word(2) == 1U);
            CHECK(result.word(1) == 0U);
            REQUIRE(result.word(0) == 0U);
        }
    }
}

SCENARIO("Subtracting two unsigned integers exactly",
         "[integer][unsigned][arithmetic][subtraction]")
{
    GIVEN("Two uinteger<N> a and b are subtracted")
    {
        WHEN("N <= word_width")
        {
            AND_WHEN("a >= b")
            {
                constexpr uinteger<32> a{15u};
                constexpr uinteger<32> b{7u};
                constexpr uinteger<32> expected{static_cast<uint32_t>(15 - 7)};
                auto constexpr result = sub(a, b);

                THEN("The result is a - b")
                {
                    REQUIRE(result == expected);
                }
            }
            AND_WHEN("a < b")
            {
                constexpr uinteger<32> a{7u};
                constexpr uinteger<32> b{15u};
                constexpr uinteger<32> expected{static_cast<uint32_t>(7 - 15)};
                auto constexpr result = sub(a, b);

                THEN("The result wraps around")
                {
                    REQUIRE(result == expected);
                }
            }
        }
        WHEN("b equals zero")
        {
            THEN("Subtracting b (i.e. zero) should not change a)")
            {
                uinteger<150> a;

                constexpr uinteger<150> b{0u};
                for (const unsigned a_num : {0u, 23u, 1337u})
                {
                    a = uinteger<150>{a_num};
                    const uinteger<150> result = sub(a, b);
                    REQUIRE(result == a);
                }
            }
        }
        WHEN("There is a borrow into the next word")
        {
            AND_WHEN("The next word is 2^64-1")
            {
                THEN("The new borrow must be set as well")
                {
                    constexpr uint64_t all_ones = static_cast<uint64_t>(-1);
                    constexpr uint64_t zero = 0u;
                    constexpr uint64_t one = 1u;

                    constexpr uinteger<192> a = uinteger<192>::from_words(one, zero, zero);
                    constexpr uinteger<192> b = uinteger<192>::from_words(zero, all_ones, one);

                    constexpr uinteger<192> result = sub(a, b);
                    constexpr uinteger<192> expected =
                        uinteger<192>::from_words(zero, zero, all_ones);

                    REQUIRE(expected == result);
                }
            }
        }
        WHEN("The size is 129 bits (three words and the third word uses only one bit) and max-1 is "
             "calculated")
        {
            THEN("The MSB should remain 1")
            {
                using uint = uinteger<129>;

                constexpr uint opd1 = ~uint(0U);
                constexpr uint one = uint(1U);

                constexpr auto res = sub(opd1, one);
                constexpr auto ref = uint::from_words(0x1, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFE);

                REQUIRE(res == ref);
            }
        }
    }

    GIVEN("Two uinteger<N> a and b are subtracted")
    {
        WHEN("N > word_width")
        {
            AND_WHEN("There is a borrow between words")
            {
                auto constexpr a = uinteger<128>::from_words(1, 0);
                auto constexpr b = uinteger<128>::from_words(0, 1);
                auto constexpr expected = uinteger<128>::from_words(0, static_cast<uint64_t>(-1));
                auto constexpr result = sub(a, b);

                THEN("The result borrow is subtracted from the next word")
                {
                    REQUIRE(result == expected);
                }
            }
            AND_WHEN("There is no borrow between words")
            {
                auto constexpr a = uinteger<128>::from_words(1, 1);
                auto constexpr b = uinteger<128>::from_words(0, 1);
                auto constexpr expected = uinteger<128>::from_words(1, 0);
                auto constexpr result = sub(a, b);

                THEN("No borrow is subtracted from the next word")
                {
                    REQUIRE(result == expected);
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Investigating max/min values", "[integer][unsigned][operations]",
                       AARITH_TEST_SIGNATURE, AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = uinteger<W, WordType>;
    GIVEN("The maximal and minimal values of uinteger<V>")
    {

        static constexpr I min = I::min();
        static constexpr I max = I::max();

        THEN("Bit-wise complement should yield the other one")
        {
            CHECK(~min == max);
            REQUIRE(~max == min);
        }

        THEN("uinteger::min and uinteger::lowest are the same")
        {
            REQUIRE(min == std::numeric_limits<I>::lowest());
        }

        WHEN("Adding to max value")
        {
            const I a = GENERATE(take(10, random_uinteger<W, WordType>(I::one(), I::max())));
            const I expected_trunc{sub(a, I::one())};

            THEN("Truncating addition is overflow modulo 2")
            {
                I result = add(max, a);
                REQUIRE(result == expected_trunc);
            }

            THEN("Expanding addition has the highest bit set and is modulo 2 otherwise")
            {
                uinteger<W + 10, WordType> result = expanding_add(max, a);
                const auto bit_set = result.bit(W);
                if (!bit_set)
                {
                    std::cout << max << "+" << a << "=" << result << "\n";
                    std::cout << to_binary(max) << "+" << to_binary(a) << "=" << to_binary(result)
                              << "\n";
                }
                CHECK(result.bit(W));
                REQUIRE(width_cast<W>(result) == expected_trunc);
            }
        }

        WHEN("Subtracting from min value")
        {
            const I a = GENERATE(take(10, random_uinteger<W, WordType>()));

            THEN("Truncating subtraction is underflow modulo 2")
            {
                const I expected = sub(I::max(), I{sub(a, I::one())});
                I result = sub(min, a);
                REQUIRE(result == expected);
            }
        }
    }
}

SCENARIO("Multiplication of numbers fitting in a uint64_t",
         "[integer][unsigned][arithmetic][multiplication]")
{
    GIVEN("A random number a")
    {
        uint64_t val_a = GENERATE(
            take(100, random(static_cast<uint64_t>(0U), std::numeric_limits<uint64_t>::max())));
        uinteger<64> a{val_a};
        AND_GIVEN("A random number b")
        {
            uint64_t val_b = GENERATE(
                take(100, random(static_cast<uint64_t>(0U), std::numeric_limits<uint64_t>::max())));
            uinteger<64> b{val_b};

            THEN("The multiplication should match its uint64_t counterpart")
            {
                uint64_t expected = val_a * val_b;
                uinteger<64> result = schoolbook_mul(a, b);
                uinteger<64> resultk = karazuba(a, b);

                CHECK(expected == result.word(0));
                REQUIRE(uinteger<64>{expected} == result);

                CHECK(expected == resultk.word(0));
                REQUIRE(uinteger<64>{expected} == resultk);
            }
        }
    }
}

SCENARIO("Multiplying two unsigned integers using the karatsuba multiplication",
         "[integer][unsigned][arithmetic][multiplication]")
{
    GIVEN("Two uinteger<N> a and b with N <= 32")
    {
        using auint = uinteger<32>;

        auto au = 0xCCCCCCCC;
        auto bu = 0xAAAAAAAA;

        auto a = auint(au);
        auto b = auint(bu);

        auto res = expanding_karazuba(a, b);
        auto ref = schoolbook_expanding_mul(a, b);

        THEN("The result should be equal to the standard multiplication.")
        {
            REQUIRE(res == ref);
        }
    }

    GIVEN("Two uinteger<N> a and uinteger<M> b with M!=N and N+M <= 64")
    {
        using uinta = uinteger<22>;
        using uintb = uinteger<14>;

        auto au = 0xCCCCC;
        auto bu = 0x2AAA;

        auto a = uinta(au);
        auto b = uintb(bu);

        auto res = expanding_karazuba(a, b);
        auto ref = schoolbook_expanding_mul(a, b);

        THEN("The result should be equal to the standard multiplication.")
        {
            REQUIRE(res == ref);
        }
    }

    GIVEN("Two uinteger<N> a and uinteger<M> b with N+M > 64")
    {
        GIVEN("N<=64 and M<=64")
        {
            GIVEN("a = 0")
            {
                using uinta = uinteger<44>;
                using uintb = uinteger<56>;

                auto au = 0x0;
                auto bu = 0xAAAAAAAAAAAAAA;

                auto a = uinta(au);
                auto b = uintb(bu);

                auto res = expanding_karazuba(a, b);
                auto ref = schoolbook_expanding_mul(a, b);

                THEN("The result should be equal to the standard multiplication.")
                {
                    REQUIRE(res == ref);
                }
            }
            GIVEN("b = 0")
            {
                using uinta = uinteger<44>;
                using uintb = uinteger<56>;

                auto au = 0xCCCCCCCCCCC;
                auto bu = 0x0;

                auto a = uinta(au);
                auto b = uintb(bu);

                auto res = expanding_karazuba(a, b);
                auto ref = schoolbook_expanding_mul(a, b);

                THEN("The result should be equal to the standard multiplication.")
                {
                    REQUIRE(res == ref);
                }
            }
            GIVEN("a > 0 and b > 0")
            {
                using uinta = uinteger<44>;
                using uintb = uinteger<56>;

                auto au = 0xCCCCCCCCCCC;
                auto bu = 0xAAAAAAAAAAAAAA;

                auto a = uinta(au);
                auto b = uintb(bu);

                auto res = expanding_karazuba(a, b);
                auto ref = schoolbook_expanding_mul(a, b);

                THEN("The result should be equal to the standard multiplication.")
                {
                    REQUIRE(res == ref);
                }
            }
            GIVEN("a = max and b = max")
            {
                using uinta = uinteger<44>;
                using uintb = uinteger<56>;

                auto au = 0xFFFFFFFFFFF;
                auto bu = 0xFFFFFFFFFFFFFF;

                auto a = uinta(au);
                auto b = uintb(bu);

                auto res = expanding_karazuba(a, b);
                auto ref = schoolbook_expanding_mul(a, b);

                THEN("The result should be equal to the standard multiplication.")
                {
                    REQUIRE(res == ref);
                }
            }
        }
        GIVEN("N>64, N<=128, M>64, and M<=128")
        {
            GIVEN("a = 0")
            {
                using uinta = uinteger<102>;
                using uintb = uinteger<84>;

                auto a = uinta(0U);
                auto b = uintb::from_words(0xAAAAAAAAAAAAAAAA, 0xAAAAA);

                auto res = expanding_karazuba(a, b);
                auto ref = schoolbook_expanding_mul(a, b);

                THEN("The result should be equal to the standard multiplication.")
                {
                    REQUIRE(res == ref);
                }
            }
            GIVEN("b = 0")
            {
                using uinta = uinteger<102>;
                using uintb = uinteger<84>;

                auto a = uinta::from_words(0xCCCCCCCCCCCCCCCC, 0xCCCCCCCCC);
                auto b = uintb(0U);

                auto res = expanding_karazuba(a, b);
                auto ref = schoolbook_expanding_mul(a, b);

                THEN("The result should be equal to the standard multiplication.")
                {
                    REQUIRE(res == ref);
                }
            }
            GIVEN("a > 0 and b > 0")
            {
                using uinta = uinteger<102>;
                using uintb = uinteger<84>;

                auto a = uinta::from_words(0xCCCCCCCCCCCCCCCC, 0xCCCCCCCCC);
                auto b = uintb::from_words(0xAAAAAAAAAAAAAAAA, 0xAAAAA);

                auto res = expanding_karazuba(a, b);
                auto ref = schoolbook_expanding_mul(a, b);

                THEN("The result should be equal to the standard multiplication.")
                {
                    REQUIRE(res == ref);
                }
            }
            GIVEN("a = max and b = max")
            {
                using uinta = uinteger<102>;
                using uintb = uinteger<84>;

                auto a = uinta::from_words(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFF);
                auto b = uintb::from_words(0xFFFFFFFFFFFFFFFF, 0xFFFFF);

                auto res = expanding_karazuba(a, b);
                auto ref = schoolbook_expanding_mul(a, b);

                THEN("The result should be equal to the standard multiplication.")
                {
                    REQUIRE(res == ref);
                }
            }
        }
        GIVEN("N>256, and M>256")
        {
            GIVEN("a = 0")
            {
                using uinta = uinteger<300>;
                using uintb = uinteger<584>;

                auto a = uinta(0U);
                auto b = uintb::from_words(
                    0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA,
                    0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA,
                    0xAAAAAAAAAAAAAAAA, 0xAA);

                auto res = expanding_karazuba(a, b);
                auto ref = schoolbook_expanding_mul(a, b);

                THEN("The result should be equal to the standard multiplication.")
                {
                    REQUIRE(res == ref);
                }
            }
            GIVEN("b = 0")
            {
                using uinta = uinteger<300>;
                using uintb = uinteger<584>;

                auto a = uinta::from_words(0xCCCCCCCCCCCCCCCC, 0xCCCCCCCCCCCCCCCC,
                                           0xCCCCCCCCCCCCCCCC, 0xCCCCCCCCCCCCCCCC, 0xCCCCCCCCCCC);
                auto b = uintb(0U);

                auto res = expanding_karazuba(a, b);
                auto ref = schoolbook_expanding_mul(a, b);

                THEN("The result should be equal to the standard multiplication.")
                {
                    REQUIRE(res == ref);
                }
            }
            GIVEN("a > 0 and b > 0")
            {
                using uinta = uinteger<300>;
                using uintb = uinteger<584>;

                auto a = uinta::from_words(0xCCCCCCCCCCCCCCCC, 0xCCCCCCCCCCCCCCCC,
                                           0xCCCCCCCCCCCCCCCC, 0xCCCCCCCCCCCCCCCC, 0xCCCCCCCCCCC);
                auto b = uintb::from_words(
                    0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA,
                    0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA, 0xAAAAAAAAAAAAAAAA,
                    0xAAAAAAAAAAAAAAAA, 0xAA);

                auto res = expanding_karazuba(a, b);
                auto ref = schoolbook_expanding_mul(a, b);

                THEN("The result should be equal to the standard multiplication.")
                {
                    REQUIRE(res == ref);
                }
            }
            GIVEN("a = F and b = F")
            {
                using uinta = uinteger<300>;
                using uintb = uinteger<584>;

                auto a = uinta::from_words(0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                                           0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFF);
                auto b = uintb::from_words(
                    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                    0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF, 0xFFFFFFFFFFFFFFFF,
                    0xFFFFFFFFFFFFFFFF, 0xFF);

                auto res = expanding_karazuba(a, b);
                auto ref = schoolbook_expanding_mul(a, b);

                THEN("The result should be equal to the standard multiplication.")
                {
                    REQUIRE(res == ref);
                }
            }
        }
    }
}

SCENARIO("Bit and Word operations work correctly", "[integer][unsigned][utility][bit_logic]")
{
    WHEN("A uinteger<N> is created using uinteger<N>::from_words")
    {
        THEN("All words should be correctly masked")
        {
            constexpr uint64_t ones = static_cast<uint64_t>(-1);
            uint64_t pos = 1;
            constexpr size_t width = 15;
            auto constexpr a = uinteger<width>::from_words(ones);

            for (auto i = 0U; i < a.word_width(); i++)
            {
                auto mask = (pos << i);
                bool is_one = a.word(0) & mask;
                bool in_word = i < width;

                if (in_word)
                {
                    CHECK(is_one);
                }
                else
                {
                    CHECK(!is_one);
                }
            }
        }
    }
    WHEN("Two uinteger<N>'s are created")
    {
        AND_WHEN("One is created via ::from_words and the other uses .set_word")
        {
            THEN("The resulting uinteger<N>'s should be identical")
            {
                constexpr uint64_t ones = static_cast<uint64_t>(-1);
                auto constexpr a = uinteger<15>::from_words(ones);
                uinteger<15> b;
                b.set_word(0, ones);

                uinteger<15> c;
                c.set_words(ones);

                // If this check fails, the generated expansion might not be helpful as only the
                // lowest 15 bits are sent to std::cout. These are the bits, were the two functions
                // aren't differing in the first place.
                CHECK(a == b);
                CHECK(a == c);
                CHECK(b == c);
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Invariants for the unsigned integer division",
                       "[integer][unsigned][arithmetic][division]", AARITH_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = uinteger<W, WordType>;

    GIVEN("A non-zero number")
    {

        const I a = GENERATE(take(10, random_uinteger<W, WordType>(I::one(), I::max())));

        WHEN("Dividing the number by itself")
        {
            THEN("The result should be 1")
            {
                REQUIRE(div(a,a) == I::one());
            }
        }
        WHEN("Dividing the number by one")
        {
            THEN("The result should be the number itself")
            {
                REQUIRE(div(a,I::one()) == a);
            }
        }
        WHEN("Dividing the number by a larger number")
        {
            THEN("The result should be zero")
            {
                // Adding one is safe as I::max() will not be returned by the generator
                REQUIRE(div(a,add(a, I::one())) == I::zero());
            }
        }
        WHEN("Dividing the number by zero")
        {
            THEN("A runtime exception should be thrown")
            {
                REQUIRE_THROWS_AS(div(a,I::zero()), std::runtime_error);
            }
        }
    }
}

SCENARIO("Dividing two unsigned integers exactly", "[integer][unsigned][arithmetic][division]")
{
    GIVEN("Two uinteger<N> a and b with N <= 32")
    {
        uint32_t val_a =
            GENERATE(1, 56567, 23, static_cast<uint32_t>(-4366), static_cast<uint32_t>(-1));
        uint32_t val_b = GENERATE(1, 56567, 23, 234, 76856, 2342353456,
                                  static_cast<uint32_t>(-4366), static_cast<uint32_t>(-1));

        const uinteger<32> a = uinteger<32>::from_words(val_a);
        const uinteger<32> b = uinteger<32>::from_words(val_b);

        THEN("Division by zero should throw an exception")
        {
            constexpr uinteger<32> zero{0U};
            CHECK_THROWS_AS(div(b, zero), std::runtime_error);
            CHECK_THROWS_AS(div(a, zero), std::runtime_error);
        }

        THEN("Division by 1 should not change the other numerator")
        {
            constexpr uinteger<32> one{1U};
            CHECK(div(a, one) == a);
            CHECK(div(b, one) == b);
        }
        THEN("Divison of 0 should result in 0")
        {
            constexpr uinteger<32> zero{0U};

            CHECK(div(zero, a) == zero);
            CHECK(div(zero, b) == zero);
        }
        THEN("The result matches the uint64_t computation")
        {
            uint64_t a_large = val_a;
            uint64_t b_large = val_b;

            uint64_t int_res = a_large / b_large;

            uint32_t small_res = static_cast<uint32_t>(int_res);
            const uinteger<32> result = div(a, b);
            CHECK(small_res == result.word(0));
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Computing the signum of an unsigned integer",
                       "[integer][unsigned][operation][utility]", AARITH_TEST_SIGNATURE,
                       AARITH_INT_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = uinteger<W, WordType>;
    GIVEN("The number  zero")
    {
        WHEN("Determining the signum")
        {
            THEN("The signum should be zero")
            {
                REQUIRE(signum(I::zero()) == 0);
            }
        }
    }
    GIVEN("A non-zero number")
    {
        WHEN("Determining the signum")
        {
            THEN("The signum should be one")
            {
                const I a = GENERATE(take(10, random_uinteger<W, WordType>(I::one(), I::max())));
                REQUIRE(signum(a) == 1);
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Invariants for computing the remainder",
         "[integer][unsigned][arithmetic][remainder][division]", AARITH_TEST_SIGNATURE,
         AARITH_INT_TEST_TEMPLATE_PARAM_RANGE) {
    using I = uinteger<W, WordType>;

    GIVEN("A non-zero number")
    {

        const I a = GENERATE(take(10, random_uinteger<W, WordType>(I::one(), I::max())));

        THEN("Computing the remainder of division by zero should throw an exception")
        {

            CHECK_THROWS_AS(remainder(a, I::zero()), std::runtime_error);
        }

        THEN("A remainder when dividing by 1 should yield zero")
        {
            CHECK(remainder(a, I::one()) == I::zero());
        }
        THEN("Computing the remainder with itself should yield zero")
        {

            CHECK(remainder(a, a) == I::zero());
        }
        THEN("Computing the remainder of zero should yield zero")
        {

            CHECK(remainder(I::zero(), a) == I::zero());
        }
    }

}

SCENARIO("Computing the remainder of two unsigned integers works as expected",
         "[integer][unsigned][arithmetic][remainder][division]")
{
    GIVEN("A fixed test case a=56567 and b=234")
    {
        constexpr uint32_t val_a = 56567;
        constexpr uint32_t val_b = 234;

        constexpr uinteger<32> a{val_a};
        constexpr uinteger<32> b{val_b};

        constexpr uinteger<32> d = div(a, b);
        constexpr uinteger<32> m = remainder(a, b);

        const uint32_t int_div = val_a / val_b;
        const uint32_t int_mod = val_a % val_b;

        CHECK(int_div == d.word(0));
        REQUIRE(int_mod == m.word(0));
    }

    GIVEN("Two uinteger<N> a and b with N <= 32")
    {

        uint32_t val_a =
            GENERATE(1, 56567, 23, static_cast<uint32_t>(-4366), static_cast<uint32_t>(-1));
        uint32_t val_b = GENERATE(1, 56567, 23, 234, 76856, 2342353456,
                                  static_cast<uint32_t>(-4366), static_cast<uint32_t>(-1));

        const uinteger<32> a = uinteger<32>::from_words(val_a);
        const uinteger<32> b = uinteger<32>::from_words(val_b);

        THEN("The result matches the uint32_t computation")
        {

            uint32_t int_res = val_a % val_b;

            uinteger<32> result = remainder(a, b);
            CHECK(int_res == result.word(0));
        }
    }
    GIVEN("Two uinteger<N> a and b with N <= 64 with different bit widths")
    {

        uint32_t val_a = GENERATE(1, 56567, 23, 234, 76856, 2342353456,
                                  static_cast<uint32_t>(-4366), static_cast<uint32_t>(-1));
        uint32_t val_b = GENERATE(1, 56567, 23, 234, 76856, 2342353456,
                                  static_cast<uint32_t>(-4366), static_cast<uint32_t>(-1));

        const uinteger<32> a = uinteger<32>::from_words(val_a);
        const uinteger<64> b = uinteger<64>::from_words(val_b);

        THEN("The result matches the uint64_t computation")
        {

            const auto [quotient, remainder] = restoring_division(a, b);

            uint64_t quot_int = val_a / val_b;
            uint64_t rem_int = val_a % val_b;
            CHECK(quot_int == quotient.word(0));
            CHECK(rem_int == remainder.word(0));
        }
    }
}
