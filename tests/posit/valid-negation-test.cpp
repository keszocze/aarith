#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"

SCENARIO("valid negation")
{
    using namespace aarith;

    using Posit = posit<3, 1>;
    using Valid = ivalid<3, 1>;

    GIVEN("arbitrary valids")
    {
        const Posit four = Posit(4.0);
        const Posit one = Posit(1.0);
        const Posit fourth = Posit(0.25);
        const Posit zero = Posit(0.0);

        const Posit nfour = Posit(-4.0);
        const Posit none = Posit(-1.0);
        const Posit nfourth = Posit(-0.25);

        constexpr auto open = interval_bound::OPEN;
        constexpr auto closed = interval_bound::CLOSED;

        THEN("assert that negation works as expected")
        {
            {
                const Valid v = Valid::from(none, closed, one, closed); // [-1, 1]
                const Valid v_negated = -v;
                REQUIRE(v_negated.in_interval_notation() == "[-1, 1]");
            }

            {
                const Valid v = Valid::from(none, open, zero, open); // (-1, 0)
                const Valid v_negated = -v;
                REQUIRE(v_negated.in_interval_notation() == "(0, 1)");
            }

            {
                const Valid v = Valid::from(nfour, closed, one, open); // [-4, 1)
                const Valid v_negated = -v;
                REQUIRE(v_negated.in_interval_notation() == "(-1, 4]");
            }

            {
                const Valid v = Valid::from(fourth, open, one, closed); // (0.25, 1]
                const Valid v_negated = -v;
                REQUIRE(v_negated.in_interval_notation() == "[-1, -0.25)");
            }

            {
                const Valid v = Valid(four);
                const Valid v_negated = -v;
                REQUIRE(v_negated.in_interval_notation() == "-4");
            }

            {
                const Valid v = Valid(nfourth);
                const Valid v_negated = -v;
                REQUIRE(v_negated.in_interval_notation() == "0.25");
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("valid negation extreme values", "[valid][posit][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;
    using Valid = ivalid<N, ES>;

    const auto vmax = Valid::max();
    const auto vmin = Valid::min();
    const auto zero = Valid::zero();
    const auto one = Valid::one();
    const auto nar = Valid::nar();

    REQUIRE(-vmax == vmin);
    REQUIRE(-vmin == vmax);
    REQUIRE(-zero == zero);
    REQUIRE(-(-one) == one);
    REQUIRE(-nar == nar);
}
