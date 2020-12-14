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

                const auto p = from_double<8, 2, uint64_t>(x);

                CHECK(to_double(-p) == Approx((-1.0) * to_double(p)));
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
            CHECK(Posit::complex_infinity() == -Posit::complex_infinity());
        }
    }
}

SCENARIO("Testing addition")
{
    GIVEN("Some arbitrary posit values")
    {
        THEN("Assert that their sum is okay")
        {
            for (int i = -50; i <= 50; ++i)
            {
                for (int j = -50; j <= 50; ++j)
                {
                    using namespace aarith;

                    constexpr size_t N = 32;
                    constexpr size_t ES = 3;

                    using Posit = posit<32, ES>;

                    const double x = static_cast<double>(i);
                    const double y = static_cast<double>(j);

                    const Posit p = from_double<N, ES>(x);
                    const Posit q = from_double<N, ES>(y);

                    CHECK(to_double(p + q) == Approx(to_double(from_double<N, ES>(x + y))));
                }
            }
        }
    }
}
