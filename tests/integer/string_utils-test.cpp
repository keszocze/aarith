#include <aarith/integer.hpp>

#include <catch.hpp>
#include <sstream>

using namespace aarith;

SCENARIO("Converting uintegers into strings", "[uinteger][string]")
{
    const uinteger<16> uint{static_cast<uint16_t>(204)};

    WHEN("Converting a uinteger into a decimal string")
    {
        THEN("The string represents the uinteger")
        {
            REQUIRE(to_decimal(uint) == "204");
        }
        THEN("Using << works as well")
        {
            std::stringstream ss;
            ss << uint;
            REQUIRE(ss.str() == "204");
        }
        AND_WHEN("All numbers from 0 to 65535 are tested.")
        {
            auto i = 0U;
            for(; i < 65536; ++i)
            {
                uinteger<16> res(i);
                if(std::to_string(i) != to_decimal(res))
                {
                    break;
                }
            }
            REQUIRE(i == 65536);
        }
    }
    WHEN("Converting a uinteger into a hexadecimal string")
    {
        THEN("The string represents the uinteger")
        {
            REQUIRE(to_hex(uint) == "00cc");
        }
        THEN("Using << works as well")
        {
            std::stringstream ss;
            ss.setf(std::ios::hex);
            ss << uint;
            REQUIRE(ss.str() == "00cc");
        }
        AND_WHEN("Using a magic number")
        {
            THEN("The number should be correctly transformed")
            {
                const uinteger<8> magic(0b00010000);

                REQUIRE(to_hex(magic) == "10");
            }
        }
    }
    WHEN("Converting a uinteger into an octal string")
    {
        THEN("The string represents the uinteger")
        {
            REQUIRE(to_octal(uint) == "000314");
        }
        THEN("Using << works as well")
        {
            std::stringstream ss;
            ss.setf(std::ios::oct);
            ss << uint;
            REQUIRE(ss.str() == "000314");
        }
        AND_WHEN("Using a magic number")
        {
            THEN("The number should be correctly transformed")
            {
                const uinteger<8> magic(0b00010000);

                REQUIRE(remove_leading_zeroes(to_octal(magic)) == "20");
            }
        }
    }
}

SCENARIO("Converting sintegers into strings", "[sinteger][string]")
{
    const sinteger<16> num{204};

    WHEN("Converting a sinteger into a decimal string")
    {
        THEN("The string represents the sinteger, both normal and in negative version")
        {
            REQUIRE(to_decimal(num) == "204");
            REQUIRE(to_decimal(-num) == "-204");
        }
        THEN("Using << works as well")
        {
            std::stringstream ss;
            ss << num;
            REQUIRE(ss.str() == "204");

            ss.str("");
            ss << -num;
            REQUIRE(ss.str() == "-204");
        }
    }
    WHEN("Converting a sinteger into a hexadecimal string")
    {
        THEN("The string represents the sinteger, both normal and in negative version")
        {
            REQUIRE(to_hex(num) == "00cc");
            REQUIRE(to_hex(-num) == "-00cc");
        }
        THEN("Using << works as well")
        {
            std::stringstream ss;
            ss.setf(std::ios::hex);
            ss << num;
            REQUIRE(ss.str() == "00cc");

            ss.str("");
            ss << -num;
            REQUIRE(ss.str() == "-00cc");
        }
    }
    WHEN("Converting a sinteger into an octal string")
    {
        THEN("The string represents the sinteger, both normal and in negative version")
        {
            REQUIRE(to_octal(num) == "000314");
            REQUIRE(to_octal(-num) == "-000314");
        }
        THEN("Using << works as well")
        {
            std::stringstream ss;
            ss.setf(std::ios::oct);
            ss << num;
            REQUIRE(ss.str() == "000314");

            ss.str("");
            ss << -num;
            REQUIRE(ss.str() == "-000314");
        }
    }
}

SCENARIO("Grouping digits in string representations", "[utilities][string]")
{
    GIVEN("group_digits is called")
    {
        WHEN("The string length is divisible by the group size")
        {
            THEN("The grouping is correct")
            {
                REQUIRE(group_digits("100000", 3, '.') == "100.000");
            }
        }
        WHEN("The string length is not divisible by the group size")
        {
            THEN("The grouping is correct")
            {
                REQUIRE(group_digits("1000", 3, '.') == "1.000");
            }
        }
        WHEN("The string length is smaller than the group size")
        {
            THEN("There is no additional grouping")
            {
                REQUIRE(group_digits("100", 3, '.') == "100");
            }
        }
    }
}

SCENARIO("Removing leading zeroes from string representations", "[utilities][string]")
{
    GIVEN("remove_leading_zeroes() is called")
    {
        WHEN("The string is all 0s")
        {
            AND_WHEN("can_be_empty is true")
            {
                THEN("The empty string is returned")
                {
                    REQUIRE(remove_leading_zeroes("000", true) == "");
                }
            }
            AND_WHEN("can_be_empty is false")
            {
                THEN("\"0\" is returned")
                {
                    REQUIRE(remove_leading_zeroes("000", false) == "0");
                }
            }
        }
        WHEN("The string is not all 0s")
        {
            THEN("All leading zeroes are removed")
            {
                REQUIRE(remove_leading_zeroes("00010") == "10");
            }
        }
    }
}
