#include <aarith/integer/approx_operations.hpp>
#include <aarith/integer_no_operators.hpp>
#include <catch.hpp>
#include <iostream>
#include <sstream>

using namespace aarith;

SCENARIO("Using the FAU adder", "[uinteger][arithmetic][approximate]")
{

    GIVEN("Two uintegers")
    {
        const uinteger<8> a{23};
        const uinteger<8> b{1337};

        WHEN("Simply using the FAU adder")
        {
            THEN("The addition should be commutative")
            {
                REQUIRE(FAUadder<8, 1>(a, b) == FAUadder<8, 1>(b, a));
                REQUIRE(FAUadder<8, 2>(a, b) == FAUadder<8, 2>(b, a));
                REQUIRE(FAUadder<8, 3>(a, b) == FAUadder<8, 3>(b, a));
                REQUIRE(FAUadder<8, 4>(a, b) == FAUadder<8, 4>(b, a));
                REQUIRE(FAUadder<8, 5>(a, b) == FAUadder<8, 5>(b, a));
                REQUIRE(FAUadder<8, 6>(a, b) == FAUadder<8, 6>(b, a));
                REQUIRE(FAUadder<8, 7>(a, b) == FAUadder<8, 7>(b, a));
            }

            THEN ("The FAU adder's results should never exceed the correct ones") {
                REQUIRE(FAUadder<8, 1, 0>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 2, 1>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 3, 2>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 4, 3>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 5, 4>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 6, 5>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 7, 6>(a, b) <= expanding_add(a, b));

                REQUIRE(FAUadder<8, 1, 0>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 2, 0>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 3, 1>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 4, 2>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 5, 3>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 6, 4>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 7, 5>(a, b) <= expanding_add(a, b));

                REQUIRE(FAUadder<8, 1, 0>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 2, 0>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 3, 0>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 4, 1>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 5, 2>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 6, 3>(a, b) <= expanding_add(a, b));
                REQUIRE(FAUadder<8, 7, 4>(a, b) <= expanding_add(a, b));
            }

            THEN("Using less shared bits should result in smaller sums")
            {

                REQUIRE(FAUadder<8, 4, 0>(a, b) <= FAUadder<8, 4, 1>(a, b));
                REQUIRE(FAUadder<8, 4, 0>(a, b) <= FAUadder<8, 4, 2>(a, b));
                REQUIRE(FAUadder<8, 4, 0>(a, b) <= FAUadder<8, 4, 3>(a, b));

                REQUIRE(FAUadder<8, 4, 1>(a, b) <= FAUadder<8, 4, 2>(a, b));
                REQUIRE(FAUadder<8, 4, 1>(a, b) <= FAUadder<8, 4, 3>(a, b));

                REQUIRE(FAUadder<8, 4, 2>(a, b) <= FAUadder<8, 4, 3>(a, b));


                REQUIRE(FAUadder<8, 5, 0>(a, b) <= FAUadder<8, 5, 1>(a, b));
                REQUIRE(FAUadder<8, 5, 0>(a, b) <= FAUadder<8, 5, 2>(a, b));
                REQUIRE(FAUadder<8, 5, 0>(a, b) <= FAUadder<8, 5, 3>(a, b));
                REQUIRE(FAUadder<8, 5, 0>(a, b) <= FAUadder<8, 5, 4>(a, b));

                REQUIRE(FAUadder<8, 5, 1>(a, b) <= FAUadder<8, 5, 2>(a, b));
                REQUIRE(FAUadder<8, 5, 1>(a, b) <= FAUadder<8, 5, 3>(a, b));
                REQUIRE(FAUadder<8, 5, 1>(a, b) <= FAUadder<8, 5, 4>(a, b));

                REQUIRE(FAUadder<8, 5, 2>(a, b) <= FAUadder<8, 5, 3>(a, b));
                REQUIRE(FAUadder<8, 5, 2>(a, b) <= FAUadder<8, 5, 4>(a, b));
            }
        }

        WHEN("Using as many shared bits as possible")
        {
            THEN("The addition should be an exact addition")
            {

                REQUIRE(FAUadder<8, 1, 1>(a, b) == expanding_add(a, b));
                REQUIRE(FAUadder<8, 2, 2>(a, b) == expanding_add(a, b));
                REQUIRE(FAUadder<8, 3, 3>(a, b) == expanding_add(a, b));
                REQUIRE(FAUadder<8, 4, 4>(a, b) == expanding_add(a, b));
                REQUIRE(FAUadder<8, 5, 5>(a, b) == expanding_add(a, b));
                REQUIRE(FAUadder<8, 6, 6>(a, b) == expanding_add(a, b));
                REQUIRE(FAUadder<8, 7, 7>(a, b) == expanding_add(a, b));
            }
        }
    }
}
