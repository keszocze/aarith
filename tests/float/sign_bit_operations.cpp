#include "gen_float.hpp"
#include <aarith/float.hpp>
#include <bitset>
#include <catch.hpp>
#include <cmath>

using namespace aarith;

template <typename N>
// it is not really "full range" but at least it works
auto full_native_range()
{
    return Catch::Generators::random<N>(std::numeric_limits<N>::lowest() / 100.0f,
                                        std::numeric_limits<N>::max() / 100.0f);
}

TEMPLATE_TEST_CASE_SIG("Addition is commutative",
                       "[normalized_float][negate][sig_bit_operation][invariant]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    using F = normalized_float<E, M>;

    GIVEN("A random floating-point number")
    {

        F a = GENERATE(take(15, random_float<E, M>()));

        WHEN("Negating the number")
        {
            F b = negate(a);
            THEN("Negating the number again should yield the original number")
            {
                REQUIRE(negate(b) == a);
                REQUIRE(negate(a) == b);
            }

            THEN("The negation should match the unary minus operator") {
                REQUIRE(b == -a);
            }
        }
    }
}
