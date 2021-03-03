#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"
#include "for_each_tile.hpp"
#include "for_each_valid.hpp"
#include "gen_tile.hpp"

TEMPLATE_TEST_CASE_SIG("addition regular case", "[valid][posit][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_EXHAUSTABLE)
{
    /*
    using namespace aarith;

    using Valid = valid<N, ES>;
    using Tile = tile<N, ES>;

    // Computing {a, b} + {c, d} = {x, y}

    const Tile a = GENERATE(take(10, random_tile<Tile>()));
    const Tile b = GENERATE(take(10, random_tile<Tile>()));
    const Tile c = GENERATE(take(10, random_tile<Tile>()));
    const Tile d = GENERATE(take(10, random_tile<Tile>()));

    const Valid v = Valid::from(a, b);
    const Valid w = Valid::from(c, d);

    const Valid sum = v + w;

    // While we generate a lot of test cases, some of them require
    // different tests. We only care about the regular cases here.
    // Which means lots of filtering out.

    if (any_irregular({a, b, c, d}) || any_irregular({v, w, sum}))
    {
        return;
    }

    // Finally we can run the real test case.

    const Tile& x = sum.get_start();
    const Tile& y = sum.get_end();

    REQUIRE(x.value() == (a.value() + c.value()));
    REQUIRE(y.value() == (b.value() + d.value()));
    */
}

SCENARIO("adding arbitrary posit-correct valids")
{
    using namespace aarith;

    using Posit = posit<3, 1>;
    using Valid = valid<3, 1>;

    GIVEN("arbitrary valids")
    {
        const Valid four = Valid(Posit(4.0));
        const Valid one = Valid(Posit(1.0));
        const Valid fourth = Valid(Posit(0.25));
        const Valid zero = Valid(Posit(0.0));

        THEN("assert that their sum is as expected")
        {
            {
                const Valid sum = one + one;
                REQUIRE(sum.in_interval_notation() == "(1, 4)");
            }

            {
                const Valid sum = one + fourth;
                REQUIRE(sum.in_interval_notation() == "(1, 4)");
            }

            {
                const Valid sum = fourth + one;
                REQUIRE(sum.in_interval_notation() == "(1, 4)");
            }

            {
                const Valid sum = fourth + zero;
                REQUIRE(sum.in_interval_notation() == "0.25");
            }

            {
                const Valid sum = four + four;
                REQUIRE(sum.in_interval_notation() == "(4, ∞)");
            }
        }
    }
}

SCENARIO("adding arbitrary interval valids")
{
    using namespace aarith;

    using Posit = posit<3, 1>;
    using Valid = valid<3, 1>;

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

        THEN("assert that their sum is as expected")
        {
            {
                const Valid ival = Valid::from(zero, open, one, open); // (0, 1)
                const Valid sum = ival + ival;
                REQUIRE(sum.in_interval_notation() == "(0, 4)");
            }

            {
                const Valid ival = Valid::from(zero, closed, one, closed); // [0, 1]
                const Valid sum = ival + ival;
                REQUIRE(sum.in_interval_notation() == "[0, 4)");
            }

            {
                const Valid ival = Valid::from(zero, open, one, closed); // (0, 1]
                const Valid sum = ival + ival;
                REQUIRE(sum.in_interval_notation() == "(0, 4)");
            }

            {
                const Valid ival = Valid::from(zero, closed, one, open); // [0, 1)
                const Valid sum = ival + ival;
                REQUIRE(sum.in_interval_notation() == "[0, 4)");
            }

            {
                const Valid ival = Valid::from(zero, open, one, open); // (0, 1)
                const Valid sum = ival + ival;
                REQUIRE(sum.in_interval_notation() == "(0, 4)");
            }

            {
                const Valid v = Valid::from(zero, open, one, open); // (0, 1)
                const Valid w = Valid::from(one, open, four, open); // (1, 4)
                const Valid sum = v + w;
                REQUIRE(sum.in_interval_notation() == "(1, ∞)");
            }

            {
                const Valid v = Valid::from(zero, open, one, open);   // (0, 1)
                const Valid w = Valid::from(one, open, four, closed); // (1, 4]
                const Valid sum = v + w;
                REQUIRE(sum.in_interval_notation() == "(1, ∞)");
            }

            {
                const Valid v = Valid::from(zero, closed, one, open); // [0, 1)
                const Valid w = Valid::from(one, open, four, closed); // (1, 4]
                const Valid sum = v + w;
                REQUIRE(sum.in_interval_notation() == "(1, ∞)");
            }

            {
                const Valid v = Valid::from(zero, closed, one, closed); // [0, 1]
                const Valid w = Valid::from(one, open, four, closed);   // (1, 4]
                const Valid sum = v + w;
                REQUIRE(sum.in_interval_notation() == "(1, ∞)");
            }

            {
                const Valid v = Valid::from(zero, closed, one, closed); // [0, 1]
                const Valid w = Valid::from(one, closed, four, closed); // [1, 4]
                const Valid sum = v + w;
                REQUIRE(sum.in_interval_notation() == "[1, ∞)");
            }

            {
                const Valid v = Valid::from(zero, closed, fourth, closed); // [0, 0.25]
                const Valid w = Valid::from(fourth, closed, one, closed);  // [0.25, 1]
                const Valid sum = v + w;
                REQUIRE(sum.in_interval_notation() == "[0.25, 4)");
            }

            {
                const Valid v = Valid::from(zero, open, fourth, open); // (0, 0.25)
                const Valid w = Valid::from(fourth, open, one, open);  // (0.25, 1)
                const Valid sum = v + w;
                REQUIRE(sum.in_interval_notation() == "(0.25, 4)");
            }

            {
                const Valid v = Valid::from(none, open, one, open); // (-1, 1)
                const Valid sum = v + v;
                REQUIRE(sum.in_interval_notation() == "(-4, 4)");
            }

            {
                const Valid v = Valid::from(none, closed, one, closed); // [-1, 1]
                const Valid sum = v + v;
                REQUIRE(sum.in_interval_notation() == "(-4, 4)");
            }

            {
                const Valid v = Valid::from(none, closed, one, open); // [-1, 1)
                const Valid sum = v + v;
                REQUIRE(sum.in_interval_notation() == "(-4, 4)");
            }

            {
                const Valid v = Valid::from(none, open, one, closed); // (-1, 1]
                const Valid sum = v + v;
                REQUIRE(sum.in_interval_notation() == "(-4, 4)");
            }

            {
                const Valid v = Valid::from(nfourth, open, zero, open); // (-0.25, 0)
                const Valid sum = v + v;
                REQUIRE(sum.in_interval_notation() == "(-1, 0)");
            }

            {
                const Valid v = Valid::from(none, open, zero, open); // (-1, 0)
                const Valid w = Valid::from(zero, open, one, open);  // (0, 1)
                const Valid sum = v + w;
                REQUIRE(sum.in_interval_notation() == "(-1, 1)");
            }

            {
                const Valid v = Valid::from(none, closed, zero, closed); // [-1, 0]
                const Valid w = Valid::from(zero, closed, one, closed);  // [0, 1]
                const Valid sum = v + w;
                REQUIRE(sum.in_interval_notation() == "[-1, 1]");
            }

            {
                const Valid v = Valid::from(none, closed, zero, closed); // [-1, 0]
                const Valid w = Valid::from(one, closed, four, closed);  // [1, 4]
                const Valid sum = v + w;
                REQUIRE(sum.in_interval_notation() == "[0, 4]");
            }

            {
                const Valid v = Valid::from(none, closed, zero, open);  // [-1, 0)
                const Valid w = Valid::from(one, closed, four, closed); // [1, 4]
                const Valid sum = v + w;
                REQUIRE(sum.in_interval_notation() == "[0, 4)");
            }

            {
                const Valid v = Valid::from(none, closed, zero, open); // [-1, 0)
                const Valid w = Valid::from(one, open, four, closed);  // (1, 4]
                const Valid sum = v + w;
                REQUIRE(sum.in_interval_notation() == "(0, 4)");
            }

            {
                const Valid v = Valid::from(none, open, zero, open);  // (-1, 0)
                const Valid w = Valid::from(one, open, four, closed); // (1, 4]
                const Valid sum = v + w;
                REQUIRE(sum.in_interval_notation() == "(0, 4)");
            }

            {
                const Valid v = Valid::from(none, open, zero, open); // (-1, 0)
                const Valid w = Valid::from(one, open, four, open);  // (1, 4)
                const Valid sum = v + w;
                REQUIRE(sum.in_interval_notation() == "(0, 4)");
            }

            {
                const Valid v = Valid::from(nfour, open, zero, open); // (-4, 0)
                const Valid w = Valid::from(zero, open, four, open);  // (0, 4)
                const Valid sum = v + w;
                REQUIRE(sum.in_interval_notation() == "(-4, 4)");
            }

            {
                const Valid v = Valid::from(nfour, closed, zero, closed); // [-4, 0]
                const Valid w = Valid::from(zero, closed, four, closed);  // [0, 4]
                const Valid sum = v + w;
                REQUIRE(sum.in_interval_notation() == "[-4, 4]");
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("adding zero does not change the result (exhaustive)",
                       "[valid][posit][template]", ((size_t N, size_t ES), N, ES),
                       AARITH_POSIT_TEST_TEMPLATE_SMALL)
{
    using namespace aarith;

    using Valid = valid<N, ES>;
    const Valid zero = Valid::zero();

    for_each_regular_valid<Valid>([&](const Valid& v) { REQUIRE((v + zero) == v); });
}

TEMPLATE_TEST_CASE_SIG("adding zero does not change the result (random)",
                       "[valid][posit][template]", ((size_t N, size_t ES), N, ES),
                       AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Posit = posit<N, ES>;
    using Valid = valid<N, ES>;

    constexpr auto open = interval_bound::OPEN;
    constexpr auto closed = interval_bound::CLOSED;

    const Valid zero = Valid::zero();

    const Posit start = GENERATE(take(10, random_posit<Posit>()));
    const Posit end = GENERATE(take(10, random_posit<Posit>()));

    {
        const Valid v = Valid::from(start, open, end, open);

        if (!v.is_irregular())
        {
            REQUIRE((v + zero) == v);
        }
    }

    {
        const Valid v = Valid::from(start, open, end, closed);

        if (!v.is_irregular())
        {
            REQUIRE((v + zero) == v);
        }
    }

    {
        const Valid v = Valid::from(start, closed, end, open);

        if (!v.is_irregular())
        {
            REQUIRE((v + zero) == v);
        }
    }

    {
        const Valid v = Valid::from(start, closed, end, closed);

        if (!v.is_irregular())
        {
            REQUIRE((v + zero) == v);
        }
    }
}
