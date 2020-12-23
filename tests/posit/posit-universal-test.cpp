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

    if (p.is_negative())
    {
        // TODO (Schärt): Also check negative values eventually!!!
        return;
    }

    static uint64_t i;

    {
        INFO("i=" << (i++));
        INFO(dump_string(p));
        INFO(dump_string(q));
        INFO("? " << dump_string(actual_posit));
        INFO("! " << dump_string(expected_posit));
        INFO("? " << to_double(actual_posit));
        INFO("! " << to_double(expected_posit));
        INFO("eval '" << to_binary(p, " ") << "' + '" << to_binary(q, " ") << "' = '"
                           << to_binary(actual_posit, " ") << "', expected '"
                           << to_binary(expected_posit, " ") << "'");

        const auto expected_bits = expected_posit.get_bits();
        const auto actual_bits = actual_posit.get_bits();

        uinteger<expected_bits.width()> bitdiff;

        if (expected_bits > actual_bits)
        {
            bitdiff = expected_bits - actual_bits;
        }
        else
        {
            bitdiff = actual_bits - expected_bits;
        }

        INFO("diff " << bitdiff);

        REQUIRE(actual_posit == expected_posit);
    }
}

#include "posit-universal-addition-test.include.cpp"
