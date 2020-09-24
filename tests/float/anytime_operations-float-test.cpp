#include "../test-signature-ranges.hpp"
#include <aarith/float.hpp>
#include <aarith/float/approx_operations.hpp>
#include <catch.hpp>

using namespace aarith;


TEMPLATE_TEST_CASE_SIG("Anytime addition", "[normalized_float][arithmetic][constexpr][checking]",
                       AARITH_FLOAT_TEST_SIGNATURE, AARIHT_FLOAT_TEMPLATE_RANGE)
{
    using F = normalized_float<E, M>;

    WHEN("Computing all mantissa bits correctly")
    {
        THEN("The result should match the usual normalized_float result")
        {

//            normalized_float<8,23> inf{std::numeric_limits<float>::infinity()};
//            normalized_float<8,24> inf_{std::numeric_limits<float>::infinity()};
//            std::cout << to_binary(inf, true) << "\t" << inf.is_nan() << "\t" << inf.is_inf() << "\n";
//            std::cout << to_binary(inf_, true) << "\t" << inf_.is_nan() << "\t" << inf_.is_inf() << "\n";
//
//            normalized_float<8,23> nan{std::numeric_limits<float>::signaling_NaN()};
//            normalized_float<8,24> nan_{std::numeric_limits<float>::quiet_NaN()};
//            std::cout << to_binary(nan, true) << "\t" << nan.is_nan() << "\t" << nan.is_inf() << "\n";
//            std::cout << to_binary(nan_, true) << "\t" << nan_.is_nan() << "\t" << nan_.is_inf() << "\n";


//            std::cout << to_binary(normalized_float<8,23>(1.5f),true) << "\t" << to_compute_string(normalized_float<9,23>(1.5f))<< "\n";
//            std::cout << to_binary(normalized_float<9,23>(1.5f),true) << "\t" << to_compute_string(normalized_float<9,23>(1.5f)) << "\n";
//            std::cout << to_binary(normalized_float<8,30>(1.5f),true) << "\n";

//            std::cout << to_binary(normalized_float<8,23>(std::numeric_limits<float>::infinity()),true) << "\n";
//            std::cout << to_binary(normalized_float<8,30>(std::numeric_limits<float>::infinity()),true) << "\n";

//            const auto onehalf = normalized_float<8,23>(1.5f);
//            const auto three = onehalf+onehalf;
//            std::cout << to_binary(onehalf,true) << "\t" << to_compute_string(onehalf) << "\n";
//            std::cout << to_binary(three,true) << "\t" << to_compute_string(three) << "\n";

            CHECK(true);
        }

        // float case
        if constexpr (E == 8 && M == 23)
        {
            THEN("The computation should match its float counterpart")
            {
                float a_d = GENERATE(take(10, random<float>(std::numeric_limits<float>::min(),
                                                            std::numeric_limits<float>::max())));
                float b_d = GENERATE(take(10, random<float>(std::numeric_limits<float>::min(),
                                                            std::numeric_limits<float>::max())));
                float res_d = a_d + b_d;
                F res_d_{res_d};

                F a{a_d};
                F b{b_d};
                F res = anytime_add(a, b, 10*M);

                F res_e = a + b;

                //                std::cout << a_d << " + " << b_d << "\n";

                if (res != res_d_)
                {
                    std::cout << a_d << " + " << b_d << " = " << res_d << "\n";
                    std::cout << to_binary(res) << "\n"
                              << to_binary(res_d_) << "\n"
                              << to_binary(res_e) << "\n";
                    std::cout << res.is_inf() << " " << res.is_nan() << "\n"
                              << res_d_.is_inf() << " " << res_d_.is_nan() << "\n"
                              << res_e.is_inf() << " " << res_e.is_nan() << "\n";
                }
                REQUIRE(res == res_d_);
            }
        }

        // double case
        if constexpr (E == 11 && M == 1)
        {
            THEN("The computation should match its double counterpart")
            {
                double a_d = GENERATE(take(10, random<double>(std::numeric_limits<double>::min(),
                                                              std::numeric_limits<double>::max())));
                double b_d = GENERATE(take(10, random<double>(std::numeric_limits<double>::min(),
                                                              std::numeric_limits<double>::max())));
                double res_d = a_d + b_d;
                F res_d_{res_d};

                F a{a_d};
                F b{b_d};
                F res = anytime_add(a, b, M);

                //                std::cout << a_d << " + " << b_d << "\n";

                if (res != res_d_)
                {
                    std::cout << a_d << "\t" << b_d << "\n";
                    std::cout << to_binary(res) << "\n" << to_binary(res_d_) << "\n";
                }
                REQUIRE(res == res_d_);
            }
        }
    }
}