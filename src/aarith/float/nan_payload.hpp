#pragma once

namespace aarith {

/**
 * @brief Extracts the payload from an NaN
 * @tparam E Exponent width
 * @tparam M Mantissa width
 * @tparam WordType The data type the underlying data is stored in
 * @param x Floating-point to extract the payload from
 * @return The payload or -1, when x is not NaN
 */
template <size_t E, size_t M, typename WordType>
normalized_float<E, M, WordType> constexpr getPayload(const normalized_float<E, M, WordType>& x)
{
    using F = normalized_float<E, M, WordType>;
    if (!x.is_nan())
    {
        return F::neg_one();
    }

    return F{x.get_sign(), F::IntegerExp::zero(), x.get_mantissa()};
}

/**
 * @brief Extracts the payload from an NaN
 * @tparam E Exponent width
 * @tparam M Mantissa width
 * @tparam WordType The data type the underlying data is stored in
 * @param x Floating-point to extract the payload from
 * @return The payload or -1, when x is not NaN
 */
template <size_t E, size_t M, typename WordType>
normalized_float<E, M, WordType> constexpr setPayload(const normalized_float<E, M, WordType>& x)
{
    using F = normalized_float<E, M, WordType>;
    if (!x.is_nan() || x.is_negative())
    {
        return F::zero();
    }

    return F{false, F::IntegerExp::all_ones(), x.get_mantissa()};
}


} // namespace aarith