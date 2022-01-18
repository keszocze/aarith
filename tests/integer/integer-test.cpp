#include <aarith/integer_no_operators.hpp>

#include <bitset>
#include <catch.hpp>
#include <sstream>

using namespace aarith;

SCENARIO("Bit shifting is possible as constexpr for signed integers",
         "[integer][signed][utility][constexpr][bit_logic]")
{
    GIVEN("A signed integer of width N")
    {
        THEN("Right-shifting should be a constexpr operation")
        {
            constexpr size_t W = 32;
            constexpr integer<W> num{4};

            constexpr integer<W> shift1{2};
            constexpr integer<W> shift2{1};
            constexpr integer<W> shift3{0};

            constexpr integer<W> shifted1 = (num >> 1);
            constexpr integer<W> shifted2 = (num >> 2);
            constexpr integer<W> shifted3 = (num >> 3);

            REQUIRE(shift1 == shifted1);
            REQUIRE(shift2 == shifted2);
            REQUIRE(shift3 == shifted3);
        }
        THEN("Left-shifting should be a constexpr operation")
        {
            constexpr size_t W = 32;
            constexpr integer<W> num{4};

            constexpr integer<W> shift1{8};
            constexpr integer<W> shift2{16};
            constexpr integer<W> shift3{32};

            constexpr integer<W> shifted1 = (num << 1);
            constexpr integer<W> shifted2 = (num << 2);
            constexpr integer<W> shifted3 = (num << 3);

            REQUIRE(shift1 == shifted1);
            REQUIRE(shift2 == shifted2);
            REQUIRE(shift3 == shifted3);
        }
    }
}

SCENARIO("Testing various operations for constexpr'nes", "[integer][signed][constexpr]")
{
    GIVEN("A signed integer")
    {
        constexpr integer<17> num{4};
        THEN("The is_negative method should be constexpr")
        {
            constexpr bool is_neg = num.is_negative();
            REQUIRE_FALSE(is_neg);
        }
        THEN("The is_zero method should be constexpr")
        {
            constexpr bool is_zero = num.is_zero();
            REQUIRE_FALSE(is_zero);
        }
    }
}

SCENARIO("Casting integers into different width", "[integer][signed][casting]")
{
    GIVEN("width_cast is called")
    {
        static constexpr uint16_t test_value = 123;
        static constexpr size_t SourceWidth = 16;
        constexpr integer<SourceWidth> uint{test_value};

        WHEN("The source width <= destination width")
        {
            static constexpr size_t DestinationWidth = 32;
            const integer<DestinationWidth> result{width_cast<DestinationWidth>(uint)};

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

SCENARIO("Copy constructor of integers with various bit widths", "[integer][signed][utility]")
{
    GIVEN("An integer<N> a")
    {
        const uint64_t val_a = GENERATE(
            take(10, random(static_cast<uint64_t>(0U), std::numeric_limits<uint64_t>::max())));
        integer<196> a(0U, val_a, 0U);

        THEN("Assignment of individual words is correct")
        {
            CHECK(a.word(0) == 0U);
            CHECK(a.word(1) == val_a);
            CHECK(a.word(2) == 0U);
        }

        AND_GIVEN("An integer<N> b")
        {
            const uint64_t val_b = 1337;
            const integer<196> b = integer<196>::from_words(val_b, 0U, 2 * val_b);

            THEN("Assignment opeator of individual words is correct")
            {

                a = b;
                CHECK(a.word(0) == 2 * val_b);
                CHECK(a.word(1) == 0U);
                CHECK(a.word(2) == val_b);
            }
        }
        AND_GIVEN("An integer<M> b")
        {
            WHEN("M < N")
            {
                const uint64_t val_b = 23;
                const integer<64> tmp = integer<64>::from_words(val_b);

                THEN("The copy constructor should work")
                {
                    integer<128> b{tmp};

                    CHECK(b.word(0) == val_b);
                    CHECK(b.word(1) == 0U);
                }
                THEN("The assignment operator should work")
                {
                    integer<128> b;
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
            //        //            const integer<150> tmp =
            //        integer<150>::from_words(0U,val_b,0U);
            //        //            std::cout << tmp << "\n";
            //        //            const integer<128> b = tmp;
            //        //            std::cout << b << "\n";
            //        //        }
        }
    }
}

SCENARIO("Left shift operator works as expected", "[integer][signed][utility][bit_logic]")
{
    GIVEN("One integer a and a number of shifted bits s")
    {

        constexpr size_t TestWidth = 16;

        WHEN("The result still fits the width")
        {
            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 4U;
            constexpr integer<TestWidth> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 112);
        }
        WHEN("The result does not fit the width anymore")
        {
            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 14U;
            constexpr integer<TestWidth> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 49152);
        }

        WHEN("Some bits are shifted to the next word")
        {
            const size_t Width = 70;

            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 63U;
            constexpr integer<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0x8000000000000000);
            REQUIRE(result.word(1) == 3);
        }
        WHEN("The bits are shifted to the second next word")
        {
            constexpr size_t Width = 130;

            static constexpr uint16_t number_a = 7;
            static constexpr auto s = 127U;
            constexpr integer<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == 0x8000000000000000);
            REQUIRE(result.word(2) == 3);
        }
        WHEN("The bits are not shifted")
        {
            constexpr size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = 0;
            constexpr integer<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 3);
            REQUIRE(result.word(1) == 0);
            REQUIRE(result.word(2) == 0);
        }
        WHEN("The bits are shifted exactly one word")
        {
            constexpr size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = static_cast<size_t>(integer<Width>::word_width());
            constexpr integer<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == 3);
            REQUIRE(result.word(2) == 0);
        }
        WHEN("The bits are shifted exactly two words")
        {
            constexpr size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = 2 * static_cast<size_t>(integer<Width>::word_width());
            constexpr integer<Width> a{number_a};

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == 0);
            REQUIRE(result.word(2) == 3);
        }
        WHEN("The bits are shifted exactly by word_width-1")
        {
            constexpr size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = static_cast<size_t>(integer<Width>::word_width()) - 1U;
            constexpr integer<Width> a{number_a};

            auto reference = a.word(0) << s;

            const auto result = a << s;
            REQUIRE(result.word(0) == reference);
            REQUIRE(result.word(1) == 1);
            REQUIRE(result.word(2) == 0);
        }
        WHEN("The bits are shifted by 2*word_width-1")
        {
            constexpr size_t Width = 192;

            static constexpr uint16_t number_a = 3;
            static constexpr auto s = 2U * static_cast<size_t>(integer<Width>::word_width()) - 1U;
            constexpr integer<Width> a{number_a};

            auto reference = a.word(0) << (s % (sizeof(integer<Width>::word_width()) * 8));

            const auto result = a << s;
            REQUIRE(result.word(0) == 0);
            REQUIRE(result.word(1) == reference);
            REQUIRE(result.word(2) == 1);
        }
    }
}

SCENARIO("Create negative integers", "[integer][signed][bit_logic]")
{
    using namespace integer_operators;
    GIVEN("A int64_t negative number")
    {
        int64_t n = GENERATE(take(100, random(-922337236854775808LL, -1LL)));
        int64_t pos_n = -n;

        integer<64> zero;

        WHEN("We create a signed integer from it")
        {

            integer<64> negative{n};
            integer<64> positive{pos_n};

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

SCENARIO("Right shift operator works as expected", "[integer][signed][bit_logic]")
{
    GIVEN("One positive integer a and a number of shifted bits s")
    {
        WHEN("The bits are not shifted")
        {
            const size_t Width = 192;

            typename integer<Width>::word_type number_a = 3;
            number_a <<= (integer<Width>::word_width() - 2);
            static constexpr auto s = 0;
            integer<Width> a(0U);
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

            typename integer<Width>::word_type number_a = 3;
            number_a <<= (integer<Width>::word_width() - 4);

            static constexpr auto s = static_cast<size_t>(integer<Width>::word_width());
            integer<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            const auto result = a >> s;

            CHECK(result.word(a.word_count() - 3) == 0);
            CHECK(result.word(a.word_count() - 2) == number_a);
            REQUIRE(result.word(a.word_count() - 1) == 0);
        }

        WHEN("The bits are shifted exactly two words")
        {
            const size_t Width = 192;

            typename integer<Width>::word_type number_a = 3;
            number_a <<= integer<Width>::word_width() - 4;
            static constexpr auto s = 2 * static_cast<size_t>(integer<Width>::word_width());
            integer<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            const auto result = a >> s;
            REQUIRE(result.word(a.word_count() - 3) == number_a);
            REQUIRE(result.word(a.word_count() - 2) == 0);
            REQUIRE(result.word(a.word_count() - 1) == 0);
        }

        WHEN("The bits are shifted exactly by word_width-2 over word boundaries")
        {
            const size_t Width = 192;

            typename integer<Width>::word_type number_a = 3;
            number_a <<= integer<Width>::word_width() - 3;
            static constexpr auto s = static_cast<size_t>(integer<Width>::word_width()) - 2;
            integer<Width> a(0U);
            a.set_word(a.word_count() - 1, number_a);

            typename integer<Width>::word_type ref = uint64_t(1) << 63U;

            const auto result = a >> s;

            REQUIRE(result.word(a.word_count() - 3) == 0);
            REQUIRE(result.word(a.word_count() - 2) == ref);
            REQUIRE(result.word(a.word_count() - 1) == 1);
        }

        WHEN("The bits are shifted by 2*word_width-1")
        {
            const size_t Width = 192;

            typename integer<Width>::word_type number_a = 3;
            number_a <<= integer<Width>::word_width() - 3;

            static constexpr auto s = 2 * static_cast<size_t>(integer<Width>::word_width()) - 1;
            integer<Width> a(0U);
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
                static constexpr size_t word_width = integer<width>::word_width();

                static constexpr integer<width> a{1U};
                static const integer<width> expected0 = integer<width>::from_words(0U, 0U, 0U, 1U);
                static const integer<width> expected1 = integer<width>::from_words(0U, 0U, 1U, 0U);
                static const integer<width> expected2 = integer<width>::from_words(0U, 1U, 0U, 0U);
                static const integer<width> expected3 = integer<width>::from_words(1U, 0U, 0U, 0U);
                static const integer<width> expected4 = integer<width>::from_words(0U, 0U, 0U, 0U);

                std::vector<integer<width>> expecteds{expected0, expected1, expected2, expected3,
                                                      expected4};

                for (auto i = 0U; i < expecteds.size(); ++i)
                {
                    integer<width> result = a << (word_width * i);
                    CHECK(result == expecteds[i]);
                }
            }
        }

        AND_WHEN("The shift amount is the integer width")
        {
            THEN("The result should still be correct")
            {
                constexpr size_t w = 128;
                constexpr integer<w> a{1U};
                constexpr integer<w> expected;
                const integer<w> result = a >> w;

                CHECK(expected == result);
            }
        }
    }
}

SCENARIO("Logical AND works as expected", "[integer][signed][bit_logic][utility]")
{
    GIVEN("Two sintegers")
    {
        WHEN("The sintegers consists of only one word")
        {
            constexpr size_t Width = 70;

            using WT = uint64_t;

            static constexpr uint16_t number_a = 7;
            static constexpr uint16_t number_b = 14;
            constexpr integer<Width,WT> a{number_a};
            constexpr integer<Width,WT> b{number_b};

            const auto result = a & b;
            const WT result_ref = number_a & number_b;
            REQUIRE(result.word(0) == result_ref);
        }
    }
}

SCENARIO("Logical OR works as expected", "[integer][signed][bit_logic][utility]")
{
    GIVEN("Two sintegers")
    {
        WHEN("The sintegers consists of only one word")
        {
            constexpr size_t Width = 70;
            using WT = uint64_t;

            static constexpr uint16_t number_a = 7;
            static constexpr uint16_t number_b = 14;
            constexpr integer<Width,WT> a{number_a};
            constexpr integer<Width,WT> b{number_b};

            constexpr auto result = a | b;
            constexpr WT result_ref = number_a | number_b;
            REQUIRE(result.word(0) == result_ref);
        }
    }
}

SCENARIO("Bit-wise NOT works as expected", "[integer][signed][bit_logic][utility]")
{
    GIVEN("One sinteger consisting of a single word")
    {

        const size_t Width = 70;
        using WT = uint64_t;
        using T = integer<Width, WT>;

        static constexpr uint16_t number_a = 7;
        constexpr T a{number_a};

        WHEN("Performing bit-wise negation")
        {

            constexpr T result = ~a;
            constexpr WT result_ref = ~number_a;
            THEN("The result for the wordtype and the sinteger should match")
            {
                REQUIRE(result.word(0) == result_ref);
            }
        }
    }

}

SCENARIO("Checking whether an integer is not equal to zero/false", "[integer][signed][utility]")
{
    GIVEN("An integer<N>=0=a for various N")
    {
        THEN("a.is_zero() should be true")
        {

            constexpr uint8_t zero = 0U;

            CHECK(integer<64>{zero}.is_zero());
            CHECK(integer<128>{zero}.is_zero());
            CHECK(integer<150>{zero}.is_zero());
            CHECK(integer<32>{zero}.is_zero());
            CHECK(integer<23>{zero}.is_zero());
            CHECK(integer<256>{zero}.is_zero());
            CHECK(integer<1337>{zero}.is_zero());
            CHECK(integer<8>{zero}.is_zero());
        }
        THEN("a should evaluate to false in a Boolean context")
        {

            constexpr uint8_t zero = 0U;

            CHECK_FALSE(integer<64>{zero});
            CHECK_FALSE(integer<128>{zero});
            CHECK_FALSE(integer<150>{zero});
            CHECK_FALSE(integer<32>{zero});
            CHECK_FALSE(integer<23>{zero});
            CHECK_FALSE(integer<256>{zero});
            CHECK_FALSE(integer<1337>{zero});
            CHECK_FALSE(integer<8>{zero});
        }
    }

    GIVEN("A non-zero integer")
    {

        uint64_t val = GENERATE(1, 2, 4, 5567868, 234, 21, 45, 56768);
        integer<64> a{val};
        integer<128> b = integer<128>::from_words(val, 0U);
        integer<128> c = integer<128>::from_words(val, val);

        integer<150> d = integer<150>::from_words(val, 0U, 0U);
        integer<256> e = integer<256>::from_words(val, val, 0U, 0U);

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

SCENARIO("Using the for loop operation feature", "[integer][signed][utility]")
{
    GIVEN("An unsigned integer")
    {
        THEN("The forward iterators should give the words as expected")
        {
            uint64_t val_a = GENERATE(0, 1, 2, 3);
            uint64_t val_b = GENERATE(3, 56, 567, 324);

            integer<64> a{val_a};
            integer<128> b = integer<128>::from_words(val_a, val_b);

            size_t index = 0;
            for (const integer<64>::word_type w : a)
            {
                CHECK(w == a.word(index));
                index++;
            }

            CHECK(index == 1);

            index = 0;

            for (const integer<128>::word_type w : b)
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

            integer<64> a{val_a};
            integer<128> b = integer<128>::from_words(val_a, val_b);

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

            integer<64> a{val_a};
            integer<128> b = integer<128>::from_words(val_a, val_b);

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

SCENARIO("Bit operations are performed correctly", "[integer][signed][bit_logic]")
{
    GIVEN("An integer<N> n")
    {
        auto val =
            GENERATE(0, 56567, 23, static_cast<uint64_t>(-4354566), static_cast<uint64_t>(-1));
        const integer<150> n = integer<150>::from_words(2 * val, val);
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
            THEN("The result should have twice the bits of the original integer")
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

SCENARIO("std::numeric_limits gets instantiated correctly", "[integer][signed][utility]")
{
    GIVEN("The bit width of 32")
    {
        THEN("The values should match the int32_t ones")
        {
            using sint = integer<32>;
            using integer_limits = std::numeric_limits<sint>;
            using int32_t_limits = std::numeric_limits<int32_t>;

            CHECK(integer_limits::is_specialized == int32_t_limits::is_specialized);
            CHECK(integer_limits::is_signed == int32_t_limits::is_signed);
            CHECK(integer_limits::is_integer == int32_t_limits::is_integer);
            CHECK(integer_limits::is_exact == int32_t_limits::is_exact);
            CHECK(integer_limits::has_infinity == int32_t_limits::has_infinity);
            CHECK(integer_limits::has_quiet_NaN == int32_t_limits::has_quiet_NaN);
            CHECK(integer_limits::has_signaling_NaN == int32_t_limits::has_signaling_NaN);
            CHECK(integer_limits::is_bounded == int32_t_limits::is_bounded);
            CHECK(integer_limits::has_denorm == int32_t_limits::has_denorm);
            CHECK(integer_limits::has_denorm_loss == int32_t_limits::has_denorm_loss);
            CHECK(integer_limits::round_style == int32_t_limits::round_style);
            CHECK(integer_limits::is_iec559 == int32_t_limits::is_iec559);
            CHECK(integer_limits::is_modulo == int32_t_limits::is_modulo);
            CHECK(integer_limits::radix == int32_t_limits::radix);
            CHECK(integer_limits::digits == int32_t_limits::digits);
            CHECK(integer_limits::digits10 == int32_t_limits::digits10);
            CHECK(integer_limits::max_digits10 == int32_t_limits::max_digits10);
            CHECK(integer_limits::min_exponent10 == int32_t_limits::min_exponent10);
            CHECK(integer_limits::min_exponent == int32_t_limits::min_exponent);
            CHECK(integer_limits::max_exponent == int32_t_limits::max_exponent);
            CHECK(integer_limits::max_digits10 == int32_t_limits::max_exponent10);
            CHECK(integer_limits::tinyness_before == int32_t_limits::tinyness_before);
            CHECK(static_cast<int32_t>(integer_limits::min().word(0)) == int32_t_limits::min());
            CHECK(static_cast<int32_t>(integer_limits::lowest().word(0)) ==
                  int32_t_limits::lowest());
            CHECK(integer_limits::max().word(0) == int32_t_limits::max());
            CHECK(integer_limits::epsilon().word(0) == int32_t_limits::epsilon());
            CHECK(integer_limits::round_error().word(0) == int32_t_limits::round_error());
            CHECK(integer_limits::infinity().word(0) == int32_t_limits::infinity());
            CHECK(integer_limits::quiet_NaN().word(0) == int32_t_limits::quiet_NaN());
            CHECK(integer_limits::signaling_NaN().word(0) == int32_t_limits::signaling_NaN());
            CHECK(integer_limits::denorm_min().word(0) == int32_t_limits::denorm_min());
        }
    }
    GIVEN("The bit width of 64")
    {
        THEN("The values should match the int64_t ones")
        {
            using sint = integer<64>;
            using integer_limits = std::numeric_limits<sint>;
            using int64_t_limits = std::numeric_limits<int64_t>;

            CHECK(integer_limits::is_specialized == int64_t_limits::is_specialized);
            CHECK(integer_limits::is_signed == int64_t_limits::is_signed);
            CHECK(integer_limits::is_integer == int64_t_limits::is_integer);
            CHECK(integer_limits::is_exact == int64_t_limits::is_exact);
            CHECK(integer_limits::has_infinity == int64_t_limits::has_infinity);
            CHECK(integer_limits::has_quiet_NaN == int64_t_limits::has_quiet_NaN);
            CHECK(integer_limits::has_signaling_NaN == int64_t_limits::has_signaling_NaN);
            CHECK(integer_limits::is_bounded == int64_t_limits::is_bounded);
            CHECK(integer_limits::has_denorm == int64_t_limits::has_denorm);
            CHECK(integer_limits::has_denorm_loss == int64_t_limits::has_denorm_loss);
            CHECK(integer_limits::round_style == int64_t_limits::round_style);
            CHECK(integer_limits::is_iec559 == int64_t_limits::is_iec559);
            CHECK(integer_limits::is_modulo == int64_t_limits::is_modulo);
            CHECK(integer_limits::radix == int64_t_limits::radix);
            CHECK(integer_limits::digits == int64_t_limits::digits);
            CHECK(integer_limits::digits10 == int64_t_limits::digits10);
            CHECK(integer_limits::max_digits10 == int64_t_limits::max_digits10);
            CHECK(integer_limits::min_exponent10 == int64_t_limits::min_exponent10);
            CHECK(integer_limits::min_exponent == int64_t_limits::min_exponent);
            CHECK(integer_limits::max_exponent == int64_t_limits::max_exponent);
            CHECK(integer_limits::max_digits10 == int64_t_limits::max_exponent10);
            CHECK(integer_limits::tinyness_before == int64_t_limits::tinyness_before);
            CHECK(static_cast<int64_t>(integer_limits::min().word(0)) == int64_t_limits::min());
            CHECK(static_cast<int64_t>(integer_limits::lowest().word(0)) ==
                  int64_t_limits::lowest());
            CHECK(static_cast<int64_t>(integer_limits::max().word(0)) == int64_t_limits::max());
            CHECK(static_cast<int64_t>(integer_limits::epsilon().word(0)) ==
                  int64_t_limits::epsilon());
            CHECK(static_cast<int64_t>(integer_limits::round_error().word(0)) ==
                  int64_t_limits::round_error());
            CHECK(static_cast<int64_t>(integer_limits::infinity().word(0)) ==
                  int64_t_limits::infinity());
            CHECK(static_cast<int64_t>(integer_limits::quiet_NaN().word(0)) ==
                  int64_t_limits::quiet_NaN());
            CHECK(static_cast<int64_t>(integer_limits::signaling_NaN().word(0)) ==
                  int64_t_limits::signaling_NaN());
            CHECK(static_cast<int64_t>(integer_limits::denorm_min().word(0)) ==
                  int64_t_limits::denorm_min());
        }
    }
}