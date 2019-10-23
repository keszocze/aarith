#include <catch2/catch.hpp>
#include <integer.hpp>
#include <iostream>

TEST_CASE("reject signed numbers")
{
    CHECK(0 == 1);
}

// for static_assert tests:
// https://stackoverflow.com/questions/30155619/expected-build-failure-tests-in-cmake