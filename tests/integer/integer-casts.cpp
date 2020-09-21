#include <aarith/integer_no_operators.hpp>
#include <catch.hpp>

using namespace aarith;

SCENARIO("Up-casting to the next larger native integer type",
         "[integer][unsigned][casting]")
{
    GIVEN("An unsigned integer with 13 bidth")
    {
        WHEN("Casting to an uint16_t")
        {
            THEN("The value should be correct")
            {
                uint16_t val_a = GENERATE(take(5, random(0, 255)));

                val_a += 1000; // shift it to leave the 8 bit range

                uinteger<13> a{val_a};

                REQUIRE(static_cast<uint16_t>(a) == val_a);
                REQUIRE(narrow_cast<uint16_t>(a) == val_a);
            }
        }
    }
}

SCENARIO("Casting unsigned integers to uint8_t", "[integer][unsigned][casting]")
{
    GIVEN("An unsigned integer width bit-width <= 8")
    {

        constexpr uint8_t val = 13;

        constexpr uinteger<8> a{val};
        constexpr uinteger<7> b{val};
        constexpr uinteger<6> c{val};
        constexpr uinteger<5> d{val};

        THEN("The narrow cast function should succeed and yield the stored value")
        {
            constexpr uint8_t a8 = narrow_cast<uint8_t>(a);
            constexpr uint8_t b8 = narrow_cast<uint8_t>(b);
            constexpr uint8_t c8 = narrow_cast<uint8_t>(c);
            constexpr uint8_t d8 = narrow_cast<uint8_t>(d);

            REQUIRE(a8 == val);
            REQUIRE(b8 == val);
            REQUIRE(c8 == val);
            REQUIRE(d8 == val);
        }

        THEN("The conversion operator should succeed and yield the stored value")
        {
            constexpr uint8_t a8 = static_cast<uint8_t>(a);
            constexpr uint8_t b8 = static_cast<uint8_t>(b);
            constexpr uint8_t c8 = static_cast<uint8_t>(c);
            constexpr uint8_t d8 = static_cast<uint8_t>(d);

            REQUIRE(a8 == val);
            REQUIRE(b8 == val);
            REQUIRE(c8 == val);
            REQUIRE(d8 == val);
        }
    }

    GIVEN("An unsigned integer with bit-width > 8")
    {

        constexpr uint16_t min = std::numeric_limits<uint8_t>::min();
        constexpr uint16_t max = std::numeric_limits<uint8_t>::max();

        WHEN("The uinteger value fits a uint8_t")
        {

            const uint16_t val = GENERATE(take(5, random(min, max)));

            uinteger<9> a{val};
            uinteger<16> b{val};
            uinteger<32> c{val};
            uinteger<64> d{val};
            uinteger<150> e{val};

            THEN("The narrow cast function should succeed and yield the stored value")
            {
                uint8_t a8 = narrow_cast<uint8_t>(a);
                uint8_t b8 = narrow_cast<uint8_t>(b);
                uint8_t c8 = narrow_cast<uint8_t>(c);
                uint8_t d8 = narrow_cast<uint8_t>(d);
                uint8_t e8 = narrow_cast<uint8_t>(e);

                REQUIRE(a8 == val);
                REQUIRE(b8 == val);
                REQUIRE(c8 == val);
                REQUIRE(d8 == val);
                REQUIRE(e8 == val);
            }

            THEN("The conversion operator should succeed and yield the stored value")
            {
                uint8_t a8 = static_cast<uint8_t>(a);
                uint8_t b8 = static_cast<uint8_t>(b);
                uint8_t c8 = static_cast<uint8_t>(c);
                uint8_t d8 = static_cast<uint8_t>(d);
                uint8_t e8 = static_cast<uint8_t>(e);

                REQUIRE(a8 == val);
                REQUIRE(b8 == val);
                REQUIRE(c8 == val);
                REQUIRE(d8 == val);
                REQUIRE(e8 == val);
            }
        }

        WHEN("The uinteger values does not fit into a uint8_t")
        {

            THEN("The conversion operator truncates the result")
            {
                const uint16_t val = GENERATE(
                    take(5, random(uint16_t{max + 1}, std::numeric_limits<uint16_t>::max())));

                uinteger<9> a{val};
                uinteger<16> b{val};
                uinteger<32> c{val};
                uinteger<64> d{val};
                uinteger<150> e{val};

                uint8_t a8 = static_cast<uint8_t>(a);
                uint8_t b8 = static_cast<uint8_t>(b);
                uint8_t c8 = static_cast<uint8_t>(c);
                uint8_t d8 = static_cast<uint8_t>(d);
                uint8_t e8 = static_cast<uint8_t>(e);

                uint8_t expected = static_cast<uint8_t>(val);

                REQUIRE(a8 == expected);
                REQUIRE(b8 == expected);
                REQUIRE(c8 == expected);
                REQUIRE(d8 == expected);
                REQUIRE(e8 == expected);
            }

            THEN("The narrow cast function throws an exception")
            {
                const uint16_t val = GENERATE(
                    take(5, random(uint16_t{max + 1}, std::numeric_limits<uint16_t>::max())));

                uinteger<16> b{val};
                uinteger<32> c{val};
                uinteger<64> d{val};
                uinteger<150> e{val};

                CHECK_THROWS_AS(narrow_cast<uint8_t>(b), std::domain_error);
                CHECK_THROWS_AS(narrow_cast<uint8_t>(c), std::domain_error);
                CHECK_THROWS_AS(narrow_cast<uint8_t>(d), std::domain_error);
                CHECK_THROWS_AS(narrow_cast<uint8_t>(e), std::domain_error);
            }
        }
    }
}

SCENARIO("Casting unsigned integers to uint16_t", "[integer][unsigned][casting]")
{
    using B = uint16_t;
    using BL = uint32_t;

    GIVEN("An unsigned integer width bit-width <= 16")
    {

        constexpr B val = 13;

        constexpr uinteger<16> a{val};
        constexpr uinteger<14> b{val};
        constexpr uinteger<8> c{val};
        constexpr uinteger<5> d{val};

        THEN("The narrow cast function should succeed and yield the stored value")
        {
            constexpr uint8_t a8 = narrow_cast<B>(a);
            constexpr uint8_t b8 = narrow_cast<B>(b);
            constexpr uint8_t c8 = narrow_cast<B>(c);
            constexpr uint8_t d8 = narrow_cast<B>(d);

            REQUIRE(a8 == val);
            REQUIRE(b8 == val);
            REQUIRE(c8 == val);
            REQUIRE(d8 == val);
        }

        THEN("The conversion operator should succeed and yield the stored value")
        {
            constexpr B a_ = static_cast<B>(a);
            constexpr B b_ = static_cast<B>(b);
            constexpr B c_ = static_cast<B>(c);
            constexpr B d_ = static_cast<B>(d);

            REQUIRE(a_ == val);
            REQUIRE(b_ == val);
            REQUIRE(c_ == val);
            REQUIRE(d_ == val);
        }
    }

    GIVEN("An unsigned integer with bit-width > 16")
    {

        constexpr uint16_t min = std::numeric_limits<B>::min();
        constexpr uint16_t max = std::numeric_limits<B>::max();

        WHEN("The uinteger value fits a uint8_t")
        {

            const uint16_t val = GENERATE(take(5, random(min, max)));

            uinteger<23> a{val};
            uinteger<16> b{val};
            uinteger<32> c{val};
            uinteger<64> d{val};
            uinteger<150> e{val};

            THEN("The narrow cast function should succeed and yield the stored value")
            {
                B a_ = narrow_cast<B>(a);
                B b_ = narrow_cast<B>(b);
                B c_ = narrow_cast<B>(c);
                B d_ = narrow_cast<B>(d);
                B e_ = narrow_cast<B>(e);

                REQUIRE(a_ == val);
                REQUIRE(b_ == val);
                REQUIRE(c_ == val);
                REQUIRE(d_ == val);
                REQUIRE(e_ == val);
            }

            THEN("The conversion operator should succeed and yield the stored value")
            {
                B a_ = static_cast<B>(a);
                B b_ = static_cast<B>(b);
                B c_ = static_cast<B>(c);
                B d_ = static_cast<B>(d);
                B e_ = static_cast<B>(e);

                REQUIRE(a_ == val);
                REQUIRE(b_ == val);
                REQUIRE(c_ == val);
                REQUIRE(d_ == val);
                REQUIRE(e_ == val);
            }
        }

        WHEN("The uinteger values does not fit into a uint16_t")
        {

            THEN("The conversion operator truncates the result")
            {
                const BL val =
                    GENERATE(take(5, random(BL{max + 1}, std::numeric_limits<BL>::max())));

                uinteger<23> a{val};
                uinteger<16> b{val};
                uinteger<32> c{val};
                uinteger<64> d{val};
                uinteger<150> e{val};

                B a_ = static_cast<B>(a);
                B b_ = static_cast<B>(b);
                B c_ = static_cast<B>(c);
                B d_ = static_cast<B>(d);
                B e_ = static_cast<B>(e);

                B expected = static_cast<B>(val);

                REQUIRE(a_ == expected);
                REQUIRE(b_ == expected);
                REQUIRE(c_ == expected);
                REQUIRE(d_ == expected);
                REQUIRE(e_ == expected);
            }

            THEN("The narrow cast function throws an exception")
            {
                const BL val =
                    GENERATE(take(5, random(BL{max + 1}, std::numeric_limits<BL>::max())));

                uinteger<9> a{val};
                uinteger<16> b{val};
                uinteger<32> c{val};
                uinteger<64> d{val};
                uinteger<150> e{val};

                REQUIRE_NOTHROW(narrow_cast<B>(a));
                REQUIRE_NOTHROW(narrow_cast<B>(b));
                CHECK_THROWS_AS(narrow_cast<uint8_t>(c), std::domain_error);
                CHECK_THROWS_AS(narrow_cast<uint8_t>(d), std::domain_error);
                CHECK_THROWS_AS(narrow_cast<uint8_t>(e), std::domain_error);
            }
        }
    }
}

SCENARIO("Casting unsigned integers with various WordTypes to uint16_t",
         "[integer][unsigned][casting]")
{
    using B = uint16_t;
    using BL = uint32_t;

    GIVEN("An unsigned integer width bit-width <= 16")
    {

        constexpr B val = 13;

        constexpr uinteger<16, uint8_t> a{val};
        constexpr uinteger<14, size_t> b{val};
        constexpr uinteger<8, uint32_t> c{val};
        constexpr uinteger<5> d{val};

        THEN("The narrow cast function should succeed and yield the stored value")
        {
            constexpr uint8_t a8 = narrow_cast<B>(a);
            constexpr uint8_t b8 = narrow_cast<B>(b);
            constexpr uint8_t c8 = narrow_cast<B>(c);
            constexpr uint8_t d8 = narrow_cast<B>(d);

            REQUIRE(a8 == val);
            REQUIRE(b8 == val);
            REQUIRE(c8 == val);
            REQUIRE(d8 == val);
        }

        THEN("The conversion operator should succeed and yield the stored value")
        {
            constexpr B a_ = static_cast<B>(a);
            constexpr B b_ = static_cast<B>(b);
            constexpr B c_ = static_cast<B>(c);
            constexpr B d_ = static_cast<B>(d);

            REQUIRE(a_ == val);
            REQUIRE(b_ == val);
            REQUIRE(c_ == val);
            REQUIRE(d_ == val);
        }
    }

    GIVEN("An unsigned integer with bit-width > 16")
    {

        constexpr uint16_t min = std::numeric_limits<B>::min();
        constexpr uint16_t max = std::numeric_limits<B>::max();

        WHEN("The uinteger value fits a uint8_t")
        {

            const uint16_t val = GENERATE(take(5, random(min, max)));

            uinteger<23> a{val};
            uinteger<16> b{val};
            uinteger<32, uint32_t> c{val};
            uinteger<64> d{val};
            uinteger<150, uint8_t> e{val};

            THEN("The narrow cast function should succeed and yield the stored value")
            {
                B a_ = narrow_cast<B>(a);
                B b_ = narrow_cast<B>(b);
                B c_ = narrow_cast<B>(c);
                B d_ = narrow_cast<B>(d);
                B e_ = narrow_cast<B>(e);

                REQUIRE(a_ == val);
                REQUIRE(b_ == val);
                REQUIRE(c_ == val);
                REQUIRE(d_ == val);
                REQUIRE(e_ == val);
            }

            THEN("The conversion operator should succeed and yield the stored value")
            {
                B a_ = static_cast<B>(a);
                B b_ = static_cast<B>(b);
                B c_ = static_cast<B>(c);
                B d_ = static_cast<B>(d);
                B e_ = static_cast<B>(e);

                REQUIRE(a_ == val);
                REQUIRE(b_ == val);
                REQUIRE(c_ == val);
                REQUIRE(d_ == val);
                REQUIRE(e_ == val);
            }
        }

        WHEN("The uinteger values does not fit into a uint16_t")
        {

            THEN("The conversion operator truncates the result")
            {
                const BL val =
                    GENERATE(take(5, random(BL{max + 1}, std::numeric_limits<BL>::max())));

                uinteger<23> a{val};
                uinteger<530, uint8_t> b{val};
                uinteger<32, uint32_t> c{val};
                uinteger<64> d{val};
                uinteger<150, size_t> e{val};

                B a_ = static_cast<B>(a);
                B b_ = static_cast<B>(b);
                B c_ = static_cast<B>(c);
                B d_ = static_cast<B>(d);
                B e_ = static_cast<B>(e);

                B expected = static_cast<B>(val);

                REQUIRE(a_ == expected);
                REQUIRE(b_ == expected);
                REQUIRE(c_ == expected);
                REQUIRE(d_ == expected);
                REQUIRE(e_ == expected);
            }

            THEN("The narrow cast function throws an exception")
            {
                const BL val =
                    GENERATE(take(5, random(BL{max + 1}, std::numeric_limits<BL>::max())));

                uinteger<32> c{val};
                uinteger<64, uint8_t> d{val};
                uinteger<64> e{val};
                uinteger<64, uint32_t> f{val};

                CHECK_THROWS_AS(narrow_cast<uint8_t>(c), std::domain_error);
                CHECK_THROWS_AS(narrow_cast<uint8_t>(d), std::domain_error);
                CHECK_THROWS_AS(narrow_cast<uint8_t>(e), std::domain_error);
                CHECK_THROWS_AS(narrow_cast<uint8_t>(f), std::domain_error);
            }
        }
    }
}

SCENARIO("Width casting of signed integers", "[integer][signed][utility][casting]")
{
    GIVEN("A positive integer")
    {
        constexpr integer<16> i16{400};
        constexpr integer<32> i32{400};
        constexpr integer<150> i150{354346546};

        WHEN("Expanding the width")
        {

            const integer<24> i16e = width_cast<24>(i16);
            const integer<50> i32e = width_cast<50>(i32);
            const integer<200> i150e = width_cast<200>(i150);

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
                const integer<8> i16r = width_cast<8>(i16);
                const integer<20> i32r = width_cast<20>(i32);
                const integer<2> i150r = width_cast<2>(i150);
                CHECK(i16r == integer<8>{400 - 256});
                CHECK(i32r == i32);
                CHECK(i150r == integer<2>{2});
                //                std::cout << group_digits(to_binary(i150),8) << "\n";
                //                std::cout << group_digits(to_binary(i150r),8) << "\n";
            }
        }
    }
    GIVEN("A negative integer")
    {
        constexpr integer<16> i16{-400};
        constexpr integer<32> i32{-400};
        constexpr integer<150> i150{-354346546};

        WHEN("Expanding the width")
        {

            const integer<24> i16e = width_cast<24>(i16);
            const integer<50> i32e = width_cast<50>(i32);
            const integer<200> i150e = width_cast<200>(i150);

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
                const integer<8> i16r = width_cast<8>(i16);
                const integer<20> i32r = width_cast<20>(i32);
                const integer<2> i150r = width_cast<2>(i150);

                CHECK(i16r == integer<8>{112});
                CHECK(i32r == i32);
                CHECK(i150r == integer<2>{2});
            }
        }
    }
}