#include <catch.hpp>

#include <aarith/posit.hpp>
#include <cstdint>
#include <fstream>

//
// This file contains quite excessive tests that compare our implementation to
// the SoftPosit implementation [1]
//
// [1] https://github.com/milankl/SoftPosit.jl
//

static void require_addition(const uint8_t lhs, const uint8_t rhs, const uint8_t expected)
{
    using namespace aarith;

    const posit8 p(lhs);
    const posit8 q(rhs);

    const posit8 actual_posit = p + q;
    const posit8 expected_posit(expected);

    {
        INFO(" " << dump_string(p));
        INFO("+" << dump_string(q));
        INFO("? " << dump_string(actual_posit));
        INFO("! " << dump_string(expected_posit));
        INFO("? " << to_double(actual_posit));
        INFO("! " << to_double(expected_posit));

        REQUIRE(actual_posit == expected_posit);
    }
}

static void require_subtraction(const uint8_t lhs, const uint8_t rhs, const uint8_t expected)
{
    using namespace aarith;

    const posit8 p(lhs);
    const posit8 q(rhs);

    const posit8 actual_posit = p - q;
    const posit8 expected_posit(expected);

    {
        INFO(" " << dump_string(p));
        INFO("-" << dump_string(q));
        INFO("? " << dump_string(actual_posit));
        INFO("! " << dump_string(expected_posit));
        INFO("? " << to_double(actual_posit));
        INFO("! " << to_double(expected_posit));

        REQUIRE(actual_posit == expected_posit);
    }
}

#include "posit-softposit-addition-test.include.cpp"
#include "posit-softposit-subtraction-test.include.cpp"
