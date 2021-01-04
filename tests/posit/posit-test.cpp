#include <catch.hpp>

#include <aarith/integer.hpp>
#include <aarith/posit.hpp>
#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>

#include "../test-signature-ranges.hpp"
#include "posit-integers.hpp"

SCENARIO("Posit Constants")
{
    using namespace aarith;

    constexpr size_t NBits = 6;
    constexpr size_t ExponentSize = 2;
    using WordType = uint16_t;

    using P8 = aarith::posit<NBits, ExponentSize, WordType>;

    GIVEN("Some posit values provided by the posit class")
    {
        THEN("Assert that they return the expected")
        {
            const P8 min = P8::min();
            CHECK(to_binary(min) == "100001");

            const P8 minpos = P8::minpos();
            CHECK(to_binary(minpos) == "000001");

            const P8 max = P8::max();
            CHECK(to_binary(max) == "011111");

            const P8 zero = P8::zero();
            CHECK(to_binary(zero) == "000000");

            const P8 one = P8::one();
            CHECK(to_binary(one) == "010000");

            const P8 cinf = P8::nar();
            CHECK(to_binary(cinf) == "100000");

            const auto npat = P8::npat();
            CHECK(to_binary(npat) == "1000000");

            const auto useed = P8::useed();
            CHECK(useed == decltype(useed)(16));
        }
    }
}

TEMPLATE_TEST_CASE_SIG("conversion from integer to <8, 2> and back", "[posit][template]",
                       ((typename Int)), AARITH_POSIT_TEST_TEMPLATE_SIGNED_INTEGERS)
{
    using namespace aarith;

    constexpr int64_t max_int = std::numeric_limits<Int>::max();
    constexpr int64_t min_int = std::numeric_limits<Int>::min();

    for (const int64_t n : integers80)
    {
        if (n >= min_int && n <= max_int)
        {
            // only if n is reprsentable w/ Int run the test

            const Int test_integer = static_cast<Int>(n);

            const posit<8, 0> converted_to_posit(test_integer);
            const Int converted_back_to_int = Int(converted_to_posit);

            CHECK(test_integer == converted_back_to_int);
        }
    }

    for (const int64_t n : integers81)
    {
        if (n >= min_int && n <= max_int)
        {
            // only if n is reprsentable w/ Int run the test

            const Int test_integer = static_cast<Int>(n);

            const posit<8, 1> converted_to_posit(test_integer);
            const Int converted_back_to_int = Int(converted_to_posit);

            CHECK(test_integer == converted_back_to_int);
        }
    }

    for (const int64_t n : integers82)
    {
        if (n >= min_int && n <= max_int)
        {
            // only if n is reprsentable w/ Int run the test

            const Int test_integer = static_cast<Int>(n);

            const posit<8, 2> converted_to_posit(test_integer);
            const Int converted_back_to_int = Int(converted_to_posit);

            CHECK(test_integer == converted_back_to_int);
        }
    }

    for (const int64_t n : integers162)
    {
        if (n >= min_int && n <= max_int)
        {
            // only if n is reprsentable w/ Int run the test

            const Int test_integer = static_cast<Int>(n);

            const posit<16, 2> converted_to_posit(test_integer);
            const Int converted_back_to_int = Int(converted_to_posit);

            CHECK(test_integer == converted_back_to_int);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("conversion from unsigned integer to <8, 2> and back", "[posit][template]",
                       ((typename Int)), AARITH_POSIT_TEST_TEMPLATE_UNSIGNED_INTEGERS)
{
    using namespace aarith;

    constexpr uint64_t max_int = std::numeric_limits<Int>::max();

    for (const uint64_t n : uintegers80)
    {
        if (n <= max_int)
        {
            // only if n is reprsentable w/ Int run the test

            const Int test_integer = static_cast<Int>(n);

            const posit<8, 0> converted_to_posit(test_integer);
            const Int converted_back_to_int = Int(converted_to_posit);

            CHECK(test_integer == converted_back_to_int);
        }
    }

    for (const uint64_t n : uintegers81)
    {
        if (n <= max_int)
        {
            // only if n is reprsentable w/ Int run the test

            const Int test_integer = static_cast<Int>(n);

            const posit<8, 1> converted_to_posit(test_integer);
            const Int converted_back_to_int = Int(converted_to_posit);

            CHECK(test_integer == converted_back_to_int);
        }
    }

    for (const uint64_t n : uintegers82)
    {
        if (n <= max_int)
        {
            // only if n is reprsentable w/ Int run the test

            const Int test_integer = static_cast<Int>(n);

            const posit<8, 2> converted_to_posit(test_integer);
            const Int converted_back_to_int = Int(converted_to_posit);

            CHECK(test_integer == converted_back_to_int);
        }
    }

    for (const uint64_t n : uintegers162)
    {
        if (n <= max_int)
        {
            // only if n is reprsentable w/ Int run the test

            const Int test_integer = static_cast<Int>(n);

            const posit<16, 2> converted_to_posit(test_integer);
            const Int converted_back_to_int = Int(converted_to_posit);

            CHECK(test_integer == converted_back_to_int);
        }
    }
}

template <size_t N, size_t ES, class WT>
static void require_cmp(const aarith::posit<N, ES, WT>& x, const aarith::posit<N, ES, WT>& y)
{
    const double xdouble = double(x);
    const double ydouble = double(y);

    if (xdouble > ydouble)
    {
        REQUIRE(x > y);
    }

    if (xdouble >= ydouble)
    {
        REQUIRE(x >= y);
    }

    if (xdouble < ydouble)
    {
        REQUIRE(x < y);
    }

    if (xdouble <= ydouble)
    {
        REQUIRE(x <= y);
    }

    if (xdouble == ydouble)
    {
        REQUIRE(x == y);
    }
}

SCENARIO("Integer conversion limits")
{
    GIVEN("Standard integers too big to represent with a given posit type")
    {
        THEN("Assert that the result is the maximum representable posit")
        {
            using namespace aarith;

            using P80 = posit<8, 0>;
            using P81 = posit<8, 1>;
            using P82 = posit<8, 2>;

            constexpr int32_t max_int = std::numeric_limits<int32_t>::max();
            constexpr int32_t min_int = std::numeric_limits<int32_t>::min();

            REQUIRE(P80(64) == P80::max());
            REQUIRE(P80(65) == P80::max());
            REQUIRE(P80(64000) == P80::max());
            REQUIRE(P80(64000000) == P80::max());
            REQUIRE(P80(max_int) == P80::max());

            REQUIRE(P80(-64) == P80::min());
            REQUIRE(P80(-65) == P80::min());
            REQUIRE(P80(-64000) == P80::min());
            REQUIRE(P80(min_int) == P80::min());

            REQUIRE(P81(4096) == P81::max());
            REQUIRE(P81(4097) == P81::max());
            REQUIRE(P81(6666666) == P81::max());
            REQUIRE(P81(max_int) == P81::max());

            REQUIRE(P81(-4096) == P81::min());
            REQUIRE(P81(-4097) == P81::min());
            REQUIRE(P81(-6666666) == P81::min());
            REQUIRE(P81(min_int) == P81::min());

            REQUIRE(P82(16777216) == P82::max());
            REQUIRE(P82(16777217) == P82::max());
            REQUIRE(P82(66666666) == P82::max());
            REQUIRE(P82(max_int) == P82::max());

            REQUIRE(P82(-16777216) == P82::min());
            REQUIRE(P82(-16777217) == P82::min());
            REQUIRE(P82(-66666666) == P82::min());
            REQUIRE(P82(min_int) == P82::min());
        }
    }
}

SCENARIO("Check comparison comparisons")
{
    // we have no trouble representing all values of a <8,2> posit with double
    // precision floats; so we just use the double conversion functions and
    // compare the returned double as a reference

    GIVEN("All (x, y) tuples with x, y in posit<8, 0>")
    {
        THEN("Assert that a given posit compares like the closest double")
        {
            using namespace aarith;

            using P8 = aarith::posit<8, 0>;

            for (uint16_t i = 0; i <= 0xffU; ++i)
            {
                const uint8_t iactual = i;
                const P8 iposit = P8::from(iactual);

                for (uint16_t j = 0; j < 0xffU; ++j)
                {
                    const uint8_t jactual = j;
                    const P8 jposit = P8::from(jactual);

                    require_cmp(iposit, jposit);
                }
            }
        }
    }

    GIVEN("All (x, y) tuples with x, y in posit<8, 1>")
    {
        THEN("Assert that a given posit compares like the closest double")
        {
            using namespace aarith;

            using P8 = aarith::posit<8, 1>;

            for (uint16_t i = 0; i <= 0xffU; ++i)
            {
                const uint8_t iactual = i;
                const P8 iposit = P8::from(iactual);

                for (uint16_t j = 0; j < 0xffU; ++j)
                {
                    const uint8_t jactual = j;
                    const P8 jposit = P8::from(jactual);

                    require_cmp(iposit, jposit);
                }
            }
        }
    }

    GIVEN("All (x, y) tuples with x, y in posit<8, 2>")
    {
        THEN("Assert that a given posit compares like the closest double")
        {
            using namespace aarith;

            using P8 = aarith::posit<8, 2>;

            for (uint16_t i = 0; i < 0xffU; ++i)
            {
                const uint8_t iactual = i;
                const P8 iposit = P8::from(iactual);

                for (uint16_t j = 0; j <= 0xffU; ++j)
                {
                    const uint8_t jactual = j;
                    const P8 jposit = P8::from(jactual);

                    require_cmp(iposit, jposit);
                }
            }
        }
    }

    GIVEN("Extreme values")
    {
        THEN("Assert that they compare correctly")
        {
            using namespace aarith;

            using Posit = posit<64, 2>;

            constexpr Posit max = Posit::max();
            constexpr Posit min = Posit::min();
            constexpr Posit zero = Posit::zero();

            REQUIRE(min < max);
            REQUIRE_FALSE(max < min);

            REQUIRE(zero < max);
            REQUIRE_FALSE(max < zero);

            REQUIRE(min < zero);
            REQUIRE_FALSE(zero < min);
        }
    }
}
