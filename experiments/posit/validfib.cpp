#include <iostream>

#include <aarith/posit.hpp>

#include "itervalid.hpp"
#include "support.hpp"

using namespace aarith;

template <typename Valid> static Valid fib(const Valid& v)
{
    const Valid zero = Valid::zero();
    const Valid one = Valid::one();
    const Valid two = Valid::one() + Valid::one();

    if (v == zero || v == one || v == two)
    {
        return one;
    }
    else
    {
        return fib(v - one) + fib(v - two);
    }
}

int main()
{
    print_header();

    auto [v8, v16, v32, v64] = zeroes();

    for (int i = 0; i <= 18; ++i)
    {
        report(i, v8, v16, v32, v64);

        v8 = fib(valid8(posit8(i)));
        v16 = fib(valid16(posit16(i)));
        v32 = fib(valid32(posit32(i)));
        v64 = fib(valid64(posit64(i)));
    }
}
