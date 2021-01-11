#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"
#include "for_each_posit.hpp"
#include "gen_posit.hpp"

TEMPLATE_TEST_CASE_SIG("conversion to and from quire preserves value", "[posit][quire][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_EXHAUSTABLE)
{
    using namespace aarith;
    using Posit = posit<N, ES>;

    for_each_posit<Posit>([](const Posit& p) {
        const Posit& expected = p;

        const auto q = quire(p);
        const Posit actual = static_cast<Posit>(q);

        INFO("N=" << Posit::width());
        INFO("ES=" << Posit::exponent_size());
        CHECK(actual == expected);
    });
}

TEMPLATE_TEST_CASE_SIG("conversion to and from random quire preserves value", "[posit][quire][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;
    using Posit = posit<N, ES>;

    const Posit& expected = GENERATE(take(1000, random_posit<Posit>()));
    const auto q = quire(expected);
    const Posit actual = static_cast<Posit>(q);

    REQUIRE(actual == expected);
}

TEMPLATE_TEST_CASE_SIG("conversion to and from special values", "[quire][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;
    using Posit = posit<N, ES>;

    constexpr auto pnar = Posit::nar();
    constexpr auto pzero = Posit::zero();

    const auto qnar = quire(pnar);
    const auto qzero = quire(pzero);

    CHECK(qnar.is_nar());
    CHECK_FALSE(qnar.is_zero());

    CHECK_FALSE(qzero.is_nar());
    CHECK(qzero.is_zero());
}
