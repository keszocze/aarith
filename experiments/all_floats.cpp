#include <functional>
#include <iostream>

#include <aarith/float.hpp>

using namespace aarith;

/**
 * @brief Run function on each float.
 *
 * @tparam FloatType The aarith floating point type to iterate on.
 * @param operation The function to call on each float.
 */
template <typename FloatType>
inline void for_each_float(const std::function<void(const FloatType&)>& operation)
{
    using namespace aarith;

    constexpr size_t M = FloatType::mantissa_width();
    constexpr size_t E = FloatType::exponent_width();
    constexpr size_t N = 1 + M + E;

    using Int = uinteger<N>;
    Int bits = Int::zero();

    do
    {
        const word_array<N> current_bits = bits;
        const FloatType current = FloatType(current_bits);
        operation(current);
    } while (++bits);
}

int main()
{
    using Float = half_precision;

    Float maxval, minval;

    for_each_float<Float>([&](const Float& x)
    {
        if (!x.is_inf() && x > maxval)
        {
            maxval = x;
        }

        if (!x.is_inf() && x < minval)
        {
            minval = x;
        }

        std::cout << x << std::endl;
    });

    std::cout << "maxval=" << maxval << std::endl;
    std::cout << "minval=" << minval << std::endl;
}
