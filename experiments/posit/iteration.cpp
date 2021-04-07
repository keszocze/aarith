/*
 * Iteration
 *
 * Problem 1.1 from Unum Literature. We iterate on some formula. 32 Bit Floats
 * fail to give the proper result, Type I Unums get it right.
 *
 * From The End of Error, pp. 173.
 */

#include <aarith/float.hpp>
#include <aarith/posit.hpp>
#include <iostream>

template <typename Number> static void report(const Number& x, const int iteration)
{
    std::cout << "  i=" << iteration << " x=" << x << std::endl;
}

template <typename Number>
static void iterate(const Number& x0, const Number& x1, const int niterations)
{
    Number xcur = x0;
    Number xnext = x1;

    std::cout << std::endl;
    report(xcur, 0);
    report(xnext, 1);

    for (int i = 0; i < niterations; ++i)
    {
        const Number xnextnext =
            Number(111.0) - Number(1130.0) / xnext + Number(3000.0) / (xcur * xnext);
        report(xnextnext, i);

        xcur = xnext;
        xnext = xnextnext;
    }

    std::cout << std::endl;
}

template <typename Number> static void iterate()
{
    const Number x0 = static_cast<Number>(2.0);
    const Number x1 = static_cast<Number>(-4.0);
    iterate(x0, x1, 128);
}

int main()
{
    std::cout << "[aarith::half_precision]" << std::endl;
    iterate<aarith::half_precision>();

    std::cout << "[aarith::single_precision]" << std::endl;
    iterate<aarith::single_precision>();

    std::cout << "[aarith::double_precision]" << std::endl;
    iterate<aarith::double_precision>();

    std::cout << "[aarith::quadruple_precision]" << std::endl;
    iterate<aarith::quadruple_precision>();

    std::cout << "[aarith::posit8]" << std::endl;
    iterate<aarith::posit8>();

    std::cout << "[aarith::posit16]" << std::endl;
    iterate<aarith::posit16>();

    std::cout << "[aarith::posit32]" << std::endl;
    iterate<aarith::posit32>();

    std::cout << "[aarith::posit64]" << std::endl;
    iterate<aarith::posit64>();

    using posit128 = aarith::posit<128, 2>;
    std::cout << "[aarith::posit<128, 2>]" << std::endl;
    iterate<posit128>();
}
