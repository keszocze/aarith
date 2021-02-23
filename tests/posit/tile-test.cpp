#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"
#include "for_each_posit.hpp"

TEMPLATE_TEST_CASE_SIG("getting endpoints from tiles", "[posit][tile][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_EXHAUSTABLE)
{
    using namespace aarith;

    using Posit = posit<N, ES>;
    using Tile = tile<N, ES>;

    for_each_posit<Posit>([](const Posit& p) {
        const Tile certain = Tile::from(p, false);
        const Tile uncertain = Tile::from(p, true);

        REQUIRE(certain.as_start_value() == p);
        REQUIRE(uncertain.as_start_value() == p);

        REQUIRE(certain.as_end_value() == p);
        REQUIRE(uncertain.as_end_value() == p.incremented());
    });
}
