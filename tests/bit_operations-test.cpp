#include <catch.hpp>

#include <aarith/types/word_container.hpp>
#include <aarith/utilities/bit_operations.hpp>
using namespace aarith;

SCENARIO("Splitting uint64_ts", "[util]")
{
    WHEN("Considering an uint64_t n")
    {
        THEN("Splitting and unsplitting should yield the original number")
        {
            uint64_t n = 0u;

            for (uint i = 0; i < 10000000; i++)
            {
                const auto [u, l] = split(n);
                const auto result = unsplit(u, l);
                REQUIRE(result == n);

                n += 15381;
            }
        }
    }
}

SCENARIO("Counting bits in word_container", "[util]")
{
    WHEN("Multiple word_containers are given")
    {
        THEN("The leading zeroe index should be computed correctly")
        {
            word_container<64> a_zero{0U};
            word_container<64> a_one{1U};
            word_container<32> b_zero{0U};
            word_container<32> b_one{1U};
            word_container<15> c_zero{0U};
            word_container<15> c_one{1U};
            word_container<150> d_zero{0U};
            word_container<150> d_one{1U};
            
            CHECK(count_leading_zeroes(a_zero) == 64);
            CHECK(count_leading_zeroes(b_zero) == 32);
            CHECK(count_leading_zeroes(c_zero) == 15);
            CHECK(count_leading_zeroes(d_zero) == 150);
            CHECK(count_leading_zeroes(a_one) == 64-1);
            CHECK(count_leading_zeroes(b_one) == 32-1);
            CHECK(count_leading_zeroes(c_one) == 15-1);
            CHECK(count_leading_zeroes(d_one) == 150-1);
        }
    }
}


SCENARIO("Casting word_containers into different width", "[word_container]")
{
    GIVEN("width_cast is called")
    {
        static constexpr uint16_t test_value = 123;
        static constexpr size_t SourceWidth = 16;
        const word_container<SourceWidth> uint{test_value};

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



SCENARIO("Copy constructor of word_containers with various bit widths", "[word_container][utility]")
{
    GIVEN("An word_container<N> a")
    {

        //        const uint64_t val_a = GENERATE(take(10, random(0U,
        //        std::numeric_limits<uint64_t>::max()));
        const uint64_t val_a = 24;
        word_container<196> a(0U,val_a,0U);

        THEN("Assignment of individual words is correct")
        {
            CHECK(a.word(0) == 0U);
            CHECK(a.word(1) == val_a);
            CHECK(a.word(2) == 0U);
        }

        AND_GIVEN("An word_container<N> b")
        {
            const uint64_t val_b = 1337;
            const word_container<196> b = word_container<196>::from_words(val_b, 0U, 2 * val_b);

            THEN("Assignment opeator of individual words is correct")
            {

                a = b;
                CHECK(a.word(0) == 2 * val_b);
                CHECK(a.word(1) == 0U);
                CHECK(a.word(2) == val_b);
            }
        }
        AND_GIVEN("An word_container<M> b")
        {
            WHEN("M < N")
            {
                const uint64_t val_b = 23;
                const word_container<64> tmp = word_container<64>::from_words(val_b);

                THEN("The copy constructor should work")
                {
                    word_container<128> b{tmp};

                    CHECK(b.word(0) == val_b);
                    CHECK(b.word(1) == 0U);
                }
                THEN("The assignment operator should work")
                {
                    word_container<128> b;
                    b = tmp;
                    CHECK(b.word(0) == val_b);
                    CHECK(b.word(1) == 0U);

                    a = tmp;
                    CHECK(a.word(0) == val_b);
                    CHECK(a.word(1) == 0U);
                    CHECK(a.word(2) == 0U);
                }
            }
        }
    }
}


SCENARIO("Calculating the word_masks of word_containers", "[word_container][utility]")
{
    // The tests all assume that word_container uses 64-bit words.
    static_assert(word_container<64>::word_width() == 64);

    GIVEN("A word_container<N> where N < word_width")
    {
        word_container<32> uint;
        THEN("The mask is correct")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffff);
        }
    }
    GIVEN("A word_container<N> where N == word_width")
    {
        word_container<64> uint;
        THEN("The mask is all 1s")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffffffffffff);
        }
    }
    GIVEN("A word_container<N> where N > word_width and N % word_width != 0")
    {
        word_container<96> uint;
        THEN("All masks except the last are all 1s")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffffffffffff);
        }
        THEN("The last mask is all ones up to the correct bit")
        {
            REQUIRE(uint.word_mask(1) == 0xffffffff);
        }
    }
    GIVEN("A word_container<N> where N > word_width and N % word_width == 0")
    {
        word_container<128> uint;
        THEN("All masks are all 1s")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffffffffffff);
            REQUIRE(uint.word_mask(1) == 0xffffffffffffffff);
        }
    }
}

SCENARIO("Left shift operator works as expected", "[word_container][utility]")
{
    GIVEN("One word_container a and a number of shifted bits s")
    {

        const size_t TestWidth = 16;

        WHEN("The result still fits the width")
        {
            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 4U;
            const word_container<TestWidth> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 112);
        }
        WHEN("The result does not fit the width anymore")
        {
            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 14U;
            const word_container<TestWidth> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 49152);
        }

        WHEN("Some bits are shifted to the next word")
        {
            const size_t Width = 70;

            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 63U;
            const word_container<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0x8000000000000000);
            REQUIRE(result.word(1) == 3);
        }
        WHEN("The bits are shifted to the second next word")
        {
            const size_t Width = 130;

            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 127U;
            const word_container<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == 0x8000000000000000);
            REQUIRE(result.word(2) == 3);
        }
        WHEN("The bits are not shifted")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = 0;
            const word_container<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 3);
            REQUIRE(result.word(1) == 0);
            REQUIRE(result.word(2) == 0);
        }
        WHEN("The bits are shifted exactly one word")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = static_cast<size_t>(word_container<Width>::word_width());
            const word_container<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == 3);
            REQUIRE(result.word(2) == 0);
        }
        WHEN("The bits are shifted exactly two words")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = 2 * static_cast<size_t>(word_container<Width>::word_width());
            const word_container<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == 0);
            REQUIRE(result.word(2) == 3);
        }
        WHEN("The bits are shifted exactly by word_width-1")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = static_cast<size_t>(word_container<Width>::word_width()) - 1U;
            const word_container<Width> a{number_a};

            auto reference = a.word(0) << s;

            const auto result = a << s;
            REQUIRE(result.word(0) == reference);
            REQUIRE(result.word(1) == 1);
            REQUIRE(result.word(2) == 0);
        }
        WHEN("The bits are shifted by 2*word_width-1")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = 2U * static_cast<size_t>(word_container<Width>::word_width()) - 1U;
            const word_container<Width> a{number_a};

            auto reference = a.word(0) << (s % (sizeof(word_container<Width>::word_width()) * 8));

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == reference);
            REQUIRE(result.word(2) == 1);
        }
    }
}