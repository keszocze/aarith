#pragma once

/**
 * support.hpp
 *
 * Inline helpers for use in posit experiments.
 */

#include <aarith/float.hpp>
#include <aarith/integer.hpp>
#include <cmath>
#include <functional>

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

/**
 * @brief Compute decimal accuracy for expected value x and actual value y.
 *
 * Based on "Beating Floating Point at its Own Game: Posit Arithmetic", 2017,
 * pp. 78.
 */
template <typename X, typename Y> inline double decimal_accuracy(const X& x, const Y& y)
{
    const double xd = static_cast<double>(x);
    const double yd = static_cast<double>(y);

    const double err = fabs(log10(xd / yd));

    if (std::isnan(err) || std::isinf(err))
    {
        return 0.0;
    }

    return err;
}
