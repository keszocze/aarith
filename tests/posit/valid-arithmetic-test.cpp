#include <catch.hpp>
#include <set>
#include <tuple>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_tile.hpp"

TEMPLATE_TEST_CASE_SIG("valid ", "[posit][template]", ((size_t N, size_t ES), N, ES),
                       AARITH_POSIT_TEST_TEMPLATE_EXHAUSTABLE)
{
    using namespace aarith;

    using Tile = tile<N, ES>;

    const Tile start = GENERATE(take(10, random_tile<Tile>()));
    const Tile end = GENERATE(take(10, random_tile<Tile>()));

    (void) start;
    (void) end;
}
