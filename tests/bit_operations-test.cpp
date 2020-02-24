#include <catch.hpp>

#include <aarith/core/word_container_comparisons.hpp>
#include <aarith/core/word_container.hpp>
#include <aarith/utilities/bit_operations.hpp>
#include <aarith/utilities/string_utils.hpp>

using namespace aarith;

SCENARIO("Counting bits in word_container", "[word_container][util]")
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
            CHECK(count_leading_zeroes(a_one) == 64 - 1);
            CHECK(count_leading_zeroes(b_one) == 32 - 1);
            CHECK(count_leading_zeroes(c_one) == 15 - 1);
            CHECK(count_leading_zeroes(d_one) == 150 - 1);
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

SCENARIO("Copy constructor of word_containers with various bit widths", "[word_container]")
{
    GIVEN("An word_container<N> a")
    {

        //        const uint64_t val_a = GENERATE(take(10, random(0U,
        //        std::numeric_limits<uint64_t>::max()));
        const uint64_t val_a = 24;
        word_container<196> a(0U, val_a, 0U);

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

SCENARIO("Calculating the word_masks of word_containers", "[word_container][bit_logic]")
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

SCENARIO("Left shift operator works as expected", "[word_container][bit_logic]")
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
            static constexpr auto s =
                2U * static_cast<size_t>(word_container<Width>::word_width()) - 1U;
            const word_container<Width> a{number_a};

            auto reference = a.word(0) << (s % (sizeof(word_container<Width>::word_width()) * 8));

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == reference);
            REQUIRE(result.word(2) == 1);
        }
    }
}

SCENARIO("Right shift operator works as expected", "[word_container][utility]")
{
    GIVEN("One word_container a and a number of shifted bits s")
    {
        WHEN("The bits are not shifted")
        {
            const size_t Width = 192;

            typename word_container<Width>::word_type number_a = 3;
            number_a <<= word_container<Width>::word_width() - 2;
            static constexpr auto s = 0;
            word_container<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            const auto result = a >> s;
            REQUIRE(result.word(a.word_count() - 3) == 0);
            REQUIRE(result.word(a.word_count() - 2) == 0);
            REQUIRE(result.word(a.word_count() - 1) == number_a);
        }
        WHEN("The bits are shifted exactly one word")
        {
            const size_t Width = 192;

            typename word_container<Width>::word_type number_a = 3;
            number_a <<= word_container<Width>::word_width() - 2;
            static constexpr auto s = static_cast<size_t>(word_container<Width>::word_width());
            word_container<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            const auto result = a >> s;
            REQUIRE(result.word(a.word_count() - 3) == 0);
            REQUIRE(result.word(a.word_count() - 2) == number_a);
            REQUIRE(result.word(a.word_count() - 1) == 0);
        }
        WHEN("The bits are shifted exactly two words")
        {
            const size_t Width = 192;

            typename word_container<Width>::word_type number_a = 3;
            number_a <<= word_container<Width>::word_width() - 2;
            static constexpr auto s = 2 * static_cast<size_t>(word_container<Width>::word_width());
            word_container<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            const auto result = a >> s;
            REQUIRE(result.word(a.word_count() - 3) == number_a);
            REQUIRE(result.word(a.word_count() - 2) == 0);
            REQUIRE(result.word(a.word_count() - 1) == 0);
        }
        WHEN("The bits are shifted exactly by word_width-1")
        {
            const size_t Width = 192;

            typename word_container<Width>::word_type number_a = 3;
            number_a <<= word_container<Width>::word_width() - 2;
            static constexpr auto s = static_cast<size_t>(word_container<Width>::word_width()) - 1;
            word_container<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            auto ref = number_a << 1;

            const auto result = a >> s;
            REQUIRE(result.word(a.word_count() - 3) == 0);
            REQUIRE(result.word(a.word_count() - 2) == ref);
            REQUIRE(result.word(a.word_count() - 1) == 1);
        }
        WHEN("The bits are shifted by 2*word_width-1")
        {
            const size_t Width = 192;

            typename word_container<Width>::word_type number_a = 3;
            number_a <<= word_container<Width>::word_width() - 2;
            static constexpr auto s =
                2 * static_cast<size_t>(word_container<Width>::word_width()) - 1;
            word_container<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            auto ref = number_a << 1;

            const auto result = a >> s;
            REQUIRE(result.word(a.word_count() - 3) == ref);
            REQUIRE(result.word(a.word_count() - 2) == 1);
            REQUIRE(result.word(a.word_count() - 1) == 0);
        }
        WHEN("The shift amount is a multiple of the word width")
        {

            THEN("The result should still be correct")
            {
                static constexpr size_t width = 256;
                static constexpr size_t word_width = word_container<width>::word_width();

                static const word_container<width> a{1U};
                static const word_container<width> expected0 =
                    word_container<width>::from_words(0U, 0U, 0U, 1U);
                static const word_container<width> expected1 =
                    word_container<width>::from_words(0U, 0U, 1U, 0U);
                static const word_container<width> expected2 =
                    word_container<width>::from_words(0U, 1U, 0U, 0U);
                static const word_container<width> expected3 =
                    word_container<width>::from_words(1U, 0U, 0U, 0U);
                static const word_container<width> expected4 =
                    word_container<width>::from_words(0U, 0U, 0U, 0U);

                std::vector<word_container<width>> expecteds{expected0, expected1, expected2,
                                                             expected3, expected4};

                for (auto i = 0U; i < expecteds.size(); ++i)
                {
                    word_container<width> result = a << (word_width * i);
                    CHECK(result == expecteds[i]);
                }
            }
        }

        AND_WHEN("The shift amount is the integer width")
        {
            THEN("The result should still be correct")
            {
                const size_t w = 128;
                const word_container<w> a{1U};
                const word_container<w> expected;
                const word_container<w> result = a << w;

                CHECK(expected == result);
            }
        }
    }
}

SCENARIO("Bitwise operations work as expected", "[word_container][bit_logic]")
{
    GIVEN("A word container a")
    {

        const size_t Width = 64;
        static constexpr uint16_t number_a = 7;
        const word_container<Width> a{number_a};

        WHEN("Negating the word container")
        {

            THEN("The result should match the NOT of the underlying WordType")
            {
                const auto result = ~a;
                const auto result_ref = ~number_a;
                REQUIRE(result.word(0) == result_ref);
            }
        }

        AND_GIVEN("Another word container b")
        {
            static constexpr uint16_t number_b = 14;
            const word_container<Width> b{number_b};

            THEN("Bitwise AND should work as expected")
            {
                const auto result = a & b;
                const auto result_ref = number_a & number_b;
                REQUIRE(result.word(0) == result_ref);
            }

            AND_THEN("Bitwise OR should work as expected")
            {
                const auto result = a | b;
                const auto result_ref = number_a | number_b;
                REQUIRE(result.word(0) == result_ref);
            }
            AND_THEN("Bitwise XOR should work as expected")
            {
                const auto result = a ^ b;
                const auto result_ref = number_a ^ number_b;
                REQUIRE(result.word(0) == result_ref);
            }
        }
    }
}

SCENARIO("Checking whether an word_container is not equal to zero/false")
{
    GIVEN("An word_container<N>=0=a for various N")
    {
        THEN("a.is_zero() should be true")
        {

            const uint8_t zero = 0U;

            CHECK(word_container<64>{zero}.is_zero());
            CHECK(word_container<128>{zero}.is_zero());
            CHECK(word_container<150>{zero}.is_zero());
            CHECK(word_container<32>{zero}.is_zero());
            CHECK(word_container<23>{zero}.is_zero());
            CHECK(word_container<256>{zero}.is_zero());
            CHECK(word_container<1337>{zero}.is_zero());
            CHECK(word_container<8>{zero}.is_zero());
        }
        THEN("a should evaluate to false in a Boolean context")
        {

            const uint8_t zero = 0U;

            CHECK_FALSE(word_container<64>{zero});
            CHECK_FALSE(word_container<128>{zero});
            CHECK_FALSE(word_container<150>{zero});
            CHECK_FALSE(word_container<32>{zero});
            CHECK_FALSE(word_container<23>{zero});
            CHECK_FALSE(word_container<256>{zero});
            CHECK_FALSE(word_container<1337>{zero});
            CHECK_FALSE(word_container<8>{zero});
        }
    }

    GIVEN("A non-zero word_container")
    {

        uint64_t val = GENERATE(1, 2, 4, 5567868, 234, 21, 45, 56768);
        word_container<64> a{val};
        word_container<128> b = word_container<128>::from_words(val, 0U);
        word_container<128> c = word_container<128>::from_words(val, val);

        word_container<150> d = word_container<150>::from_words(val, 0U, 0U);
        word_container<256> e = word_container<256>::from_words(val, val, 0U, 0U);

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

SCENARIO("Bit operations are performed correctly", "[word_container][bit_logic]")
{
    GIVEN("An word_container<N> n")
    {
        // TODO Understand how to generate various bit widths
        auto val =
            GENERATE(0, 56567, 23, static_cast<uint64_t>(-4354566), static_cast<uint64_t>(-1));
        const word_container<150> n = word_container<150>::from_words(2 * val, val);
        WHEN("One zero word is prepended")
        {
            const auto prepended_n = width_cast<214>(n);
            THEN("The result should have one additional word")
            {

                CHECK(prepended_n.word_count() == n.word_count() + 1);
            }
            THEN("The prepended word should equal zero and the other values should have been "
                 "copied")
            {
                for (auto i = 0U; i < n.word_count(); ++i)
                {
                    CHECK(prepended_n.word(i) == n.word(i));
                }
                for (auto i = n.word_count(); i < prepended_n.word_count(); ++i)
                {
                    CHECK(prepended_n.word(n.word_count()) == static_cast<uint64_t>(0));
                }
            }
        }
        WHEN("The size is doubled")
        {
            const auto doubled = width_cast<300>(n);
            THEN("The result should have twice the bits of the original word_container")
            {
                CHECK(doubled.width() == 2 * n.width());
            }
            THEN("The result must have twice or twice minus one the number of words")
            {
                const auto two_n_words = doubled.word_count();
                const auto n_words = n.word_count();
                CHECK(((two_n_words == 2 * n_words) || (two_n_words == (2 * n_words) - 1)));
            }
            THEN("The prepended words should equal zero and the other values should have been "
                 "copied")
            {

                for (auto i = 0U; i < n.word_count(); ++i)
                {
                    CHECK(doubled.word(i) == n.word(i));
                }
                for (auto i = n.word_count(); i < doubled.word_count(); ++i)
                {
                    CHECK(doubled.word(n.word_count()) == static_cast<uint64_t>(0));
                }
            }
        }
    }
}


// TODO actually write tests..... :(
//SCENARIO("Splitting a word_container/extraction from a word_container", "[word_container],[util]")
//{
//    GIVEN("A word_container w")
//    {
//
//        const word_container<32> w1{5675676};
//        const word_container<50> w2{5675676};
//        const word_container<7> w3{21};
//
//        WHEN("The container is split")
//        {
//            THEN("The parts are as expected")
//            {
//                std::cout << "w\t" << to_binary(w3) << "\n";
//                auto s3f = split<2>(w3).first;
//                auto s3s = split<2>(w3).second;
//                std::cout << "fst\t" << to_binary(s3f) << "\n";
//                std::cout << "snd\t" << to_binary(s3s) << "\n";
//            }
//        }
//        WHEN("Ranges are extraccted")
//        {
//            THEN("The parts are as expected")
//            {
//                std::cout << "w\t" << to_binary(w3) << "\n";
//
//                auto e3 = bit_range<6, 2>(w3);
//
//                std::cout << to_binary(e3) << "\n";
//            }
//        }
//    }
//}