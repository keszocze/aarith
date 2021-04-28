/*
 * Iteration
 *
 * [Problem 1] from Unum Literature. We iterate on some formula. 32 Bit Floats
 * fail to give the proper result, Type I Unums get it right.
 *
 * From The End of Error, pp. 173.
 */

#include <iostream>
#include <vector>

#include <aarith/float.hpp>
#include <aarith/posit.hpp>

template <typename Number> static void report(const Number& x, const int iteration)
{
    std::cout << "  i=" << iteration << " x=" << x << std::endl;
}

template <typename Number>
static std::vector<Number> iterate(const Number& x0, const Number& x1, const int niterations)
{
    Number xcur = x0;
    Number xnext = x1;

    std::vector<Number> xs;

    xs.push_back(xcur);
    xs.push_back(xnext);

    for (int i = 0; i < niterations; ++i)
    {
        const Number xnextnext =
            Number(111.0) - Number(1130.0) / xnext + Number(3000.0) / (xcur * xnext);
        xs.push_back(xnextnext);

        xcur = xnext;
        xnext = xnextnext;
    }

    return xs;
}

template <typename Number> static std::vector<Number> iterate(const int niterations)
{
    const Number x0 = static_cast<Number>(2.0);
    const Number x1 = static_cast<Number>(-4.0);

    return iterate(x0, x1, niterations);
}

int main()
{
    using posit128 = aarith::posit<128, 2>;
    constexpr int niterations = 64;

    const auto f16 = iterate<aarith::half_precision>(niterations);
    const auto f32 = iterate<aarith::single_precision>(niterations);
    const auto f64 = iterate<aarith::double_precision>(niterations);
    const auto f128 = iterate<aarith::quadruple_precision>(niterations);

    const auto p16 = iterate<aarith::posit16>(niterations);
    const auto p32 = iterate<aarith::posit32>(niterations);
    const auto p64 = iterate<aarith::posit64>(niterations);
    const auto p128 = iterate<posit128>(niterations);

    std::cout << "i;";
    std::cout << "f16;";
    std::cout << "f32;";
    std::cout << "f64;";
    std::cout << "f128;";
    std::cout << "p16;";
    std::cout << "p32;";
    std::cout << "p64;";
    std::cout << "p128;";
    std::cout << "\n";

    for (int i = 0; i < niterations; ++i)
    {
        std::cout << i << ";";
        std::cout << f16.at(i) << ";";
        std::cout << f32.at(i) << ";";
        std::cout << f64.at(i) << ";";
        std::cout << f128.at(i) << ";";
        std::cout << p16.at(i) << ";";
        std::cout << p32.at(i) << ";";
        std::cout << p64.at(i) << ";";
        std::cout << p128.at(i) << ";";
        std::cout << "\n";
    }

    std::cout << std::endl;
}
