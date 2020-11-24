#include <catch.hpp>

#include <aarith/core.hpp>
#include <aarith/core/string_utils.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_word_array.hpp"

using namespace aarith;

SCENARIO("Casting word_containers into different width", "[word_array][casting]")
{
    GIVEN("width_cast is called")
    {
        static constexpr uint16_t test_value = 123;
        static constexpr size_t SourceWidth = 16;
        constexpr word_array<SourceWidth> uint{test_value};

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

SCENARIO("Copy constructor of word_containers with various bit widths", "[word_array]")
{
    GIVEN("An word_array<N> a")
    {

        //        const uint64_t val_a = GENERATE(take(10, random(0U,
        //        std::numeric_limits<uint64_t>::max()));
        const uint64_t val_a = 24;
        word_array<196> a(0U, val_a, 0U);

        THEN("Assignment of individual words is correct")
        {
            CHECK(a.word(0) == 0U);
            CHECK(a.word(1) == val_a);
            CHECK(a.word(2) == 0U);
        }

        AND_GIVEN("An word_array<N> b")
        {
            const uint64_t val_b = 1337;
            constexpr word_array<196> b = word_array<196>::from_words(val_b, 0U, 2 * val_b);

            THEN("Assignment operator of individual words is correct")
            {

                a = b;
                CHECK(a.word(0) == 2 * val_b);
                CHECK(a.word(1) == 0U);
                CHECK(a.word(2) == val_b);
            }
        }
        AND_GIVEN("An word_array<M> b")
        {
            WHEN("M < N")
            {
                const uint64_t val_b = 23;
                constexpr word_array<64> tmp = word_array<64>::from_words(val_b);

                THEN("The copy constructor should work")
                {
                    constexpr word_array<128> b{tmp};

                    CHECK(b.word(0) == val_b);
                    CHECK(b.word(1) == 0U);
                }
                THEN("The assignment operator should work")
                {
                    word_array<128> b;
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

SCENARIO("Left shift operator works as expected", "[word_array][bit_logic]")
{
    GIVEN("One word_array a and a number of shifted bits s")
    {

        const size_t TestWidth = 16;

        WHEN("The result still fits the width")
        {
            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 4U;
            constexpr word_array<TestWidth> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 112);
        }
        WHEN("The result does not fit the width anymore")
        {
            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 14U;
            constexpr word_array<TestWidth> a{number_a};

            const auto result = a << s;

            REQUIRE(result.word(0) == 49152);
        }

        WHEN("Some bits are shifted to the next word")
        {
            const size_t Width = 70;

            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 63U;
            constexpr word_array<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0x8000000000000000);
            REQUIRE(result.word(1) == 3);
        }
        WHEN("The bits are shifted to the second next word")
        {
            const size_t Width = 130;

            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 127U;
            constexpr word_array<Width> a{number_a};

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
            constexpr word_array<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 3);
            REQUIRE(result.word(1) == 0);
            REQUIRE(result.word(2) == 0);
        }
        WHEN("The bits are shifted exactly one word")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = static_cast<size_t>(word_array<Width>::word_width());
            constexpr word_array<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == 3);
            REQUIRE(result.word(2) == 0);
        }
        WHEN("The bits are shifted exactly two words")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = 2 * static_cast<size_t>(word_array<Width>::word_width());
            constexpr word_array<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == 0);
            REQUIRE(result.word(2) == 3);
        }
        WHEN("The bits are shifted exactly by word_width-1")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = static_cast<size_t>(word_array<Width>::word_width()) - 1U;
            constexpr word_array<Width> a{number_a};

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
                2U * static_cast<size_t>(word_array<Width>::word_width()) - 1U;
            constexpr word_array<Width> a{number_a};

            auto reference = a.word(0) << (s % (sizeof(word_array<Width>::word_width()) * 8));

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == reference);
            REQUIRE(result.word(2) == 1);
        }
    }
}

SCENARIO("Left shift assignment works as expected", "[word_array][bit_logic]")
{
    GIVEN("One word_array a and a number of shifted bits s")
    {

        const size_t TestWidth = 16;

        WHEN("The result still fits the width")
        {
            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 4U;
            word_array<TestWidth> a{number_a};

            const auto result = a << s;
            a <<= s;
            REQUIRE(result.word(0) == 112);
            REQUIRE(a.word(0) == 112);
            REQUIRE(a == result);
        }
        WHEN("The result does not fit the width anymore")
        {
            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 14U;
            word_array<TestWidth> a{number_a};

            const auto result = a << s;
            a <<= s;

            REQUIRE(result.word(0) == 49152);
            REQUIRE(a.word(0) == 49152);
            REQUIRE(a == result);
        }

        WHEN("Some bits are shifted to the next word")
        {
            const size_t Width = 70;

            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 63U;
            word_array<Width> a{number_a};

            const auto result = a << s;
            a <<= s;
            REQUIRE(result.word(0) == 0x8000000000000000);
            REQUIRE(result.word(1) == 3);
            REQUIRE(a.word(0) == 0x8000000000000000);
            REQUIRE(a.word(1) == 3);
            REQUIRE(a == result);
        }
        WHEN("The bits are shifted to the second next word")
        {
            const size_t Width = 130;

            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 127U;
            word_array<Width> a{number_a};

            const auto result = a << s;
            a <<= s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == 0x8000000000000000);
            REQUIRE(result.word(2) == 3);
            REQUIRE(a.word(0) == 0);
            REQUIRE(a.word(1) == 0x8000000000000000);
            REQUIRE(a.word(2) == 3);
            REQUIRE(a == result);
        }
        WHEN("The bits are not shifted")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = 0;
            word_array<Width> a{number_a};

            const auto result = a << s;
            a <<= s;
            REQUIRE(result.word(0) == 3);
            REQUIRE(result.word(1) == 0);
            REQUIRE(result.word(2) == 0);
            REQUIRE(a.word(0) == 3);
            REQUIRE(a.word(1) == 0);
            REQUIRE(a.word(2) == 0);
            REQUIRE(a == result);
        }
        WHEN("The bits are shifted exactly one word")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = static_cast<size_t>(word_array<Width>::word_width());
            word_array<Width> a{number_a};

            const auto result = a << s;
            a <<= s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == 3);
            REQUIRE(result.word(2) == 0);
            REQUIRE(a.word(0) == 0);
            REQUIRE(a.word(1) == 3);
            REQUIRE(a.word(2) == 0);
            REQUIRE(a == result);
        }
        WHEN("The bits are shifted exactly two words")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = 2 * static_cast<size_t>(word_array<Width>::word_width());
            word_array<Width> a{number_a};

            const auto result = a << s;
            a <<= s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == 0);
            REQUIRE(result.word(2) == 3);
            REQUIRE(a.word(0) == 0);
            REQUIRE(a.word(1) == 0);
            REQUIRE(a.word(2) == 3);
            REQUIRE(a == result);
        }
        WHEN("The bits are shifted exactly by word_width-1")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = static_cast<size_t>(word_array<Width>::word_width()) - 1U;
            word_array<Width> a{number_a};

            auto reference = a.word(0) << s;

            const auto result = a << s;
            a <<= s;
            REQUIRE(result.word(0) == reference);
            REQUIRE(result.word(1) == 1);
            REQUIRE(result.word(2) == 0);
            REQUIRE(a.word(0) == reference);
            REQUIRE(a.word(1) == 1);
            REQUIRE(a.word(2) == 0);
            REQUIRE(a == result);
        }
        WHEN("The bits are assignment shifted by 2*word_width-1")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s =
                2U * static_cast<size_t>(word_array<Width>::word_width()) - 1U;
            word_array<Width> a{number_a};
            auto reference = a.word(0) << (s % (sizeof(word_array<Width>::word_width()) * 8));

            //            const auto skip_words = s / a.word_width();
            //            const auto shift_word_left = s - skip_words * a.word_width();
            //            const auto shift_word_right = a.word_width() - shift_word_left;
            //            std::cout << "s " << s << "\t skip_words " << skip_words << "\t sleft " <<
            //            shift_word_left << "\t sright " << shift_word_right << "\n"; std::cout <<
            //            group_digits(to_binary(a),64) << "\n";

            const auto result = a << s;

            //            std::cout << group_digits(to_binary(result),64) << "\n";

            a <<= s;

            //            std::cout << group_digits(to_binary(a),64) << "\n";
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == reference);
            REQUIRE(result.word(2) == 1);
            REQUIRE(a.word(0) == 0);
            REQUIRE(a.word(1) == reference);
            REQUIRE(a.word(2) == 1);
            REQUIRE(a == result);
        }
    }
}

SCENARIO("Right shift operator works as expected", "[word_array][utility][bit_logic]")
{
    GIVEN("One word_array a and a number of shifted bits s")
    {
        WHEN("The bits are not shifted")
        {
            const size_t Width = 192;

            typename word_array<Width>::word_type number_a = 3;
            number_a <<= word_array<Width>::word_width() - 2;
            static constexpr auto s = 0;
            word_array<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            const auto result = a >> s;
            REQUIRE(result.word(a.word_count() - 3) == 0);
            REQUIRE(result.word(a.word_count() - 2) == 0);
            REQUIRE(result.word(a.word_count() - 1) == number_a);
        }
        WHEN("The bits are shifted exactly one word")
        {
            const size_t Width = 192;

            typename word_array<Width>::word_type number_a = 3;
            number_a <<= word_array<Width>::word_width() - 2;
            static constexpr auto s = static_cast<size_t>(word_array<Width>::word_width());
            word_array<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            const auto result = a >> s;
            REQUIRE(result.word(a.word_count() - 3) == 0);
            REQUIRE(result.word(a.word_count() - 2) == number_a);
            REQUIRE(result.word(a.word_count() - 1) == 0);
        }
        WHEN("The bits are shifted exactly two words")
        {
            const size_t Width = 192;

            typename word_array<Width>::word_type number_a = 3;
            number_a <<= word_array<Width>::word_width() - 2;
            static constexpr auto s = 2 * static_cast<size_t>(word_array<Width>::word_width());
            word_array<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            const auto result = a >> s;
            REQUIRE(result.word(a.word_count() - 3) == number_a);
            REQUIRE(result.word(a.word_count() - 2) == 0);
            REQUIRE(result.word(a.word_count() - 1) == 0);
        }
        WHEN("The bits are shifted exactly by word_width-1")
        {
            const size_t Width = 192;

            typename word_array<Width>::word_type number_a = 3;
            number_a <<= word_array<Width>::word_width() - 2;
            static constexpr auto s = static_cast<size_t>(word_array<Width>::word_width()) - 1;
            word_array<Width> a(0U);
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

            typename word_array<Width>::word_type number_a = 3;
            number_a <<= word_array<Width>::word_width() - 2;
            static constexpr auto s = 2 * static_cast<size_t>(word_array<Width>::word_width()) - 1;
            word_array<Width> a(0U);
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
                static constexpr size_t word_width = word_array<width>::word_width();

                static constexpr word_array<width> a =
                    word_array<width>::from_words(1U, 0U, 0U, 0U);
                static constexpr word_array<width> expected0 =
                    word_array<width>::from_words(1U, 0U, 0U, 0U);
                static constexpr word_array<width> expected1 =
                    word_array<width>::from_words(0U, 1U, 0U, 0U);
                static constexpr word_array<width> expected2 =
                    word_array<width>::from_words(0U, 0U, 1U, 0U);
                static constexpr word_array<width> expected3 =
                    word_array<width>::from_words(0U, 0U, 0U, 1U);
                static constexpr word_array<width> expected4 =
                    word_array<width>::from_words(0U, 0U, 0U, 0U);

                std::vector<word_array<width>> expecteds{expected0, expected1, expected2, expected3,
                                                         expected4};

                for (auto i = 0U; i < expecteds.size(); ++i)
                {
                    word_array<width> result = a >> (word_width * i);
                    CHECK(result == expecteds[i]);
                }
            }
        }

        AND_WHEN("The shift amount is the integer width")
        {
            THEN("The result should still be correct")
            {
                constexpr size_t w = 128;
                constexpr word_array<w> a{1U};
                constexpr word_array<w> expected;
                const word_array<w> result = a << w;

                CHECK(expected == result);
            }
        }
    }
}

SCENARIO("Right shift assignment operator works as expected", "[word_array][utility][bit_logic")
{
    GIVEN("One word_array a and a number of shifted bits s")
    {
        WHEN("The bits are not shifted")
        {
            const size_t Width = 192;

            typename word_array<Width>::word_type number_a = 3;
            number_a <<= word_array<Width>::word_width() - 2;
            static constexpr auto s = 0;
            word_array<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            const auto result = a >> s;
            a >>= s;
            REQUIRE(result.word(a.word_count() - 3) == 0);
            REQUIRE(result.word(a.word_count() - 2) == 0);
            REQUIRE(result.word(a.word_count() - 1) == number_a);
            REQUIRE(a.word(a.word_count() - 3) == 0);
            REQUIRE(a.word(a.word_count() - 2) == 0);
            REQUIRE(a.word(a.word_count() - 1) == number_a);
            REQUIRE(a == result);
        }
        WHEN("The bits are shifted exactly one word")
        {
            const size_t Width = 192;

            typename word_array<Width>::word_type number_a = 3;
            number_a <<= word_array<Width>::word_width() - 2;
            static constexpr auto s = static_cast<size_t>(word_array<Width>::word_width());
            word_array<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            const auto result = a >> s;
            a >>= s;
            REQUIRE(result.word(a.word_count() - 3) == 0);
            REQUIRE(result.word(a.word_count() - 2) == number_a);
            REQUIRE(result.word(a.word_count() - 1) == 0);
            REQUIRE(result.word(a.word_count() - 3) == 0);
            REQUIRE(result.word(a.word_count() - 2) == number_a);
            REQUIRE(result.word(a.word_count() - 1) == 0);
            REQUIRE(a == result);
        }
        WHEN("The bits are shifted exactly two words")
        {
            const size_t Width = 192;

            typename word_array<Width>::word_type number_a = 3;
            number_a <<= word_array<Width>::word_width() - 2;
            static constexpr auto s = 2 * static_cast<size_t>(word_array<Width>::word_width());
            word_array<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            const auto result = a >> s;
            a >>= s;
            REQUIRE(result.word(a.word_count() - 3) == number_a);
            REQUIRE(result.word(a.word_count() - 2) == 0);
            REQUIRE(result.word(a.word_count() - 1) == 0);
            REQUIRE(result.word(a.word_count() - 3) == number_a);
            REQUIRE(result.word(a.word_count() - 2) == 0);
            REQUIRE(result.word(a.word_count() - 1) == 0);
            REQUIRE(a == result);
        }
        WHEN("The bits are shifted exactly by word_width-1")
        {
            const size_t Width = 192;

            typename word_array<Width>::word_type number_a = 3;
            number_a <<= word_array<Width>::word_width() - 2;
            static constexpr auto s = static_cast<size_t>(word_array<Width>::word_width()) - 1;
            word_array<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            auto ref = number_a << 1;

            const auto result = a >> s;
            a >>= s;

            REQUIRE(result.word(a.word_count() - 3) == 0);
            REQUIRE(result.word(a.word_count() - 2) == ref);
            REQUIRE(result.word(a.word_count() - 1) == 1);
            REQUIRE(result.word(a.word_count() - 3) == 0);
            REQUIRE(result.word(a.word_count() - 2) == ref);
            REQUIRE(result.word(a.word_count() - 1) == 1);
            REQUIRE(a == result);
        }
        WHEN("The bits are shifted by 2*word_width-1")
        {
            const size_t Width = 192;

            typename word_array<Width>::word_type number_a = 3;
            number_a <<= word_array<Width>::word_width() - 2;
            static constexpr auto s = 2 * static_cast<size_t>(word_array<Width>::word_width()) - 1;
            word_array<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            auto ref = number_a << 1;

            const auto result = a >> s;
            a >>= s;
            REQUIRE(result.word(a.word_count() - 3) == ref);
            REQUIRE(result.word(a.word_count() - 2) == 1);
            REQUIRE(result.word(a.word_count() - 1) == 0);
            REQUIRE(result.word(a.word_count() - 3) == ref);
            REQUIRE(result.word(a.word_count() - 2) == 1);
            REQUIRE(result.word(a.word_count() - 1) == 0);
            REQUIRE(a == result);
        }
        WHEN("The shift amount is a multiple of the word width")
        {

            THEN("The result should still be correct")
            {
                static constexpr size_t width = 256;
                static constexpr size_t word_width = word_array<width>::word_width();

                word_array<width> a = word_array<width>::from_words(1U, 0U, 0U, 0U);
                ;
                static constexpr word_array<width> expected1 =
                    word_array<width>::from_words(0U, 1U, 0U, 0U);
                static constexpr word_array<width> expected2 =
                    word_array<width>::from_words(0U, 0U, 1U, 0U);
                static constexpr word_array<width> expected3 =
                    word_array<width>::from_words(0U, 0U, 0U, 1U);
                static constexpr word_array<width> expected4 =
                    word_array<width>::from_words(0U, 0U, 0U, 0U);

                std::vector<word_array<width>> expecteds{expected1, expected2, expected3,
                                                         expected4};

                for (auto i = 0U; i < expecteds.size(); ++i)
                {
                    a >>= word_width;
                    CHECK(a == expecteds[i]);
                }
            }
        }

        AND_WHEN("The shift amount is the integer width")
        {
            THEN("The result should still be correct")
            {
                constexpr size_t w = 128;
                word_array<w> a{1U};
                constexpr word_array<w> expected;
                const word_array<w> result = a >> w;

                a >>= w;
                CHECK(expected == result);
                REQUIRE(a == result);
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Bitwise operations work as expected", "[word_array][bit_logic]",
                       AARITH_INT_TEST_SIGNATURE, AARITH_WORD_ARRAY_TEST_TEMPLATE_PARAM_RANGE)
{
    GIVEN("An word_array a")
    {

        using I = word_array<W, WordType>;
        I a = GENERATE(take(10, random_word_array<W, WordType>()));

        WHEN("Negating the word container")
        {

            THEN("The result should match the NOT of the underlying WordType")
            {
                const auto result = ~a;
                for (size_t i = 0; i < a.word_count(); ++i)
                {
                    REQUIRE(result.word(i) == (~a.word(i) & a.word_mask(i)));
                }
            }
        }

        AND_GIVEN("Another word container b")
        {

            I b = GENERATE(take(10, random_word_array<W, WordType>()));
            THEN("Bitwise AND should work as expected")
            {
                const auto result = a & b;
                for (size_t i = 0; i < a.word_count(); ++i)
                {
                    REQUIRE(result.word(i) == ((a.word(i) & b.word(i)) & a.word_mask(i)));
                }
            }

            AND_THEN("Bitwise OR should work as expected")
            {
                const auto result = a | b;
                for (size_t i = 0; i < a.word_count(); ++i)
                {
                    REQUIRE(result.word(i) == ((a.word(i) | b.word(i)) & a.word_mask(i)));
                }
            }
            AND_THEN("Bitwise XOR should work as expected")
            {
                const auto result = a ^ b;
                for (size_t i = 0; i < a.word_count(); ++i)
                {
                    REQUIRE(result.word(i) == ((a.word(i) ^ b.word(i)) & a.word_mask(i)));
                }
            }
        }
    }
}

SCENARIO("Bit shifting is possible as constexpr", "[word_array][utility][bit_logic][constexpr]")
{
    GIVEN("A word_array of width N")
    {
        THEN("Right-shifting should be a constexpr operation")
        {
            constexpr size_t W = 32;
            constexpr word_array<W> num{4};

            constexpr word_array<W> shift1{2};
            constexpr word_array<W> shift2{1};
            constexpr word_array<W> shift3{0};

            constexpr word_array<W> shifted1 = (num >> 1);
            constexpr word_array<W> shifted2 = (num >> 2);
            constexpr word_array<W> shifted3 = (num >> 3);

            REQUIRE(shift1 == shifted1);
            REQUIRE(shift2 == shifted2);
            REQUIRE(shift3 == shifted3);
        }
        THEN("Left-shifting should be a constexpr operation")
        {
            constexpr size_t W = 32;
            constexpr word_array<W> num{4};

            constexpr word_array<W> shift1{8};
            constexpr word_array<W> shift2{16};
            constexpr word_array<W> shift3{32};

            constexpr word_array<W> shifted1 = (num << 1);
            constexpr word_array<W> shifted2 = (num << 2);
            constexpr word_array<W> shifted3 = (num << 3);

            REQUIRE(shift1 == shifted1);
            REQUIRE(shift2 == shifted2);
            REQUIRE(shift3 == shifted3);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Bit operations are performed correctly", "[word_array][bit_logic]",
                       AARITH_INT_TEST_SIGNATURE, AARITH_WORD_ARRAY_TEST_TEMPLATE_PARAM_RANGE)
{
    GIVEN("An word_array<N> w")
    {

        using I = word_array<W, WordType>;
        I w = GENERATE(take(10, random_word_array<W, WordType>()));

        WHEN("One zero word is prepended")
        {
            const auto prepended_w = width_cast<W + I::word_width()>(w);
            THEN("The result should have one additional word")
            {
                CHECK(prepended_w.word_count() == w.word_count() + 1);
            }
            THEN("The prepended word should equal zero and the other values should have been "
                 "copied")
            {
                for (auto i = 0U; i < w.word_count(); ++i)
                {
                    CHECK(prepended_w.word(i) == w.word(i));
                }
                if constexpr(prepended_w.word_count() > w.word_count())
                {
                    CHECK(prepended_w.word(w.word_count()) == static_cast<WordType>(0U));
                }
            }
        }
        WHEN("The size is doubled")
        {
            //const auto doubled = width_cast<2 * W>(w);
            auto doubled = width_cast<2 * W>(w);
            THEN("The result should have twice the bits of the original word_array")
            {
                CHECK(doubled.width() == 2 * w.width());
            }
            THEN("The result must have twice or twice minus one the number of words")
            {
                const auto two_n_words = doubled.word_count();
                const auto n_words = w.word_count();
                CHECK(((two_n_words == 2 * n_words) || (two_n_words == (2 * n_words) - 1)));
            }
            THEN("The prepended words should equal zero and the other values should have been "
                 "copied")
            {

                for (auto i = 0U; i < w.word_count(); ++i)
                {
                    CHECK(doubled.word(i) == w.word(i));
                }
                if constexpr(doubled.word_count() > w.word_count())
                {
                    CHECK(doubled.word(w.word_count()) == static_cast<WordType>(0U));
                }
            }
        }
    }
}
