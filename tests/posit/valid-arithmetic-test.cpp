#include <catch.hpp>
#include <set>
#include <tuple>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_posit.hpp"

using namespace aarith;

template <size_t N, size_t ES>
static std::tuple<tile<N, ES>, tile<N, ES>> endpoints(const valid<N, ES>& v)
{
    tile<N, ES> start = v.get_start();
    tile<N, ES> end = v.get_end();
    return std::make_tuple(start, end);
}

TEMPLATE_TEST_CASE_SIG("valid ", "[posit][template]", ((size_t N, size_t ES), N, ES),
                       AARITH_POSIT_TEST_TEMPLATE_EXHAUSTABLE)
{
    const auto [start, end] = endpoints(valid<N, ES>::one());

    REQUIRE(start == end);
}
