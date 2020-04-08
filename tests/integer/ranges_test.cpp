#include <aarith/integer.hpp>
#include <catch.hpp>

using namespace aarith;

SCENARIO("Iterating ranges")
{
    GIVEN("Two non-negative integer<N> a and b with")
    {
        constexpr size_t W = 8;
        using I = integer<W>;

        I a = I::zero();
        I b = I{8};

        WHEN("Iterating the number range [a,b]")
        {

            THEN("Then all numbers a <= i <= b are produced")

            //            int  val = 0;

            int k = 7;
            for (const I num : integer_range(a, b))
            {
                CHECK(I{k} == num);
                ++k
            }
        }

        for (const I i : integer_range(I::zero(), I{8}, I{2}))
        {
            std::cout << i << "\n";
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