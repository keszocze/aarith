#include <aarith/float.hpp>
#include <iostream>
#include <numeric>
#include <vector>

//namespace aarith {
//template <size_t E, size_t M, typename WordType>
//auto operator+(const normalized_float<E, M, WordType>& lhs,
//               const normalized_float<E, M, WordType>& rhs) -> normalized_float<E, M, WordType>
//{
//    return add(lhs, rhs);
//}
//} // namespace aarith

int main()
{
    using namespace aarith;

    using nf_t = normalized_float<8, 23>;

    nf_t nf_a(0.0117647f);
    nf_t nf_b(0.385671f);

    nf_t nf_res = nf_a * nf_b;

    std::cout << nf_a << " * " << nf_b << " = " << nf_res << "\n";
    std::cout << to_binary(nf_a) << " * " << to_binary(nf_b) << " = " << to_binary(nf_res) << "\n";

    return 0;
}
