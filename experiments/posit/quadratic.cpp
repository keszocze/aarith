/**
 * Solve Quadratic Polynomial
 *
 * Solve quadratic polynomials, i.e.
 *
 *   ax^2 + bc + c
 *
 * with the well known solution
 *
 *   x = (-b +/- sqrt(b^2 - 4ac)) / 2a.
 *
 * From "The End of Error", pp. 98.
 */

#include <cmath>
#include <iostream>

#include <aarith/float.hpp>
#include <aarith/posit.hpp>

#include "support.hpp"

template <typename Number>
static void report(const Number& a, const Number& b, const Number& c, const Number& x1,
                   const Number& x2)
{
    std::cout << std::endl;
    std::cout << "  a=" << a << std::endl;
    std::cout << "  b=" << b << std::endl;
    std::cout << "  c=" << c << std::endl;
    std::cout << "  x1=" << x1 << std::endl;
    std::cout << "  x2=" << x2 << std::endl;
    std::cout << std::endl;
}

template <typename Number> static void solve(const Number& a, const Number& b, const Number& c)
{
    const Number two = static_cast<Number>(2.0);
    const Number four = static_cast<Number>(4.0);

    const Number det = sqrt(b * b - four * a * c);

    const Number x1 = (-b + det) / (two * a);
    const Number x2 = (-b - det) / (two * a);

    report(a, b, c, x1, x2);
}

template <typename Number> static void solve()
{
    const Number a = static_cast<Number>(3.0);
    const Number b = static_cast<Number>(100.0);
    const Number c = static_cast<Number>(2.0);

    solve(a, b, c);
}

int main()
{
    std::cout << "[support::quarter_precision]" << std::endl;
    solve<quarter_precision>();

    std::cout << "[aarith::half_precision]" << std::endl;
    solve<aarith::half_precision>();

    std::cout << "[aarith::single_precision]" << std::endl;
    solve<aarith::single_precision>();

    std::cout << "[aarith::double_precision]" << std::endl;
    solve<aarith::double_precision>();

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

    std::cout << "[aarith::valid8]" << std::endl;
    solve<aarith::valid8>();

    std::cout << "[aarith::valid16]" << std::endl;
    solve<aarith::valid16>();

    std::cout << "[aarith::valid32]" << std::endl;
    solve<aarith::valid32>();

    std::cout << "[aarith::valid64]" << std::endl;
    solve<aarith::valid64>();
}