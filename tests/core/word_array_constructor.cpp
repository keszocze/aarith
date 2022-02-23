#include <catch.hpp>

#include "../test-signature-ranges.hpp"
#include <aarith/core.hpp>
#include <aarith/core/core_string_utils.hpp>

#include <bitset>
#include <string>
using namespace aarith;

SCENARIO("Generating word arrays from strings", "[word_array]")
{

    constexpr size_t W1 = 32;
    constexpr size_t W2 = 15;
    constexpr size_t W3 = 64;
    constexpr size_t W4 = 80;
    using T1 = aarith::word_array<W1>;
    using T2 = aarith::word_array<W2>;
    using T3 = aarith::word_array<W3>;
    using T4 = aarith::word_array<W4>;

    GIVEN("A random uint32_t n")
    {
        uint32_t n = GENERATE(take(50, random(std::numeric_limits<uint32_t>::min(),
                                              std::numeric_limits<uint32_t>::max())));
        WHEN("n exactly fits the word_array (with a single word stored)")
        {
            THEN("Using n and the string representation of n should yield the same word_array")
            {
                REQUIRE(T1{n} == T1{std::bitset<W1>(n).to_string()});
            }
        }
        WHEN("n has more bits than the word array (with a single word stored)")
        {
            THEN("Using n and the string representation of n should yield the same word_array")
            {
                //                std::cout << T2{n} << " == " <<
                //                T2{std::bitset<W2>(n).to_string()) << "\n";
                REQUIRE(T2{n} == T2{std::bitset<W2>(n).to_string()});
            }
        }
        WHEN("n has less bits than the word array (with a single word stored)")
        {
            THEN("Using n and the string representation of n should yield the same word_array")
            {
                //                std::cout << T2{n} << " == " <<
                //                T2{std::bitset<W2>(n).to_string()) << "\n";
                REQUIRE(T3{n} == T3{std::bitset<W3>(n).to_string()});
            }
        }

        WHEN("n has less bits than the word array (with a single word stored)")
        {
            THEN("Using n and the string representation of n should yield the same word_array")
            {
                //                std::cout << T2{n} << " == " <<
                //                T2{std::bitset<W2>(n).to_string()) << "\n";
                REQUIRE(T3{n} == T3{std::bitset<W3>(n).to_string()});
            }
        }

        WHEN("n has less bits than the word array (with two stored words)")
        {
            THEN("Using n and the string representation of n should yield the same word_array")
            {
                //                std::cout << T2{n} << " == " <<
                //                T2{std::bitset<W2>(n).to_string()) << "\n";
                REQUIRE(T4{n} == T4{std::bitset<W4>(n).to_string()});
            }
        }
    }

    GIVEN("A random uint64_t n")
    {
        uint64_t n = GENERATE(take(50, random(std::numeric_limits<uint64_t>::min(),
                                              std::numeric_limits<uint64_t>::max())));
        WHEN("n exactly fits the word_array (with a single word stored)")
        {
            THEN("Using n and the string representation of n should yield the same word_array")
            {
                REQUIRE(T3{n} == T3{std::bitset<W3>(n).to_string()});
            }
        }
        WHEN("n has more bits than the word array (with a single word stored)")
        {
            THEN("Using n and the string representation of n should yield the same word_array")
            {
                //                std::cout << T2{n} << " == " <<
                //                T2{std::bitset<W2>(n).to_string()) << "\n";
                REQUIRE(T1{n} == T1{std::bitset<W1>(n).to_string()});
                REQUIRE(T2{n} == T2{std::bitset<W2>(n).to_string()});
            }
        }
        WHEN("n has less bits than the word array (with a single word stored)")
        {
            THEN("Using n and the string representation of n should yield the same word_array")
            {
                //                std::cout << T2{n} << " == " <<
                //                T2{std::bitset<W2>(n).to_string()) << "\n";
                REQUIRE(T3{n} == T3{std::bitset<W3>(n).to_string()});
            }
        }

        WHEN("n has less bits than the word array (with two stored words)")
        {
            THEN("Using n and the string representation of n should yield the same word_array")
            {
                //                std::cout << T2{n} << " == " <<
                //                T2{std::bitset<W2>(n).to_string()) << "\n";
                REQUIRE(T4{n} == T4{std::bitset<W4>(n).to_string()});
            }
        }
    }
}

SCENARIO("from_bit_string is constexpr")
{
    // simply use a hand-picked constant value to check, if this compiles everything should be fine
    static const aarith::word_array<32> vals{"1101"};
    static constexpr aarith::word_array<32> valn{13};


    REQUIRE(vals == valn);
}

SCENARIO("Feeding from_bit_string with invalid data")
{
    GIVEN("A string with characters not '0' or '1'")
    {
        constexpr std::string_view s{"11incorrect input"};

        WHEN("Trying to create a word_array from this data")
        {
            THEN("The function should throw an exception")
            {
                REQUIRE_THROWS(aarith::word_array<24>{s});
            }
        }
    }
}
SCENARIO("Bit strings lengths' and word_arrays's lengths ")
{
    GIVEN("A bit string that is longer than the word_array")
    {
        constexpr std::string_view s{"100011"};

        WHEN("Constructing the word_array from the bit string")
        {
            const aarith::word_array<5> a{aarith::word_array<5>{s}};

            THEN("The correct bits should be truncated correctly")
            {
                aarith::word_array<5> expected{3};
                REQUIRE(a == expected);
            }
        }
    }
    GIVEN("A bit string that is shorter than the word_array")
    {
        constexpr std::string_view w{"11111"};
        WHEN("Constructing the word_array from the bit string")
        {
            const aarith::word_array<10> b{aarith::word_array<10>{w}};
            THEN("The correct bits should be truncated correctly")
            {
                aarith::word_array<10> ex{31};
                REQUIRE(b == ex);
            }
        }
    }
}