#include <aarith/integer.hpp>
#include <catch.hpp>

using namespace aarith;

SCENARIO("Adding two positive integers", "[integer][arithmetic][addition]")
{
    GIVEN("Two positive integer<N> a and b with N <= word_width")
    {
        constexpr size_t W = 8;
        using I = integer<W>;
        integer_range<I> r = std::make_pair(I::one(), 8U);

        for (const I i: r) {
            std::cout << i << "\n";
        }
    }
}