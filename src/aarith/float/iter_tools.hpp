#pragma once

#include <aarith/float.hpp>
#include <functional>

namespace aarith {

/**
 * @brief Run function on each float in some arbitrary order.
 *
 * @tparam FloatType The aarith floating point type to iterate on.
 * @param operation The function to call on each float.
 */
template <typename FloatType>
inline void for_each_float(const std::function<void(const FloatType&)>& operation)
{
    constexpr size_t M = FloatType::mantissa_width();
    constexpr size_t E = FloatType::exponent_width();
    constexpr size_t N = 1 + M + E;

    using Int = uinteger<N>;

    const Int min_bits = FloatType::min().get_bits();
    Int bits = min_bits;

    do
    {
        const word_array<N> current_bits = bits;
        const FloatType current = FloatType(current_bits);
        operation(current);
    } while (++bits != min_bits);
}

/*
 * @brief Run function on each regular float.
 *
 * This function starts with the smallest representable value and then
 * calls operation with increasingly bigger values.
 *
 * Regular floats are all those floats which are neither NaN nor some
 * kind of infinity.
 *
 * @tparam FloatType The aarith floating point type to iterate on.
 * @param operation The function to call on each regular float.
 */
template <typename FloatType>
inline void for_each_regular_float(const std::function<void(const FloatType&)>& operation)
{
    const FloatType start = FloatType::min();
    const FloatType limit = FloatType::pos_infinity();

    FloatType current = start;

    do
    {
        operation(current);
        current = nextafter(current, limit);
    } while (current != start && current != limit);
}

} // namespace aarith
