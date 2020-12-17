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
 * @brief Creates quiet NaN with a specified payload
 *
 * For some reason, negative parameters result in +0 to be returned. Don't ask me, ask the standard.
 *
 * @tparam E Exponent width
 * @tparam M Mantissa width
 * @tparam WordType The data type the underlying data is stored in
 * @param x Floating-point number to take the payload to be stored from
 * @return A quiet NaN with the specified payload, +0 in case of error
 */
template <size_t E, size_t M, typename WordType>
normalized_float<E, M, WordType> constexpr setPayload(const normalized_float<E, M, WordType>& x)
{
    using F = normalized_float<E, M, WordType>;

    if (x.is_nan() || x.is_negative())
    {
        return F::zero();
    }

    // need to set the msb correctly
    typename F::IntegerFrac new_mantissa{x.get_mantissa()};
    new_mantissa.set_msb(true);
    const F qNaN = F{false, F::IntegerExp::all_ones(), new_mantissa};
    return qNaN;
}


/**
 * @brief Creates signaling NaN with a specified payload
 *
 * For some reason, negative parameters result in +0 to be returned. Don't ask me, ask the standard.
 *
 * @tparam E Exponent width
 * @tparam M Mantissa width
 * @tparam WordType The data type the underlying data is stored in
 * @param x Floating-point number to take the payload to be stored from
 * @return A signaling NaN with the specified payload, +0 in case of error
 */
template <size_t E, size_t M, typename WordType>
normalized_float<E, M, WordType> constexpr setPayloadSignaling(const normalized_float<E, M, WordType>& x)
{
    using F = normalized_float<E, M, WordType>;

    if (x.is_nan() || x.is_negative())
    {
        return F::zero();
    }

    // need to set the msb correctly
    typename F::IntegerFrac new_mantissa{x.get_mantissa()};
    new_mantissa.set_msb(false);

    // when the mantissa has become zeros only, we need to catch this as otherwise
    // positive infinity would be returned
    if (new_mantissa == F::IntegerFrac::zero()) {
        return F::zero();
    }
    const F sNaN = F{false, F::IntegerExp::all_ones(), new_mantissa};
    return sNaN;
}

} // namespace aarith