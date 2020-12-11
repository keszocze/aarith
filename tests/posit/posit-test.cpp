#include <catch.hpp>
#include <iostream>

#include <aarith/integer.hpp>
#include <aarith/posit.hpp>
#include <iostream>

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

            const P8 cinf = P8::complex_infinity();
            CHECK(to_binary(cinf) == "100000");

            const auto npat = P8::npat();
            CHECK(to_binary(npat) == "1000000");

            const auto useed = P8::useed();
            CHECK(useed == decltype(useed)(16));
        }
    }
}

template <size_t N, size_t ES, class WT>
static void require_cmp(const aarith::posit<N, ES, WT>& x, const aarith::posit<N, ES, WT>& y)
{
    const double xdouble = aarith::to_double(x);
    const double ydouble = aarith::to_double(y);

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
                const P8 iposit(iactual);

                for (uint16_t j = 0; j < 0xffU; ++j)
                {
                    const uint8_t jactual = j;
                    const P8 jposit(jactual);

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
                const P8 iposit(iactual);

                for (uint16_t j = 0; j < 0xffU; ++j)
                {
                    const uint8_t jactual = j;
                    const P8 jposit(jactual);

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
                const P8 iposit(iactual);

                for (uint16_t j = 0; j <= 0xffU; ++j)
                {
                    const uint8_t jactual = j;
                    const P8 jposit(jactual);

                    require_cmp(iposit, jposit);
                }
            }
        }
    }
}
