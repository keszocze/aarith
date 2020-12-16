#include <catch.hpp>
#include <iostream>

#include <aarith/posit.hpp>
#include <iostream>

SCENARIO("Standard Posit Types")
{
    GIVEN("Standard posit types")
    {
        using namespace aarith;

        THEN("Check that N is correct")
        {
            CHECK(posit8::n() == 8);
            CHECK(posit16::n() == 16);
            CHECK(posit32::n() == 32);
            CHECK(posit64::n() == 64);
        }

        THEN("Check that ES is correct")
        {
            CHECK(posit8::es() == 2);
            CHECK(posit16::es() == 2);
            CHECK(posit32::es() == 2);
            CHECK(posit64::es() == 2);
        }

        THEN("Check that the types are not bigger than necessary")
        {
            CHECK(sizeof(posit8) == 1);
            CHECK(sizeof(posit16) == 2);
            CHECK(sizeof(posit32) == 4);
            CHECK(sizeof(posit64) == 8);
        }
    }
}
