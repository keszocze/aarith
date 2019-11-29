#include "aarith/operations/comparisons.hpp"
#include "aarith/operations/exact_operations.hpp"
#include "aarith/types/integer.hpp"
#include <catch.hpp>

using namespace aarith;

SCENARIO("Adding two uintegers exactly", "[uinteger][arithmetic][addition]")
{
    GIVEN("Two uinteger<N> a and b with N <= word_width")
    {
        static constexpr size_t TestWidth = 16;
        static_assert(uinteger<TestWidth>::word_count() == 1);

        WHEN("The result a+b still fits into N bits")
        {
            static constexpr uint8_t number_a = 32;
            static constexpr uint8_t number_b = 16;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};
            const uinteger<TestWidth> result = add(a, b);

            THEN("It should be the correct sum")
            {
                REQUIRE(result.word(0) == number_a + number_b);
            }
        }
        WHEN("The result a+b does not fit into N bits")
        {
            static constexpr uint16_t number_a = (1 << TestWidth) - 1;
            static constexpr uint16_t number_b = 1;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};
            auto const result = add(a, b);

            THEN("It should be the masked to fit")
            {
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
            static constexpr uint64_t number_a = 1ULL << 63;
            static constexpr uint64_t number_b = 1ULL << 63;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};
            auto const result = add(a, b);

            THEN("It is added to the next word")
            {
                REQUIRE(result.word(1) == 1);
            }
        }
        
        WHEN("There is no carry into the next word")
        {
            static constexpr uint64_t number_a = 1ULL << 63;
            static constexpr uint64_t number_b = 0;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};
            auto const result = add(a, b);

            THEN("The next word is unchanged")
            {
                REQUIRE(result.word(1) == 0);
            }
        }
    }
}

SCENARIO("Subtracting two uintegers exactly", "[uinteger][arithmetic][subtraction]")
{
    GIVEN("Two uinteger<N> a and b are subtracted")
    {
        WHEN("N <= word_width")
        {
            AND_WHEN("a >= b")
            {
                const uinteger<32> a{15u};
                const uinteger<32> b{7u};
                const uinteger<32> expected{static_cast<uint32_t>(15 - 7)};
                auto const result = sub(a, b);

                THEN("The result is a - b")
                {
                    REQUIRE(result == expected);
                }
            }
            AND_WHEN("a < b")
            {
                const uinteger<32> a{7u};
                const uinteger<32> b{15u};
                const uinteger<32> expected{static_cast<uint32_t>(7 - 15)};
                auto const result = sub(a, b);

                THEN("The result wraps around")
                {
                    REQUIRE(result == expected);
                }
            }
        }
        WHEN("b equals zero")
        {
            const uinteger<150> b{0u};
            THEN("Subtracting b (i.e. zero) should not change a")
            {
                uinteger<150> a;

                for (const unsigned a_num : {0u, 23u, 1337u})
                {
                    a = uinteger<150>{a_num};
                    uinteger<150> result = sub(a, b);
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
                    const uint64_t all_ones = static_cast<uint64_t>(-1);
                    const uint64_t zero = 0u;
                    const uint64_t one = 1u;

                    const uinteger<192> a = uinteger<192>::from_words(one, zero, zero);
                    const uinteger<192> b = uinteger<192>::from_words(zero, all_ones, one);

                    const uinteger<192> result = sub(a, b);
                    const uinteger<192> expected = uinteger<192>::from_words(zero, zero, all_ones);

                    REQUIRE(expected == result);
                }
            }
        }
    }

    GIVEN("Two uinteger<N> a and b are subtracted")
    {
        WHEN("N > word_width")
        {
            AND_WHEN("There is a borrow between words")
            {
                auto const a = uinteger<128>::from_words(1, 0);
                auto const b = uinteger<128>::from_words(0, 1);
                auto const expected = uinteger<128>::from_words(0, static_cast<uint64_t>(-1));
                auto const result = sub(a, b);

                THEN("The result borrow is subtracted from the next word")
                {
                    REQUIRE(result == expected);
                }
            }
            AND_WHEN("There is no borrow between words")
            {
                auto const a = uinteger<128>::from_words(1, 1);
                auto const b = uinteger<128>::from_words(0, 1);
                auto const expected = uinteger<128>::from_words(1, 0);
                auto const result = sub(a, b);

                THEN("No borrow is subtracted from the next word")
                {
                    REQUIRE(result == expected);
                }
            }
        }
    }
}

SCENARIO("Multiplying two uintegers exactly", "[uinteger][arithmetic][multiplication]")
{

    GIVEN("Two uinteger<N> a and b with N <= 32")
    {

        uint32_t val_a =
            GENERATE(0, 1, 56567, 23, static_cast<uint32_t>(-4366), static_cast<uint32_t>(-1));
        uint32_t val_b = GENERATE(0, 1, 56567, 23, 234, 76856, 2342353456,
                                  static_cast<uint32_t>(-4366), static_cast<uint32_t>(-1));

        const uinteger<32> a = uinteger<32>::from_words(val_a);
        const uinteger<32> b = uinteger<32>::from_words(val_b);

        THEN("Multiplication should be commutative")
        {
            CHECK(mul(a, b) == mul(b, a));
        }

        THEN("Multiplication by 1 should not change the other multiplicant")
        {
            const uinteger<32> one{1U};
            CHECK(mul(a, one) == a);
            CHECK(mul(one, a) == a);
            CHECK(mul(b, one) == b);
            CHECK(mul(one, b) == b);
        }
        THEN("Multiplication by 0 should result in 0")
        {
            const uinteger<32> zero{0U};
            CHECK(mul(a, zero) == zero);
            CHECK(mul(zero, a) == zero);
            CHECK(mul(b, zero) == zero);
            CHECK(mul(zero, b) == zero);
        }
    }

    GIVEN("Two uinteger<N> a and b to be multiplied")
    {
        uint64_t val = 1;
        val = val << 35;
        auto const a = uinteger<128>::from_words(1, val);
        auto const c = uinteger<128>::from_words(13435, 345897);
        auto const d =
            uinteger<128>::from_words(static_cast<typename uinteger<128>::word_type>(-1),
                                      static_cast<typename uinteger<128>::word_type>(-1));
        auto const zero = uinteger<128>::from_words(0, 0);
        auto const one = uinteger<128>::from_words(0, 1);

        const std::vector<uinteger<128>> numbers{a, c, d, one, zero};

        THEN("The operation should be commutative")
        {
            for (const uinteger<128>& num_a : numbers)
            {
                for (const uinteger<128>& num_b : numbers)
                {
                    CHECK(mul(num_a, num_b) == mul(num_b, num_a));
                }
            }
        }

        WHEN("One multiplicant is zero")
        {

            THEN("The result should be zero")
            {
                for (const uinteger<128>& num : numbers)
                {
                    CHECK(mul(num, zero) == zero);
                    CHECK(mul(zero, num) == zero);
                }
            }
        }
        WHEN("One multiplicant is one")
        {
            THEN("Multiplication does not do much")
            {

                for (const uinteger<128>& num : numbers)
                {
                    CHECK(mul(num, one) == num);
                    CHECK(mul(one, num) == num);
                }
            }
        }
        WHEN("Both multiplicands are maximum")
        {
            THEN("The product is 1")
            {
                REQUIRE(mul(d, d) == one);
            }
        }
    }
}

SCENARIO("Multiplication of numbers fitting in a uint64_t",
         "[uinteger][arithmetic][multiplication]")
{
    GIVEN("A random number a")
    {
        uint64_t val_a = GENERATE(
            take(100, random(static_cast<uint64_t>(0U), std::numeric_limits<uint64_t>::max())));
        uinteger<64> a{val_a};
        AND_GIVEN("A random number b")
        {
            uint64_t val_b = GENERATE(take(
                1000, random(static_cast<uint64_t>(0U), std::numeric_limits<uint64_t>::max())));
            uinteger<64> b{val_b};

            THEN("The multiplication should match its uint64_t counterpart")
            {
                uint64_t expected = val_a * val_b;
                uinteger<64> result = mul(a, b);

                REQUIRE(expected == result.word(0));
                REQUIRE(uinteger<64>{expected} == result);
            }
        }
    }
}

SCENARIO("Bit and Word operations work correctly", "[uinteger][utility]")
{
    WHEN("A uinteger<N> is created using uinteger<N>::from_words")
    {
        THEN("All words should be correctly masked")
        {
            const uint64_t ones = static_cast<uint64_t>(-1);
            uint64_t pos = 1;
            constexpr size_t width = 15;
            auto const a = uinteger<width>::from_words(ones);

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
                    //                                    std::cout << uinteger<64>{mask} << "\t" <<
                    //                                    a << "\t" << uinteger<64>{a.word(0)} <<
                    //                                    "\n";
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
                const uint64_t ones = static_cast<uint64_t>(-1);
                auto const a = uinteger<15>::from_words(ones);
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

SCENARIO("Dividing two uintegers exactly", "[uinteger][arithmetic]")
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
            const uinteger<32> zero{0U};
            CHECK_THROWS_AS(div(b, zero), std::runtime_error);
            CHECK_THROWS_AS(div(a, zero), std::runtime_error);
        }

        THEN("Division by 1 should not change the other numerator")
        {
            const uinteger<32> one{1U};
            CHECK(div(a, one) == a);
            CHECK(div(b, one) == b);
        }
        THEN("Divison of 0 should result in 0")
        {
            const uinteger<32> zero{0U};

            CHECK(div(zero, a) == zero);
            CHECK(div(zero, b) == zero);
        }
        THEN("The result matches the uint64_t computation")
        {
            uint64_t a_large = val_a;
            uint64_t b_large = val_b;

            uint64_t int_res = a_large / b_large;

            uint32_t small_res = static_cast<uint32_t>(int_res);
            uinteger<32> result = div(a, b);
            CHECK(small_res == result.word(0));
        }
    }
}

SCENARIO("Computing the remainder of two uintegers works as expected", "[uinteger][arithmetic]")
{
    GIVEN("A fixed test case a=56567 and b=234")
    {
        const uint32_t val_a = 56567;
        const uint32_t val_b = 234;

        const uinteger<32> a{val_a};
        const uinteger<32> b{val_b};

        const uinteger<32> d = div(a, b);
        const uinteger<32> m = remainder(a, b);

        const uint32_t int_div = val_a / val_b;
        const uint32_t int_mod = val_a % val_b;

        CHECK(int_div == d.word(0));
        REQUIRE(int_mod == m.word(0));
    }

    GIVEN("An uinteger<N> a")
    {

        const size_t width = 150;
        auto val_a = GENERATE(take(100, random(1U, 10000000U)));

        uinteger<width> a{val_a};
        const uinteger<width> zero{0U};
        const uinteger<width> one{1U};

        THEN("Computing the remainder of division by zero should trhow an exception")
        {

            CHECK_THROWS_AS(remainder(a, zero), std::runtime_error);
        }

        THEN("A remainder when dividing by 1 should yield zero")
        {
            CHECK(remainder(a, one) == zero);
        }
        THEN("Computing the remainder of zero should yield 0")
        {

            CHECK(remainder(zero, a) == zero);
        }
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