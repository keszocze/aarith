#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"

SCENARIO("Check if sqrt returns correct results")
{
    using namespace aarith;

    GIVEN("Some arbitrary posits")
    {
        THEN("Assert that sqrt terminates and returns correct results")
        {
            {
                using Posit = posit<3, 1>;
                REQUIRE(sqrt(Posit(2)) == Posit(1));
            }

            {
                using Posit = posit8;
                REQUIRE(sqrt(Posit(2)) == Posit(1.5));
            }

            {
                using Posit = posit16;
                REQUIRE(sqrt(Posit(2)) == Posit(1.4140625));
            }

            {
                using Posit = posit32;
                REQUIRE(sqrt(Posit(2)) == Posit(1.414213560521602630615234375));
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("sqrt special cases", "[posit][template]", ((size_t N, size_t ES), N, ES),
                       AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;
    using Posit = posit<N, ES>;

    const Posit nar = Posit::nar();
    REQUIRE(sqrt(nar) == nar);

    const Posit zero = Posit::zero();
    REQUIRE(sqrt(zero) == zero);

    const Posit one = Posit::one();
    REQUIRE(sqrt(one) == one);

    const Posit none = -Posit::one();
    REQUIRE(sqrt(none) == nar);

    const Posit min = Posit::min();
    REQUIRE(sqrt(min) == nar);
}
