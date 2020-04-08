#include <aarith/integer.hpp>
#include <catch.hpp>

using namespace aarith;

SCENARIO("Adding two positive integers", "[integer][arithmetic][addition]")
{
    GIVEN("Two positive integer<N> a and b with N <= word_width")
    {
        constexpr size_t W = 8;
        using I = integer<W>;

        for (const I i : integer_range(I::zero(), I{8}))
        {
            std::cout << i << "\n";
        }

        for (const I i : integer_range(I::zero(), I{8}, I{2}))
        {
            std::cout << i << "\n";
        }
    }
}