#include <aarith/fixed_point.hpp>
#include <catch.hpp>
#include <sstream>

#include "../test-signature-ranges.hpp"
#include "gen_fixed_point.hpp"

SCENARIO("Check printing", "[integer][signed][utility][comparison]")
{
    GIVEN("A fixed point number")
    {
        using namespace aarith;
        using Fixed = fixed_point<32, 32>;

        const auto starts_with = [](const std::string& needle, const std::string& hay) -> bool {
            // https://stackoverflow.com/questions/1878001/
            return (hay.rfind(needle, 0) == 0);
        };

        const auto fixed_to_string = [](const Fixed& q) -> std::string {
            std::stringstream ss;
            ss << q;
            return ss.str();
        };

        WHEN("Looking at powers of two")
        {
            THEN("Check that the results starts correctly")
            {
                constexpr Fixed fp = Fixed::one();

                CAPTURE(fp, fp >> 1, fp >> 2, fp >> 3, fp >> 4, fp >> 5);
                CHECK(starts_with("1.0", fixed_to_string(fp)));
                CHECK(starts_with("0.5", fixed_to_string(fp >> 1)));
                CHECK(starts_with("0.25", fixed_to_string(fp >> 2)));
                CHECK(starts_with("0.125", fixed_to_string(fp >> 3)));
                CHECK(starts_with("0.0625", fixed_to_string(fp >> 4)));
                CHECK(starts_with("0.03125", fixed_to_string(fp >> 5)));
            }
        }

        WHEN("Looking at zero")
        {
            THEN("Check that the result is correct")
            {
                constexpr Fixed zero = Fixed::zero();
                CHECK(starts_with("0.0", fixed_to_string(zero)));
            }
        }
    }
}
