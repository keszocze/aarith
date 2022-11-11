#include <aarith/fixed_point.hpp>
#include <catch.hpp>
#include <sstream>

#include "../test-signature-ranges.hpp"
#include "gen_fixed_point.hpp"

using namespace aarith;

const auto starts_with = [](const std::string& needle, const std::string& hay) -> bool {
    // https://stackoverflow.com/questions/1878001/
    return (hay.rfind(needle, 0) == 0);
};

template <typename F>
const auto fixed_to_string = [](const F& q) -> std::string {
    std::stringstream ss;
    ss << q;
    return ss.str();
};

SCENARIO("Check printing", "[fixed_point][signed][utility][string]")
{
    GIVEN("A fixed point number")
    {
        using Fixed = fixed_point<32, 32>;

        WHEN("Looking at powers of two")
        {
            THEN("Check that the results starts correctly")
            {
                constexpr Fixed fp = Fixed::one();

                CAPTURE(fp, fp >> 1, fp >> 2, fp >> 3, fp >> 4, fp >> 5);
                CHECK(starts_with("1", fixed_to_string<Fixed>(fp)));
                CHECK(starts_with("0.5", fixed_to_string<Fixed>(fp >> 1)));
                CHECK(starts_with("0.25", fixed_to_string<Fixed>(fp >> 2)));
                CHECK(starts_with("0.125", fixed_to_string<Fixed>(fp >> 3)));
                CHECK(starts_with("0.0625", fixed_to_string<Fixed>(fp >> 4)));
                CHECK(starts_with("0.03125", fixed_to_string<Fixed>(fp >> 5)));
            }
        }

        WHEN("Looking at negative powers of two")
        {
            THEN("Check that the results starts correctly")
            {
                const Fixed fp = -Fixed::one();

                CAPTURE(fp, fp >> 1, fp >> 2, fp >> 3, fp >> 4, fp >> 5);
                CHECK(starts_with("-1", fixed_to_string<Fixed>(fp)));
                CHECK(starts_with("-0.5", fixed_to_string<Fixed>(fp >> 1)));
                CHECK(starts_with("-0.25", fixed_to_string<Fixed>(fp >> 2)));
                CHECK(starts_with("-0.125", fixed_to_string<Fixed>(fp >> 3)));
                CHECK(starts_with("-0.0625", fixed_to_string<Fixed>(fp >> 4)));
                CHECK(starts_with("-0.03125", fixed_to_string<Fixed>(fp >> 5)));
            }
        }

        WHEN("Looking at zero")
        {
            THEN("Check that the result is correct")
            {
                constexpr Fixed zero = Fixed::zero();
                CHECK(starts_with("0", fixed_to_string<Fixed>(zero)));
            }
        }
    }
}

SCENARIO("Testing the to_binary method")
{
    GIVEN("The number zero in various bit widths")
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

SCENARIO("Testing to string method for unsigned fixed points",
         "[utility][unsigned][string][fixed_point]")
{
    GIVEN("A hand-picked value with MSB=1")
    {
        using F = ufixed_point<3, 5>;
        F a = F::from_bitstring(aarith::word_array<8>{0b11111111});

        CAPTURE(a, to_binary(a));

        THEN("The number string must not start with a minus sign")
        {
            CHECK_FALSE(starts_with("-", fixed_to_string<F>(a)));
        }
    }
}
