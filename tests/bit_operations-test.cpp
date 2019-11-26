#include <catch.hpp>

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