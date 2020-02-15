#include <aarith/types/uinteger.hpp>
#include <aarith/utilities/string_utils.hpp>
#include <aarith/operations/approx_operations.hpp>
#include <catch.hpp>
#include <sstream>
#include <iostream>

using namespace aarith;

SCENARIO("Using the FAU adder","[uinteger][arithmetic][approximate]")
{
    const uinteger<8> a{23};
    const uinteger<8> b{1337};

    FAUadder<8,3>(a,b);
}