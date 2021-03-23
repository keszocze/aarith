#include <aarith/float.hpp>
#include <aarith/float/numeric_limits.hpp>

#include <catch.hpp>
#include <iostream>

using namespace aarith; // NOLINT

SCENARIO("std::numeric_limits are instantiated correctly", "[floating-pint][utility]")
{
    GIVEN("The aarith single_precision floating-point type")
    {
        THEN("The numeric_limit values should match the float ones")
        {
            using fbase = std::numeric_limits<float>;
            using abase = std::numeric_limits<single_precision >;
            
            CHECK(abase::is_specialized == fbase::is_specialized);
            CHECK(abase::is_signed == fbase::is_signed);
            CHECK(abase::is_integer == fbase::is_integer);
            CHECK(abase::is_exact == fbase::is_exact);
            CHECK(abase::has_infinity == fbase::has_infinity);
            CHECK(abase::has_quiet_NaN == fbase::has_quiet_NaN);
            CHECK(abase::has_signaling_NaN == fbase::has_signaling_NaN);
            CHECK(abase::is_bounded == fbase::is_bounded);
            CHECK(abase::has_denorm == fbase::has_denorm);
            CHECK(abase::has_denorm_loss == fbase::has_denorm_loss);
            CHECK(abase::round_style == fbase::round_style);
            CHECK(abase::is_iec559 == fbase::is_iec559);
            CHECK(abase::is_modulo == fbase::is_modulo);
            CHECK(abase::radix == fbase::radix);
            CHECK(abase::digits == fbase::digits);
            CHECK(abase::digits10 == fbase::digits10);
            CHECK(abase::max_digits10 == fbase::max_digits10);
            CHECK(abase::min_exponent10 == fbase::min_exponent10);
            CHECK(abase::min_exponent == fbase::min_exponent);
            CHECK(abase::max_exponent == fbase::max_exponent);
            CHECK(abase::max_digits10 == fbase::max_exponent10);
            CHECK(abase::tinyness_before == fbase::tinyness_before);
//            CHECK(static_cast<base>(abase::min().word(0)) == fbase::min());
//            CHECK(static_cast<base>(abase::lowest().word(0)) == fbase::lowest());
//            CHECK(abase::max().word(0) == fbase::max());
//            CHECK(abase::epsilon().word(0) == fbase::epsilon());
//            CHECK(abase::round_error().word(0) == fbase::round_error());
//            CHECK(abase::infinity().word(0) == fbase::infinity());
//            CHECK(abase::quiet_NaN().word(0) == fbase::quiet_NaN());
//            CHECK(abase::signaling_NaN().word(0) == fbase::signaling_NaN());
//            CHECK(abase::denorm_min().word(0) == fbase::denorm_min());
        }
    }
}