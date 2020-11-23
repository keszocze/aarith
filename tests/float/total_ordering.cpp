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

    const F s = F::sNaN(integer<M>{56});
    const F q = F::qNaN(integer<M>{45});

    std::cout << to_binary(s) << "\n";
    std::cout << to_binary(q) << "\n";

    std::cout << s.is_nan() << "\t" << s.is_qNaN() << "\t" << s.is_sNaN() << "\n";
    std::cout << q.is_nan() << "\t" << q.is_qNaN() << "\t" << q.is_sNaN() << "\n";
}