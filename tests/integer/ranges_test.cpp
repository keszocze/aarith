#include <aarith/integer.hpp>
#include <catch.hpp>

using namespace aarith;

SCENARIO("Iterating ranges")
{
    constexpr size_t W = 8;
    using I = integer<W>;

    GIVEN("Two non-negative integers a and b with a < b")
    {

        I a = I::zero();
        I b = I{8};

        WHEN("Iterating the number range [a,b]")
        {

            THEN("Then all numbers a <= i <= b are produced")

            //            int  val = 0;

            //            int k = 7;
            for (const I num : integer_range(a, b))
            {
                //                CHECK(I{k} == num);
                //                ++k
                std::cout << num << "\n";
            }
        }

        for (const I i : integer_range(I::zero(), I{8}, I{2}))
        {
            std::cout << i << "\n";
        }
    }

    GIVEN("Two non-negative integers a and b with a > b")
    {
        I a = I{23};
        I b = I{13};
        WHEN("Creating the range [a,b]") {
            THEN("The range should be empty") {

                std::cout << "Iterating over an empty range:\n";
                for (const I num :integer_range(a,b)) {
                    std::cout << num << "\n";
                }
                std::cout << "\n";
            }
        }
    }

    GIVEN("A single non-negative integer a")
    {
        I a{15};
        WHEN("Iterating the range [a,a]")
        {
            THEN("There should be one value, namely a")
            {
                for (const I num : integer_range(a, a))
                {
                    std::cout << num << "\n";
                }
            }
        }
    }
}

SCENARIO("Using non-positive strides")
{
    GIVEN("Two unsigned integers")
    {
        constexpr size_t W = 8;
        using I = uinteger<W>;

        I a{12};
        I b{3};

        THEN("A  stride of zero should throw an exception")
        {
            CHECK_THROWS_AS(integer_range(a, b, I::zero()), std::invalid_argument);
        }
    }

    GIVEN("Two unsigned integers")
    {
        constexpr size_t W = 8;
        using I = integer<W>;

        I a{-12};
        I b{3};

        THEN("A stride of zero should throw an exception")
        {
            CHECK_THROWS_AS(integer_range(a, b, I::zero()), std::invalid_argument);
        }
        THEN("A negative stride should throw an exception")
        {
            CHECK_THROWS_AS(integer_range(a, b, I::minus_one()), std::invalid_argument);
            CHECK_THROWS_AS(integer_range(a, b, I{-4}), std::invalid_argument);
        }
    }
}