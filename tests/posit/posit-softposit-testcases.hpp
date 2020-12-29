#include <catch.hpp>

#include <aarith/posit.hpp>

template <size_t N, size_t ES>
static void require_addition(const unsigned lhs, const unsigned rhs, const unsigned expected)
{
    using namespace aarith;

    const posit<N, ES> p(lhs);
    const posit<N, ES> q(rhs);

    const posit<N, ES> actual_posit = p + q;
    const posit<N, ES> expected_posit(expected);

    {
        INFO("  " << dump_string(p));
        INFO("+ " << dump_string(q));
        INFO("  " << to_double(p));
        INFO("+ " << to_double(q));
        INFO("? " << dump_string(actual_posit));
        INFO("! " << dump_string(expected_posit));
        INFO("? " << to_double(actual_posit));
        INFO("! " << to_double(expected_posit));

        REQUIRE(actual_posit == expected_posit);
    }
}

template <size_t N, size_t ES>
static void require_subtraction(const unsigned lhs, const unsigned rhs, const unsigned expected)
{
    using namespace aarith;

    const posit<N, ES> p(lhs);
    const posit<N, ES> q(rhs);

    const posit<N, ES> actual_posit = p - q;
    const posit<N, ES> expected_posit(expected);

    {
        INFO("  " << dump_string(p));
        INFO("- " << dump_string(q));
        INFO("  " << to_double(p));
        INFO("- " << to_double(q));
        INFO("? " << dump_string(actual_posit));
        INFO("! " << dump_string(expected_posit));
        INFO("? " << to_double(actual_posit));
        INFO("! " << to_double(expected_posit));

        REQUIRE(actual_posit == expected_posit);
    }
}
