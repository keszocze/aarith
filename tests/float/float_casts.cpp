#include <aarith/float.hpp>
#include <bitset>
#include <catch.hpp>
using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Casting to and from the native data types should be lossless",
                       "[normalized_float][casting][utility]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    using F = normalized_float<E, M>;

    Native native = GENERATE(take(15, random<Native>(std::numeric_limits<Native>::min(),
                                                     std::numeric_limits<Native>::max())));

    F from_native{native};

    Native casted_back = static_cast<Native>(from_native);

    REQUIRE(casted_back == native);
}

TEMPLATE_TEST_CASE_SIG("Infinity and NaNs are created correctly",
                       "[normalized_float][casting][utility]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    using F = normalized_float<E, M>;

    Native nan_native = std::numeric_limits<Native>::quiet_NaN();
    Native pos_inf_native = std::numeric_limits<Native>::infinity();
    F nan{F::NaN()};
    F nan_from_native{nan_native};
    F pos_inf{F::pos_infinity()};


    Native pos_inf_casted = static_cast<Native>(pos_inf);

    CHECK(nan_from_native == nan);

    REQUIRE(pos_inf_casted == pos_inf_native);
}