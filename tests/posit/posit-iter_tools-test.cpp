#include "../test-signature-ranges.hpp"
#include <aarith/posit.hpp>
#include <catch.hpp>

using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Iteration starts at smallest and arrives at biggest", "[posit][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_EXHAUSTABLE)
{
    using Posit = aarith::posit<N, ES>;

    Posit last = Posit::nar();

    for_each_regular_posit<Posit>([&](const Posit& current) {
        if (!last.is_nar())
        {
            REQUIRE(current > last);
        }

        last = current;
    });
}
