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
                REQUIRE(T1{n} == T1::from_bit_string(std::bitset<W1>(n).to_string()));
            }
        }
        WHEN("n has more bits than the word array (with a single word stored)")
        {
            THEN("Using n and the string representation of n should yield the same word_array")
            {
                //                std::cout << T2{n} << " == " <<
                //                T2::from_bit_string(std::bitset<W2>(n).to_string()) << "\n";
                REQUIRE(T2{n} == T2::from_bit_string(std::bitset<W2>(n).to_string()));
            }
        }
        WHEN("n has less bits than the word array (with a single word stored)")
        {
            THEN("Using n and the string representation of n should yield the same word_array")
            {
                //                std::cout << T2{n} << " == " <<
                //                T2::from_bit_string(std::bitset<W2>(n).to_string()) << "\n";
                REQUIRE(T3{n} == T3::from_bit_string(std::bitset<W3>(n).to_string()));
            }
        }

        WHEN("n has less bits than the word array (with a single word stored)")
        {
            THEN("Using n and the string representation of n should yield the same word_array")
            {
                //                std::cout << T2{n} << " == " <<
                //                T2::from_bit_string(std::bitset<W2>(n).to_string()) << "\n";
                REQUIRE(T3{n} == T3::from_bit_string(std::bitset<W3>(n).to_string()));
            }
        }

        WHEN("n has less bits than the word array (with two stored words)")
        {
            THEN("Using n and the string representation of n should yield the same word_array")
            {
                //                std::cout << T2{n} << " == " <<
                //                T2::from_bit_string(std::bitset<W2>(n).to_string()) << "\n";
                REQUIRE(T4{n} == T4::from_bit_string(std::bitset<W4>(n).to_string()));
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
                REQUIRE(T3{n} == T3::from_bit_string(std::bitset<W3>(n).to_string()));
            }
        }
        WHEN("n has more bits than the word array (with a single word stored)")
        {
            THEN("Using n and the string representation of n should yield the same word_array")
            {
                //                std::cout << T2{n} << " == " <<
                //                T2::from_bit_string(std::bitset<W2>(n).to_string()) << "\n";
                REQUIRE(T1{n} == T1::from_bit_string(std::bitset<W1>(n).to_string()));
                REQUIRE(T2{n} == T2::from_bit_string(std::bitset<W2>(n).to_string()));
            }
        }
        WHEN("n has less bits than the word array (with a single word stored)")
        {
            THEN("Using n and the string representation of n should yield the same word_array")
            {
                //                std::cout << T2{n} << " == " <<
                //                T2::from_bit_string(std::bitset<W2>(n).to_string()) << "\n";
                REQUIRE(T3{n} == T3::from_bit_string(std::bitset<W3>(n).to_string()));
            }
        }

        WHEN("n has less bits than the word array (with two stored words)")
        {
            THEN("Using n and the string representation of n should yield the same word_array")
            {
                //                std::cout << T2{n} << " == " <<
                //                T2::from_bit_string(std::bitset<W2>(n).to_string()) << "\n";
                REQUIRE(T4{n} == T4::from_bit_string(std::bitset<W4>(n).to_string()));
            }
        }
    }
}

SCENARIO("from bit string is constexpr")
{
    static constexpr aarith::word_array<32> vals = aarith::word_array<32>::from_bit_string("1101");
    static constexpr aarith::word_array<32> valn = aarith::word_array<32>{13};
    REQUIRE(vals == valn);
}