#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"
#include "all_valids.hpp"
#include "gen_posit.hpp"

template <typename Valid>
static void require_union(const Valid& v, const Valid& w, const Valid& expected)
{
    CAPTURE(v, w, expected);
    REQUIRE(v.merge_with(w) == expected);
}

SCENARIO("merging arbitrary valids")
{
    using namespace aarith;

    using Posit = posit<3, 1>;
    using Valid = valid<3, 1>;

    GIVEN("arbitrary valids")
    {
        const Valid v = Valid(Posit(0), Posit(1));
        const Valid w = Valid(Posit(0.25), Posit(-1));
        const Valid x = Valid(Posit(-1), Posit(0));

        THEN("assert that their union is as expected")
        {
            require_union(v, w, Valid(Posit(0), Posit(-1)));
            require_union(x, v, Valid(Posit(-1), Posit(1)));
            require_union(x, w, Valid::full());
        }
    }
}

TEMPLATE_TEST_CASE_SIG("merging valid with special values", "[valid][posit][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Posit = posit<N, ES>;
    using Valid = valid<N, ES>;

    const Posit start = GENERATE(take(5, random_posit<Posit>()));
    const Posit end = GENERATE(take(5, random_posit<Posit>()));

    const Valid empty = Valid::empty();
    const Valid full = Valid::full();
    const Valid all_reals = Valid::all_reals();

    for (const Valid& v : all_valids(start, end))
    {
        {
            const Valid merged_with_self = v.merge_with(v);
            REQUIRE(merged_with_self == v);
        }

        {
            const Valid merged_with_empty = v.merge_with(empty);
            REQUIRE(merged_with_empty == v);
        }

        {
            const Valid merged_with_full = v.merge_with(full);
            REQUIRE(merged_with_full == full);
        }

        {
            const Valid merged_with_all_reals = v.merge_with(all_reals);

            if (v.is_irregular())
            {
                REQUIRE(merged_with_all_reals == full);
            }
            else
            {
                REQUIRE(merged_with_all_reals == all_reals);
            }
        }
    }
}
