#include <aarith/integer.hpp>
#include <bitset>
#include <catch.hpp>
#include <sstream>

using namespace aarith;

SCENARIO("Casting sintegers into different width", "[sinteger]")
{
    GIVEN("width_cast is called")
    {
        static constexpr uint16_t test_value = 123;
        static constexpr size_t SourceWidth = 16;
        const sinteger<SourceWidth> uint{test_value};

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

SCENARIO("Copy constructor of sintegers with various bit widths", "[sinteger][utility]")
{
    GIVEN("An sinteger<N> a")
    {
        const uint64_t val_a = GENERATE(
            take(10, random(static_cast<uint64_t>(0U), std::numeric_limits<uint64_t>::max())));
        sinteger<196> a(0U, val_a, 0U);

        THEN("Assignment of individual words is correct")
        {
            CHECK(a.word(0) == 0U);
            CHECK(a.word(1) == val_a);
            CHECK(a.word(2) == 0U);
        }

        AND_GIVEN("An sinteger<N> b")
        {
            const uint64_t val_b = 1337;
            const sinteger<196> b = sinteger<196>::from_words(val_b, 0U, 2 * val_b);

            THEN("Assignment opeator of individual words is correct")
            {

                a = b;
                CHECK(a.word(0) == 2 * val_b);
                CHECK(a.word(1) == 0U);
                CHECK(a.word(2) == val_b);
            }
        }
        AND_GIVEN("An sinteger<M> b")
        {
            WHEN("M < N")
            {
                const uint64_t val_b = 23;
                const sinteger<64> tmp = sinteger<64>::from_words(val_b);

                THEN("The copy constructor should work")
                {
                    sinteger<128> b{tmp};

                    CHECK(b.word(0) == val_b);
                    CHECK(b.word(1) == 0U);
                }
                THEN("The assignment operator should work")
                {
                    sinteger<128> b;
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
            //        //            const sinteger<150> tmp =
            //        sinteger<150>::from_words(0U,val_b,0U);
            //        //            std::cout << tmp << "\n";
            //        //            const sinteger<128> b = tmp;
            //        //            std::cout << b << "\n";
            //        //        }
        }
    }
}

SCENARIO("Left shift operator works as expected", "[sinteger][bit_logic]")
{
    GIVEN("One sinteger a and a number of shifted bits s")
    {

        const size_t TestWidth = 16;

        WHEN("The result still fits the width")
        {
            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 4U;
            const sinteger<TestWidth> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 112);
        }
        WHEN("The result does not fit the width anymore")
        {
            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 14U;
            const sinteger<TestWidth> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 49152);
        }

        WHEN("Some bits are shifted to the next word")
        {
            const size_t Width = 70;

            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 63U;
            const sinteger<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0x8000000000000000);
            REQUIRE(result.word(1) == 3);
        }
        WHEN("The bits are shifted to the second next word")
        {
            const size_t Width = 130;

            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 127U;
            const sinteger<Width> a{number_a};

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
            const sinteger<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 3);
            REQUIRE(result.word(1) == 0);
            REQUIRE(result.word(2) == 0);
        }
        WHEN("The bits are shifted exactly one word")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = static_cast<size_t>(sinteger<Width>::word_width());
            const sinteger<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == 3);
            REQUIRE(result.word(2) == 0);
        }
        WHEN("The bits are shifted exactly two words")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = 2 * static_cast<size_t>(sinteger<Width>::word_width());
            const sinteger<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == 0);
            REQUIRE(result.word(2) == 3);
        }
        WHEN("The bits are shifted exactly by word_width-1")
        {
            const size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = static_cast<size_t>(sinteger<Width>::word_width()) - 1U;
            const sinteger<Width> a{number_a};

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
            static constexpr auto s = 2U * static_cast<size_t>(sinteger<Width>::word_width()) - 1U;
            const sinteger<Width> a{number_a};

            auto reference = a.word(0) << (s % (sizeof(sinteger<Width>::word_width()) * 8));

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == reference);
            REQUIRE(result.word(2) == 1);
        }
    }
}

SCENARIO("Create negative sintegers", "[sinteger][bit_logic]")
{
    GIVEN("A int64_t negative number")
    {
        int64_t n = GENERATE(take(100, random(-922337236854775808, -1L)));
        int64_t pos_n = -n;

        sinteger<64> zero;

        WHEN("We create a signed integer from it")
        {

            sinteger<64> negative{n};
            sinteger<64> positive{pos_n};

            THEN("Then it behaves as expected")
            {
                CHECK(add(negative, positive) == zero);
            }

            THEN("Negation works as expected")
            {
                CHECK(-negative == positive);
                CHECK(-(-negative) == negative);
                CHECK(-(-positive) == positive);
            }
            THEN("Computing absolute value works as expected")
            {
                CHECK(abs(negative) == positive);
                CHECK(abs(negative) == -negative);
                CHECK(abs(abs(negative)) == positive);
                CHECK(abs(positive) == positive);
            }
        }
    }
}

 SCENARIO("Right shift operator works as expected", "[sinteger][bit-logic]")
{
    GIVEN("One positive sinteger a and a number of shifted bits s")
    {
        WHEN("The bits are not shifted")
        {
            const size_t Width = 192;

            typename sinteger<Width>::word_type number_a = 3;
            number_a <<= (sinteger<Width>::word_width() - 2);
            static constexpr auto s = 0;
            sinteger<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            const auto result = a >> s;
            REQUIRE(result.word(a.word_count() - 3) == 0);
            REQUIRE(result.word(a.word_count() - 2) == 0);
            REQUIRE(result.word(a.word_count() - 1) == number_a);
        }

        // This test will fail spectacularly for negative numbers!
        WHEN("The bits are shifted exactly one word")
        {
            const size_t Width = 192;

            typename sinteger<Width>::word_type number_a = 3;
            number_a <<= (sinteger<Width>::word_width() - 4);


            static constexpr auto s = static_cast<size_t>(sinteger<Width>::word_width());
            sinteger<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);


            const auto result = a >> s;


            CHECK(result.word(a.word_count() - 3) == 0);
            CHECK(result.word(a.word_count() - 2) == number_a);
            REQUIRE(result.word(a.word_count() - 1) == 0);
        }

        WHEN("The bits are shifted exactly two words")
        {
            const size_t Width = 192;

            typename sinteger<Width>::word_type number_a = 3;
            number_a <<= sinteger<Width>::word_width() - 4;
            static constexpr auto s = 2 * static_cast<size_t>(sinteger<Width>::word_width());
            sinteger<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            const auto result = a >> s;
            REQUIRE(result.word(a.word_count() - 3) == number_a);
            REQUIRE(result.word(a.word_count() - 2) == 0);
            REQUIRE(result.word(a.word_count() - 1) == 0);
        }


        WHEN("The bits are shifted exactly by word_width-2 over word boundaries")
        {
            const size_t Width = 192;

            typename sinteger<Width>::word_type number_a = 3;
            number_a <<= sinteger<Width>::word_width() - 3;
            static constexpr auto s = static_cast<size_t>(sinteger<Width>::word_width()) - 2;
            sinteger<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            typename sinteger<Width>::word_type ref = uint64_t(1) << 63U;

            const auto result = a >> s;

            REQUIRE(result.word(a.word_count() - 3) == 0);
            REQUIRE(result.word(a.word_count() - 2) == ref);
            REQUIRE(result.word(a.word_count() - 1) == 1);
        }


        WHEN("The bits are shifted by 2*word_width-1")
        {
            const size_t Width = 192;

            typename sinteger<Width>::word_type number_a = 3;
            number_a <<= sinteger<Width>::word_width() - 3;

            static constexpr auto s = 2 * static_cast<size_t>(sinteger<Width>::word_width()) - 1;
            sinteger<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            auto ref = number_a << 1;

            const auto result = a >> s;

            REQUIRE(result.word(a.word_count() - 3) == ref);
            REQUIRE(result.word(a.word_count() - 2) == 0);
            REQUIRE(result.word(a.word_count() - 1) == 0);
        }


        WHEN("The shift amount is a multiple of the word width")
        {

            THEN("The result should still be correct")
            {
                static constexpr size_t width = 256;
                static constexpr size_t word_width = sinteger<width>::word_width();

                static const sinteger<width> a{1U};
                static const sinteger<width> expected0 =
                    sinteger<width>::from_words(0U, 0U, 0U, 1U);
                static const sinteger<width> expected1 =
                    sinteger<width>::from_words(0U, 0U, 1U, 0U);
                static const sinteger<width> expected2 =
                    sinteger<width>::from_words(0U, 1U, 0U, 0U);
                static const sinteger<width> expected3 =
                    sinteger<width>::from_words(1U, 0U, 0U, 0U);
                static const sinteger<width> expected4 =
                    sinteger<width>::from_words(0U, 0U, 0U, 0U);

                std::vector<sinteger<width>> expecteds{expected0, expected1, expected2, expected3,
                                                       expected4};

                for (auto i = 0U; i < expecteds.size(); ++i)
                {
                    sinteger<width> result = a << (word_width * i);
                    CHECK(result == expecteds[i]);
                }
            }
        }

        AND_WHEN("The shift amount is the integer width")
        {
            THEN("The result should still be correct")
            {
                const size_t w = 128;
                const sinteger<w> a{1U};
                const sinteger<w> expected;
                const sinteger<w> result = a >> w;

                CHECK(expected == result);
            }
        }
    }
}

SCENARIO("Logical AND works as expected", "[sinteger][arithmetic]")
{
    GIVEN("Two sintegers")
    {
        WHEN("The sintegers consists of only one word")
        {
            const size_t Width = 70;

            static constexpr uint16_t number_a = 7;
            static constexpr uint16_t number_b = 14;
            const sinteger<Width> a{number_a};
            const sinteger<Width> b{number_b};

            const auto result = a & b;
            const auto result_ref = number_a & number_b;
            REQUIRE(result.word(0) == result_ref);
        }
    }
}

SCENARIO("Logical OR works as expected", "[sinteger][arithmetic]")
{
    GIVEN("Two sintegers")
    {
        WHEN("The sintegers consists of only one word")
        {
            const size_t Width = 70;

            static constexpr uint16_t number_a = 7;
            static constexpr uint16_t number_b = 14;
            const sinteger<Width> a{number_a};
            const sinteger<Width> b{number_b};

            const auto result = a | b;
            const auto result_ref = number_a | number_b;
            REQUIRE(result.word(0) == result_ref);
        }
    }
}

SCENARIO("Logical NOT works as expected", "[sinteger][arithmetic]")
{
    GIVEN("One sintegers")
    {
        WHEN("The sinteger consists of only one word")
        {
            const size_t Width = 70;

            static constexpr uint16_t number_a = 7;
            const sinteger<Width> a{number_a};

            const auto result = ~a;
            const auto result_ref = ~number_a;
            REQUIRE(result.word(0) == result_ref);
        }
    }
}

SCENARIO("Checking whether an sinteger is not equal to zero/false")
{
    GIVEN("An sinteger<N>=0=a for various N")
    {
        THEN("a.is_zero() should be true")
        {

            const uint8_t zero = 0U;

            CHECK(sinteger<64>{zero}.is_zero());
            CHECK(sinteger<128>{zero}.is_zero());
            CHECK(sinteger<150>{zero}.is_zero());
            CHECK(sinteger<32>{zero}.is_zero());
            CHECK(sinteger<23>{zero}.is_zero());
            CHECK(sinteger<256>{zero}.is_zero());
            CHECK(sinteger<1337>{zero}.is_zero());
            CHECK(sinteger<8>{zero}.is_zero());
        }
        THEN("a should evaluate to false in a Boolean context")
        {

            const uint8_t zero = 0U;

            CHECK_FALSE(sinteger<64>{zero});
            CHECK_FALSE(sinteger<128>{zero});
            CHECK_FALSE(sinteger<150>{zero});
            CHECK_FALSE(sinteger<32>{zero});
            CHECK_FALSE(sinteger<23>{zero});
            CHECK_FALSE(sinteger<256>{zero});
            CHECK_FALSE(sinteger<1337>{zero});
            CHECK_FALSE(sinteger<8>{zero});
        }
    }

    GIVEN("A non-zero sinteger")
    {

        uint64_t val = GENERATE(1, 2, 4, 5567868, 234, 21, 45, 56768);
        sinteger<64> a{val};
        sinteger<128> b = sinteger<128>::from_words(val, 0U);
        sinteger<128> c = sinteger<128>::from_words(val, val);

        sinteger<150> d = sinteger<150>::from_words(val, 0U, 0U);
        sinteger<256> e = sinteger<256>::from_words(val, val, 0U, 0U);

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

            sinteger<64> a{val_a};
            sinteger<128> b = sinteger<128>::from_words(val_a, val_b);

            size_t index = 0;
            for (const sinteger<64>::word_type w : a)
            {
                CHECK(w == a.word(index));
                index++;
            }

            CHECK(index == 1);

            index = 0;

            for (const sinteger<128>::word_type w : b)
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

            sinteger<64> a{val_a};
            sinteger<128> b = sinteger<128>::from_words(val_a, val_b);

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

            sinteger<64> a{val_a};
            sinteger<128> b = sinteger<128>::from_words(val_a, val_b);

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

SCENARIO("Bit operations are performed correctly", "[sinteger][bit]")
{
    GIVEN("An sinteger<N> n")
    {
        auto val =
            GENERATE(0, 56567, 23, static_cast<uint64_t>(-4354566), static_cast<uint64_t>(-1));
        const sinteger<150> n = sinteger<150>::from_words(2 * val, val);
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
            THEN("The result should have twice the bits of the original sinteger")
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


SCENARIO("std::numeric_limits gets instantiated correctly", "[sinteger][utility]")
{
    GIVEN("The bit width of 32")
    {
        THEN("The values should match the int32_t ones")
        {
            using sint = sinteger<32>;
            using nl = std::numeric_limits<sint>;
            using il = std::numeric_limits<int32_t>;

            CHECK(nl::is_specialized == il::is_specialized);
            CHECK(nl::is_signed == il::is_signed);
            CHECK(nl::is_integer == il::is_integer);
            CHECK(nl::is_exact == il::is_exact);
            CHECK(nl::has_infinity == il::has_infinity);
            CHECK(nl::has_quiet_NaN == il::has_quiet_NaN);
            CHECK(nl::has_signaling_NaN == il::has_signaling_NaN);
            CHECK(nl::is_bounded == il::is_bounded);
            CHECK(nl::has_denorm == il::has_denorm);
            CHECK(nl::has_denorm_loss == il::has_denorm_loss);
            CHECK(nl::round_style == il::round_style);
            CHECK(nl::is_iec559 == il::is_iec559);
            CHECK(nl::is_modulo == il::is_modulo);
            CHECK(nl::radix == il::radix);
            CHECK(nl::digits == il::digits);
            // TODO remove this when log10 becomes constexpr in clang's stdlibc
            #ifndef __clang__
            CHECK(nl::digits10 == il::digits10);
            #endif
            CHECK(nl::max_digits10 == il::max_digits10);
            CHECK(nl::min_exponent10 == il::min_exponent10);
            CHECK(nl::min_exponent == il::min_exponent);
            CHECK(nl::max_exponent == il::max_exponent);
            CHECK(nl::max_digits10 == il::max_exponent10);
            CHECK(nl::traps == il::traps);
            CHECK(nl::tinyness_before == il::tinyness_before);
            CHECK(static_cast<int32_t>(nl::min().word(0)) == il::min());
            CHECK(static_cast<int32_t>(nl::lowest().word(0)) == il::lowest());
            CHECK(nl::max().word(0) == il::max());
            CHECK(nl::epsilon().word(0) == il::epsilon());
            CHECK(nl::round_error().word(0) == il::round_error());
            CHECK(nl::infinity().word(0) == il::infinity());
            CHECK(nl::quiet_NaN().word(0) == il::quiet_NaN());
            CHECK(nl::signaling_NaN().word(0) == il::signaling_NaN());
            CHECK(nl::denorm_min().word(0) == il::denorm_min());
        }
    }
    GIVEN("The bit width of 64")
    {
        THEN("The values should match the int64_t ones")
        {
            using sint = sinteger<64>;
            using nl = std::numeric_limits<sint>;
            using il = std::numeric_limits<int64_t>;

            CHECK(nl::is_specialized == il::is_specialized);
            CHECK(nl::is_signed == il::is_signed);
            CHECK(nl::is_integer == il::is_integer);
            CHECK(nl::is_exact == il::is_exact);
            CHECK(nl::has_infinity == il::has_infinity);
            CHECK(nl::has_quiet_NaN == il::has_quiet_NaN);
            CHECK(nl::has_signaling_NaN == il::has_signaling_NaN);
            CHECK(nl::is_bounded == il::is_bounded);
            CHECK(nl::has_denorm == il::has_denorm);
            CHECK(nl::has_denorm_loss == il::has_denorm_loss);
            CHECK(nl::round_style == il::round_style);
            CHECK(nl::is_iec559 == il::is_iec559);
            CHECK(nl::is_modulo == il::is_modulo);
            CHECK(nl::radix == il::radix);
            CHECK(nl::digits == il::digits);
            // TODO remove this when log10 becomes constexpr in clang's stdlibc
            #ifndef __clang__
            CHECK(nl::digits10 == il::digits10);
            #endif
            CHECK(nl::max_digits10 == il::max_digits10);
            CHECK(nl::min_exponent10 == il::min_exponent10);
            CHECK(nl::min_exponent == il::min_exponent);
            CHECK(nl::max_exponent == il::max_exponent);
            CHECK(nl::max_digits10 == il::max_exponent10);
            CHECK(nl::traps == il::traps);
            CHECK(nl::tinyness_before == il::tinyness_before);
            CHECK(static_cast<int64_t>(nl::min().word(0)) == il::min());
            CHECK(static_cast<int64_t>(nl::lowest().word(0)) == il::lowest());
            CHECK(nl::max().word(0) == il::max());
            CHECK(nl::epsilon().word(0) == il::epsilon());
            CHECK(nl::round_error().word(0) == il::round_error());
            CHECK(nl::infinity().word(0) == il::infinity());
            CHECK(nl::quiet_NaN().word(0) == il::quiet_NaN());
            CHECK(nl::signaling_NaN().word(0) == il::signaling_NaN());
            CHECK(nl::denorm_min().word(0) == il::denorm_min());
        }
    }
}