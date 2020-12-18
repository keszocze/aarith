#include <catch.hpp>

#include <aarith/posit.hpp>
#include <cstdint>
#include <fstream>

//
// This file contains quite excessive tests that compare out implementation to
// the universal implementation [1].
//
// [1] https://github.com/stillwater-sc/universal
//

static void require_addition(const uint8_t lhs, const uint8_t rhs, const uint8_t expected)
{
    using namespace aarith;

    const posit8 p(lhs);
    const posit8 q(rhs);

    const posit8 actual_posit = p + q;
    const posit8 expected_posit(expected);

    {
        INFO("evaluated " << p << " + " << q << " = " << actual_posit << ", expected "
                          << expected_posit);
        REQUIRE(actual_posit == expected_posit);
    }
}

#include "posit-universal-addition-test.include.cpp"
