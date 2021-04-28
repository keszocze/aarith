/*
 * Iteration
 *
 * [Problem 1] from Unum Literature. We iterate on some formula. 32 Bit Floats
 * fail to give the proper result, Type I Unums get it right.
 *
 * From The End of Error, pp. 173.
 */

#include <iostream>
#include <stdexcept>
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
        // Depending on the current state of the valid implementation, some operations
        // will throw exceptions if Number is a valid type. This is why we are extra
        // careful and wrap the iteration step in a try/catch block. If an exception is
        // thrown, xnextnext will be recorded as simply zero.

        Number xnextnext = Number::zero();

        try
        {
            xnextnext = Number(111.0) - Number(1130.0) / xnext + Number(3000.0) / (xcur * xnext);
        }
        catch (const std::exception &e)
        {
            std::cerr << "warning: " << e.what() << std::endl;
        }

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

static void print_cell(const int k)
{
    std::cout << k << ";";
}

template <size_t E, size_t M, typename WT>
static void print_cell(const aarith::floating_point<E, M, WT> &x)
{
    std::cout << x << ";";
}

template <size_t N, size_t ES, typename WT>
static void print_cell(const aarith::posit<N, ES, WT> &p)
{
    std::cout << p << ";";
}

template <size_t N, size_t ES, typename WT>
static void print_cell(const aarith::valid<N, ES, WT> &v)
{
    if (v.is_nar() || v.is_full())
    {
        std::cout << "-inf" << ";";
        std::cout << "inf" << ";";
    }
    else
    {
        std::cout << v.get_start_value() << ";";
        std::cout << v.get_end_value() << ";";
    }
}

int main()
{
    using posit128 = aarith::posit<128, 2>;
    using valid128 = aarith::valid<128, 2>;

    constexpr int niterations = 50;

    const auto f16 = iterate<aarith::half_precision>(niterations);
    const auto f32 = iterate<aarith::single_precision>(niterations);
    const auto f64 = iterate<aarith::double_precision>(niterations);
    const auto f128 = iterate<aarith::quadruple_precision>(niterations);

    const auto p16 = iterate<aarith::posit16>(niterations);
    const auto p32 = iterate<aarith::posit32>(niterations);
    const auto p64 = iterate<aarith::posit64>(niterations);
    const auto p128 = iterate<posit128>(niterations);

    const auto v16 = iterate<aarith::valid16>(niterations);
    const auto v32 = iterate<aarith::valid32>(niterations);
    const auto v64 = iterate<aarith::valid64>(niterations);
    const auto v128 = iterate<valid128>(niterations);

    std::cout << "i;";
    std::cout << "f16;";
    std::cout << "f32;";
    std::cout << "f64;";
    std::cout << "f128;";
    std::cout << "p16;";
    std::cout << "p32;";
    std::cout << "p64;";
    std::cout << "p128;";
    std::cout << "v16start;";
    std::cout << "v16end;";
    std::cout << "v32start;";
    std::cout << "v32end;";
    std::cout << "v64start;";
    std::cout << "v64end;";
    std::cout << "v128start;";
    std::cout << "v128end;";
    std::cout << "\n";

    for (int i = 0; i < niterations; ++i)
    {
        print_cell(i);
        print_cell(f16.at(i));
        print_cell(f32.at(i));
        print_cell(f64.at(i));
        print_cell(f128.at(i));
        print_cell(p16.at(i));
        print_cell(p32.at(i));
        print_cell(p64.at(i));
        print_cell(p128.at(i));
        print_cell(v16.at(i));
        print_cell(v32.at(i));
        print_cell(v64.at(i));
        print_cell(v128.at(i));
        std::cout << "\n";
    }

    std::cout << std::endl;
}
