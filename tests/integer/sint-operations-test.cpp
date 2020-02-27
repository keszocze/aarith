#include <aarith/integer.hpp>
#include <catch.hpp>

using namespace aarith;

SCENARIO("FOO", "[foo]")
{
    GIVEN("Two sinteger with different word_count")
    {
        const sinteger<64> a{16};
        const sinteger<128> b{32, 8};
        const sinteger<128> expected{32,24};
        WHEN("Adding the integers")
        {
            THEN("All words need to be respected in the operation")
            {
                const auto result = expanding_add(b, a);
                std::cout << group_digits(to_binary(a), 64) << "\n";
                std::cout << group_digits(to_binary(b), 64) << "\n";
                std::cout << group_digits(to_binary(result), 64) << "\n";
                std::cout << group_digits(to_binary(expected), 64) << "\n";
                REQUIRE(result == expected);
            }
        }
    }
}

SCENARIO("Adding two positive sintegers exactly", "[sinteger][arithmetic][addition]")
{
    GIVEN("Two positive sinteger<N> a and b with N <= word_width")
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
            const sinteger<TestWidth> result_fun = fun_add(a, b);

            THEN("It should be the correct sum")
            {
                CHECK(result_fun == result);
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
            const sinteger<TestWidth> result_fun = fun_add(a, b);

            THEN("It should be the masked to fit")
            {
                CHECK(result_fun == result);
                REQUIRE(result.word(0) == 0);
            }
        }
    }

    GIVEN("Two positive sinteger<N> a and b with N > word_width")
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
            const sinteger<TestWidth> result_fun = fun_add(a, b);

            THEN("It is added to the next word")
            {
                CHECK(result_fun == result);
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
            const sinteger<TestWidth> result_fun = fun_add(a, b);

            THEN("The next word is unchanged")
            {
                REQUIRE(result_fun == result);
                REQUIRE(result.word(1) == 0);
            }
        }
    }

    GIVEN("A sinteger and its negative")
    {
        const sinteger<16> a(15);
        const sinteger<16> a_(-15);
        const sinteger<16> zero16(0);
        const sinteger<16> sum16 = add(a, a_);
        const sinteger<16> sum16_fun = fun_add(a, a_);

        const sinteger<64> b(150);
        const sinteger<64> b_(-150);
        const sinteger<64> zero64(0);
        const sinteger<64> sum64 = add(b, b_);
        const sinteger<64> sum64_fun = fun_add(b, b_);

        const sinteger<150> c(1337);
        const sinteger<150> c_(-1337);
        const sinteger<150> zero150(0);
        const sinteger<150> sum150_fun = fun_add(c, c_);
        const sinteger<150> sum150 = add(c, c_);

        THEN("The sum should be zero")
        {

            CHECK(sum16_fun == sum16);
            REQUIRE(sum16 == zero16);
            REQUIRE(sum16.is_zero());
            REQUIRE(sum16 == add(a_, a));

            CHECK(sum64_fun == sum64);
            REQUIRE(sum64 == zero64);
            REQUIRE(sum64.is_zero());
            REQUIRE(sum64 == add(b_, b));

            CHECK(sum150_fun == sum150);
            REQUIRE(sum150 == zero150);
            REQUIRE(sum150.is_zero());
            REQUIRE(sum150 == add(c_, c));
        }

        THEN("The sum should be negative")
        {
            REQUIRE_FALSE(sum16.is_negative());
            REQUIRE_FALSE(sum64.is_negative());
            REQUIRE_FALSE(sum150.is_negative());
        }
    }

    GIVEN("A positive sinteger and a negative sinteger with larger absolute value")
    {
        const sinteger<16> a(15);
        const sinteger<16> a_(-16);
        const sinteger<16> zero16(0);
        const sinteger<16> sum16 = add(a, a_);
        const sinteger<16> sum16_fun = fun_add(a, a_);

        const sinteger<64> b(150);
        const sinteger<64> b_(-235);
        const sinteger<64> zero64(0);
        const sinteger<64> sum64 = add(b, b_);
        const sinteger<64> sum64_fun = fun_add(b, b_);

        const sinteger<150> c(1337);
        const sinteger<150> c_(-5000);
        const sinteger<150> zero150(0);
        const sinteger<150> sum150 = add(c, c_);
        const sinteger<150> sum150_fun = fun_add(c, c_);

        THEN("The sum should be negative")
        {
            CHECK(sum150_fun == sum150);
            CHECK(sum16_fun == sum16);
            CHECK(sum64_fun == sum64);
            REQUIRE(sum16.is_negative());
            REQUIRE(sum64.is_negative());
            REQUIRE(sum150.is_negative());
        }
    }
}

SCENARIO("Division of signed integers", "[sinteger][arithmetic]")
{

    GIVEN("The number 1 << 65")
    {
        const sinteger<70> m = (sinteger<70>::one() << 65);

        const sinteger<70> two{2};

        WHEN("Repeatedly dividing by two")
        {
            THEN("The division should work correctly over word boundaries")
            {
                const auto div1 = restoring_division(m, two);

                CHECK(div1.first.word(1) == 1U);
                CHECK(div1.first.word(0) == 0U);
                CHECK(div1.second == sinteger<70>::zero());

                const auto div2 = restoring_division(div1.first, two);

                CHECK(div2.first.word(1) == 0U);
                CHECK(div2.first.word(0) == (int64_t(1) << int64_t(63)));
                CHECK(div2.second == sinteger<70>::zero());
            }
        }
    }

    GIVEN("A signed integer<64> n != INT_MIN")
    {

        const int64_t n_ = GENERATE(take(
            100, random(std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::max())));
        const sinteger<64> n{n_};

        WHEN("Dividing zero by this number")
        {
            THEN("The result should be zero")
            {
                if (!n.is_zero())
                {
                    const auto result = restoring_division(sinteger<64>::zero(), n);
                    CHECK(result.second == sinteger<64>::zero());
                    REQUIRE(result.first == sinteger<64>::zero());
                }
                else
                {
                    REQUIRE_THROWS_AS(restoring_division(n, n), std::runtime_error);
                }
            }
        }

        WHEN("Dividing by itself")
        {
            AND_WHEN("n is not equal to zero")
            {
                THEN("The result should be one without remainder")
                {
                    if (!n.is_zero())
                    {
                        const auto result = restoring_division(n, n);
                        CHECK(result.second == sinteger<64>::zero());
                        REQUIRE(result.first == sinteger<61>::one());
                    }
                    else
                    {
                        REQUIRE_THROWS_AS(restoring_division(n, n), std::runtime_error);
                    }
                }
            }
        }

        WHEN("Dividing by zero")
        {
            THEN("An exception should be raised")
            {
                REQUIRE_THROWS_AS(restoring_division(n, sinteger<64>::zero()), std::runtime_error);
            }
        }

        WHEN("Dividing by one")
        {
            THEN("The result should be the number itself")
            {

                const auto result = restoring_division(n, sinteger<64>::one());
                CHECK(result.second == sinteger<64>::zero());
                REQUIRE(result.first == n);
            }
        }

        WHEN("Dividing by minus one")
        {
            THEN("The result should flip its sign")
            {

                const auto result = restoring_division(n, sinteger<64>::minus_one());
                CHECK(result.second == sinteger<64>::zero());
                REQUIRE(result.first == -n);
            }
        }

        AND_GIVEN("Another signed integer<64> m != INT_MIN")
        {
            const int64_t m_ = GENERATE(take(100, random(std::numeric_limits<int64_t>::max(),
                                                         std::numeric_limits<int64_t>::max())));
            const sinteger<64> m{m_};
            WHEN("Dividing the numbers")
            {
                THEN("The behaviour should match its int64_t counterpart")
                {
                    const auto result64_div = n_ / m_;
                    const auto result64_rem = n_ % m_;
                    const auto result = restoring_division(n, m);
                    CHECK(result.first.word(0) == result64_div);
                    CHECK(result.second.word(0) == result64_rem);
                }
            }
        }
    }

    GIVEN("INT_MIN")
    {
        AND_GIVEN("Minus one")
        {
            WHEN("Dividing INT_MIN by minus one")
            {
                THEN("The result should be INT_MIN")
                {
                    const auto result =
                        restoring_division(sinteger<64>::min(), sinteger<64>::minus_one());
                    CHECK(result.first == sinteger<64>::min());
                    CHECK(result.second == sinteger<64>::zero());
                }
            }
        }
    }
}
SCENARIO("Multiplying signed integers", "[sinteger][arithmetic]")
{
    GIVEN("Two signed integers m and r")
    {
        THEN("Then the multiplication should be done correctly")
        {
        }

        WHEN("m is the most negative number")
        {
            THEN("The algorithm should still work")
            {
                const sinteger<8> m{-16};
                const sinteger<8> r{2};

                const sinteger<8> res = mul(m, r);

                int8_t mi = -16;
                int8_t ri = 2;

                int8_t resi = mi * ri;
                CHECK((uint8_t)res.word(0) == (uint8_t)resi);
            }
        }
    }
}

SCENARIO("Absolute value computation", "[sinteger][utility]")
{
    GIVEN("The smallest possible value")
    {
        const sinteger<150> min = std::numeric_limits<sinteger<150>>::min();
        THEN("The absolute value of that value is the value again")
        {
            REQUIRE(abs(min) == min);
        }

        THEN("The the 'real' absolute value is 2^(W-1)")
        {
            uinteger<150> abs = expanding_abs(min);
            CHECK(abs.word(0) == 0U);
            CHECK(abs.word(1) == 0U);
            REQUIRE(abs.word(2) == (1U << 21U));
        }
    }

    GIVEN("Any non-smallest value")
    {
        using sint = sinteger<64>;
        const int32_t val_32 =
            GENERATE(take(50, random(std::numeric_limits<int32_t>::min() + 1, -1)));
        const int64_t val_64 = GENERATE(
            take(50, random(std::numeric_limits<int64_t>::min() + 1, static_cast<int64_t>(-1))));
        const sint a{val_32};
        const sint b{val_64};

        THEN("Computing abs is idempotent")
        {
            REQUIRE(abs(abs(a)) == abs(a));
            REQUIRE(abs(abs(b)) == abs(b));

            // not "really" applying it twice but close enough
            REQUIRE(abs(sint(expanding_abs(a))) == sint(expanding_abs(a)));
            REQUIRE(abs(sint(expanding_abs(b))) == sint(expanding_abs(b)));
        }

        // does not work for int64 as, for some reason, there is no matching abs function for that
        THEN("Computing abs should match its int32 type counterpart")
        {
            REQUIRE(abs(a).word(0) == abs(val_32));
            REQUIRE(expanding_abs(a).word(0) == abs(val_32));
        }
    }
}

SCENARIO("Expanding subtraction works correctly", "[sinteger][arithmetic]")
{
    GIVEN("A n-bit min and a m-bit (m>n)  max")
    {
        static const sinteger<4> min4 = sinteger<4>::min();
        static const sinteger<8> max8 = sinteger<8>::max();
        static const sinteger<8> expected = sinteger<8>{-135};

        THEN("Subtracting max from min should correctly yield (min-max) as it now fits the width")
        {
            auto result = expanding_sub(min4, max8);
            REQUIRE(result == expected);
        }
    }

    GIVEN("A n-bit min and a m-bit (m<n) max")
    {
        static const sinteger<8> min8 = sinteger<8>::min();
        static const sinteger<4> max4 = sinteger<4>::max();
        static const sinteger<8> expected =
            add(sub(sinteger<8>::max(), sinteger<8>(sinteger<4>::max())), sinteger<8>{1U});

        THEN("Subtracting max from min should give (max-min)+1")
        {
            auto const result = expanding_sub(min8, max4);
            REQUIRE(result == expected);
        }
    }
}

SCENARIO("Width casting of signed integers", "[sinteger][utility]")
{
    GIVEN("A positive sinteger")
    {
        sinteger<16> i16{400};
        sinteger<32> i32{400};
        sinteger<150> i150{354346546};

        WHEN("Expanding the width")
        {

            sinteger<24> i16e = width_cast<24>(i16);
            sinteger<50> i32e = width_cast<50>(i32);
            sinteger<200> i150e = width_cast<200>(i150);

            THEN("The numerical value should not have changed")
            {
                CHECK(i16 == i16e);
                CHECK(i32 == i32e);
                REQUIRE(i150 == i150e);
            }
        }
        WHEN("Reducing the width")
        {
            THEN("The first bits are simply dropped")
            {
                sinteger<8> i16r = width_cast<8>(i16);
                sinteger<20> i32r = width_cast<20>(i32);
                sinteger<2> i150r = width_cast<2>(i150);
                CHECK(i16r == sinteger<8>{400 - 256});
                CHECK(i32r == i32);
                CHECK(i150r == sinteger<2>{2});
                //                std::cout << group_digits(to_binary(i150),8) << "\n";
                //                std::cout << group_digits(to_binary(i150r),8) << "\n";
            }
        }
    }
    GIVEN("A negative sinteger")
    {
        sinteger<16> i16{-400};
        sinteger<32> i32{-400};
        sinteger<150> i150{-354346546};

        WHEN("Expanding the width")
        {

            sinteger<24> i16e = width_cast<24>(i16);
            sinteger<50> i32e = width_cast<50>(i32);
            sinteger<200> i150e = width_cast<200>(i150);

            THEN("The numerical value should not have changed")
            {
                CHECK(i16 == i16e);
                CHECK(i32 == i32e);
                REQUIRE(i150 == i150e);
            }
        }
        WHEN("Reducing the width")
        {
            THEN("The first bits are simply dropped")
            {
                sinteger<8> i16r = width_cast<8>(i16);
                sinteger<20> i32r = width_cast<20>(i32);
                sinteger<2> i150r = width_cast<2>(i150);

                CHECK(i16r == sinteger<8>{112});
                CHECK(i32r == i32);
                CHECK(i150r == sinteger<2>{2});
            }
        }
    }
}

SCENARIO("Unary minus operation", "[sinteger][utility]")
{
    GIVEN("The smallest possible value")
    {
        const sinteger<150> min = std::numeric_limits<sinteger<150>>::min();
        THEN("The unary minus value of that value is the value again")
        {
            REQUIRE(-min == min);
        }
    }

    GIVEN("Any non-smallest value")
    {
        using sint = sinteger<64>;
        const int32_t val_32 =
            GENERATE(take(50, random(std::numeric_limits<int32_t>::min() + 1, -1)));
        const int64_t val_64 = GENERATE(
            take(50, random(std::numeric_limits<int64_t>::min() + 1, static_cast<int64_t>(-1))));
        const sint a{val_32};
        const sint b{val_64};

        THEN("Unary minus is self-inverse")
        {
            REQUIRE(-(-a) == a);
            REQUIRE(-(-b) == b);
        }

        // does not work for int64 as, for some reason, there is no matching abs function for that
        THEN("Computing abs should match its int32 type counterpart")
        {
            REQUIRE((-a).word(0) == -val_32);
        }
    }
}

SCENARIO("MIN/MAX Values behave as expected", "[sinteger][utility]")
{
    GIVEN("The min and max value")
    {
        constexpr size_t w = 50;
        sinteger<w> min = sinteger<w>::min();
        sinteger<w> max = sinteger<w>::max();
        sinteger<w> one(1U);
        [[maybe_unused]] sinteger<w> zero(0U);
        THEN("Adding/subtracting one should wrap around")
        {
            REQUIRE(add(max, one) == min);
            REQUIRE(add(max, one) == fun_add(max, one));

            REQUIRE(sub(min, one) == max);
        }

        THEN("Taking the absolute value of the min value yields the same value")
        {
            REQUIRE(abs(min) == min);
        }
    }
}

SCENARIO("Left/right shifting sintegers")
{
    GIVEN("A positive sinteger")
    {
        const sinteger<150> a{0U, 1U, 0U};
        const sinteger<150> b{8, 8, 8};
        const sinteger<150> b_{4, 4, 4};
        const sinteger<150> b__{2, 2, 2};
        const sinteger<150> b___{1, 1, 1};
        WHEN("Right Shfiting")
        {
            THEN("It should behave like division by a power of two")
            {
                //                std::cout << group_digits(to_binary(a),64) << "\n";
                //                std::cout << group_digits(to_binary(b), 64) << "\n";
                //                std::cout << group_digits(to_binary(b >> 1), 64) << "\n";
                //                std::cout << group_digits(to_binary(b >> 2), 64) << "\n";
                //                std::cout << group_digits(to_binary(b >> 4), 64) << "\n";

                REQUIRE((b >> 1) == b_);
                REQUIRE((b >> 2) == b__);
                REQUIRE((b_ >> 1) == b__);
                REQUIRE((b >> 3) == b___);
                REQUIRE((b__ >> 1) == b___);
            }
            THEN("It should move correctly over word boundaries")
            {
                const auto k = (a >> 1);
                const auto b = sinteger<150>(0U, 0U, (uint64_t(1) << 63U));
                //                std::cout << group_digits(to_binary(k), 64) << "\n";
                //                std::cout << group_digits(to_binary(b), 64) << "\n";
                REQUIRE(k == b);
            }

            THEN("The it should also work when moving farther than the word width")
            {
                const sinteger<150> c{12U, 0U, 0U};
                const sinteger c_shifted = c >> 68;
                const auto b = sinteger<150>(0U, 0U, (uint64_t(11) << 62U));
                REQUIRE(c_shifted == b);
                //                std::cout << group_digits(to_binary(c), 64) << "\n";
                //                std::cout << group_digits(to_binary(c_shifted), 64) << "\n";
            }
        }
        WHEN("Left Shifting")
        {
            THEN("It should behave like multiplication by a power of two")
            {

                sinteger<150> _b___ = b___ << 1;
                sinteger<150> __b___ = b___ << 2;
                sinteger<150> ___b___ = b___ << 3;
                REQUIRE(_b___ == b__);
                REQUIRE(__b___ == b_);
                REQUIRE(___b___ == b);
                //                        REQUIRE((b___<<2) == b_);
                //                        REQUIRE((b___<<3) == b);
            }
        }
    }

    GIVEN("The sinteger -1")
    {
        WHEN("Right shifting")
        {
            THEN("-1 should not be affected")
            {
                const sinteger<150> minus_one(-1);
                const sinteger<150> shifted1 = minus_one >> 1;
                const sinteger<150> shifted2 = minus_one >> 22;
                const sinteger<150> shifted3 = minus_one >> 23;
                const sinteger<150> shifted4 = minus_one >> 149;
                const sinteger<150> shifted5 = minus_one >> 150;
                const sinteger<150> shifted6 = minus_one >> 1151;

                CHECK(shifted1 == minus_one);
                CHECK(shifted2 == minus_one);
                REQUIRE(shifted3 == minus_one);
                REQUIRE(shifted4 == minus_one);
                REQUIRE(shifted5 == minus_one);
                REQUIRE(shifted6 == minus_one);
            }
        }
    }
}