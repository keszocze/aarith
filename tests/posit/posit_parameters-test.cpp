#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_posit.hpp"

TEMPLATE_TEST_CASE_SIG("conversion to and from positparam preserves value", "[posit][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_EXHAUSTABLE)
{
    using namespace aarith;
    using Posit = posit<N, ES>;

    for_each_posit<Posit>([](const Posit& p) {
        const Posit& expected = p;

        const auto pp = posit_parameters(p);
        const Posit actual = static_cast<Posit>(pp);

        REQUIRE(actual == expected);
    });
}

TEMPLATE_TEST_CASE_SIG("conversion to and from random positparam preserves value",
                       "[posit][template]", ((size_t N, size_t ES), N, ES),
                       AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;
    using Posit = posit<N, ES>;

    const Posit& expected = GENERATE(take(1000, random_posit<Posit>()));
    const auto pp = posit_parameters(expected);
    const Posit actual = static_cast<Posit>(pp);

    REQUIRE(actual == expected);
}
