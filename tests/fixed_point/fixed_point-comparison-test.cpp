#include <catch.hpp>

#include "gen_fixed_point.hpp"
#include <aarith/fixed_point.hpp>

#include "../test-signature-ranges.hpp"

using namespace aarith; // NO-LINT

// TODO make it work for signed and unsigned fixed_point numbers
TEMPLATE_TEST_CASE_SIG("Comparing two fixed-point numbers",
                       "[fixed_point][signed][unsigned][util][comparison]]",
                       AARITH_FIXED_TEST_EXTENDED_SIGNATURE, AARITH_FIXED_TEST_EXTENDED_PARAM_RANGE)
{
    GIVEN("Two fixed-point numbers storing the same value")
    {
        WHEN("The numbers store the same value")
        {
            using Fixed = fixed<I, F, BaseInt, WordType>;
            Fixed a = GENERATE(take(10, random_fixed_point<Fixed>()));

            std::cout << is_signed_v<Fixed> << " " << a << "\t" << to_binary(a) << "\n";

            AND_WHEN("The bit widths are identical")
            {

                Fixed b{a};

                CAPTURE(is_signed_v<Fixed>,I, F, a, b, to_binary(a), to_binary(b));

                THEN("The numbers should be equal")
                {
                    CHECK(a == b);
                    REQUIRE_FALSE(a != b);
                }
            }
            AND_WHEN("One number has more bits in the integral part")
            {

                using G = fixed<I + 5, F, BaseInt, WordType>;
                G b = width_cast<G::int_width, G::frac_width>(a);

                CAPTURE(is_signed_v<Fixed>, is_signed_v<G>, I, F, G::int_width, G::frac_width, a, b, to_binary(a),
                        to_binary(b));



                THEN("The numbers should be equal")
                {
                    CHECK(a == b);
                    REQUIRE_FALSE(a != b);
                }
            }

            AND_WHEN("One number has more bits in the fractional part")
            {

                using G = fixed<I, F + 10, BaseInt, WordType>;
                G b = width_cast<G::int_width, G::frac_width>(a);

                CAPTURE(is_signed_v<Fixed>, is_signed_v<G>, I, F, G::int_width, G::frac_width, a, b, to_binary(a),
                        to_binary(b));

                THEN("The numbers should be equal")
                {
                    CHECK(a == b);
                    REQUIRE_FALSE(a != b);
                }
            }
        }
    }
}