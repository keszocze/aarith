#include "aarith/operations/comparisons.hpp"
#include "aarith/operations/sinteger_operations.hpp"
#include "aarith/types/sinteger.hpp"
#include "aarith/utilities/string_utils.hpp"
#include <aarith/operations/sinteger_comparisons.hpp>
#include <catch.hpp>

using namespace aarith;

SCENARIO("Adding two positive sintegers exactly", "[sinteger][arithmetic][addition]")
{
    GIVEN("Two sinteger<N> a and b with N <= word_width")
    {
        static constexpr size_t TestWidth = 16;
        static_assert(sinteger<TestWidth>::word_count() == 1);

        WHEN("The result a+b still fits into N bits")
        {
            static constexpr uint8_t number_a = 32;
            static constexpr uint8_t number_b = 16;
            const sinteger<TestWidth> a{number_a};
            const sinteger<TestWidth> b{number_b};
            const sinteger<TestWidth> result = add(a, b);

            THEN("It should be the correct sum")
            {
                REQUIRE(result.word(0) == number_a + number_b);
            }
        }
        WHEN("The result a+b does not fit into N bits")
        {
            static constexpr uint16_t number_a = (1 << TestWidth) - 1;
            static constexpr uint16_t number_b = 1;
            const sinteger<TestWidth> a{number_a};
            const sinteger<TestWidth> b{number_b};
            auto const result = add(a, b);

            THEN("It should be the masked to fit")
            {
                REQUIRE(result.word(0) == 0);
            }
        }
    }

    GIVEN("Two sinteger<N> a and b with N > word_width")
    {
        static constexpr size_t TestWidth = 128;
        static_assert(sinteger<TestWidth>::word_count() > 1);

        WHEN("There is a carry into the next word")
        {
            static constexpr uint64_t number_a = 1ULL << 63;
            static constexpr uint64_t number_b = 1ULL << 63;
            const sinteger<TestWidth> a{number_a};
            const sinteger<TestWidth> b{number_b};
            auto const result = add(a, b);

            THEN("It is added to the next word")
            {
                REQUIRE(result.word(1) == 1);
            }
        }

        WHEN("There is no carry into the next word")
        {
            static constexpr uint64_t number_a = 1ULL << 63;
            static constexpr uint64_t number_b = 0;
            const sinteger<TestWidth> a{number_a};
            const sinteger<TestWidth> b{number_b};
            auto const result = add(a, b);

            THEN("The next word is unchanged")
            {
                REQUIRE(result.word(1) == 0);
            }
        }
    }
}

SCENARIO("Absolute value computation", "[sinteger][utility]")
{
    GIVEN("The smallest possible value")
    {
        const sinteger<150> min = std::numeric_limits<sinteger<150>>::min();
        THEN("The absolute value of that value is the value again")
        {
            REQUIRE(abs(min) == min);
        }

        THEN("The the 'real' absolute value is 2^(W-1)") 
        {
            uinteger<150> abs = expanding_abs(min);
            CHECK(abs.word(0) == 0U);
            CHECK(abs.word(1) == 0U);
            REQUIRE(abs.word(2) == (1U << 21U));
        }
    }

    GIVEN("Any non-smallest value")
    {
        using sint = sinteger<64>;
        const int32_t val_32 =
            GENERATE(take(50, random(std::numeric_limits<int32_t>::min() + 1, -1)));
        const int64_t val_64 = GENERATE(
            take(50, random(std::numeric_limits<int64_t>::min() + 1, static_cast<int64_t>(-1))));
        const sint a{val_32};
        const sint b{val_64};

        THEN("Computing abs is idempotent")
        {
            REQUIRE(abs(abs(a)) == abs(a));
            REQUIRE(abs(abs(b)) == abs(b));

            // not "really" applying it twice but close enough
            REQUIRE(abs(sint(expanding_abs(a))) == sint(expanding_abs(a)));
            REQUIRE(abs(sint(expanding_abs(b))) == sint(expanding_abs(b)));
        }

        // does not work for int64 as, for some reason, there is no matching abs function for that
        THEN("Computing abs should match its int32 type counterpart")
        {
            REQUIRE(abs(a).word(0) == abs(val_32));
            REQUIRE(expanding_abs(a).word(0) == abs(val_32));
        }
    }
}

SCENARIO("Unary minus operation", "[sinteger][utility]")
{
    GIVEN("The smallest possible value")
    {
        const sinteger<150> min = std::numeric_limits<sinteger<150>>::min();
        THEN("The unary minus value of that value is the value again")
        {
            REQUIRE(-min == min);
        }
    }

    GIVEN("Any non-smallest value")
    {
        using sint = sinteger<64>;
        const int32_t val_32 =
            GENERATE(take(50, random(std::numeric_limits<int32_t>::min() + 1, -1)));
        const int64_t val_64 = GENERATE(
            take(50, random(std::numeric_limits<int64_t>::min() + 1, static_cast<int64_t>(-1))));
        const sint a{val_32};
        const sint b{val_64};

        THEN("Unary minus is self-inverse")
        {
            REQUIRE(-(-a) == a);
            REQUIRE(-(-b) == b);
        }

        // does not work for int64 as, for some reason, there is no matching abs function for that
        THEN("Computing abs should match its int32 type counterpart")
        {
            REQUIRE((-a).word(0) == -val_32);
        }
    }
}

// TODO finish this
// SCENARIO("MIN/MAX Values behave as expected", "[sinteger][utility]")
//{
//    GIVEN("The min and max value")
//    {
//        constexpr w = 50;
//        sinteger<w> min=sinteger<w>::ma
//        THEN("Adding/subtracting one should wrap araound")
//        {
//        }
//    }
//}