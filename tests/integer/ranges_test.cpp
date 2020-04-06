#include <aarith/integer.hpp>
#include <catch.hpp>

using namespace aarith;

SCENARIO("Adding two positive integers", "[integer][arithmetic][addition]")
{
    GIVEN("Two positive integer<N> a and b with N <= word_width")
    {
        constexpr size_t W = 8;
        using I = integer<W>;

        for (const I i : irange<I>(I::zero(), 8U))
        {
            std::cout << i << "\n";
        }
    }
}