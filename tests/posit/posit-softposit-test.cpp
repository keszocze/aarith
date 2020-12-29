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

template <size_t N, size_t ES>
static void require_addition(const uint8_t lhs, const uint8_t rhs, const uint8_t expected)
{
    using namespace aarith;

    const posit<N, ES> p(lhs);
    const posit<N, ES> q(rhs);

    const posit<N, ES> actual_posit = p + q;
    const posit<N, ES> expected_posit(expected);

    {
        INFO("  " << dump_string(p));
        INFO("+ " << dump_string(q));
        INFO("  " << to_double(p));
        INFO("+ " << to_double(q));
        INFO("? " << dump_string(actual_posit));
        INFO("! " << dump_string(expected_posit));
        INFO("? " << to_double(actual_posit));
        INFO("! " << to_double(expected_posit));

        REQUIRE(actual_posit == expected_posit);
    }
}

template <size_t N, size_t ES>
static void require_subtraction(const uint8_t lhs, const uint8_t rhs, const uint8_t expected)
{
    using namespace aarith;

    const posit<N, ES> p(lhs);
    const posit<N, ES> q(rhs);

    const posit<N, ES> actual_posit = p - q;
    const posit<N, ES> expected_posit(expected);

    {
        INFO("  " << dump_string(p));
        INFO("- " << dump_string(q));
        INFO("  " << to_double(p));
        INFO("- " << to_double(q));
        INFO("? " << dump_string(actual_posit));
        INFO("! " << dump_string(expected_posit));
        INFO("? " << to_double(actual_posit));
        INFO("! " << to_double(expected_posit));

        REQUIRE(actual_posit == expected_posit);
    }
}

#include "posit-softposit-8-0-addition-test.include.cpp"
#include "posit-softposit-8-0-subtraction-test.include.cpp"
#include "posit-softposit-8-2-addition-test.include.cpp"
#include "posit-softposit-8-2-subtraction-test.include.cpp"
