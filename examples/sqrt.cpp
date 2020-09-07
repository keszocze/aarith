#include <aarith/float.hpp>
#include <aarith/float/approx_operations.hpp>
#include <cmath>

using namespace aarith;

template<size_t E, size_t M>
auto iterative_square_root(const normalized_float<E, M> a, const unsigned int iterations)
-> normalized_float<E, M>
{
    //using namespace aarith::exact_operators;

  //std::cout << std::endl;
  //std::cout << a << std::endl;
    const normalized_float<E, M> half(0.5f);
  //std::cout << half << " (" << to_binary(half) << ")" << std::endl;
    auto result = a * half;
  //std::cout << result << " (" << to_binary(result) << ")" << std::endl;
    for(auto i = 0U; i < iterations; ++i)
    {
  //    std::cout << "iteration " << i << std::endl;
  //    std::cout << "result " << result << " (" << to_binary(result) << ")" << std::endl;
        const auto quot = a / result;
  //    std::cout << "quot: " << quot << " (" << to_binary(quot) << ")" << std::endl;
        const auto sum = result + quot;
  //    std::cout << "sum: " << sum << " (" << to_binary(sum) << ")" << std::endl;
        result = half * sum;
  //    std::cout << "new result: " << result << " (" << to_binary(result) << ")" << std::endl;
    }
    return result;
}

template<size_t E, size_t M>
auto iterative_square_root(const normalized_float<E, M> a, const unsigned int iterations, const unsigned int bits)
-> normalized_float<E, M>
{
  //std::cout << std::endl;
  //std::cout << a << std::endl;
    const normalized_float<E, M> half(0.5f);
  //std::cout << half << " (" << to_binary(half) << ")" << std::endl;
    auto result = anytime_mul(a, half, bits);
  //std::cout << result << " (" << to_binary(result) << ")" << std::endl;
    for(auto i = 0U; i < iterations; ++i)
    {
  //    std::cout << "iteration " << i << std::endl;
  //    std::cout << "result " << result << " (" << to_binary(result) << ")" << std::endl;
        const auto quot = anytime_div(a, result, bits);
  //    std::cout << "quot: " << quot << " (" << to_binary(quot) << ")" << std::endl;
        const auto sum = anytime_add(result, quot, bits);
  //    std::cout << "sum: " << sum << " (" << to_binary(sum) << ")" << std::endl;
        result = anytime_mul(half, sum, bits);
      //result = anytime_mul(half, anytime_add(result, anytime_div(a, result, bits), bits), bits);
  //    std::cout << "new result: " << result << " (" << to_binary(result) << ")" << std::endl;
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
              << "\taarith::float (exact): " << iterative_square_root(a, 6) << std::endl
              << "\taarith::float (anytime, full precision): " << iterative_square_root(a, 6, 48) << std::endl
              << "\taarith::float (anytime, 10 MSBs): " << iterative_square_root(a, 6, 10) << std::endl
              << "\taarith::float (FAU adder): " << iterative_square_root_FAU<8, 23, 8, 4>(a, 6) << std::endl;

    return 0;
}
