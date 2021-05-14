#include <iostream>
#include <string>
#include <tuple>

#include <aarith/float.hpp>
#include <aarith/posit.hpp>

#include "support.hpp"

using namespace aarith;

template <typename Num>
static std::tuple<Num, Num> get_limits_for()
{
    const Num min = std::numeric_limits<Num>::min();
    const Num max = std::numeric_limits<Num>::max();

    return std::make_tuple(min, max);
}

template <typename Num> static void print_limits_with(const std::string& name)
{
    const auto [min, max] = get_limits_for<Num>();
    std::cout << name << ": min=" << min << " max=" << max << "\n";
}

#define print_limits_for(TYPE) (print_limits_with<TYPE>(#TYPE))

int main()
{
    print_limits_for(float8);
    print_limits_for(float16);
    print_limits_for(float32);
    print_limits_for(float64);

    print_limits_for(posit8);
    print_limits_for(posit16);
    print_limits_for(posit32);
    print_limits_for(posit64);
}
