#include <catch.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_word_array.hpp"
#include <aarith/core.hpp>
#include <aarith/core/string_utils.hpp>

using namespace aarith;

SCENARIO("Circular shift")
{
    GIVEN("The examplary number from wikipedia")
    {
        // https://en.wikipedia.org/wiki/Bitwise_operation#Bit_shifts

        using W = word_array<8>;
        const W num{0b00010111};

        WHEN("Shifting left once")
        {
            W a{num};

            rotate_left(a);

            THEN("The result is as expected")
            {
                W expected{0b00101110};
                REQUIRE(a == expected);
            }
        }
        WHEN("Shifting right once")
        {
            W a{num};
            rotate_right(a);
            {
                W expected{0b10001011};
                REQUIRE(a == expected);
            }
        }

        WHEN("Shifting left thrice")
        {
            W a{num};

            rotate_left(a, 3);

            THEN("The result is as expected")
            {
                W expected{0b10111000};
                REQUIRE(a == expected);
            }
        }
        WHEN("Shifting right thrice")
        {
            W a{num};
            rotate_right(a, 3);
            {
                W expected{0b11100010};
                REQUIRE(a == expected);
            }
        }
    }
}

SCENARIO("Rotate through carry")
{
    GIVEN("The examplary number from wikipedia")
    {
        // https://en.wikipedia.org/wiki/Bitwise_operation#Bit_shifts

        using W = word_array<8>;
        const W num{0b00010111};

        WHEN("Shifting left once")
        {
            W a{num};

            rotate_through_carry_left(a, true);

            THEN("The result is as expected")
            {
                W expected{0b00101111};
                REQUIRE(a == expected);
            }
        }
        WHEN("Shifting right once")
        {
            W a{num};
            rotate_through_carry_right(a, true);
            {
                W expected{0b10001011};
                REQUIRE(a == expected);
            }
        }

        AND_WHEN("Shifting left thrice")
        {
            W a{num};

            rotate_through_carry_left(a, true, 3);

            THEN("The result is as expected")
            {
                W expected{0b10111100};
                REQUIRE(a == expected);
            }
        }
        WHEN("Shifting right thrice")
        {
            W a{num};
            rotate_through_carry_right(a, true, 3);
            {
                W expected{0b11100010};
                REQUIRE(a == expected);
            }
        }
    }
}

SCENARIO("Manually setting ranges of bits", "[word_array][utility]")
{
    GIVEN("A word array")
    {

        word_array<32> w;

        WHEN("Manually settings bits")
        {
            w.set_bits(0, word_array<2>::all_ones());
            word_array<32> b;
            b.set_bit(0, true);
            b.set_bit(1, true);
            CHECK(w == b);

            w.set_bits(10, word_array<3>::all_ones());

            b.set_bit(10, true);
            b.set_bit(11, true);
            b.set_bit(12, true);

            REQUIRE(w == b);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Counting zero bits in word_array", "[word_array][utility]",
                       AARITH_INT_TEST_SIGNATURE, AARITH_WORD_ARRAY_TEST_TEMPLATE_PARAM_RANGE)
{
    GIVEN("A word container")
    {
        using I = word_array<W, WordType>;

        THEN("The leading zero count should be computed correctly")
        {
            constexpr I zero{I::all_zeroes()};
            constexpr I one{1U};
            constexpr I two{2U};
            constexpr I three{3U};
            constexpr I ones{I::all_ones()};

            CHECK(count_leading_zeroes(zero) == W);
            CHECK(count_leading_zeroes(zero, W) == 0);
            REQUIRE(count_leading_zeroes(one) == W - 1);
            REQUIRE(count_leading_zeroes(two) == W - 2);
            REQUIRE(count_leading_zeroes(one, 1) == W - 1 - 1);
            REQUIRE(count_leading_zeroes(two, 2) == W - 2 - 2);
            REQUIRE(count_leading_zeroes(three) == W - 2);
            REQUIRE(count_leading_zeroes(ones) == 0);
            REQUIRE(count_leading_zeroes(ones) == 0);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Counting one bits in word_array", "[word_array][utility]",
                       AARITH_INT_TEST_SIGNATURE, AARITH_WORD_ARRAY_TEST_TEMPLATE_PARAM_RANGE)
{
    GIVEN("A word container")
    {
        using I = word_array<W, WordType>;

        THEN("The leading one count should be computed correctly")
        {
            constexpr I zero{I::all_zeroes()};
            constexpr I ones{I::all_ones()};

            CHECK(count_leading_ones(zero) == 0);
            REQUIRE(count_leading_ones(ones, 1) == W - 1);
            REQUIRE(count_leading_ones(ones, 2) == W - 2);
            REQUIRE(count_leading_ones(ones, W) == 0);
            REQUIRE(count_leading_ones(ones) == W);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Checking whether an word_array is not equal to zero/false",
                       "[word_array][utility]", AARITH_INT_TEST_SIGNATURE,
                       AARITH_WORD_ARRAY_TEST_TEMPLATE_PARAM_RANGE)
{
    using I = word_array<W, WordType>;

    GIVEN("An word_array<N>=0=a for various N")
    {
        THEN("a.is_zero() should be true")
        {

            CHECK(I{0U}.is_zero());
            REQUIRE(I::all_zeroes().is_zero());
        }
        THEN("a should evaluate to false in a Boolean context")
        {

            CHECK_FALSE(I{0U});
            REQUIRE_FALSE(I::all_zeroes());
        }
    }

    GIVEN("A non-zero word_array")
    {

        I w = GENERATE(take(20, random_word_array<W, WordType>()));
        w.set_bit(0, true); // make it non-zero

        THEN("is_zero should correctly return this fact")
        {
            REQUIRE_FALSE(w.is_zero());
        }
        THEN("The integer should evaluate to true in a Boolean context")
        {
            REQUIRE(w);
        }
    }
}

SCENARIO("Calculating the word_masks of word_containers", "[word_array][bit_logic]")
{
    // The tests all assume that word_array uses 64-bit words.
    static_assert(word_array<64>::word_width() == 64);

    GIVEN("A word_array<N> where N < word_width")
    {
        constexpr word_array<32> uint;
        THEN("The mask is correct")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffff);
        }
    }
    GIVEN("A word_array<N> where N == word_width")
    {
        constexpr word_array<64> uint;
        THEN("The mask is all 1s")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffffffffffff);
        }
    }
    GIVEN("A word_array<N> where N > word_width and N % word_width != 0")
    {
        constexpr word_array<96> uint;
        THEN("All masks except the last are all 1s")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffffffffffff);
        }
        THEN("The last mask is all ones up to the correct bit")
        {
            REQUIRE(uint.word_mask(1) == 0xffffffff);
        }
    }
    GIVEN("A word_array<N> where N > word_width and N % word_width == 0")
    {
        constexpr word_array<128> uint;
        THEN("All masks are all 1s")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffffffffffff);
            REQUIRE(uint.word_mask(1) == 0xffffffffffffffff);
        }
    }
}
