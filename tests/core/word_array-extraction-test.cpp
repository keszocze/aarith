#include <catch.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_word_array.hpp"
#include <aarith/core.hpp>
#include <aarith/core/core_string_utils.hpp>

using namespace aarith;

SCENARIO("Splitting a word_array/extraction from a word_array", "[word_array][utility]")
{
    GIVEN("A word_array w")
    {

        const word_array<7> w3{21};
        word_array<8> a{0b00111001};

        WHEN("The container is split")
        {
            THEN("The parts are as expected")
            {
                const auto [w3_left, w3_right] = split<2>(w3);
                CHECK(w3_left.width() == 4);
                CHECK(w3_right.width() == 3);
                CHECK(w3_left == word_array<4>{2});
                CHECK(w3_right == word_array<3>{5});

                const auto [a_left, a_right] = split<2>(a);
                CHECK(a_left.width() == 5);
                CHECK(a_right.width() == 3);
                CHECK(a_left == word_array<5>{0b00111});
                REQUIRE(a_right == word_array<3>{0b001});
            }
        }
        WHEN("Ranges are extracted correctly")
        {
            THEN("The parts are as expected")
            {
                const auto r1 = bit_range<6, 1>(w3);
                const auto r2 = bit_range<6, 2>(w3);
                const auto r3 = bit_range<5, 1>(w3);

                CHECK(r1.width() == 6);
                CHECK(r2.width() == 5);
                CHECK(r3.width() == 5);
                CHECK(r1 == word_array<6>{10});
                CHECK(r2 == word_array<5>{5});
                CHECK(r3 == word_array<5>{10});

                const auto s1 = bit_range<6, 1>(a);
                const auto s2 = bit_range<6, 2>(a);
                const auto s3 = bit_range<5, 1>(a);

                CHECK(s1.width() == 6);
                CHECK(s2.width() == 5);
                CHECK(s3.width() == 5);
                CHECK(s1 == word_array<6>{0b11100});
                CHECK(s2 == word_array<5>{0b01110});
                REQUIRE(s3 == word_array<5>{0b11100});
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Invariant: Concatenation is associative", "[word_array][utility]",
                       AARITH_INT_TEST_SIGNATURE, AARITH_WORD_ARRAY_TEST_TEMPLATE_PARAM_RANGE)
{
    GIVEN("Three different words")
    {
        using I = word_array<W, WordType>;

        const I w1 = GENERATE(take(5, random_word_array<W, WordType>()));
        const I w2 = GENERATE(take(5, random_word_array<W, WordType>()));
        const I w3 = GENERATE(take(5, random_word_array<W, WordType>()));

        WHEN("Concatenating the words")
        {
            THEN("The operation should be associative")
            {
                REQUIRE(concat(concat(w1, w2), w3) == concat(w1, concat(w2, w3)));
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Concatenating undoes range extraction", "[word_array][utility]",
                       AARITH_INT_TEST_SIGNATURE, AARITH_WORD_ARRAY_TEST_TEMPLATE_PARAM_RANGE)
{
    GIVEN("A word_array")
    {
        using I = word_array<W, WordType>;

        const I w = GENERATE(take(20, random_word_array<W, WordType>()));

        WHEN("Extracting a range from including one end (i.e. basically splitting the word array)")
        {
            THEN("Using concat to put the word back together should yield the original word")
            {

                const auto left = bit_range<W - 1, W / 2>(w);
                const auto right = bit_range<(W / 2) - 1, 0>(w);
                REQUIRE(concat(left, right) == w);
            }
        }

        WHEN("Extracting a part from the middle of the word array")
        {
            THEN("Using concat to put the word back together should yield the original word")
            {

                const auto left = bit_range<W - 1, 2 * (W / 3)>(w);
                const auto middle = bit_range<(2 * (W / 3)) - 1, W / 3>(w);
                const auto right = bit_range<(W / 3) - 1, 0>(w);
                REQUIRE(concat(concat(left, middle), right) == w);
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Concatenating undoes splitting", "[word_array][utility]",
                       AARITH_INT_TEST_SIGNATURE, AARITH_WORD_ARRAY_TEST_TEMPLATE_PARAM_RANGE)
{
    GIVEN("A word_array")
    {
        using I = word_array<W, WordType>;

        const I w = GENERATE(take(20, random_word_array<W, WordType>()));

        WHEN("Splitting the word at some position")
        {
            THEN("Concatenating the resulting parts should yield the original word array")
            {
                {
                    const auto [left, right] = split<0>(w);
                    REQUIRE(concat(left, right) == w);
                }

                {
                    const auto [left, right] = split<W / 2>(w);
                    REQUIRE(concat(left, right) == w);
                }
                {
                    const auto [left, right] = split<W / 3>(w);
                    REQUIRE(concat(left, right) == w);
                }

                {
                    const auto [left, right] = split<2 * (W / 3)>(w);
                    REQUIRE(concat(left, right) == w);
                }

                {
                    const auto [left, right] = split<W - 2>(w);
                    REQUIRE(concat(left, right) == w);
                }
            }
        }
    }
}