#include <catch.hpp>

#include <aarith/core.hpp>
#include <aarith/core/string_utils.hpp>

using namespace aarith;

SCENARIO("Splitting a word_array/extraction from a word_array", "[word_array],[utility]")
{
    GIVEN("A word_array w")
    {

        const word_array<7> w3{21};

        WHEN("The container is split")
        {
            THEN("The parts are as expected")
            {
                const auto [w3_left, w3_right] = split<2>(w3);
                REQUIRE(w3_left == word_array<4>{2});
                REQUIRE(w3_right == word_array<3>{5});
            }
        }
        WHEN("Ranges are extracted correctly")
        {
            THEN("The parts are as expected")
            {
                REQUIRE(bit_range<6, 1>(w3) == word_array<6>{10});
                REQUIRE(bit_range<6, 2>(w3) == word_array<5>{5});
                REQUIRE(bit_range<5, 1>(w3) == word_array<5>{10});
            }
        }
    }
}

SCENARIO("Invariant: Concatenation is associative", "[word_array][utility]")
{
    GIVEN("Three different words")
    {
        const uint32_t n1 = GENERATE(take(100, random(std::numeric_limits<uint32_t>::max(),
                                                      std::numeric_limits<uint32_t>::max())));
        const uint32_t n2 = GENERATE(take(100, random(std::numeric_limits<uint32_t>::max(),
                                                      std::numeric_limits<uint32_t>::max())));
        const uint32_t n3 = GENERATE(take(100, random(std::numeric_limits<uint32_t>::max(),
                                                      std::numeric_limits<uint32_t>::max())));

        const word_array<32, uint8_t> w1_8{word_array<32, uint32_t>{n1}};
        const word_array<32, uint64_t> w1_64{n1};
        const word_array<32, uint8_t> w2_8{word_array<32, uint32_t>{n2}};
        const word_array<32, uint64_t> w2_64{n2};
        const word_array<32, uint8_t> w3_8{word_array<32, uint32_t>{n3}};
        const word_array<32, uint64_t> w3_64{n3};

        WHEN("Concatenating the words")
        {
            THEN("The operation should be associative")
            {
                REQUIRE(concat(concat(w1_8, w2_8), w3_8) == concat(w1_8, concat(w2_8, w3_8)));
                REQUIRE(concat(concat(w1_64, w2_64), w3_64) == concat(w1_64, concat(w2_64, w3_64)));
            }
        }
    }
}

SCENARIO("Concatenating undoes range extraction", "[word_array][utility]")
{
    GIVEN("A word_array")
    {
        const uint32_t n = GENERATE(take(100, random(std::numeric_limits<uint32_t>::max(),
                                                     std::numeric_limits<uint32_t>::max())));
        const word_array<32, uint8_t> w8{word_array<32, uint32_t>{n}};
        const word_array<32, uint64_t> w64{n};

        WHEN("Extracting a range from including one end (i.e. basically splitting the word array)")
        {
            THEN("Using concat to put the word back together should yield the original word")
            {

                const auto left8 = bit_range<31, 15>(w8);
                const auto right8 = bit_range<14, 0>(w8);
                REQUIRE(concat(left8, right8) == w8);

                const auto left64 = bit_range<31, 15>(w64);
                const auto right64 = bit_range<14, 0>(w64);
                REQUIRE(concat(left64, right64) == w64);
            }
        }

        WHEN("Extracting a part from the middle of the word array")
        {
            THEN("Using concat to put the word back together should yield the original word")
            {

                const auto left8 = bit_range<31, 20>(w8);
                const auto middle8 = bit_range<19, 10>(w8);
                const auto right8 = bit_range<9, 0>(w8);
                REQUIRE(concat(concat(left8, middle8), right8) == w8);

                const auto left64 = bit_range<31, 20>(w64);
                const auto middle64 = bit_range<19, 10>(w64);
                const auto right64 = bit_range<9, 0>(w64);
                REQUIRE(concat(concat(left64, middle64), right64) == w64);
            }
        }
    }
}

SCENARIO("Concatenating undoes splitting", "[word_array][utility]")
{
    GIVEN("A word_array")
    {
        const uint8_t n8_ = GENERATE(take(
            100, random(std::numeric_limits<uint8_t>::max(), std::numeric_limits<uint8_t>::max())));
        const uint8_t n32_ = GENERATE(take(100, random(std::numeric_limits<uint32_t>::max(),
                                                       std::numeric_limits<uint32_t>::max())));

        const word_array<9, uint8_t> n8{n8_};
        const word_array<9, uint16_t> n16{n8_};

        const word_array<32, uint8_t> m8{n32_};
        const word_array<32, uint16_t> m16{n32_};

        WHEN("Splitting the word at some position")
        {
            THEN("Concatenating the resulting parts should yield the original word array")
            {
                {
                    const auto [left8, right8] = split<0>(n8);
                    const auto [left16, right16] = split<0>(n16);
                    REQUIRE(concat(left8, right8) == n8);
                    REQUIRE(concat(left16, right16) == n16);
                }
                {
                    const auto [left8, right8] = split<1>(n8);
                    const auto [left16, right16] = split<1>(n16);
                    REQUIRE(concat(left8, right8) == n8);
                    REQUIRE(concat(left16, right16) == n16);
                }
                {
                    const auto [left8, right8] = split<2>(n8);
                    const auto [left16, right16] = split<2>(n16);
                    REQUIRE(concat(left8, right8) == n8);
                    REQUIRE(concat(left16, right16) == n16);
                }
                {
                    const auto [left8, right8] = split<3>(n8);
                    const auto [left16, right16] = split<3>(n16);
                    REQUIRE(concat(left8, right8) == n8);
                    REQUIRE(concat(left16, right16) == n16);
                }
                {
                    const auto [left8, right8] = split<4>(n8);
                    const auto [left16, right16] = split<4>(n16);
                    REQUIRE(concat(left8, right8) == n8);
                    REQUIRE(concat(left16, right16) == n16);
                }

                {
                    const auto [left8, right8] = split<7>(n8);
                    const auto [left16, right16] = split<7>(n16);
                    REQUIRE(concat(left8, right8) == n8);
                    REQUIRE(concat(left16, right16) == n16);
                }

                {
                    const auto [left8, right8] = split<0>(m8);
                    const auto [left16, right16] = split<0>(m16);
                    REQUIRE(concat(left8, right8) == m8);
                    REQUIRE(concat(left16, right16) == m16);
                }
                {
                    const auto [left8, right8] = split<1>(m8);
                    const auto [left16, right16] = split<1>(m16);
                    REQUIRE(concat(left8, right8) == m8);
                    REQUIRE(concat(left16, right16) == m16);
                }
                {
                    const auto [left8, right8] = split<2>(m8);
                    const auto [left16, right16] = split<2>(m16);
                    REQUIRE(concat(left8, right8) == m8);
                    REQUIRE(concat(left16, right16) == m16);
                }
                {
                    const auto [left8, right8] = split<10>(m8);
                    const auto [left16, right16] = split<10>(m16);
                    REQUIRE(concat(left8, right8) == m8);
                    REQUIRE(concat(left16, right16) == m16);
                }
                {
                    const auto [left8, right8] = split<20>(m8);
                    const auto [left16, right16] = split<20>(m16);
                    REQUIRE(concat(left8, right8) == m8);
                    REQUIRE(concat(left16, right16) == m16);
                }

                {
                    const auto [left8, right8] = split<30>(m8);
                    const auto [left16, right16] = split<30>(m16);
                    REQUIRE(concat(left8, right8) == m8);
                    REQUIRE(concat(left16, right16) == m16);
                }
            }
        }
    }
}