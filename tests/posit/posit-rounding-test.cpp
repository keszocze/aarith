#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"
#include "for_each_posit.hpp"

SCENARIO("Check if the returned rounding modes seem correct")
{
    using namespace aarith;

    GIVEN("Some arbitrary posit<4, 1> values")
    {
        using Posit = posit<4, 1>;

        constexpr auto down = rounding_event::ROUNDED_DOWN;
        constexpr auto up = rounding_event::ROUNDED_UP;

        // Get the four values on the u-lattice.

        const Posit one = Posit(1);
        const Posit two = Posit(2);
        const Posit four = Posit(4);
        const Posit sixteen = Posit(16);

        // Check that they convert correctly. This is a bit paranoid, but
        // it does not hurt to do it.

        REQUIRE(static_cast<double>(one) == 1.0);
        REQUIRE(static_cast<double>(two) == 2.0);
        REQUIRE(static_cast<double>(four) == 4.0);
        REQUIRE(static_cast<double>(sixteen) == 16.0);

        THEN("Assert that rounding for positive values behaves as expected")
        {
            {
                const auto [sum, rbit] = add(one, two);
                REQUIRE(sum == four);
                REQUIRE(rbit == up);
            }

            {
                const auto [sum, rbit] = add(one, four);
                REQUIRE(sum == four);
                REQUIRE(rbit == down);
            }

            {
                const auto [sum, rbit] = add(one, sixteen);
                REQUIRE(sum == sixteen);
                REQUIRE(rbit == down);
            }

            {
                const auto [sum, rbit] = add(two, four);
                REQUIRE(sum == four);
                REQUIRE(rbit == down);
            }

            {
                const auto [sum, rbit] = add(two, sixteen);
                REQUIRE(sum == sixteen);
                REQUIRE(rbit == down);
            }

            {
                const auto [sum, rbit] = add(four, sixteen);
                REQUIRE(sum == sixteen);
                REQUIRE(rbit == down);
            }
        }

        THEN("Assert that rounding for negative values behaves as expected")
        {
            const Posit neg_one = -one;
            const Posit neg_two = -two;
            const Posit neg_four = -four;
            const Posit neg_sixteen = -sixteen;

            {
                const auto [sum, rbit] = add(neg_one, neg_two);
                REQUIRE(sum == neg_four);
                REQUIRE(rbit == down);
            }

            {
                const auto [sum, rbit] = add(neg_one, neg_four);
                REQUIRE(sum == neg_four);
                REQUIRE(rbit == up);
            }

            {
                const auto [sum, rbit] = add(neg_one, neg_sixteen);
                REQUIRE(sum == neg_sixteen);
                REQUIRE(rbit == up);
            }

            {
                const auto [sum, rbit] = add(neg_two, neg_four);
                REQUIRE(sum == neg_four);
                REQUIRE(rbit == up);
            }

            {
                const auto [sum, rbit] = add(neg_two, neg_sixteen);
                REQUIRE(sum == neg_sixteen);
                REQUIRE(rbit == up);
            }

            {
                const auto [sum, rbit] = add(neg_four, neg_sixteen);
                REQUIRE(sum == neg_sixteen);
                REQUIRE(rbit == up);
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("adding zero means no rounding", "[posit][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_EXHAUSTABLE)
{
    using namespace aarith;

    using Posit = posit<N, ES>;

    const auto zero = Posit::zero();
    const auto not_rounded = rounding_event::NOT_ROUNDED;

    for_each_posit<Posit>([&](const Posit& p) {
        const auto [sum, rbit] = add(p, zero);
        REQUIRE(rbit == not_rounded);
    });
}
