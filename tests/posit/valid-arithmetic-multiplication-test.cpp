#include <vector>

#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_posit.hpp"

template <size_t N, size_t ES, typename WT>
static std::vector<aarith::valid<N, ES, WT>> all_valids(const aarith::posit<N, ES, WT>& start,
                                                        const aarith::posit<N, ES, WT>& end)
{
    using namespace aarith;

    using Valid = valid<N, ES, WT>;

    constexpr auto open = interval_bound::OPEN;
    constexpr auto closed = interval_bound::CLOSED;

    return {Valid::from(start, open, end, open), Valid::from(start, open, end, closed),
            Valid::from(start, closed, end, open), Valid::from(start, closed, end, closed)};
}

TEMPLATE_TEST_CASE_SIG("multiplying one does not change the result (random)",
                       "[valid][posit][template]", ((size_t N, size_t ES), N, ES),
                       AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Posit = posit<N, ES>;
    using Valid = valid<N, ES>;

    const Posit start = GENERATE(take(5, random_posit<Posit>()));
    const Posit end = GENERATE(take(5, random_posit<Posit>()));

    const Valid one = Valid::one();

    for (const Valid& v : all_valids(start, end))
    {
        if (v.is_nar())
        {
            REQUIRE((v * one) == Valid::nar());
            REQUIRE((one * v) == Valid::nar());
        }
        else if (v.is_regular())
        {
            REQUIRE((v * one) == v);
            REQUIRE((one * v) == v);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("multiplying zero works as expected (random)", "[valid][posit][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Posit = posit<N, ES>;
    using Valid = valid<N, ES>;

    const Posit start = GENERATE(take(5, random_posit<Posit>()));
    const Posit end = GENERATE(take(5, random_posit<Posit>()));

    const Valid zero = Valid::zero();

    for (const Valid& v : all_valids(start, end))
    {
        if (v.is_nar())
        {
            REQUIRE((v * zero) == Valid::nar());
            REQUIRE((zero * v) == Valid::nar());
        }
        else if (v.is_empty())
        {
            REQUIRE((v * zero) == Valid::empty());
            REQUIRE((zero * v) == Valid::empty());
        }
        else if (v.is_full())
        {
            REQUIRE((v * zero) == Valid::full());
            REQUIRE((zero * v) == Valid::full());
        }
        else if (v.is_regular())
        {
            REQUIRE((v * zero) == zero);
            REQUIRE((zero * v) == zero);
        }
    }
}
