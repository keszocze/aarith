#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_posit.hpp"

TEMPLATE_TEST_CASE_SIG("multiplying one does not change the result (random)",
                       "[valid][posit][template]", ((size_t N, size_t ES), N, ES),
                       AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Posit = posit<N, ES>;
    using Valid = valid<N, ES>;

    constexpr auto open = interval_bound::OPEN;
    constexpr auto closed = interval_bound::CLOSED;

    const Valid one = Valid::one();

    const Posit start = GENERATE(take(5, random_posit<Posit>()));
    const Posit end = GENERATE(take(5, random_posit<Posit>()));

    {
        const Valid v = Valid::from(start, open, end, open);

        if (v.is_regular())
        {
            REQUIRE((v * one) == v);
            REQUIRE((one * v) == v);
        }
    }

    {
        const Valid v = Valid::from(start, open, end, closed);

        if (v.is_regular())
        {
            REQUIRE((v * one) == v);
            REQUIRE((one * v) == v);
        }
    }

    {
        const Valid v = Valid::from(start, closed, end, open);

        if (v.is_regular())
        {
            REQUIRE((v * one) == v);
            REQUIRE((one * v) == v);
        }
    }

    {
        const Valid v = Valid::from(start, closed, end, closed);

        if (v.is_regular())
        {
            REQUIRE((v * one) == v);
            REQUIRE((one * v) == v);
        }
    }
}
