#include <catch.hpp>

#include <aarith/posit.hpp>

#include "gen_bool.hpp"

#include <cstdint>

//
// This file contains tests for the posit test case infrastructure.  Yes,
// that's test cases for test cases, kind of.
//

TEST_CASE("random bools are actually random")
{
    using namespace aarith;

    constexpr uint64_t total_runs = 500'000;

    static uint64_t ntrue = 0;
    static uint64_t nfalse = 0;
    static uint64_t runs_so_far = 0;

    const bool dice_roll = GENERATE(take(total_runs, random_bool()));

    if (dice_roll)
    {
        ntrue += 1;
    }
    else
    {
        nfalse += 1;
    }

    runs_so_far += 1;

    // If we have accumulated enough runs, we can evaluate the results so far.
    // For N total_runs, we expect at least N/2 - N/8 results to fall on either
    // side.

    if (runs_so_far == total_runs)
    {
        constexpr uint64_t expected = total_runs / 2 - total_runs / 8;

        CAPTURE(runs_so_far, ntrue, nfalse);
        REQUIRE(ntrue >= expected);
        REQUIRE(nfalse >= expected);
    }
}
