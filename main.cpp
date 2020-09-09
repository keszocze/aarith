#include <aarith/float.hpp>
#include <iostream>
#include <numeric>
#include <vector>

int main()
{
    using namespace aarith;
    using namespace aarith::float_operators;

    using nf_t = normalized_float<4, 4>;

    nf_t nf_a(1.5f);
    nf_t nf_b(2.5f);
    nf_t nf_zero(0.0f);
    std::vector<nf_t> nf_vec;

    nf_vec.push_back(nf_a);
    nf_vec.push_back(nf_b);

    const auto acc = std::accumulate(nf_vec.begin(), nf_vec.end(), nf_zero,
                                     [](const nf_t acc, const nf_t & num) {
                                         return acc + num;
                                     });
    const auto non_working_acc = std::accumulate(nf_vec.begin(), nf_vec.end(), nf_zero);

    const auto normal = add(nf_a, nf_b );
    const auto op = nf_a + nf_b;

    const bool all_equal = (acc == normal) && (acc == op) && (normal == op);

    std::cout << "A: " << nf_vec[0] << " B: " << nf_vec[1] << std::endl
              << "accumulated: " << acc  << "\t" << "add function: " << add(nf_a, nf_b) << "\t" << "plus operator: " << op << "\n";
    std::cout << "All equal: " << all_equal << "\n";
    return 0;
}
