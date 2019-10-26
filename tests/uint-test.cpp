#include "aarith/operations/exact_operations.hpp"
#include "aarith/types/integer.hpp"
#include <aarith/operations/comparisons.hpp>
#include <catch.hpp>
#include <sstream>

using namespace aarith;

SCENARIO("Outputting uintegers", "[uinteger]")
{
    WHEN("Writing a uinteger into a stream")
    {
        const uinteger<16> uint{static_cast<uint16_t>(0b11001100)};
        std::stringstream ss;
        ss << uint;

        THEN("Its bit representation is output")
        {
            REQUIRE(ss.str() == "0000000011001100");
        }
    }
}

SCENARIO("Casting unsigned integers into different width", "[uinteger]")
{
    GIVEN("width_cast is called")
    {
        static constexpr uint16_t test_value = 123;
        static constexpr size_t SourceWidth = 16;
        const uinteger<SourceWidth> uint{test_value};

        WHEN("The source width <= destination width")
        {
            static constexpr size_t DestinationWidth = 32;
            auto const result = width_cast<DestinationWidth>(uint);

            THEN("The result has the destination width")
            {
                REQUIRE(result.width() == DestinationWidth);
            }
            AND_THEN("The result is not cut off")
            {
                REQUIRE(result.word(0) == test_value);
            }
        }
        WHEN("The source width > destination width")
        {
            static constexpr size_t DestinationWidth = 8;
            auto const result = width_cast<DestinationWidth>(uint);

            THEN("The result has the destination width")
            {
                REQUIRE(result.width() == DestinationWidth);
            }
            AND_THEN("The result is cut off")
            {
                REQUIRE(result.word(0) == (test_value & 0xff));
            }
        }
    }
}

TEMPLATE_TEST_CASE("word_mask works nicely", "[uinteger][utility]", uinteger<8>, uinteger<12>,
                   uinteger<13>, uinteger<16>, uinteger<23>, uinteger<32>, uinteger<45>,
                   uinteger<63>, uinteger<64>, uinteger<65>, uinteger<75>)
{
    static constexpr uint8_t number_a = 23U; // random number without semantic meaning

    const TestType a{number_a};

    static constexpr uint64_t zero{0U};

//    std::cout << uinteger<64>{a.word_mask(0)} << "\n";

    CHECK(a.word_mask(0) != zero);
}

TEMPLATE_TEST_CASE("addition actually works as expected", "[uinteger][arithmetic]", uinteger<8>,
                   uinteger<12>, uinteger<13>, uinteger<16>, uinteger<23>, uinteger<32>,
                   uinteger<45>, uinteger<63>, uinteger<64>, uinteger<65>, uinteger<75>)
{
    static constexpr uint8_t number_a = 17U;
    static constexpr uint8_t number_b = 23U;

    TestType a{number_a};
    TestType b{number_b};
    const TestType result = exact_uint_add(a, b);

    CHECK(result.word(0) == number_a + number_b);
}

SCENARIO("Exact sum of unsigned integers", "[uinteger][arithmetic]")
{
    GIVEN("Two uintegers with width < word_width")
    {
        static constexpr size_t TestWidth = 16;
        static_assert(uinteger<TestWidth>::word_count() == 1);

        WHEN("The result still fits the width")
        {
            static constexpr uint8_t number_a = 32;
            static constexpr uint8_t number_b = 16;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};
            auto const result = exact_uint_add(a, b);

            THEN("It should be the correct sum")
            {
                REQUIRE(result.word(0) == number_a + number_b);
            }
        }
        WHEN("The result does not fit the width")
        {
            static constexpr uint16_t number_a = (1 << TestWidth) - 1;
            static constexpr uint16_t number_b = 1;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};
            auto const result = exact_uint_add(a, b);

            THEN("It should be the masked to fit")
            {
                REQUIRE(result.word(0) == 0);
            }
        }
    }
}

SCENARIO("Comparison operators work as expected", "[uinteger][utility]")
{
    GIVEN("Two uintegers a and b")
    {

        const size_t TestWidth = 16;

        WHEN("a < b")
        {
            static constexpr uint16_t number_a = 7;
            static constexpr uint16_t number_b = 23;
            const uinteger<TestWidth> a{number_a};
            const uinteger<TestWidth> b{number_b};

            REQUIRE(a < b);
            REQUIRE(a <= b);
            REQUIRE(!(a > b));
            REQUIRE(!(a >= b));
        }
    }
}

SCENARIO("Exact difference of unsigned integers", "[uinteger][arithmetic]")
{

    static constexpr uint8_t number_a = 20;
    static constexpr uint8_t number_b = 100;

    static constexpr uint8_t num_res1 = number_a - number_b;
    static constexpr uint8_t num_res2 = number_b - number_a;

    static constexpr size_t TestWidth1 = 8;

    const uinteger<TestWidth1> a{number_a};
    const uinteger<TestWidth1> b{number_b};

    static constexpr uint8_t number_zero =0;

    static const uinteger<TestWidth1> zero{number_zero};

    auto const result1 = exact_uint_sub(a, b);
    auto const result2 = exact_uint_sub(b, a);
    auto const result3 = exact_uint_sub(a, a);
    auto const result4 = exact_uint_sub(b, b);

    CHECK(result1.word(0) == num_res1);
    CHECK(result2.word(0) == num_res2);
    CHECK(result1 == uinteger<TestWidth1>{num_res1});
    CHECK(result2 == uinteger<TestWidth1>{num_res2});
    CHECK(result3 == zero);
    CHECK(result4 == zero);
    CHECK(result3.word(0) == 0U);
    CHECK(result4.word(0) == 0U);
}

// for static_assert tests:
// https://stackoverflow.com/questions/30155619/expected-build-failure-tests-in-cmake