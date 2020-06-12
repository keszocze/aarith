#include <aarith/float/float_comparisons.hpp>
#include <aarith/float/float_operations.hpp>
#include <aarith/float/normalized_float.hpp>
#include <aarith/float/string_utils.hpp>
#include <aarith/integer.hpp>

#include <bitset>
#include <iostream>
#include <numeric>
#include <vector>

int main()
{
    using namespace aarith;
    using namespace aarith::arithmetic_operators;
    using nf_t = normalized_float<4, 4>;

    nf_t nf_a(1.5f);
    nf_t nf_b(2.5f);
    nf_t nf_zero(0.0f);
    std::vector<nf_t> nf_vec;

    nf_vec.push_back(nf_a);
    nf_vec.push_back(nf_b);

    std::cout << "A: " << nf_vec[0] << " B: " << nf_vec[1] << std::endl
              << std::accumulate(nf_vec.begin(), nf_vec.end(), nf_zero);
    return 0;
}
