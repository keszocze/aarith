#include <catch.hpp>

#include <aarith/types/word_container.hpp>
#include <aarith/utilities/bit_operations.hpp>
using namespace aarith;

SCENARIO("Splitting uint64_ts", "[util]")
{
    WHEN("Considering an uint64_t n")
    {
        THEN("Splitting and unsplitting should yield the original number")
        {
            uint64_t n = 0u;

            for (uint i = 0; i < 10000000; i++)
            {
                const auto [u, l] = split(n);
                const auto result = unsplit(u, l);
                REQUIRE(result == n);

                n += 15381;
            }
        }
    }
}

SCENARIO("Counting bits in word_container", "[util]")
{
    WHEN("Multiple word_containers are given")
    {
        THEN("The leading zeroes should be counted correctly")
        {
            word_container<64> a_zero{0U};
            word_container<64> a_one{1U};
            word_container<32> b_zero{0U};
            word_container<32> b_one{1U};
            word_container<15> c_zero{0U};
            word_container<15> c_one{1U};
            word_container<150> d_zero{0U};
            word_container<150> d_one{1U};
            
            CHECK(count_leading_zeroes(a_zero) == 64);
        }
    }
}