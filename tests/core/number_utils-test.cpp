#include <catch.hpp>

#include <aarith/core.hpp>

using namespace aarith;

SCENARIO("Finding the first set bit", "[word_array][utility]")
{
    GIVEN("A power of two")
    {
        THEN("Finding the first set bit should work")
        {
            constexpr size_t one = 1UL;

            for (size_t i = 0UL; i < 64UL; ++i)
            {
                const size_t val = (one << i);
                CHECK(first_set_bit(val) == (i + 1UL));
            }
        }
    }
}

SCENARIO("Rounding down to the largest power of two", "[word_array][utility]")
{
    GIVEN("A power of two")
    {
        THEN("The rounding operation should not change the number")
        {
            constexpr size_t one = 1UL;

            for (size_t i = 0; i < 64UL; ++i)
            {
                const size_t val = (one << i);
                CHECK(floor_to_pow(val) == val);
            }
        }
    }
    GIVEN("A random number")
    {
        WHEN("Rounding down to the next power of two")
        {
            THEN("The rounded number must not be larger than the original number")
            {
                const size_t val = GENERATE(take(100, random(std::numeric_limits<size_t>::max(),
                                                             std::numeric_limits<size_t>::max())));

                CHECK(floor_to_pow(val) <= val);
            }
        }
    }
}
