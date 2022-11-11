#include <catch.hpp>

#include <aarith/posit.hpp>
#include <cstdint>
#include <functional>

using namespace aarith;

template <size_t N, size_t ES>
using Op = std::function<posit<N, ES>(const posit<N, ES>&, const posit<N, ES>&)>;

template <size_t N, size_t ES>
static void require_arithmetic(const uint32_t lhs, const uint32_t rhs, const uint32_t expected,
                               const Op<N, ES>& operation)
{
    const posit<N, ES> p = posit<N, ES>::from(lhs);
    const posit<N, ES> q = posit<N, ES>::from(rhs);

    const posit<N, ES> actual_posit = operation(p, q);
    const posit<N, ES> expected_posit = posit<N, ES>::from(expected);

    {
        INFO("  " << dump_string(p));
        INFO("& " << dump_string(q));
        INFO("  " << double(p));
        INFO("& " << double(q));
        INFO("? " << dump_string(actual_posit));
        INFO("! " << dump_string(expected_posit));
        INFO("? " << double(actual_posit));
        INFO("! " << double(expected_posit));

        REQUIRE(actual_posit == expected_posit);
    }
}

template <size_t N, size_t ES>
static void require_addition(const uint32_t lhs, const uint32_t rhs, const uint32_t expected)
{
    static const Op<N, ES> operation = [](auto x, auto y) { return x + y; };
    require_arithmetic(lhs, rhs, expected, operation);
}

template <size_t N, size_t ES>
static void require_subtraction(const uint32_t lhs, const uint32_t rhs, const uint32_t expected)
{
    static const Op<N, ES> operation = [](auto x, auto y) { return x - y; };
    require_arithmetic(lhs, rhs, expected, operation);
}

template <size_t N, size_t ES>
static void require_multiplication(const uint32_t lhs, const uint32_t rhs, const uint32_t expected)
{
    static const Op<N, ES> operation = [](auto x, auto y) { return x * y; };
    require_arithmetic(lhs, rhs, expected, operation);
}

template <size_t N, size_t ES>
static void require_division(const uint32_t lhs, const uint32_t rhs, const uint32_t expected)
{
    static const Op<N, ES> operation = [](auto x, auto y) { return x / y; };
    require_arithmetic(lhs, rhs, expected, operation);
}
