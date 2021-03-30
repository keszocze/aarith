#include <aarith/float.hpp>
#include <aarith/float/float_approx_operations.hpp>
#include <cmath>

using namespace aarith;

int main()
{
    using hf = floating_point<5, 10>;
    using ui4 = uinteger<4>;
    
    std::cout << to_base_2n<1>(ui4(0U)) << " | " << to_binary(hf(0.f)) << " | " << to_binary(hf::pos_infinity())  << " | " << to_binary(hf(1.f/static_cast<float>(0.f)))<< std::endl;
    for (auto c = 1U; c < 16; ++c)
    {
        ui4 num(c);
        hf fnum(static_cast<float>(c));
        hf one(1.0);
        hf inv = one / fnum;
        std::cout << to_base_2n<1>(num) << " | " << to_binary(fnum) << " | " << to_binary(inv) << " | " << to_binary(hf(1.f/static_cast<float>(c))) << std::endl;
    }

    return 0;
}
