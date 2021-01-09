#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"
#include "for-each-posit.hpp"

TEMPLATE_TEST_CASE_SIG("conversion to and from quire preserves value", "[posit][template]",
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
