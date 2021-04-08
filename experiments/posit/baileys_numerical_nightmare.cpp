/**
 * Bailey's Numerical Nightmare
 *
 * Solve a particularly ugly system of linear equations. The expected
 * result is x = -1, y = 2, but floating points give us different
 * results.
 *
 * From "Posit arithmetic", 2017, pp. 98.
 */

#include <aarith/float.hpp>
#include <aarith/posit.hpp>
#include <iostream>

template <typename Number>
static void report(const Number& x, const Number& y)
{
	std::cout << std::endl;
	std::cout << "  x=" << x << std::endl;
	std::cout << "  y=" << y << std::endl;
	std::cout << std::endl;
}

template <typename Number>
static void solve()
{
	// Given system of linear equations
	//
	//   ax + by = u
	//   cx + dy = v
	//
	// we solve with Cramer's rule, viz.
	//
	//   det := ad - bc
	//   x := (ud - bv) / det
	//   y := (av - uc) / det

	const Number a = static_cast<Number>(0.25510582);
	const Number b = static_cast<Number>(0.527461970);
	const Number u = static_cast<Number>(0.79981812);
	const Number c = static_cast<Number>(0.80143857);
	const Number d = static_cast<Number>(1.65707065);
	const Number v = static_cast<Number>(2.51270273);

	const Number det = a * d - b * c;
	const Number x = (u * d - b * v) / det;
	const Number y = (a * v - u * c) / det;

	report(x, y);
}

int main()
{
    std::cout << "[expected]" << std::endl;
    report(-1, 2);

    std::cout << "[aarith::half_precision]" << std::endl;
    solve<aarith::half_precision>();

    std::cout << "[aarith::single_precision]" << std::endl;
    solve<aarith::single_precision>();

    std::cout << "[aarith::double_precision]" << std::endl;
    solve<aarith::double_precision>();

    std::cout << "[aarith::quadruple_precision]" << std::endl;
    solve<aarith::quadruple_precision>();

    std::cout << "[float]" << std::endl;
    solve<float>();

    std::cout << "[double]" << std::endl;
    solve<double>();

    std::cout << "[aarith::posit8]" << std::endl;
    solve<aarith::posit8>();

    std::cout << "[aarith::posit16]" << std::endl;
    solve<aarith::posit16>();

    std::cout << "[aarith::posit32]" << std::endl;
    solve<aarith::posit32>();

    std::cout << "[aarith::posit64]" << std::endl;
    solve<aarith::posit64>();

    using posit128 = aarith::posit<128, 2>;
    std::cout << "[aarith::posit<128, 2>]" << std::endl;
    solve<posit128>();
}
