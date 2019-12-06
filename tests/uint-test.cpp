#include <aarith/operations/comparisons.hpp>
#include <aarith/types/integer.hpp>
#include <aarith/utilities/string_utils.hpp>
#include <catch.hpp>
#include <sstream>
#include <iostream>

using namespace aarith;

SCENARIO("Creating uintegers using the from_words method")
{
    WHEN("The uinteger does not have a bit width as a multiple of the word width")
    {
        THEN("The constructed uinteger should be correct")
        {
            const uint64_t uzero = 0U;
            const uint64_t uones = ~uzero;




            const uinteger<89> from_word=uinteger<89>::from_words(uzero,uones);
            uinteger<89> manually;
            manually.set_word(0,uones);
            manually.set_word(1,uzero);

            CHECK(from_word == manually);
            CHECK(from_word.word(0) == uones);
            CHECK(from_word.word(1) == uzero);
            CHECK(manually.word(0) == uones);
            REQUIRE(manually.word(1) == uzero);
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

 SCENARIO("Copy constructor of uintegers with various bit widths", "[uinteger][utility]")
{
    GIVEN("An uinteger<N> a")
    {

        //        const uint64_t val_a = GENERATE(take(10, random(0U,
        //        std::numeric_limits<uint64_t>::max()));
        const uint64_t val_a = 24;
        uinteger<196> a = uinteger<196>::from_words(0U, val_a, 0U);

        THEN("Assignment of individual words is correct")
        {
            CHECK(a.word(0) == 0U);
            CHECK(a.word(1) == val_a);
            CHECK(a.word(2) == 0U);
        }

        AND_GIVEN("An uinteger<N> b")
        {
            const uint64_t val_b = 1337;
            const uinteger<196> b = uinteger<196>::from_words(val_b, 0U, 2 * val_b);

            THEN("Assignment opeator of individual words is correct")
            {

                a = b;
                CHECK(a.word(0) == 2 * val_b);
                CHECK(a.word(1) == 0U);
                CHECK(a.word(2) == val_b);
            }
        }
        AND_GIVEN("An uinteger<M> b")
        {
            WHEN("M < N")
            {
                const uint64_t val_b = 23;
                const uinteger<64> tmp = uinteger<64>::from_words(val_b);

                THEN("The copy constructor should work")
                {
                    uinteger<128> b{tmp};

                    CHECK(b.word(0) == val_b);
                    CHECK(b.word(1) == 0U);
                }
                THEN("The assignment operator should work")
                {
                    uinteger<128> b;
                    b = tmp;
                    CHECK(b.word(0) == val_b);
                    CHECK(b.word(1) == 0U);

                    a = tmp;
                    CHECK(a.word(0) == val_b);
                    CHECK(a.word(1) == 0U);
                    CHECK(a.word(2) == 0U);
                }
            }
            //        // does not compile, as expected and intended
            //        //        WHEN("M > N")
            //        //        {
            //        //            const uint64_t val_b = 23;
            //        //            const uinteger<150> tmp =
            //        uinteger<150>::from_words(0U,val_b,0U);
            //        //            std::cout << tmp << "\n";
            //        //            const uinteger<128> b = tmp;
            //        //            std::cout << b << "\n";
            //        //        }
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
        WHEN("The bits are not shifted")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = 0;
            const uinteger<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 3);
            REQUIRE(result.word(1) == 0);
            REQUIRE(result.word(2) == 0);
        }
        WHEN("The bits are shifted exactly one word")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = static_cast<size_t>(uinteger<Width>::word_width());
            const uinteger<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == 3);
            REQUIRE(result.word(2) == 0);
        }
        WHEN("The bits are shifted exactly two words")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = 2 * static_cast<size_t>(uinteger<Width>::word_width());
            const uinteger<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == 0);
            REQUIRE(result.word(2) == 3);
        }
        WHEN("The bits are shifted exactly by word_width-1")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = static_cast<size_t>(uinteger<Width>::word_width()) - 1U;
            const uinteger<Width> a{number_a};

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
            static constexpr auto s = 2U * static_cast<size_t>(uinteger<Width>::word_width()) -
            1U; const uinteger<Width> a{number_a};

            auto reference = a.word(0) << (s % (sizeof(uinteger<Width>::word_width()) * 8));

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == reference);
            REQUIRE(result.word(2) == 1);
        }
    }
}

 SCENARIO("Right shift operator works as expected", "[uinteger][utility]")
{
    GIVEN("One uinteger a and a number of shifted bits s")
    {
        WHEN("The bits are not shifted")
        {
            const size_t Width = 192;

            typename uinteger<Width>::word_type number_a = 3;
            number_a <<= uinteger<Width>::word_width() - 2;
            static constexpr auto s = 0;
            uinteger<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            const auto result = a >> s;
            REQUIRE(result.word(a.word_count() - 3) == 0);
            REQUIRE(result.word(a.word_count() - 2) == 0);
            REQUIRE(result.word(a.word_count() - 1) == number_a);
        }
        WHEN("The bits are shifted exactly one word")
        {
            const size_t Width = 192;

            typename uinteger<Width>::word_type number_a = 3;
            number_a <<= uinteger<Width>::word_width() - 2;
            static constexpr auto s = static_cast<size_t>(uinteger<Width>::word_width());
            uinteger<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            const auto result = a >> s;
            REQUIRE(result.word(a.word_count() - 3) == 0);
            REQUIRE(result.word(a.word_count() - 2) == number_a);
            REQUIRE(result.word(a.word_count() - 1) == 0);
        }
        WHEN("The bits are shifted exactly two words")
        {
            const size_t Width = 192;

            typename uinteger<Width>::word_type number_a = 3;
            number_a <<= uinteger<Width>::word_width() - 2;
            static constexpr auto s = 2 * static_cast<size_t>(uinteger<Width>::word_width());
            uinteger<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            const auto result = a >> s;
            REQUIRE(result.word(a.word_count() - 3) == number_a);
            REQUIRE(result.word(a.word_count() - 2) == 0);
            REQUIRE(result.word(a.word_count() - 1) == 0);
        }
        WHEN("The bits are shifted exactly by word_width-1")
        {
            const size_t Width = 192;

            typename uinteger<Width>::word_type number_a = 3;
            number_a <<= uinteger<Width>::word_width() - 2;
            static constexpr auto s = static_cast<size_t>(uinteger<Width>::word_width()) - 1;
            uinteger<Width> a(0U);
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

            typename uinteger<Width>::word_type number_a = 3;
            number_a <<= uinteger<Width>::word_width() - 2;
            static constexpr auto s = 2 * static_cast<size_t>(uinteger<Width>::word_width()) - 1;
            uinteger<Width> a(0U);
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
                static constexpr size_t word_width = uinteger<width>::word_width();

                static const uinteger<width> a{1U};
                static const uinteger<width> expected0 =
                    uinteger<width>::from_words(0U, 0U, 0U, 1U);
                static const uinteger<width> expected1 =
                    uinteger<width>::from_words(0U, 0U, 1U, 0U);
                static const uinteger<width> expected2 =
                    uinteger<width>::from_words(0U, 1U, 0U, 0U);
                static const uinteger<width> expected3 =
                    uinteger<width>::from_words(1U, 0U, 0U, 0U);
                static const uinteger<width> expected4 =
                    uinteger<width>::from_words(0U, 0U, 0U, 0U);

                std::vector<uinteger<width>> expecteds{expected0, expected1, expected2, expected3,
                                                       expected4};

                for (auto i = 0U; i < expecteds.size(); ++i)
                {
                    uinteger<width> result = a << (word_width * i);
                    CHECK(result == expecteds[i]);
                }
            }
        }

        AND_WHEN("The shift amount is the integer width")
        {
            THEN("The result should still be correct")
            {
                const size_t w = 128;
                const uinteger<w> a{1U};
                const uinteger<w> expected;
                const uinteger<w> result = a << w;

                CHECK(expected == result);
            }
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

        uint64_t val = GENERATE(1, 2, 4, 5567868, 234, 21, 45, 56768);
        uinteger<64> a{val};
        uinteger<128> b = uinteger<128>::from_words(val, 0U);
        uinteger<128> c = uinteger<128>::from_words(val, val);

        uinteger<150> d = uinteger<150>::from_words(val, 0U, 0U);
        uinteger<256> e = uinteger<256>::from_words(val, val, 0U, 0U);

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
            uint64_t val_a = GENERATE(0, 1, 2, 3);
            uint64_t val_b = GENERATE(3, 56, 567, 324);

            uinteger<64> a{val_a};
            uinteger<128> b = uinteger<128>::from_words(val_a, val_b);

            size_t index = 0;
            for (const uinteger<64>::word_type w : a)
            {
                CHECK(w == a.word(index));
                index++;
            }

            CHECK(index == 1);

            index = 0;

            for (const uinteger<128>::word_type w : b)
            {
                CHECK(w == b.word(index));
                index++;
            }

            CHECK(index == 2);
        }
        THEN("Using forward iterators explicitly should give the words as expected")
        {
            uint64_t val_a = GENERATE(0, 1, 2, 3);
            uint64_t val_b = GENERATE(3, 56, 567, 324);

            uinteger<64> a{val_a};
            uinteger<128> b = uinteger<128>::from_words(val_a, val_b);

            size_t index = 0;
            for (auto iter = a.begin(); iter != a.end(); ++iter)
            {
                CHECK(*iter == a.word(index));
                index++;
            }

            CHECK(index == 1);

            index = 0;

            for (auto iter = b.begin(); iter != b.end(); ++iter)
            {
                CHECK(*iter == b.word(index));
                index++;
            }

            CHECK(index == 2);
        }

        THEN("Using backwards iterators explicitly should give the words as expected")
        {
            uint64_t val_a = GENERATE(0, 1, 2, 3);
            uint64_t val_b = GENERATE(3, 56, 567, 324);

            uinteger<64> a{val_a};
            uinteger<128> b = uinteger<128>::from_words(val_a, val_b);

            size_t index = 0;
            for (auto iter = a.rbegin(); iter != a.rend(); ++iter)
            {
                CHECK(*iter == a.word((a.word_count() - 1) - index));
                index++;
            }

            CHECK(index == 1);

            index = 0;

            for (auto iter = b.rbegin(); iter != b.rend(); ++iter)
            {
                CHECK(*iter == b.word((b.word_count() - 1) - index));
                index++;
            }

            CHECK(index == 2);
        }
    }
}

 SCENARIO("Bit operations are performed correctly", "[uinteger][bit]")
{
    GIVEN("An uinteger<N> n")
    {
        // TODO Understand how to generate various bit widths
        auto val =
            GENERATE(0, 56567, 23, static_cast<uint64_t>(-4354566), static_cast<uint64_t>(-1));
        const uinteger<150> n = uinteger<150>::from_words(2 * val, val);
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
        WHEN("The size is oubled")
        {
            const auto doubled = width_cast<300>(n);
            THEN("The result should have twice the bits of the original uinteger")
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

//// for static_assert tests:
//// https://stackoverflow.com/questions/30155619/expected-build-failure-tests-in-cmake
