#include <catch.hpp>

#include <aarith/posit.hpp>

using namespace aarith;

template <size_t N, size_t ES, typename WT>
static void require_mult(const posit<N, ES, WT>& px, const bound_sign sx,
                         const posit<N, ES, WT>& py, const bound_sign sy,
                         const posit<N, ES, WT>& expected_value, bound_sign expected_sign)
{
    using Bound = bound<N, ES, WT>;

    const Bound x = Bound(px, sx);
    const Bound y = Bound(py, sy);
    const Bound xy = x * y;

    CAPTURE(x, y, xy);
    REQUIRE(xy.get_value() == expected_value);
    REQUIRE(xy.get_sign() == expected_sign);
}

SCENARIO("Bound Multiplication")
{
    GIVEN("Bound Type")
    {
        constexpr auto exact = bound_sign::EXACT;
        constexpr auto plus = bound_sign::PLUS_EPS;
        constexpr auto minus = bound_sign::MINUS_EPS;
        constexpr auto unsure = bound_sign::UNSURE;

        using Posit = posit<8, 2>;

        const Posit zero = Posit::zero();
        const Posit one = Posit::one();

        THEN("Ensure that multiplication with value 1 behaves as expected.")
        {
            require_mult(one, exact, one, exact, one, exact);
            require_mult(one, plus, one, exact, one, plus);
            require_mult(one, minus, one, exact, one, minus);
            require_mult(one, exact, one, plus, one, plus);
            require_mult(one, plus, one, plus, one, plus);
            require_mult(one, minus, one, plus, one, unsure);
            require_mult(one, exact, one, minus, one, minus);
            require_mult(one, plus, one, minus, one, unsure);
            require_mult(one, minus, one, minus, one, unsure);
        }

        THEN("Ensure that multiplication with value -1 behaves as expected.")
        {
            require_mult(-one, exact, one, exact, -one, exact);
            require_mult(-one, plus, one, exact, -one, plus);
            require_mult(-one, minus, one, exact, -one, minus);
            require_mult(-one, exact, one, plus, -one, minus);
            require_mult(-one, plus, one, plus, -one, unsure);
            require_mult(-one, minus, one, plus, -one, unsure);
            require_mult(-one, exact, one, minus, -one, plus);
            require_mult(-one, plus, one, minus, -one, unsure);
            require_mult(-one, minus, one, minus, -one, unsure);
        }

        THEN("Ensure that multiplication with value 0 behaves as expected.")
        {
            require_mult(zero, exact, one, exact, zero, exact);
            require_mult(zero, plus, one, exact, zero, plus);
            require_mult(zero, minus, one, exact, zero, minus);
            require_mult(zero, exact, one, plus, zero, exact);
            require_mult(zero, plus, one, plus, zero, plus);
            require_mult(zero, minus, one, plus, zero, unsure);
            require_mult(zero, exact, one, minus, zero, exact);
            require_mult(zero, plus, one, minus, zero, unsure);
            require_mult(zero, minus, one, minus, zero, unsure);
        }
    }
}
