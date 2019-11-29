#include "aarith/operations/comparisons.hpp"
#include "aarith/operations/sinteger_operations.hpp"
#include "aarith/types/sinteger.hpp"
#include "aarith/utilities/string_utils.hpp"
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
