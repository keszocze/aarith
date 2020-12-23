#include <aarith/posit.hpp>
#include <catch.hpp>

#include "../test-signature-ranges.hpp"

TEMPLATE_TEST_CASE_SIG("conversion to and from positparam preserves value", "[posit][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_EXHAUSTABLE)
{
    using namespace aarith;

    using Posit = posit<N, ES>;
    using WT = typename Posit::word_type;

    for (uint64_t bits = 0; bits < (1ULL << Posit::width()); ++bits)
    {
        const Posit expected(bits);

        const auto pp = positparams<N, ES, WT>(expected);
        const Posit actual = static_cast<Posit>(pp);

        REQUIRE(actual == expected);
    }
}
