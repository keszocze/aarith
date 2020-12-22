#include <aarith/float.hpp>
#include <bitset>
#include <catch.hpp>

#include "../test-signature-ranges.hpp"
using namespace aarith;

TEMPLATE_TEST_CASE_SIG("String generation for the zeros",
                       "[normalized_float][utility][string][invariant]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    using F = normalized_float<E, M>;

    GIVEN("Positive and negative zero")
    {

        F pos = F::zero();
        F neg = F::neg_zero();

        WHEN("Checking for 'zeroness'")
        {
            THEN("The answer is 'yes' twice")
            {
                CHECK(pos.is_zero());
                REQUIRE(neg.is_zero());
            }
            AND_THEN("The sign is stored correctly")
            {
                CHECK(pos.is_positive());
                CHECK(!pos.is_negative());
                CHECK(!neg.is_positive());
                REQUIRE(neg.is_negative());
            }
        }

        WHEN("Creating the compute string")
        {
            const std::string pos_compute_string = to_compute_string(pos);
            const std::string neg_compute_string = to_compute_string(neg);
            THEN("The result should be '0' /  '-0'")
            {
                const std::string expected_pos_zero_compute_string("0");
                const std::string expected_neg_zero_compute_string("-0");
                CHECK(expected_pos_zero_compute_string == pos_compute_string);
                REQUIRE(expected_neg_zero_compute_string == neg_compute_string);
                //                std::cout << "pos: " << pos_compute_string << "\n";
                //                std::cout << "neg: " << neg_compute_string << "\n";
            }
        }

        WHEN("Creating the  scientific notation string")
        {
            const std::string pos_scientific_string = to_sci_string(pos);
            const std::string neg_scientific_string = to_sci_string(neg);

            THEN("The result should be '0' /  '-0'")
            {
                const std::string expected_pos_zero_scientific_string("0");
                const std::string expected_neg_zero_scientific_string("-0");
                CHECK(pos_scientific_string == expected_pos_zero_scientific_string);
                REQUIRE(neg_scientific_string == expected_neg_zero_scientific_string);
                //                std::cout << "pos: " << pos_compute_string << "\n";
                //                std::cout << "neg: " << neg_compute_string << "\n";
            }
        }
    }
}
