#include <aarith/float.hpp>
#include <bitset>
#include <catch.hpp>
using namespace aarith;


TEMPLATE_TEST_CASE_SIG("Comparisons should match the native counterparts",
                       "[normalized_float][comparison][utility]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    using F = normalized_float<E, M>;

    std::cout << to_binary(F::sNaN(integer<M>{56})) << "\n";
    std::cout << to_binary(F::qNaN(integer<M>{45})) << "\n";
}