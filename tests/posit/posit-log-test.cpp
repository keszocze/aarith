#include <catch.hpp>

#include <aarith/posit.hpp>
#include <sstream>
#include <string>

#include "../test-signature-ranges.hpp"

template <typename Posit> static std::string to_string(const Posit& p)
{
    std::stringstream ss;
    ss << p;
    return ss.str();
}

template <typename Posit> static bool starts_with(const std::string& needle, const Posit& hay)
{
    const std::string haystr = to_string(hay);
    return (haystr.rfind(needle, 0) == 0);
}

SCENARIO("Check if log returns correct results")
{
    using namespace aarith;

    GIVEN("Some arbitrary posits")
    {
        THEN("Assert that log terminates and returns correct results")
        {
            REQUIRE(starts_with("0.693147", log(posit32(2))));
            REQUIRE(starts_with("1.098612", log(posit32(3))));
            REQUIRE(starts_with("1.386294", log(posit32(4))));
            REQUIRE(starts_with("1.609437", log(posit32(5))));
            REQUIRE(starts_with("1.791759", log(posit32(6))));
            REQUIRE(starts_with("1.945910", log(posit32(7))));
            REQUIRE(starts_with("2.079441", log(posit32(8))));
            REQUIRE(starts_with("2.197224", log(posit32(9))));
        }
    }
}

TEMPLATE_TEST_CASE_SIG("log special cases", "[posit][template]", ((size_t N, size_t ES), N, ES),
                       AARITH_POSIT_TEST_TEMPLATE_EXHAUSTABLE)
{
    using namespace aarith;
    using Posit = posit<N, ES>;

    const Posit nar = Posit::nar();
    REQUIRE(log(nar) == nar);

    const Posit zero = Posit::zero();
    REQUIRE(log(zero) == nar);

    const Posit one = Posit::one();
    REQUIRE(log(one) == zero);

    const Posit none = -Posit::one();
    REQUIRE(log(none) == nar);

    const Posit min = Posit::min();
    REQUIRE(log(min) == nar);
}
