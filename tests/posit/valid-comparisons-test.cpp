#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_posit.hpp"

TEMPLATE_TEST_CASE_SIG("exact valids compare like posits", "[posit][valid][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Posit = posit<N, ES>;
    using Valid = valid<N, ES>;

    const Posit& p = GENERATE(take(50, random_posit<Posit>()));
    const Posit& q = GENERATE(take(50, random_posit<Posit>()));

    const Valid v(p);
    const Valid w(q);

    REQUIRE((p == q) == (v == w));
    REQUIRE((p < q) == (v < w));
    REQUIRE((p <= q) == (v <= w));
    REQUIRE((p > q) == (v > w));
    REQUIRE((p >= q) == (v >= w));
}

TEMPLATE_TEST_CASE_SIG("full set is never less than anything", "[posit][valid][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_SMALL)
{
    using namespace aarith;

    using Valid = valid<N, ES>;
    const Valid full = Valid::full();

    for_each_valid<Valid>([&](const Valid& v) { REQUIRE_FALSE(v < full); });
}

SCENARIO("arbitrary valid comparisons")
{
    using namespace aarith;

    using Posit = posit<3, 1>;
    using Valid = valid<3, 1>;

    GIVEN("random endpoints")
    {
        const Posit four = Posit(4.0);
        const Posit one = Posit(1.0);
        const Posit fourth = Posit(0.25);
        const Posit zero = Posit(0.0);
        const Posit nfourth = Posit(-0.25);
        const Posit none = Posit(-1.0);
        const Posit nfour = Posit(-4.0);

        constexpr auto open = interval_bound::OPEN;
        constexpr auto closed = interval_bound::CLOSED;

        THEN("assert that valids compare as expected")
        {
            {
                const Valid v = Valid::from(zero, open, one, open); // (0, 1)
                const Valid w = Valid::from(one, open, four, open); // (1, 4)
                REQUIRE(v < w);
            }

            {
                const Valid v = Valid::from(zero, open, one, open);   // (0, 1)
                const Valid w = Valid::from(one, open, four, closed); // (1, 4]
                REQUIRE(v < w);
            }

            {
                const Valid v = Valid::from(zero, closed, one, open); // [0, 1)
                const Valid w = Valid::from(one, open, four, closed); // (1, 4]
                REQUIRE(v < w);
            }

            {
                const Valid v = Valid::from(zero, closed, one, closed); // [0, 1]
                const Valid w = Valid::from(one, open, four, closed);   // (1, 4]
                REQUIRE(v < w);
            }

            {
                const Valid v = Valid::from(zero, closed, one, closed); // [0, 1]
                const Valid w = Valid::from(one, closed, four, closed); // [1, 4]
                REQUIRE_FALSE(v < w);
            }

            {
                const Valid v = Valid::from(zero, closed, fourth, closed); // [0, 0.25]
                const Valid w = Valid::from(fourth, closed, one, closed);  // [0.25, 1]
                REQUIRE_FALSE(v < w);
            }

            {
                const Valid v = Valid::from(zero, open, fourth, open); // (0, 0.25)
                const Valid w = Valid::from(fourth, open, one, open);  // (0.25, 1)
                REQUIRE(v < w);
            }

            {
                const Valid v = Valid::from(none, open, zero, open); // (-1, 0)
                const Valid w = Valid::from(zero, open, one, open);  // (0, 1)
                REQUIRE(v < w);
            }

            {
                const Valid v = Valid::from(none, closed, zero, closed); // [-1, 0]
                const Valid w = Valid::from(zero, closed, one, closed);  // [0, 1]
                REQUIRE_FALSE(v < w);
            }

            {
                const Valid v = Valid::from(none, closed, zero, closed); // [-1, 0]
                const Valid w = Valid::from(one, closed, four, closed);  // [1, 4]
                REQUIRE(v < w);
            }

            {
                const Valid v = Valid::from(none, closed, zero, open);  // [-1, 0)
                const Valid w = Valid::from(one, closed, four, closed); // [1, 4]
                REQUIRE(v < w);
            }

            {
                const Valid v = Valid::from(none, closed, zero, open); // [-1, 0)
                const Valid w = Valid::from(one, open, four, closed);  // (1, 4]
                REQUIRE(v < w);
            }

            {
                const Valid v = Valid::from(none, open, zero, open);  // (-1, 0)
                const Valid w = Valid::from(one, open, four, closed); // (1, 4]
                REQUIRE(v < w);
            }

            {
                const Valid v = Valid::from(none, open, zero, open); // (-1, 0)
                const Valid w = Valid::from(one, open, four, open);  // (1, 4)
                REQUIRE(v < w);
            }

            {
                const Valid v = Valid::from(nfour, open, zero, open); // (-4, 0)
                const Valid w = Valid::from(zero, open, four, open);  // (0, 4)
                REQUIRE(v < w);
            }

            {
                const Valid v = Valid::from(nfour, closed, zero, closed); // [-4, 0]
                const Valid w = Valid::from(zero, closed, four, closed);  // [0, 4]
                REQUIRE_FALSE(v < w);
            }

            {
                const Valid v = Valid::from(none, open, zero, open); // (-1, 0)

                const Valid pfour = Valid(four);
                const Valid pone = Valid(one);
                const Valid pfourth = Valid(fourth);
                const Valid pzero = Valid(zero);
                const Valid pnfourth = Valid(nfourth);
                const Valid pnone = Valid(none);
                const Valid pnfour = Valid(nfour);

                REQUIRE(v < pfour);
                REQUIRE(v < pone);
                REQUIRE(v < pfourth);
                REQUIRE(v < pzero);
                REQUIRE_FALSE(v < pnfourth);
                REQUIRE_FALSE(v < pnone);
                REQUIRE(pnone < v);
                REQUIRE_FALSE(v < pnfour);
            }
        }
    }
}
