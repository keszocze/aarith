#include <catch.hpp>

#include <aarith/fixed_point.hpp>

using namespace aarith;

SCENARIO("Constructor and extraction functions work", "[fixed point]")
{
    GIVEN("An integral value")
    {
        unsigned a = 7;
        unsigned long b = 512;
        unsigned long c = std::numeric_limits<unsigned long>::max();

        int l = -5;
        int m = std::numeric_limits<int>::min();
        int n = std::numeric_limits<int>::max();

        THEN("The fixed point number should store it correctly")
        {

            // TODO what happens when an usigned number is added to a signed number?
            ufixed_point<32, 3> ufa{a};
            ufixed_point<64, 2> ufb{b};
            ufixed_point<64, 2> ufc{c};

            fixed_point<32, 5> fl{l};
            fixed_point<32, 5> fm{m};
            fixed_point<32, 5> fn{n};

            fixed_point<33, 2> fk{a};
            fixed_point<65, 2> fo{b};
            fixed_point<65, 2> fp{c};

            //            std::cout << to_binary(ufa) << "\n";
            //            std::cout << to_binary(ufb) << "\n";
            //            std::cout << to_binary(ufc) << "\n";

            //            std::cout << to_binary(fl) << "\n";
            //            std::cout << to_binary(fm) << "\n";
            //            std::cout << to_binary(fn) << "\n";

            //            std::cout << to_binary(fk) << "\n";
            //            std::cout << to_binary(fo) << "\n";
            //            std::cout << to_binary(fp) << "\n";

            REQUIRE(ufa.fractional_part() == uinteger<3>::zero());
            REQUIRE(ufb.fractional_part() == uinteger<2>::zero());
            REQUIRE(ufc.fractional_part() == uinteger<2>::zero());

            REQUIRE(fl.fractional_part() == uinteger<5>::zero());
            REQUIRE(fm.fractional_part() == uinteger<5>::zero());
            REQUIRE(fn.fractional_part() == uinteger<5>::zero());

            REQUIRE(fk.fractional_part() == uinteger<2>::zero());
            REQUIRE(fo.fractional_part() == uinteger<2>::zero());
            REQUIRE(fp.fractional_part() == uinteger<2>::zero());

            REQUIRE(ufa.integer_part() == uinteger<32>{a});
            REQUIRE(ufb.integer_part() == uinteger<64>{b});
            REQUIRE(ufc.integer_part() == uinteger<64>{c});

            REQUIRE(fl.integer_part() == integer<32>{l});
            REQUIRE(fm.integer_part() == integer<32>{m});
            REQUIRE(fn.integer_part() == integer<32>{n});

            REQUIRE(fk.integer_part() == integer<33>{a});
            REQUIRE(fo.integer_part() == integer<65>{b});
            REQUIRE(fp.integer_part() == integer<65>{c});
        }
    }
    GIVEN("The number zero in various bit combinations")
    {
        // TODO move this to the string utils section
        THEN("To binary should give the correct bit string")
        {
            fixed_point<3, 4> zero_3_4;
            fixed_point<6, 4> zero_6_4;
            fixed_point<3, 15> zero_3_15;

            REQUIRE(to_binary(zero_3_4) == "000.0000");
            REQUIRE(to_binary(zero_6_4) == "000000.0000");
            REQUIRE(to_binary(zero_3_15) == "000.000000000000000");

            ufixed_point<3, 4> uzero_3_4;
            ufixed_point<6, 4> uzero_6_4;
            ufixed_point<3, 15> uzero_3_15;

            REQUIRE(to_binary(uzero_3_4) == "000.0000");
            REQUIRE(to_binary(uzero_6_4) == "000000.0000");
            REQUIRE(to_binary(uzero_3_15) == "000.000000000000000");
        }
    }
}