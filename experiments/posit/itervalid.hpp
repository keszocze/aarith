#pragma once

#include <cstdio>
#include <iostream>
#include <tuple>

#include <aarith/posit.hpp>

inline void print_header()
{
    puts("i;v8lo;v8hi;v16lo;16hi;v32lo;v32hi;v64lo;v64hi");
}

inline void report(const int& i, const aarith::valid8& v8, const aarith::valid16& v16,
                   const aarith::valid32& v32, const aarith::valid64& v64)
{
    std::cout << i << ";";
    std::cout << v8.get_start_value() << ";";
    std::cout << v8.get_end_value() << ";";
    std::cout << v16.get_start_value() << ";";
    std::cout << v16.get_end_value() << ";";
    std::cout << v32.get_start_value() << ";";
    std::cout << v32.get_end_value() << ";";
    std::cout << v64.get_start_value() << ";";
    std::cout << v64.get_end_value() << "\n";
}

inline std::tuple<aarith::valid8, aarith::valid16, aarith::valid32, aarith::valid64> zeroes()
{
    using namespace aarith;

    const valid8 v8 = valid8::zero();
    const valid16 v16 = valid16::zero();
    const valid32 v32 = valid32::zero();
    const valid64 v64 = valid64::zero();

    return std::make_tuple(v8, v16, v32, v64);
}
