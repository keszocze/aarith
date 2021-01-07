#include <catch.hpp>

#include <aarith/posit.hpp>

#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

SCENARIO("Quire Sizes")
{
    using namespace aarith;

    GIVEN("Standard Draft Types")
    {
        THEN("Assert that the computed sizes are as expected")
        {
            CHECK(quire_integer_width(8, 2) == 48);
            CHECK(quire_fraction_width(8, 2) == 48);
            CHECK(quire_carry_width(8, 2) == 31);
            CHECK(quire_width(8, 2) == 128);

            CHECK(quire_integer_width(16, 2) == 112);
            CHECK(quire_fraction_width(16, 2) == 112);
            CHECK(quire_carry_width(16, 2) == 31);
            CHECK(quire_width(16, 2) == 256);

            CHECK(quire_integer_width(32, 2) == 240);
            CHECK(quire_fraction_width(32, 2) == 240);
            CHECK(quire_carry_width(32, 2) == 31);
            CHECK(quire_width(32, 2) == 512);

            CHECK(quire_integer_width(64, 2) == 496);
            CHECK(quire_fraction_width(64, 2) == 496);
            CHECK(quire_carry_width(64, 2) == 31);
            CHECK(quire_width(64, 2) == 1024);
        }
    }

    GIVEN("Esoteric Types")
    {
        THEN("Assert that the computed sizes are as expected")
        {
            CHECK(quire_integer_width(8, 0) == 12);
            CHECK(quire_fraction_width(8, 0) == 12);
            CHECK(quire_carry_width(8, 0) == 39);
            CHECK(quire_width(8, 0) == 64);

            CHECK(quire_integer_width(99, 2) == 776);
            CHECK(quire_fraction_width(99, 2) == 776);
            CHECK(quire_carry_width(99, 2) == 495);
            CHECK(quire_width(99, 2) == 2048);
        }
    }
}
