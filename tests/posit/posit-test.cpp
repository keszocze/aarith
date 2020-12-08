#include <catch.hpp>
#include <iostream>

#include <aarith/integer.hpp>
#include <aarith/posit.hpp>
#include <iostream>

using namespace aarith;

constexpr size_t NBits = 6;
constexpr size_t ExponentSize = 2;
using WordType = uint16_t;

using P8 = aarith::posit<NBits, ExponentSize, WordType>;

SCENARIO("Posit Constants")
{
    GIVEN("Some posit values provided by the posit class")
    {
        THEN("Assert that they return the expected")
        {
            const P8 min = P8::min();
            CHECK(to_binary(min) == "100001");

            const P8 minpos = P8::minpos();
            CHECK(to_binary(minpos) == "000001");

            const P8 max = P8::max();
            CHECK(to_binary(max) == "011111");

            const P8 zero = P8::zero();
            CHECK(to_binary(zero) == "000000");

            const P8 one = P8::one();
            CHECK(to_binary(one) == "010000");

            const P8 cinf = P8::complex_infinity();
            CHECK(to_binary(cinf) == "100000");

            const auto npat = P8::npat();
            CHECK(to_binary(npat) == "1000000");
        }
    }
}
