#include <aarith/float.hpp>
#include <aarith/float/approx_operations.hpp>
#include <cmath>

using namespace aarith;

template<size_t E, size_t M>
auto iterative_square_root(const normalized_float<E, M> a, const unsigned int iterations)
-> normalized_float<E, M>
{
    const normalized_float<E, M> half(0.5F);
    auto result = a * half;
    for(auto i = 0U; i < iterations; ++i)
    {
        const auto quot = a / result;
        const auto sum = result + quot;
        result = half * sum;
    }
    return result;
}

template<size_t E, size_t M>
auto iterative_square_root(const normalized_float<E, M> a, const unsigned int iterations, const unsigned int bits)
-> normalized_float<E, M>
{
    const normalized_float<E, M> half(0.5F);
    auto result = anytime_mul(a, half, bits);
    for(auto i = 0U; i < iterations; ++i)
    {
        const auto quot = anytime_div(a, result, bits);
        const auto sum = anytime_add(result, quot, bits);
        result = anytime_mul(half, sum, bits);
      //result = anytime_mul(half, anytime_add(result, anytime_div(a, result, bits), bits), bits);
    }
    return result;
}

template<size_t E, size_t M, size_t LSP, size_t SHARED>
auto iterative_square_root_FAU(const normalized_float<E, M> a, const unsigned int iterations)
-> normalized_float<E, M>
{
    const normalized_float<E, M> half(0.5F);
    auto result = a * half;
    for(auto i = 0U; i < iterations; ++i)
    {
        const auto quot = a / result;
        const auto sum = FAU_add<E, M, LSP, SHARED>(result, quot);
        const auto prod = half * sum;
        result = prod;
      //result = half * (FAU_add<E, M, LSP, SHARED>(result, a / result));
    }
    return result;
}

int main()
{
    float ref = 1.5F;
    normalized_float<8, 23> a(ref);

    std::cout << "sqrt(" << ref << ") = " << std::endl
              << "\tstandard: " << std::sqrt(ref) << std::endl
              << "\taarith::float (exact): " << iterative_square_root(a, 6) << std::endl
              << "\taarith::float (anytime, full precision): " << iterative_square_root(a, 6, 48) << std::endl
              << "\taarith::float (anytime, 10 MSBs): " << iterative_square_root(a, 6, 10) << std::endl
              << "\taarith::float (FAU adder): " << iterative_square_root_FAU<8, 23, 8, 4>(a, 6) << std::endl;

    return 0;
}
