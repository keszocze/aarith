#include <catch.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_word_array.hpp"
#include <aarith/core.hpp>
#include <aarith/core/string_utils.hpp>

using namespace aarith;

// TEMPLATE_TEST_CASE_SIG("Invariant: Concatenation is associative", "[word_array][utility]",
//                       AARITH_TEST_SIGNATURE, AARITH_WORD_ARRAY_TEST_TEMPLATE_PARAM_RANGE)
//{
//    GIVEN("Three different words")
//    {
//        using I = word_array<W, WordType>;

TEMPLATE_TEST_CASE_SIG("Counting bits in word_array", "[word_array][util]", AARITH_TEST_SIGNATURE,
                       AARITH_WORD_ARRAY_TEST_TEMPLATE_PARAM_RANGE)
{
    GIVEN("A word container")
    {
        using I = word_array<W, WordType>;

        THEN("The leading zero index should be computed correctly")
        {
            constexpr I zero{I::all_zeroes()};
            constexpr I one{1U};
            constexpr I two{2U};
            constexpr I three{3U};
            constexpr I ones{I::all_ones()};

            CHECK(count_leading_zeroes(zero) == W);
            REQUIRE(count_leading_zeroes(one) == W - 1);
            REQUIRE(count_leading_zeroes(two) == W - 2);
            REQUIRE(count_leading_zeroes(three) == W - 2);
            REQUIRE(count_leading_zeroes(ones) == 0);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Checking whether an word_array is not equal to zero/false",
                       "[word_array][utility]", AARITH_TEST_SIGNATURE,
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
