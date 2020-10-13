#include <aarith/float.hpp>
#include <iostream>
#include <numeric>
#include <vector>


int main()
{
    using namespace aarith;

    using nf_t = normalized_float<8, 23>;
    float a = 0.0117647f;
    float b = 0.385671f;
    nf_t nf_a(0.0117647f);
    nf_t nf_b(0.385671f);

    auto result_f = a * b;
    auto result = mul(nf_a, nf_b);
    std::cout << result << "\n";
    std::cout << to_binary(nf_t(result_f)) << "\n";
    std::cout << to_binary(result) << "\n";


    return 0;
}
