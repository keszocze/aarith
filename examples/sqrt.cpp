#include <aarith/types/normfloat.hpp>
#include <aarith/operations/float_operations.hpp>
#include <aarith/operations/float_approx_operations.hpp>
#include <aarith/utilities/string_utils.hpp>
#include <cmath>

using namespace aarith;

template<size_t E, size_t M>
auto iterative_square_root(const normfloat<E, M> a, const unsigned int iterations, const unsigned int bits)
-> normfloat<E, M>
{
    const normfloat<E, M> half(0.5f);
    auto result = anytime_mul(a, half, bits);
    for(auto i = 0U; i < iterations; ++i)
    {
        result = anytime_mul(half, anytime_add(result, anytime_div(a, result, bits), bits), bits);
    }
    return result;
}

template<size_t E, size_t M>
auto iterative_square_root(const normfloat<E, M> a, const unsigned int iterations)
-> normfloat<E, M>
{
    using namespace aarith::exact_operators;

    const normfloat<E, M> half(0.5f);
    auto result = a * half;
    for(auto i = 0U; i < iterations; ++i)
    {
        result = half * (result + a / result);
    }
    return result;
}

int main()
{
    float ref = 1.5f;
    normfloat<6, 16> a(ref);

    std::cout << "sqrt(" << ref << ") = " << std::endl
              << "\tstandard: " << std::sqrt(ref) << std::endl
              << "\taarith::float (exact): " << iterative_square_root(a, 6) << std::endl
              << "\taarith::float (anytime): " << iterative_square_root(a, 6, 10) << std::endl;

    return 0;
}
