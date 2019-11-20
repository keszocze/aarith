#include "aarith/types/integer.hpp"
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

SCENARIO("Casting uintegers into different width", "[uinteger]")
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

SCENARIO("Calculating the word_masks of uintegers", "[uinteger][utility]")
{
    // The tests all assume that uinteger uses 64-bit words.
    static_assert(uinteger<64>::word_width() == 64);

    GIVEN("A uinteger<N> where N < word_width")
    {
        uinteger<32> uint;
        THEN("The mask is correct")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffff);
        }
    }
    GIVEN("A uinteger<N> where N == word_width")
    {
        uinteger<64> uint;
        THEN("The mask is all 1s")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffffffffffff);
        }
    }
    GIVEN("A uinteger<N> where N > word_width and N % word_width != 0")
    {
        uinteger<96> uint;
        THEN("All masks except the last are all 1s")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffffffffffff);
        }
        THEN("The last mask is all ones up to the correct bit")
        {
            REQUIRE(uint.word_mask(1) == 0xffffffff);
        }
    }
    GIVEN("A uinteger<N> where N > word_width and N % word_width == 0")
    {
        uinteger<128> uint;
        THEN("All masks are all 1s")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffffffffffff);
            REQUIRE(uint.word_mask(1) == 0xffffffffffffffff);
        }
    }
}

SCENARIO("Left shift operator works as expected", "[uinteger][utility]")
{
    GIVEN("One uinteger a and a number of shifted bits s")
    {

        const size_t TestWidth = 16;

        WHEN("The result still fits the width")
        {
            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 4U;
            const uinteger<TestWidth> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 112);
        }
        WHEN("The result does not fit the width anymore")
        {
            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 14U;
            const uinteger<TestWidth> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 49152);
        }

        WHEN("Some bits are shifted to the next word")
        {
            const size_t Width = 70;

            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 63U;
            const uinteger<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0x8000000000000000);
            REQUIRE(result.word(1) == 3);
        }
        WHEN("The bits are shifted to the second next word")
        {
            const size_t Width = 130;

            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 127U;
            const uinteger<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == 0x8000000000000000);
            REQUIRE(result.word(2) == 3);
        }
    }
}

SCENARIO("Logical AND works as expected", "[uinteger][arithmetic]")
{
    GIVEN("Two uintegers")
    {
        WHEN("The uintegers consists of only one word")
        {
            const size_t Width = 70;

            static constexpr uint16_t number_a = 7;
            static constexpr uint16_t number_b = 14;
            const uinteger<Width> a{number_a};
            const uinteger<Width> b{number_b};

            const auto result = a & b;
            const auto result_ref = number_a & number_b;
            REQUIRE(result.word(0) == result_ref);
          
        }
    }
}

SCENARIO("Logical OR works as expected", "[uinteger][arithmetic]")
{
    GIVEN("Two uintegers")
    {
        WHEN("The uintegers consists of only one word")
        {
            const size_t Width = 70;

            static constexpr uint16_t number_a = 7;
            static constexpr uint16_t number_b = 14;
            const uinteger<Width> a{number_a};
            const uinteger<Width> b{number_b};

            const auto result = a | b;
            const auto result_ref = number_a | number_b;
            REQUIRE(result.word(0) == result_ref);
          
        }
    }
}

SCENARIO("Logical NOT works as expected", "[uinteger][arithmetic]")
{
    GIVEN("One uintegers")
    {
        WHEN("The uinteger consists of only one word")
        {
            const size_t Width = 70;

            static constexpr uint16_t number_a = 7;
            const uinteger<Width> a{number_a};

            const auto result = ~a;
            const auto result_ref = ~number_a;
            REQUIRE(result.word(0) == result_ref);
          
        }
    }
}

SCENARIO("Checking whether an uinteger is not equal to zero/false")
{
    GIVEN("An uinteger<N>=0=a for various N")
    {
        THEN("a.is_zero() should be true")
        {

            const uint8_t zero = 0U;

            CHECK(uinteger<64>{zero}.is_zero());
            CHECK(uinteger<128>{zero}.is_zero());
            CHECK(uinteger<150>{zero}.is_zero());
            CHECK(uinteger<32>{zero}.is_zero());
            CHECK(uinteger<23>{zero}.is_zero());
            CHECK(uinteger<256>{zero}.is_zero());
            CHECK(uinteger<1337>{zero}.is_zero());
            CHECK(uinteger<8>{zero}.is_zero());

        }
        THEN("a should evaluate to false in a Boolean context")
        {

            const uint8_t zero = 0U;

            CHECK_FALSE(uinteger<64>{zero});
            CHECK_FALSE(uinteger<128>{zero});
            CHECK_FALSE(uinteger<150>{zero});
            CHECK_FALSE(uinteger<32>{zero});
            CHECK_FALSE(uinteger<23>{zero});
            CHECK_FALSE(uinteger<256>{zero});
            CHECK_FALSE(uinteger<1337>{zero});
            CHECK_FALSE(uinteger<8>{zero});

        }
    }

    GIVEN("An non-zero uinteger")
    {

        uint64_t val = GENERATE(1,2,4,5567868,234,21,45,56768);
        uinteger<64> a{val};
        uinteger<128> b=uinteger<128>::from_words(val,0U);
        uinteger<128> c=uinteger<128>::from_words(val,val);

        uinteger<150> d=uinteger<150>::from_words(val,0U,0U);
        uinteger<256> e=uinteger<256>::from_words(val,val,0U,0U);

        THEN("is_zero should correctly return this fact")
        {
            CHECK_FALSE(a.is_zero());
            CHECK_FALSE(b.is_zero());
            CHECK_FALSE(c.is_zero());
            CHECK_FALSE(d.is_zero());
            CHECK_FALSE(e.is_zero());
        }
        THEN("The integer should evaluate to true in a Boolean context")
        {
            CHECK(a);
            CHECK(b);
            CHECK(c);
            CHECK(d);
            CHECK(e);
        }
    }
}

SCENARIO("Using the for loop operation feature from ")
{
    GIVEN("An unsigned integer")
    {
        THEN("The forward iterators should give the words as expected")
        {
            uint64_t val_a = GENERATE(0,1,2,3);
            uint64_t val_b = GENERATE(3,56,567,324);

            uinteger<64> a{val_a};
            uinteger<128> b = uinteger<128>::from_words(val_a,val_b);
//            std::array<uinteger<64>::word_type ,2> b_arr{val_a,val_b};

            size_t index=0;
            for (const uinteger<64>::word_type w: a) {
                CHECK(w == a.word(index));
                index++;
            }

            CHECK(index == 1);

            index=0;


            for (const uinteger<128>::word_type w: b) {
                CHECK(w == b.word(index));
                index++;
            }

            CHECK(index == 2);
        }
        THEN("Using forward iterators explicitly should give the words as expected")
        {
            uint64_t val_a = GENERATE(0,1,2,3);
            uint64_t val_b = GENERATE(3,56,567,324);

            uinteger<64> a{val_a};
            uinteger<128> b = uinteger<128>::from_words(val_a,val_b);
//            std::array<uinteger<64>::word_type ,2> b_arr{val_a,val_b};

            size_t index=0;
            for (auto iter = a.begin(); iter != a.end(); ++iter) {
                CHECK(*iter == a.word(index));
                index++;
            }

            CHECK(index == 1);

            index=0;

            for (auto iter = b.begin(); iter != b.end(); ++iter) {
                CHECK(*iter == b.word(index));
                index++;
            }

            CHECK(index == 2);
        }

        THEN("Using backwards iterators explicitly should give the words as expected")
        {
            uint64_t val_a = GENERATE(0,1,2,3);
            uint64_t val_b = GENERATE(3,56,567,324);

            uinteger<64> a{val_a};
            uinteger<128> b = uinteger<128>::from_words(val_a,val_b);
//            std::array<uinteger<64>::word_type ,2> b_arr{val_a,val_b};

            size_t index=0;
            for (auto iter = a.rbegin(); iter != a.rend(); ++iter) {
                CHECK(*iter == a.word((a.word_count()-1)-index));
                index++;
            }

            CHECK(index == 1);

            index=0;

            for (auto iter = b.rbegin(); iter != b.rend(); ++iter) {
                CHECK(*iter == b.word((b.word_count()-1)-index));
                index++;
            }

            CHECK(index == 2);
        }
    }
}

// for static_assert tests:
// https://stackoverflow.com/questions/30155619/expected-build-failure-tests-in-cmake
