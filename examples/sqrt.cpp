#include <aarith/float.hpp>
#include <aarith/float/approx_operations.hpp>
#include <cmath>

using namespace aarith;

template<size_t E, size_t M>
auto iterative_square_root(const normalized_float<E, M> a, const unsigned int iterations)
-> normalized_float<E, M>
{
    //using namespace aarith::exact_operators;

    std::cout << a << std::endl;
    const normalized_float<E, M> half(0.5f);
    std::cout << half << std::endl;
    auto result = a * half;
    std::cout << result << std::endl;
    for(auto i = 0U; i < iterations; ++i)
    {
        std::cout << "iteration " << i << std::endl;
        std::cout << "result " << result << std::endl;
        const auto quot = a / result;
        std::cout << "quot: " << quot << std::endl;
        const auto sum = result + quot;
        result = half * sum;
        std::cout << "new result: " << result << std::endl;
    }
    return result;
}

template<size_t E, size_t M>
auto iterative_square_root(const normalized_float<E, M> a, const unsigned int iterations, const unsigned int bits)
-> normalized_float<E, M>
{
    const normalized_float<E, M> half(0.5f);
    auto result = anytime_mul(a, half, bits);
    for(auto i = 0U; i < iterations; ++i)
    {
        result = anytime_mul(half, anytime_add(result, anytime_div(a, result, bits), bits), bits);
    }
    return result;
}

template<size_t E, size_t M, size_t LSP, size_t SHARED>
auto iterative_square_root_FAU(const normalized_float<E, M> a, const unsigned int iterations)
-> normalized_float<E, M>
{
    //using namespace aarith::exact_operators;

    const normalized_float<E, M> half(0.5f);
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
    normalized_float<8, 23> a(ref);

    std::cout << "sqrt(" << ref << ") = " << std::endl
              << "\tstandard: " << std::sqrt(ref) << std::endl
              << "\taarith::float (exact): " << iterative_square_root(a, 20) << std::endl
              << "\taarith::float (anytime): " << iterative_square_root(a, 20, 10) << std::endl
              << "\taarith::float (FAU adder): " << iterative_square_root_FAU<8, 23, 8, 4>(a, 20) << std::endl;

    return 0;
}
