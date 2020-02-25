#include <aarith/float.hpp>
#include <aarith/float/approx_operations.hpp>
#include <cmath>

using namespace aarith;

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

template<size_t E, size_t M, size_t LSP, size_t SHARED>
auto iterative_square_root_FAU(const normfloat<E, M> a, const unsigned int iterations)
-> normfloat<E, M>
{
    using namespace aarith::exact_operators;

    const normfloat<E, M> half(0.5f);
    auto result = a * half;
    for(auto i = 0U; i < iterations; ++i)
    {
        result = half * (FAU_add<E, M, LSP, SHARED>(result, a / result));
    }
    return result;
}

int main()
{
    float ref = 1.5f;
    normfloat<8, 24> a(ref);

    std::cout << "sqrt(" << ref << ") = " << std::endl
              << "\tstandard: " << std::sqrt(ref) << std::endl
              << "\taarith::float (exact): " << iterative_square_root(a, 6) << std::endl
              << "\taarith::float (anytime): " << iterative_square_root(a, 6, 10) << std::endl
              << "\taarith::float (FAU adder): " << iterative_square_root_FAU<8, 24, 8, 4>(a, 6) << std::endl;

    return 0;
}
