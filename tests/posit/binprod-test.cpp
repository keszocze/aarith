#include <catch.hpp>
#include <iostream>

#include "../test-signature-ranges.hpp"
#include <aarith/posit.hpp>
#include <aarith/posit/binprod.hpp>
#include <cstdint>

SCENARIO("Encoding and decoding between posit and binprod")
{
    GIVEN("All posit<8, 2> values")
    {
        THEN("Assert that conversion to binprod preserves the value")
        {
            for (uint16_t bits = 0; bits <= 0x00ffU; ++bits)
            {
                using namespace aarith;
                using Posit = posit<8, 2, uint64_t>;

                const uint8_t actual = static_cast<uint8_t>(bits);
                const Posit p = Posit(actual);

                if (p != p.complex_infinity())
                {
                    std::cout << "p=" << to_binary(p, " ") << std::endl;

                    const binprod<8> bp(p);
                    const Posit converted_back = bp.to_posit<8, 2, uint64_t>();

                    REQUIRE(to_binary(p, " ") == to_binary(converted_back, " "));
                }
            }
        }
    }
}
