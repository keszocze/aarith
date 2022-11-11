#include <iostream>

#include <aarith/posit.hpp>

#include "itervalid.hpp"
#include "support.hpp"

using namespace aarith;

int main()
{
    print_header();

    auto [v8, v16, v32, v64] = zeroes();

    for (int i = 0; i <= 100; ++i)
    {
        report(i, v8, v16, v32, v64);

        if (i % 2 == 0)
        {
            v8 += valid8::one();
            v16 += valid16::one();
            v32 += valid32::one();
            v64 += valid64::one();
        }
        else
        {
            v8 -= valid8::one();
            v16 -= valid16::one();
            v32 -= valid32::one();
            v64 -= valid64::one();
        }
    }
}
