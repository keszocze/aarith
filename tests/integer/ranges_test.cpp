#include <aarith/integer.hpp>
#include <catch.hpp>

using namespace aarith;

SCENARIO("Iterating ranges", "[integer][signed][ranges][operation][utility]")
{
    constexpr size_t W = 8;
    using I = integer<W>;

    GIVEN("Two non-negative integers a and b with a < b")
    {

        const I a = I::zero();
        const I b = I{8};

        WHEN("Iterating the number range [a,b]")
        {

            THEN("Then all numbers a <= i <= b are produced")
            {

                int k = 0;
                for (const I num : integer_range(a, b))
                {
                    REQUIRE(I{k} == num);
                    k++;
                }
            }
        }
        AND_WHEN("A stride of two is used")
        {
            THEN("Only the even numbers are produced")
            {
                int k = 0;
                for (const I i : integer_range(a, b, I{2}))
                {
                    REQUIRE(I{k} == i);
                    k += 2;
                }
            }
        }

        AND_WHEN("A stride of three is used")
        {
            THEN("There is no overflow")
            {
                const std::array<int, 3> nums{0, 3, 6};
                int k = 0;
                for (const I i : integer_range(a, b, I{3}))
                {
                    REQUIRE(I{nums[k]} == i);
                    k++;
                }
                REQUIRE(k == 3);
            }
        }
    }

    GIVEN("Two integers a and b with a < 0 < b")
    {

        const I a{-5};
        const I b{5};

        WHEN("They form a range")
        {
            THEN("The iterations works fine")
            {

                int k = -5;
                for (const I i : integer_range(a, b, I{1}))
                {
                    REQUIRE(I{k} == i);
                    ++k;
                }
            }
        }
    }

    GIVEN("Two non-negative integers a and b with a > b")
    {
        I a = I{23};
        I b = I{13};
        WHEN("Creating the range [a,b]")
        {
            THEN("The range should be empty")
            {

                size_t count = 0;
                for ([[maybe_unused]] const I num : integer_range(a, b))
                {
                    ++count;
                }
                REQUIRE(count == 0);
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
                size_t count = 0;
                for ([[maybe_unused]] const I num : integer_range(a, a))
                {
                    ++count;
                }
                REQUIRE(count == 1);
            }
        }
    }
}

SCENARIO("Reverse iterating integer ranges",
         "[integer][signed][ranges][operation][utility][reverse]")
{
    constexpr size_t W = 8;
    using I = integer<W>;
    GIVEN("An integer range")
    {

        I a = I{-4};
        I b = I{4};
        WHEN("Using the reverse iterator")
        {
            AND_WHEN("Using the default stride")
            {
                THEN("The numbers should be correctly iterated backwards")
                {
                    int k = 4;
                    const auto range = integer_range(a, b);

                    for (auto iter = range.rbegin(); iter != range.rend(); ++iter)
                    {
                        REQUIRE(I{k} == *iter);
                        --k;
                    }
                }
            }
            AND_WHEN("Using a stride of two")
            {
                THEN("The numbers should be correctly iterated backwards")
                {
                    int k = 4;
                    const auto range = integer_range(a, b, I{2});

                    for (auto iter = range.rbegin(); iter != range.rend(); ++iter)
                    {
                        REQUIRE(I{k} == *iter);
                        k -= 2;
                    }
                }
            }
            AND_WHEN("Using a stride of three")
            {
                THEN("The numbers should be correctly iterated backwards and no underflow happens")
                {
                    std::vector<int> values{4, 1, -2};
                    int k = 0;
                    const auto range = integer_range(a, b, I{3});

                    for (auto iter = range.rbegin(); iter != range.rend(); ++iter)
                    {
                        REQUIRE(I{values[k]} == *iter);
                        ++k;
                    }
                }
            }
        }
    }
    GIVEN("Two non-negative integers a and b with a > b")
    {
        I a = I{23};
        I b = I{13};
        WHEN("Creating the range [a,b]")
        {
            THEN("The range should be empty")
            {

                size_t count = 0;
                const auto range = integer_range(a, b);
                for (auto iter = range.rbegin(); iter != range.rend(); ++iter)
                {
                    ++count;
                }
                REQUIRE(count == 0);
            }
        }
    }
}

SCENARIO("Using non-positive strides", "[integer][unsigned][ranges][operation][utility]")
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

    GIVEN("Two integers")
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