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

TEMPLATE_TEST_CASE_SIG("Negation works as intended",
                       "[normalized_float][negate][sig_bit_operation][invariant]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    using F = normalized_float<E, M>;

    GIVEN("A random floating-point number")
    {

        F a = GENERATE(take(15, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        WHEN("Negating the number")
        {
            F b = negate(a);
            THEN("Negating the number again should yield the original number")
            {
                // we need to use this equality as NaNs do not compare equal otherwise
                REQUIRE(bitwise_equality(negate(b), a));
                REQUIRE(bitwise_equality(negate(a), b));
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Copying a floating-point number works",
                       "[normalized_float][copy][sig_bit_operation][invariant]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    using F = normalized_float<E, M>;

    GIVEN("A random floating-point number")
    {

        F a = GENERATE(take(15, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        WHEN("Copying the number")
        {
            F b = copy(a);

            THEN("The result compares equal")
            {
                // we need to use this equality as NaNs do not compare equal otherwise
                REQUIRE(bitwise_equality(a, b));
            }
        }
    }
}
TEMPLATE_TEST_CASE_SIG("Copying a floating-point number with the sign of another number works",
                       "[normalized_float][copySign][sig_bit_operation][invariant]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    using F = normalized_float<E, M>;

    GIVEN("A random floating-point number")
    {

        F a = GENERATE(take(15, random_float<E, M, FloatGenerationModes::FullyRandom>()));
        F b = GENERATE(take(15, random_float<E, M, FloatGenerationModes::FullyRandom>()));

        WHEN("Copying the number with the sign of another number")
        {
            F c = copySign(a, b);

            THEN("The result compares equal")
            {
                REQUIRE(c.get_sign() == b.get_sign());
                REQUIRE(abs(c) == abs(a));
            }
        }
    }
}