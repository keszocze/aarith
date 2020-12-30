#include <catch.hpp>
#include <iostream>

#include "../test-signature-ranges.hpp"
#include <aarith/posit.hpp>
#include <cstdint>

SCENARIO("Testing unary minus")
{
    GIVEN("Some arbitrary posit values")
    {
        THEN("That the unary minus operator behaves as expected")
        {
            using namespace aarith;

            for (int i = -1000; i <= 1000; ++i)
            {
                const double x = static_cast<double>(i);

                const auto p = posit<8, 2, uint64_t>(x);

                CHECK(double(-p) == Approx((-1.0) * double(p)));
                CHECK(-(-p) == p);
            }
        }
    }

    GIVEN("Special cases")
    {
        THEN("That the unary minus operator behaves as expected")
        {
            using namespace aarith;

            using Posit = posit<16, 3>;

            CHECK(Posit::zero() == -Posit::zero());
            CHECK(Posit::nar() == -Posit::nar());
        }
    }
}

SCENARIO("Testing arithmetic")
{
    GIVEN("Sums way too big for the given posit type")
    {
        THEN("Addition should not overflow")
        {
            using namespace aarith;

            using Posit = posit<8, 2>;

            constexpr auto one = Posit::one();
            constexpr auto max = Posit::max();

            REQUIRE(max + one == max);
            REQUIRE(max + max == max);
        }
    }

    GIVEN("Sums way too small for the given posit type")
    {
        THEN("Addition should not overflow")
        {
            using namespace aarith;

            using Posit = posit<8, 2>;

            constexpr auto one = Posit::one();
            constexpr auto min = Posit::min();

            REQUIRE(min - one == min);
            REQUIRE(min + min == min);
        }
    }

    GIVEN("Special cases")
    {
        THEN("Assert that arithmetic works as expected")
        {
            using namespace aarith;

            using Posit = posit<8, 2>;

            constexpr auto inf = Posit::nar();
            constexpr auto zero = Posit::nar();

            CHECK(inf + inf == inf);
            CHECK(inf + zero == inf);
            CHECK(zero + inf == inf);
            CHECK(zero + zero == zero);

            CHECK(inf - inf == inf);
            CHECK(inf - zero == inf);
            CHECK(zero - inf == inf);
            CHECK(zero - zero == zero);
        }
    }
}
