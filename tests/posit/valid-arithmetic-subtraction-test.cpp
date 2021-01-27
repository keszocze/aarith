#include <catch.hpp>

#include "../test-signature-ranges.hpp"
#include "any_irregular.hpp"
#include "gen_tile.hpp"
#include <aarith/posit.hpp>

TEMPLATE_TEST_CASE_SIG("subtraction regular case", "[valid][posit][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_EXHAUSTABLE)
{
    using namespace aarith;

    using Valid = valid<N, ES>;
    using Tile = tile<N, ES>;

    // Computing {a, b} - {c, d} = {x, y}

    const Tile a = GENERATE(take(10, random_tile<Tile>()));
    const Tile b = GENERATE(take(10, random_tile<Tile>()));
    const Tile c = GENERATE(take(10, random_tile<Tile>()));
    const Tile d = GENERATE(take(10, random_tile<Tile>()));

    const Valid v = Valid::from(a, b);
    const Valid w = Valid::from(c, d);

    const Valid diff = v - w;

    // While we generate a lot of test cases, some of them require
    // different tests. We only care about the regular cases here.
    // Which means lots of filtering out.

    if (any_irregular({a, b, c, d}) || any_irregular({v, w, diff}))
    {
        return;
    }

    // Finally we can run the real test case.

    const Tile& x = diff.get_start();
    const Tile& y = diff.get_end();

    REQUIRE(x.value() == (a.value() - d.value()));
    REQUIRE(y.value() == (b.value() - c.value()));
}
