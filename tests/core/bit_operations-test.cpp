#include <catch.hpp>

#include <aarith/core.hpp>

using namespace aarith;

SCENARIO("Counting bits in word_array", "[word_array][util]")
{
    WHEN("Multiple word_containers are given")
    {
        THEN("The leading zeroe index should be computed correctly")
        {
            constexpr word_array<64> a_zero{0U};
            constexpr word_array<64> a_one{1U};
            constexpr word_array<32> b_zero{0U};
            constexpr word_array<32> b_one{1U};
            constexpr word_array<15> c_zero{0U};
            constexpr word_array<15> c_one{1U};
            constexpr word_array<150> d_zero{0U};
            constexpr word_array<150> d_one{1U};

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

SCENARIO("Casting word_containers into different width", "[word_array]")
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

SCENARIO("Calculating the word_masks of word_containers", "[word_array][bit_logic]")
{
    // The tests all assume that word_array uses 64-bit words.
    static_assert(word_array<64>::word_width() == 64);

    GIVEN("A word_array<N> where N < word_width")
    {
        constexpr word_array<32> uint;
        THEN("The mask is correct")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffff);
        }
    }
    GIVEN("A word_array<N> where N == word_width")
    {
        constexpr word_array<64> uint;
        THEN("The mask is all 1s")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffffffffffff);
        }
    }
    GIVEN("A word_array<N> where N > word_width and N % word_width != 0")
    {
        constexpr word_array<96> uint;
        THEN("All masks except the last are all 1s")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffffffffffff);
        }
        THEN("The last mask is all ones up to the correct bit")
        {
            REQUIRE(uint.word_mask(1) == 0xffffffff);
        }
    }
    GIVEN("A word_array<N> where N > word_width and N % word_width == 0")
    {
        constexpr word_array<128> uint;
        THEN("All masks are all 1s")
        {
            REQUIRE(uint.word_mask(0) == 0xffffffffffffffff);
            REQUIRE(uint.word_mask(1) == 0xffffffffffffffff);
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

SCENARIO("Right shift operator works as expected", "[word_array][utility]")
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

                static constexpr word_array<width> a{1U};
                static constexpr word_array<width> expected0 =
                    word_array<width>::from_words(0U, 0U, 0U, 1U);
                static constexpr word_array<width> expected1 =
                    word_array<width>::from_words(0U, 0U, 1U, 0U);
                static constexpr word_array<width> expected2 =
                    word_array<width>::from_words(0U, 1U, 0U, 0U);
                static constexpr word_array<width> expected3 =
                    word_array<width>::from_words(1U, 0U, 0U, 0U);
                static constexpr word_array<width> expected4 =
                    word_array<width>::from_words(0U, 0U, 0U, 0U);

                std::vector<word_array<width>> expecteds{expected0, expected1, expected2, expected3,
                                                         expected4};

                for (auto i = 0U; i < expecteds.size(); ++i)
                {
                    word_array<width> result = a << (word_width * i);
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

SCENARIO("Bitwise operations work as expected", "[word_array][bit_logic]")
{
    GIVEN("A word container a")
    {

        const size_t Width = 64;
        static constexpr uint16_t number_a = 7;
        constexpr word_array<Width> a{number_a};

        WHEN("Negating the word container")
        {

            THEN("The result should match the NOT of the underlying WordType")
            {
                constexpr auto result = ~a;
                constexpr auto result_ref = ~number_a;
                REQUIRE(result.word(0) == result_ref);
            }
        }

        AND_GIVEN("Another word container b")
        {
            static constexpr uint16_t number_b = 14;
            constexpr word_array<Width> b{number_b};

            THEN("Bitwise AND should work as expected")
            {
                constexpr auto result = a & b;
                constexpr auto result_ref = number_a & number_b;
                REQUIRE(result.word(0) == result_ref);
            }

            AND_THEN("Bitwise OR should work as expected")
            {
                constexpr auto result = a | b;
                constexpr auto result_ref = number_a | number_b;
                REQUIRE(result.word(0) == result_ref);
            }
            AND_THEN("Bitwise XOR should work as expected")
            {
                constexpr auto result = a ^ b;
                constexpr auto result_ref = number_a ^ number_b;
                REQUIRE(result.word(0) == result_ref);
            }
        }
    }
}

SCENARIO("Checking whether an word_array is not equal to zero/false")
{
    GIVEN("An word_array<N>=0=a for various N")
    {
        THEN("a.is_zero() should be true")
        {

            const uint8_t zero = 0U;

            CHECK(word_array<64>{zero}.is_zero());
            CHECK(word_array<128>{zero}.is_zero());
            CHECK(word_array<150>{zero}.is_zero());
            CHECK(word_array<32>{zero}.is_zero());
            CHECK(word_array<23>{zero}.is_zero());
            CHECK(word_array<256>{zero}.is_zero());
            CHECK(word_array<1337>{zero}.is_zero());
            CHECK(word_array<8>{zero}.is_zero());
        }
        THEN("a should evaluate to false in a Boolean context")
        {

            const uint8_t zero = 0U;

            CHECK_FALSE(word_array<64>{zero});
            CHECK_FALSE(word_array<128>{zero});
            CHECK_FALSE(word_array<150>{zero});
            CHECK_FALSE(word_array<32>{zero});
            CHECK_FALSE(word_array<23>{zero});
            CHECK_FALSE(word_array<256>{zero});
            CHECK_FALSE(word_array<1337>{zero});
            CHECK_FALSE(word_array<8>{zero});
        }
    }

    GIVEN("A non-zero word_array")
    {

        uint64_t val = GENERATE(1, 2, 4, 5567868, 234, 21, 45, 56768);
        const word_array<64> a{val};
        const word_array<128> b = word_array<128>::from_words(val, 0U);
        const word_array<128> c = word_array<128>::from_words(val, val);

        const word_array<150> d = word_array<150>::from_words(val, 0U, 0U);
        const word_array<256> e = word_array<256>::from_words(val, val, 0U, 0U);

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

SCENARIO("Bit shifting is possible as constexpr")
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

SCENARIO("Bit operations are performed correctly", "[word_array][bit_logic]")
{
    GIVEN("An word_array<N> n")
    {
        // TODO Understand how to generate various bit widths
        auto val =
            GENERATE(0, 56567, 23, static_cast<uint64_t>(-4354566), static_cast<uint64_t>(-1));
        const word_array<150> n = word_array<150>::from_words(2 * val, val);
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
            THEN("The result should have twice the bits of the original word_array")
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

SCENARIO("Splitting a word_array/extraction from a word_array", "[word_array],[util]")
{
    GIVEN("A word_array w")
    {

        const word_array<7> w3{21};

        WHEN("The container is split")
        {
            THEN("The parts are as expected")
            {
                const auto [w3_left, w3_right] = split<2>(w3);
                REQUIRE(w3_left == word_array<4>{2});
                REQUIRE(w3_right == word_array<3>{5});
            }
        }
        WHEN("Ranges are extracted correctly")
        {
            THEN("The parts are as expected")
            {
                REQUIRE(bit_range<6, 1>(w3) == word_array<6>{10});
                REQUIRE(bit_range<6, 2>(w3) == word_array<5>{5});
                REQUIRE(bit_range<5, 1>(w3) == word_array<5>{10});
            }
        }
    }
}


SCENARIO("Concatenation is associative", "[word_array][util]") {
    GIVEN("Three different words") {
        const uint32_t n1 = GENERATE(take(100, random(std::numeric_limits<uint32_t>::max(),
                                                     std::numeric_limits<uint32_t>::max())));
        const uint32_t n2 = GENERATE(take(100, random(std::numeric_limits<uint32_t>::max(),
                                                     std::numeric_limits<uint32_t>::max())));
        const uint32_t n3 = GENERATE(take(100, random(std::numeric_limits<uint32_t>::max(),
                                                     std::numeric_limits<uint32_t>::max())));

        const word_array<32, uint8_t> w1_8{word_array<32,uint32_t>{n1}};
        const word_array<32, uint64_t> w1_64{n1};
        const word_array<32, uint8_t> w2_8{word_array<32,uint32_t>{n2}};
        const word_array<32, uint64_t> w2_64{n2};
        const word_array<32, uint8_t> w3_8{word_array<32,uint32_t>{n3}};
        const word_array<32, uint64_t> w3_64{n3};

        WHEN("Concatenating the words") {
            THEN("The operation should be associative") {
                REQUIRE(concat(concat(w1_8,w2_8),w3_8) == concat(w1_8,concat(w2_8,w3_8)));
                REQUIRE(concat(concat(w1_64,w2_64),w3_64) == concat(w1_64,concat(w2_64,w3_64)));
            }
        }
    }
}

SCENARIO("Concatenating undoes range extraction", "[word_array][util]")
{
    GIVEN("A word_array")
    {
        const uint32_t n = GENERATE(take(100, random(std::numeric_limits<uint32_t>::max(),
                                                    std::numeric_limits<uint32_t>::max())));
        const word_array<32, uint8_t> w8{word_array<32,uint32_t>{n}};
        const word_array<32, uint64_t> w64{n};

        WHEN("Extracting a range from including one end (i.e. basically splitting the word array)")
        {
            THEN("Using concat to put the word back together should yield the original word")
            {

                const auto left8 = bit_range<31, 15>(w8);
                const auto right8 = bit_range<14, 0>(w8);
                REQUIRE(concat(left8, right8) == w8);

                const auto left64 = bit_range<31, 15>(w64);
                const auto right64 = bit_range<14, 0>(w64);
                REQUIRE(concat(left64, right64) == w64);
            }
        }

        WHEN("Extracting a part from the middle of the word array")
        {
            THEN("Using concat to put the word back together should yield the original word")
            {

                const auto left8 = bit_range<31, 20>(w8);
                const auto middle8 = bit_range<19,10>(w8);
                const auto right8 = bit_range<9, 0>(w8);
                REQUIRE(concat(concat(left8, middle8), right8) == w8);

                const auto left64 = bit_range<31, 20>(w64);
                const auto middle64 = bit_range<19,10>(w64);
                const auto right64 = bit_range<9, 0>(w64);
                REQUIRE(concat(concat(left64, middle64), right64) == w64);
            }
        }
    }
}

SCENARIO("Concatenating undoes splitting", "[word_array][util]")
{
    GIVEN("A word_array")
    {
        const uint8_t n8_ = GENERATE(take(
            100, random(std::numeric_limits<uint8_t>::max(), std::numeric_limits<uint8_t>::max())));
        const uint8_t n32_ = GENERATE(take(100, random(std::numeric_limits<uint32_t>::max(),
                                                       std::numeric_limits<uint32_t>::max())));

        const word_array<9, uint8_t> n8{n8_};
        const word_array<9, uint16_t> n16{n8_};

        const word_array<32, uint8_t> m8{n32_};
        const word_array<32, uint16_t> m16{n32_};

        WHEN("Splitting the word at some position")
        {
            THEN("Concatenating the resulting parts should yield the original word array")
            {
                {
                    const auto [left8, right8] = split<0>(n8);
                    const auto [left16, right16] = split<0>(n16);
                    REQUIRE(concat(left8, right8) == n8);
                    REQUIRE(concat(left16, right16) == n16);
                }
                {
                    const auto [left8, right8] = split<1>(n8);
                    const auto [left16, right16] = split<1>(n16);
                    REQUIRE(concat(left8, right8) == n8);
                    REQUIRE(concat(left16, right16) == n16);
                }
                {
                    const auto [left8, right8] = split<2>(n8);
                    const auto [left16, right16] = split<2>(n16);
                    REQUIRE(concat(left8, right8) == n8);
                    REQUIRE(concat(left16, right16) == n16);
                }
                {
                    const auto [left8, right8] = split<3>(n8);
                    const auto [left16, right16] = split<3>(n16);
                    REQUIRE(concat(left8, right8) == n8);
                    REQUIRE(concat(left16, right16) == n16);
                }
                {
                    const auto [left8, right8] = split<4>(n8);
                    const auto [left16, right16] = split<4>(n16);
                    REQUIRE(concat(left8, right8) == n8);
                    REQUIRE(concat(left16, right16) == n16);
                }

                {
                    const auto [left8, right8] = split<7>(n8);
                    const auto [left16, right16] = split<7>(n16);
                    REQUIRE(concat(left8, right8) == n8);
                    REQUIRE(concat(left16, right16) == n16);
                }

                {
                    const auto [left8, right8] = split<0>(m8);
                    const auto [left16, right16] = split<0>(m16);
                    REQUIRE(concat(left8, right8) == m8);
                    REQUIRE(concat(left16, right16) == m16);
                }
                {
                    const auto [left8, right8] = split<1>(m8);
                    const auto [left16, right16] = split<1>(m16);
                    REQUIRE(concat(left8, right8) == m8);
                    REQUIRE(concat(left16, right16) == m16);
                }
                {
                    const auto [left8, right8] = split<2>(m8);
                    const auto [left16, right16] = split<2>(m16);
                    REQUIRE(concat(left8, right8) == m8);
                    REQUIRE(concat(left16, right16) == m16);
                }
                {
                    const auto [left8, right8] = split<10>(m8);
                    const auto [left16, right16] = split<10>(m16);
                    REQUIRE(concat(left8, right8) == m8);
                    REQUIRE(concat(left16, right16) == m16);
                }
                {
                    const auto [left8, right8] = split<20>(m8);
                    const auto [left16, right16] = split<20>(m16);
                    REQUIRE(concat(left8, right8) == m8);
                    REQUIRE(concat(left16, right16) == m16);
                }

                {
                    const auto [left8, right8] = split<30>(m8);
                    const auto [left16, right16] = split<30>(m16);
                    REQUIRE(concat(left8, right8) == m8);
                    REQUIRE(concat(left16, right16) == m16);
                }
            }
        }
    }
}