#include <catch.hpp>
#include <iostream>

#include <aarith/posit.hpp>

SCENARIO("Extend size of posits")
{
    GIVEN("All posit8 values")
    {
        using namespace aarith;

        THEN("Assert that conversion to bigger widths does not change the value")
        {
            for (uint16_t u16 = 0; u16 <= 0x00ffU; ++u16)
            {
                const uint8_t u8 = static_cast<uint8_t>(u16);

                const posit8 p8 = posit8::from(u8);

                const posit16 extended = width_cast<posit16>(p8);
                const posit16 actual = posit16::from(u16 << 8);

                REQUIRE(extended == actual);
            }
        }
    }
}
