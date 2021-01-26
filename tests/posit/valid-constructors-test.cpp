#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_posit.hpp"

TEMPLATE_TEST_CASE_SIG("valid zero constructors and constants", "[posit][valid][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Tile = tile<N, ES>;
    using Valid = valid<N, ES>;

    {
        const Valid v = Valid();

        REQUIRE(v.is_zero());
        REQUIRE_FALSE(v.is_nar());
        REQUIRE_FALSE(v.is_empty());
        REQUIRE(v.contains(Tile::zero()));
        REQUIRE_FALSE(v.contains(Tile::one()));
    }

    {
        const Valid v = Valid::zero();

        REQUIRE(v.is_zero());
        REQUIRE_FALSE(v.is_nar());
        REQUIRE_FALSE(v.is_empty());
        REQUIRE(v.contains(Tile::zero()));
        REQUIRE_FALSE(v.contains(Tile::one()));
    }

    {
        const Valid v = Valid::one();

        REQUIRE_FALSE(v.is_zero());
        REQUIRE_FALSE(v.is_nar());
        REQUIRE_FALSE(v.is_empty());
        REQUIRE_FALSE(v.contains(Tile::zero()));
        REQUIRE(v.contains(Tile::one()));
    }

    {
        const Valid v = Valid::nar();

        REQUIRE_FALSE(v.is_zero());
        REQUIRE(v.is_nar());
        REQUIRE_FALSE(v.is_empty());
        REQUIRE_FALSE(v.contains(Tile::zero()));
        REQUIRE_FALSE(v.contains(Tile::one()));
    }

    {
        const Valid v = Valid::empty();

        REQUIRE_FALSE(v.is_zero());
        REQUIRE_FALSE(v.is_nar());
        REQUIRE(v.is_empty());
        REQUIRE_FALSE(v.contains(Tile::zero()));
        REQUIRE_FALSE(v.contains(Tile::one()));
    }
}

TEMPLATE_TEST_CASE_SIG("valid concrete constructor", "[posit][valid][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Posit = posit<N, ES>;
    using Tile = tile<N, ES>;
    using Valid = valid<N, ES>;

    const Posit& concrete = GENERATE(take(1000, random_posit<Posit>()));
    const Tile t = Tile::from(concrete, false);
    const Valid v(concrete);

    REQUIRE_FALSE(t.is_uncertain());
    REQUIRE(v.contains(t));
    REQUIRE(concrete.is_nar() == v.is_nar());
}

TEMPLATE_TEST_CASE_SIG("valid concrete constructor contains correct values",
                       "[posit][valid][template]", ((size_t N, size_t ES), N, ES),
                       AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Posit = posit<N, ES>;
    using Tile = tile<N, ES>;
    using Valid = valid<N, ES>;

    // First we throw the dice some concrete posit value. From this value we
    // create a valid.

    const Posit& concrete = GENERATE(take(10, random_posit<Posit>()));
    const Valid v(concrete);

    // Then we get another random posit. If per chance both random posits are
    // equal, the created valid should contain that given posit value.  If the
    // two random posits are different (most cases), the "other" posit should
    // not be contained in "v".

    const Posit& other = GENERATE(take(100, random_posit<Posit>()));
    const Tile t = Tile::from(other, false);

    if (concrete == other)
    {
        REQUIRE(v.contains(t));
    }
    else
    {
        REQUIRE_FALSE(v.contains(t));
    }
}
