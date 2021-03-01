#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"
#include "any_irregular.hpp"
#include "for_each_tile.hpp"
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

TEMPLATE_TEST_CASE_SIG("adding zero does not change the result (exhaustive)",
                       "[valid][posit][template]", ((size_t N, size_t ES), N, ES),
                       AARITH_POSIT_TEST_TEMPLATE_SMALL)
{
    using namespace aarith;

    using Tile = tile<N, ES>;
    using Valid = valid<N, ES>;

    for_each_tile<Tile>([](const Tile& t) {
        const Valid zero = Valid::zero();
        const Valid v = Valid::from(t, t);

        REQUIRE((v + zero) == v);
    });
}

TEMPLATE_TEST_CASE_SIG("adding zero does not change the result (random)",
                       "[valid][posit][template]", ((size_t N, size_t ES), N, ES),
                       AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Tile = tile<N, ES>;
    using Valid = valid<N, ES>;

    const Tile t = GENERATE(take(200, random_tile<Tile>()));

    const Valid zero = Valid::zero();
    const Valid v = Valid::from(t, t);

    REQUIRE((v + zero) == v);
}
