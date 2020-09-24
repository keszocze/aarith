#include <aarith/float.hpp>
#include <catch.hpp>
#include "../test-signature-ranges.hpp"

using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Arithmetic should be constexpr", "[integer][signed][arithmetic][constexpr]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = normalized_float<8,23>;

    float a_float{-1337.35f};
    float b_float{420815.0f};

    constexpr float delta = {0.123f};
    constexpr float delta2 = {3.33f};

    for (size_t i = 0; i < std::numeric_limits<size_t>::max()-1; ++i) {
        F a{a_float};
        F b{b_float};

        F res = a+b;
        float res_float = a_float + b_float;

        F res_float_{res_float};
        float res_ = static_cast<float>(res);


        if (res_float_ != res) {
            std::cout << to_binary(res) << "\t" << to_binary(res_float_) << "\n";
        }

        CHECK(res_float_ == res);
        REQUIRE(res_ == res_float);


        a_float+=delta;
        b_float+=delta2;
    }
}