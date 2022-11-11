#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"

TEMPLATE_TEST_CASE_SIG("pow special cases", "[posit][template]", ((size_t N, size_t ES), N, ES),
                       AARITH_POSIT_TEST_TEMPLATE_BIG)
{
    using namespace aarith;

    using Posit = posit<N, ES>;
    using Int = integer<32>;

    const Posit pquart = static_cast<Posit>(0.25);
    const Posit phalf = static_cast<Posit>(0.5);
    const Posit pzero = static_cast<Posit>(0);
    const Posit pone = static_cast<Posit>(1);
    const Posit ptwo = static_cast<Posit>(2);
    const Posit pfour = static_cast<Posit>(4);

    const Int izero = static_cast<Int>(0);
    const Int ione = static_cast<Int>(1);
    const Int itwo = static_cast<Int>(2);

    const auto nar = Posit::nar();

    REQUIRE(pow(pzero, -itwo) == nar);
    REQUIRE(pow(pzero, -ione) == nar);
    REQUIRE(pow(pzero, izero) == nar);
    REQUIRE(pow(pzero, ione) == pzero);
    REQUIRE(pow(pzero, itwo) == pzero);

    REQUIRE(pow(pone, -itwo) == pone);
    REQUIRE(pow(pone, -ione) == pone);
    REQUIRE(pow(pone, izero) == pone);
    REQUIRE(pow(pone, ione) == pone);
    REQUIRE(pow(pone, itwo) == pone);

    REQUIRE(pow(ptwo, -itwo) == pquart);
    REQUIRE(pow(ptwo, -ione) == phalf);
    REQUIRE(pow(ptwo, izero) == pone);
    REQUIRE(pow(ptwo, ione) == ptwo);
    REQUIRE(pow(ptwo, itwo) == pfour);

    REQUIRE(pow(-pzero, -itwo) == nar);
    REQUIRE(pow(-pzero, -ione) == nar);
    REQUIRE(pow(-pzero, izero) == nar);
    REQUIRE(pow(-pzero, ione) == pzero);
    REQUIRE(pow(-pzero, itwo) == pzero);

    REQUIRE(pow(-pone, -itwo) == pone);
    REQUIRE(pow(-pone, -ione) == -pone);
    REQUIRE(pow(-pone, izero) == pone);
    REQUIRE(pow(-pone, ione) == -pone);
    REQUIRE(pow(-pone, itwo) == pone);

    REQUIRE(pow(-ptwo, -itwo) == pquart);
    REQUIRE(pow(-ptwo, -ione) == -phalf);
    REQUIRE(pow(-ptwo, izero) == pone);
    REQUIRE(pow(-ptwo, ione) == -ptwo);
    REQUIRE(pow(-ptwo, itwo) == pfour);
}
