#include <catch.hpp>

#include <aarith/posit.hpp>
#include <cstdint>

template <size_t N, size_t ES>
static void require_addition(const uint32_t lhs, const uint32_t rhs, const uint32_t expected)
{
    using namespace aarith;

    const posit<N, ES> p = posit<N, ES>::from(lhs);
    const posit<N, ES> q = posit<N, ES>::from(rhs);

    const posit<N, ES> actual_posit = p + q;
    const posit<N, ES> expected_posit = posit<N, ES>::from(expected);

    {
        INFO("  " << dump_string(p));
        INFO("+ " << dump_string(q));
        INFO("  " << double(p));
        INFO("+ " << double(q));
        INFO("? " << dump_string(actual_posit));
        INFO("! " << dump_string(expected_posit));
        INFO("? " << double(actual_posit));
        INFO("! " << double(expected_posit));

        REQUIRE(actual_posit == expected_posit);
    }
}

template <size_t N, size_t ES>
static void require_subtraction(const uint32_t lhs, const uint32_t rhs, const uint32_t expected)
{
    using namespace aarith;

    const posit<N, ES> p = posit<N, ES>::from(lhs);
    const posit<N, ES> q = posit<N, ES>::from(rhs);

    const posit<N, ES> actual_posit = p - q;
    const posit<N, ES> expected_posit = posit<N, ES>::from(expected);

    {
        INFO("  " << dump_string(p));
        INFO("- " << dump_string(q));
        INFO("  " << double(p));
        INFO("- " << double(q));
        INFO("? " << dump_string(actual_posit));
        INFO("! " << dump_string(expected_posit));
        INFO("? " << double(actual_posit));
        INFO("! " << double(expected_posit));

        REQUIRE(actual_posit == expected_posit);
    }
}

template <size_t N, size_t ES>
static void require_multiplication(const uint32_t lhs, const uint32_t rhs, const uint32_t expected)
{
    using namespace aarith;
    std::cout << std::endl;


    const posit<N, ES> p = posit<N, ES>::from(lhs);
    const posit<N, ES> q = posit<N, ES>::from(rhs);

    const posit<N, ES> actual_posit = p * q;
    const posit<N, ES> expected_posit = posit<N, ES>::from(expected);

    static int i;

    {
        INFO("i " << i++);
        INFO("  " << dump_string(p));
        INFO("× " << dump_string(q));
        INFO("  " << double(p));
        INFO("× " << double(q));
        INFO("? " << dump_string(actual_posit));
        INFO("! " << dump_string(expected_posit));
        INFO("? " << double(actual_posit));
        INFO("! " << double(expected_posit));

        REQUIRE(actual_posit == expected_posit);
    }
}
