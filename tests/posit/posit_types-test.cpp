#include <aarith/posit.hpp>
#include <catch.hpp>

SCENARIO("Standard Posit Types")
{
    GIVEN("Standard posit types")
    {
        using namespace aarith;

        THEN("Check that N is correct")
        {
            CHECK(posit8::width() == 8);
            CHECK(posit16::width() == 16);
            CHECK(posit32::width() == 32);
            CHECK(posit64::width() == 64);
        }

        THEN("Check that ES is correct")
        {
            CHECK(posit8::exponent_size() == 2);
            CHECK(posit16::exponent_size() == 2);
            CHECK(posit32::exponent_size() == 2);
            CHECK(posit64::exponent_size() == 2);
        }
    }
}
