/**
 * Thin Triangle Problem
 *
 * Compute the area of some triangle with coordinates really close to zero.
 *
 * From "Posit Arithmetic", 2017, pp. 75.
 */

#include <aarith/float.hpp>
#include <aarith/integer.hpp>
#include <aarith/posit.hpp>
#include <cmath>
#include <iostream>

using namespace aarith;

template <typename Number>
static void report(const Number& a, const Number& b, const Number& c, const Number& s,
                   const Number& A)
{
    std::cout << std::endl;
    std::cout << "  a=" << a << std::endl;
    std::cout << "  b=" << b << std::endl;
    std::cout << "  c=" << c << std::endl;
    std::cout << "  s=" << s << std::endl;
    std::cout << "  A=" << A << std::endl;
    std::cout << std::endl;
}

template <typename Number>
static void compute_area(const Number& a, const Number& b, const Number& c)
{
    const Number two = static_cast<Number>(2.0);

    const Number s = (a + b + c) / two;
    const Number A = s * (s - a) * (s - b) * (s - c);

    report(a, b, c, s, A);
}

template <typename Number> static void compute_area()
{
    const Number seven = static_cast<Number>(7.0);
    const Number three = static_cast<Number>(3.0);
    const Number two = static_cast<Number>(2.0);

    const auto exponent = integer<32>(-111);
    const Number power = pow(two, exponent);

    const Number a = seven;
    const Number b = seven + three * power;
    const Number c = b;

    compute_area(a, b, c);
}

int main()
{
    /*
    std::cout << "[float]" << std::endl;
    compute_area<float>();

    std::cout << "[double]" << std::endl;
    compute_area<double>();
    */

    std::cout << "[aarith::posit8]" << std::endl;
    compute_area<aarith::posit8>();

    std::cout << "[aarith::posit16]" << std::endl;
    compute_area<aarith::posit16>();

    std::cout << "[aarith::posit32]" << std::endl;
    compute_area<aarith::posit32>();

    std::cout << "[aarith::posit64]" << std::endl;
    compute_area<aarith::posit64>();

    using posit128 = aarith::posit<128, 2>;
    std::cout << "[aarith::posit<128, 2>]" << std::endl;
    compute_area<posit128>();
}
