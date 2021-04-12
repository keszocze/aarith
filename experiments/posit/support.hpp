#pragma once

/**
 * support.hpp
 *
 * Inline helpers for use in posit experiments.
 *
 */

#include <aarith/float.hpp>
#include <aarith/integer.hpp>
#include <functional>

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
