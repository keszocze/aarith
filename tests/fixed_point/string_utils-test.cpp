#include <catch.hpp>

#include <aarith/fixed_point.hpp>

using namespace aarith;


SCENARIO("Testing the to_binary method") {
    GIVEN("The number zero in various bit combinations")
    {
        THEN("To binary should give the correct bit string")
        {
            fixed_point<3, 4> zero_3_4;
            fixed_point<6, 4> zero_6_4;
            fixed_point<3, 15> zero_3_15;

            REQUIRE(to_binary(zero_3_4) == "000.0000");
            REQUIRE(to_binary(zero_6_4) == "000000.0000");
            REQUIRE(to_binary(zero_3_15) == "000.000000000000000");

            ufixed_point<3, 4> uzero_3_4;
            ufixed_point<6, 4> uzero_6_4;
            ufixed_point<3, 15> uzero_3_15;

            REQUIRE(to_binary(uzero_3_4) == "000.0000");
            REQUIRE(to_binary(uzero_6_4) == "000000.0000");
            REQUIRE(to_binary(uzero_3_15) == "000.000000000000000");
        }
    }
}