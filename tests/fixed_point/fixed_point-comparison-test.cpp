#include <catch.hpp>

#include <aarith/fixed_point.hpp>
#include "gen_fixed_point.hpp"

using namespace aarith;


SCENARIO("Comparing fixed-point numbers") {
    GIVEN("Two fixed-point numbers") {
        WHEN("The numbers store the same value") {
            AND_WHEN("The bit widths are identical") {

                using F = fixed_point<32,17,uint32_t>;

                F a = GENERATE(take(10, random_fixed_point<32,17,fixed_point,uint32_t>()));
                F b{a};
                THEN("The numbers should be equal") {
                    CHECK(a == b);
                    REQUIRE_FALSE(a != b);
                }
            }
            AND_WHEN("One number has more bits in the integral part") {

                using F = fixed_point<32,17,uint32_t>;
                using G = fixed_point<40,17,uint32_t>;
                F a = GENERATE(take(10, random_fixed_point<32,17,fixed_point,uint32_t>()));

                G b = width_cast<40,17>(a);

                THEN("The numbers should be equal") {
                    CHECK(a == b);
                    REQUIRE_FALSE(a != b);
                }
            }

            AND_WHEN("One number has more bits in the fractional part") {

                using F = fixed_point<32,17,uint32_t>;
                using G = fixed_point<32,20,uint32_t>;
                F a = GENERATE(take(10, random_fixed_point<32,17,fixed_point,uint32_t>()));

                G b = width_cast<32,20>(a);

                THEN("The numbers should be equal") {
                    CHECK(a == b);
                    REQUIRE_FALSE(a != b);
                }
            }
        }
    }
}