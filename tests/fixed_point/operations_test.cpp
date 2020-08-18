#include <catch.hpp>

#include <aarith/fixed_point.hpp>

using namespace aarith;

SCENARIO("Addition of fixed point numbers", "[fixed point]")
{
    GIVEN("Two fixed point numbers")
    {
        constexpr fixed_point<32, 3> a{8};
        constexpr fixed_point<32, 3> b{4};
        {
            WHEN("Adding these numbers")
            {
                THEN("The result should be correct")
                {

                    constexpr size_t twelve{12};
                    constexpr fixed_point<33, 3> expected = fixed_point<33, 3>::from_integral(twelve);

                    std::cout << expected << "\n";
                                        const auto result = expanding_add(a, b);
                    //
                                        REQUIRE(result == expected);
                }
            }
        }
    }
}