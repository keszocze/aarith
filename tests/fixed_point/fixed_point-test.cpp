#include <catch.hpp>

#include <aarith/fixed_point.hpp>

using namespace aarith;

SCENARIO("Constructor and extraction functions work", "[fixed point]")
{

    GIVEN("Random integral values")
    {
        constexpr size_t N = 5;
        int64_t n64 = GENERATE(take(
            20, random(std::numeric_limits<int64_t>::min(), std::numeric_limits<int64_t>::max())));
        int32_t n32 = GENERATE(take(
            20, random(std::numeric_limits<int32_t>::min(), std::numeric_limits<int32_t>::max())));
        uint32_t un32 = GENERATE(take(N, random(std::numeric_limits<uint32_t>::min(),
                                                 std::numeric_limits<uint32_t>::max())));
        uint64_t un64 = GENERATE(take(N, random(std::numeric_limits<uint64_t>::min(),
                                                 std::numeric_limits<uint64_t>::max())));

        WHEN("Using the constructor taking native data types")
        {

            THEN("The fixed point number should store it correctly")
            {
                ufixed_point<32, 4> uf32{un32};
                ufixed_point<64, 4> uf64{un64};

                fixed_point<32, 4> f32{n32};
                fixed_point<64, 4> f64{n64};

                fixed_point<33, 4> f33{un32};
                fixed_point<65, 4> f65{un64};

                REQUIRE(uf32.fractional_part() == uinteger<4>::zero());
                REQUIRE(uf64.fractional_part() == uinteger<4>::zero());
                REQUIRE(f32.fractional_part() == uinteger<4>::zero());
                REQUIRE(f64.fractional_part() == uinteger<4>::zero());
                REQUIRE(f33.fractional_part() == uinteger<4>::zero());
                REQUIRE(f65.fractional_part() == uinteger<4>::zero());

                REQUIRE(uf32.integer_part() == uinteger<32>{un32});
                REQUIRE(uf64.integer_part() == uinteger<64>{un64});

                REQUIRE(f32.integer_part() == integer<32>{n32});
                REQUIRE(f64.integer_part() == integer<64>{n64});
            }
        }
        WHEN("Using the constructor taking native data types")
        {
            THEN("The fixed point number should store it correctly")
            {

                uinteger<32> ui32 = uinteger<32>{un32};
                uinteger<64> ui64 = uinteger<64>{un64};

                integer<32> i32 = integer<32>{n32};
                integer<64> i64 = integer<64>{n64};

                ufixed_point<32, 4> uf32{un32};
                ufixed_point<64, 4> uf64{un64};

                fixed_point<32, 4> f32{n32};
                fixed_point<64, 4> f64{n64};

                fixed_point<33, 4> f33{un32};
                fixed_point<65, 4> f65{un64};

                REQUIRE(uf32.fractional_part() == uinteger<4>::zero());
                REQUIRE(uf64.fractional_part() == uinteger<4>::zero());
                REQUIRE(f32.fractional_part() == uinteger<4>::zero());
                REQUIRE(f64.fractional_part() == uinteger<4>::zero());
                REQUIRE(f33.fractional_part() == uinteger<4>::zero());
                REQUIRE(f65.fractional_part() == uinteger<4>::zero());

                REQUIRE(uf32.integer_part() == ui32);
                REQUIRE(uf64.integer_part() == ui64);

                REQUIRE(f32.integer_part() == i32);
                REQUIRE(f64.integer_part() == i64);
            }
        }
    }

    GIVEN("A fixed unsigned aarith integer")
    {
        THEN("It can be used to directly set the value of the fixed point number")
        {
            unsigned a_ = 0b1000100;
            unsigned int_part_ = 0b1000;
            unsigned frac_part_ = 0b100;
            uinteger<32> a{a_};
            uinteger<32> int_part{int_part_};
            uinteger<3> frac_part{frac_part_};
            ufixed_point<32, 3> fa = ufixed_point<32, 3>::from_bitstring(a);

            REQUIRE(fa.integer_part() == int_part);
            REQUIRE(fa.fractional_part() == frac_part);
        }
    }
}

SCENARIO("Width casting fixed point numbers")
{
    GIVEN("A fixed point number")
    {

        unsigned u_ = 0b1001100100;
        int s_ = -255;
        fixed_point<32, 4> s{s_};
        auto u = ufixed_point<32, 4>::from_bitstring(uinteger<32>{u_});

        WHEN("Increasing the the integer part width")
        {
            const auto s_cast = width_cast<40, 4>(s);
            const auto u_cast = width_cast<33, 4>(u);

            THEN("The result should be a correctly expanded fixed point number")
            {
//                std::cout << to_binary(s) << "\n"
//                          << to_binary(s_cast) << "\n"
//                          << s_cast.width() << "," << s_cast.int_width() << ","
//                          << s_cast.frac_width() << "\n";
//                std::cout << to_binary(u) << "\n"
//                          << to_binary(u_cast) << "\n"
//                          << u_cast.width() << "," << u_cast.int_width() << ","
//                          << u_cast.frac_width() << "\n";
                REQUIRE(s_cast.int_width() == 40);
                REQUIRE(u_cast.int_width() == 33);
                REQUIRE(s.frac_width() == s_cast.frac_width());
                REQUIRE(u.frac_width() == u_cast.frac_width());
            }
        }

        WHEN("Increasing the the fraction part width")
        {
            const auto s_cast = width_cast<32, 8>(s);
            const auto u_cast = width_cast<32, 8>(u);

            THEN("The result should be a correctly expanded fixed point number")
            {
//                std::cout << to_binary(s) << "\n"
//                          << to_binary(s_cast) << "\n"
//                          << s_cast.width() << "," << s_cast.int_width() << ","
//                          << s_cast.frac_width() << "\n";
//                std::cout << to_binary(u) << "\n"
//                          << to_binary(u_cast) << "\n"
//                          << u_cast.width() << "," << u_cast.int_width() << ","
//                          << u_cast.frac_width() << "\n";
                REQUIRE(s.int_width() == s_cast.int_width());
                REQUIRE(u.int_width() == u_cast.int_width());
                REQUIRE(s_cast.frac_width() == 8);
                REQUIRE(u_cast.frac_width() == 8);
            }
        }

        WHEN("Decreasing the the integer part width")
        {
            const auto s_cast = width_cast<10, 4>(s);
            const auto u_cast = width_cast<10, 4>(u);

            THEN("The result should be a correctly shrunk fixed point number")
            {
//                std::cout << to_binary(s) << "\n"
//                          << to_binary(s_cast) << "\n"
//                          << s_cast.width() << "," << s_cast.int_width() << ","
//                          << s_cast.frac_width() << "\n";
//                std::cout << to_binary(u) << "\n"
//                          << to_binary(u_cast) << "\n"
//                          << u_cast.width() << "," << u_cast.int_width() << ","
//                          << u_cast.frac_width() << "\n";
                REQUIRE(s_cast.int_width() == 10);
                REQUIRE(u_cast.int_width() == 10);
                REQUIRE(s.frac_width() == s_cast.frac_width());
                REQUIRE(u.frac_width() == u_cast.frac_width());
            }
        }

        WHEN("Decreasing the the fraction part width")
        {
            const auto s_cast = width_cast<32, 2>(s);
            const auto u_cast = width_cast<32, 2>(u);

            THEN("The result should be a correctly shrunk fixed point number")
            {
//                std::cout << to_binary(s) << "\n"
//                          << to_binary(s_cast) << "\n"
//                          << s_cast.width() << "," << s_cast.int_width() << ","
//                          << s_cast.frac_width() << "\n";
//                std::cout << to_binary(u) << "\n"
//                          << to_binary(u_cast) << "\n"
//                          << u_cast.width() << "," << u_cast.int_width() << ","
//                          << u_cast.frac_width() << "\n";
                REQUIRE(s.int_width() == s_cast.int_width());
                REQUIRE(u.int_width() == u_cast.int_width());
                REQUIRE(s_cast.frac_width() == 2);
                REQUIRE(u_cast.frac_width() == 2);
            }
        }

        WHEN("Both widths are decreased")
        {
            const auto s_cast = width_cast<10, 2>(s);
            const auto u_cast = width_cast<10, 2>(u);

            THEN("The result should be a correctly shrunk fixed point number")
            {
//                std::cout << to_binary(s) << "\n"
//                          << to_binary(s_cast) << "\n"
//                          << s_cast.width() << "," << s_cast.int_width() << ","
//                          << s_cast.frac_width() << "\n";
//                std::cout << to_binary(u) << "\n"
//                          << to_binary(u_cast) << "\n"
//                          << u_cast.width() << "," << u_cast.int_width() << ","
//                          << u_cast.frac_width() << "\n";
                REQUIRE(s_cast.int_width() == 10);
                REQUIRE(u_cast.int_width() == 10);
                REQUIRE(s_cast.frac_width() == 2);
                REQUIRE(u_cast.frac_width() == 2);
            }
        }

        WHEN("Both widths are increased")
        {
            const auto s_cast = width_cast<35, 5>(s);
            const auto u_cast = width_cast<35, 5>(u);

            THEN("The result should be a correctly expanded fixed point number")
            {
//                std::cout << to_binary(s) << "\n"
//                          << to_binary(s_cast) << "\n"
//                          << s_cast.width() << "," << s_cast.int_width() << ","
//                          << s_cast.frac_width() << "\n";
//                std::cout << to_binary(u) << "\n"
//                          << to_binary(u_cast) << "\n"
//                          << u_cast.width() << "," << u_cast.int_width() << ","
//                          << u_cast.frac_width() << "\n";
                REQUIRE(s_cast.int_width() == 35);
                REQUIRE(u_cast.int_width() == 35);
                REQUIRE(s_cast.frac_width() == 5);
                REQUIRE(u_cast.frac_width() == 5);
            }
        }
    }
}

SCENARIO("Bit shifting fixed point numbers")
{
    GIVEN("A fixed point number")
    {
        unsigned u_ = 0b1000100;
        int s_ = -255;
        fixed_point<32, 4> s{s_};
        ufixed_point<32, 4> u{u_};

        WHEN("Shifting left")
        {
            THEN("The result is a correctly shifted fixed point number")
            {
                // TODO das hier mit constexpr zum laufen bekommen, dazu aber erst noch die
                // entsprechenden Konstruktoren bauen
                //                                constexpr auto shift1 = s << 5;
                //                constexpr auto shift2 = u << 5;
//                std::cout << to_binary(s) << "\t" << to_binary(s << 5) << "\n";
//                std::cout << to_binary(u) << "\t" << to_binary(u << 5) << "\n";
            }
        }
        WHEN("Shifting right")
        {
            THEN("The result is a correctly shifted fixed point number")
            {
//                std::cout << to_binary(u) << "\t" << to_binary(u >> 5) << "\n";
//                std::cout << to_binary(u) << "\t" << to_binary(u >> 4) << "\n";
//                std::cout << to_binary(s) << "\t" << to_binary(s >> 5) << "\n";
//                std::cout << to_binary(s) << "\t" << to_binary(s >> 4) << "\n";
            }
        }
    }
}

// SCENARIO("Computing the min/max of two fixed point numbers")
//{
//    GIVEN("Two fixed point numbers")
//    {
//        int64_t n64 = GENERATE(take(
//            20, random(std::numeric_limits<int64_t>::min(),
//            std::numeric_limits<int64_t>::max())));
//        int32_t n32 = GENERATE(take(
//            20, random(std::numeric_limits<int32_t>::min(),
//            std::numeric_limits<int32_t>::max())));
//        fixed_point<32, 4> f32{n32};
//        fixed_point<64, 4> f64{n64};
//
//        WHEN("Computing the min/max of both of them")
//        {
//
//            auto fmin = min(f32,f64);
//
//
//            THEN("The correct value should be chosen")
//            {
//                fixed_point<64,4>
//            }
//        }
//    }
//}