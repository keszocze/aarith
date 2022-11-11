#include <functional>
#include <iostream>

#include <aarith/float.hpp>

using namespace aarith;

int main()
{
    using Float = half_precision;

    Float maxval, minval;

    for_each_float<Float>([&](const Float& x) {
        if (!x.is_inf() && x > maxval)
        {
            maxval = x;
        }

        if (!x.is_inf() && x < minval)
        {
            minval = x;
        }

        std::cout << x << std::endl;
    });

    std::cout << "maxval=" << maxval << std::endl;
    std::cout << "minval=" << minval << std::endl;
}
