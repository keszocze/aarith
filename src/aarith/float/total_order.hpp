#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/float/normalized_float.hpp>

namespace aarith {

/**
 * @brief Checks for total ordering x < y of two floating-point numbers.
 *
 * These numbers *may* be NaNs or infinities.
 *
 * @tparam E Exponent width
 * @tparam M Mantissa width
 * @tparam WordType The data type the underlying data is stored in
 * @param x The first/lhs floating-point number
 * @param y The second/rhs floating-point number
 * @return true iff x < y
 */
template <size_t E, size_t M, typename WordType>
bool constexpr totalOrder(const normalized_float<E, M, WordType>& x,
                          const normalized_float<E, M, WordType>& y)
{

    //    d) If x and y are unordered numerically because x or y is a NaN:

    // The decision to make the checks for NaN first is so that later on the < comparison can be
    // safely used. It would return false when one of the operands is NaN which, in this context,
    // would not the intended behaviour.

    // 1) totalOrder(−NaN, y) is true where −NaN represents a NaN with negative sign bit and y is a
    // floating-point number.
    //                          combined with
    // 4) totalOrder(+NaN, y) is false where +NaN represents a NaN with positive sign bit and y is a
    // floating-point number.
    if (x.is_nan() && !y.is_nan())
    {
        return x.is_negative();
    }

    // 2) totalOrder(x, −NaN) is false where −NaN represents a NaN with negative sign bit and x is a
    // floating-point number.
    //                          combined with
    // 3) totalOrder(x, +NaN) is true where +NaN represents a NaN with positive sign bit and x is a
    // floating-point number.
    if (!x.is_nan() && y.is_nan())
    {
        return y.is_positive();
    }

    // 5) If x and y are both NaNs, then totalOrder reflects a total ordering based on:
    if (x.is_nan() && y.is_nan())
    {

        // i) negative sign orders below positive sign
        if (x.is_negative() && y.is_positive())
        {
            return true;
        }
        if (x.is_positive() && y.is_negative())
        {
            return false;
        }

        // ii) signaling orders below quiet for +NaN, reverse for −NaN
        if (x.is_sNaN() && y.is_qNaN())
        {
            return x.is_positive() && y.is_positive();
        }

        // ii) signaling orders below quiet for +NaN, reverse for −NaN

        // I am guessing that this is meant by "reverse"
        if (x.is_qNaN() && y.is_sNaN())
        {
            return x.is_negative() && y.is_negative();
        }

        // iii) otherwise, the order of NaNs is implementation-defined

        // x < x should always be false
        return false;
    }

    // a) If x < y, totalOrder(x, y) is true.
    if (x < y)
    {
        return true;
    }

    // b) If x > y, totalOrder(x, y) is false.
    if (x > y)
    {
        return false;
    }

    // c) If x = y:
    if (x == y)
    { // this test "makes sense" as -0 == 0

        // 1) totalOrder(−0, +0) is true.
        if (x.is_neg_zero() && y.is_pos_zero())
        {
            return true;
        }

        // 2) totalOrder(+0, −0) is false.
        if (x.is_pos_zero() && y.is_neg_zero())
        {
            return false;
        }

        // I totally fail to understand the following clause (see below) as the equality check above
        // already made sure that the values are identical and only a differentiation between the
        // two zeros seem to make sense. The introductory text of the clause explicitly talks about
        // "canonical members". So I assume that returning false is the sane thing to do.

        // 3) If x and y represent the same floating-point datum:
        // i) If x and y have negative sign,
        // totalOrder(x, y) is true if and only if the exponent of x ≥ the exponent of y
        // ii) otherwise,
        // totalOrder(x, y) is true if and only if the exponent of x ≤ the exponent of y.

        return true;
    }

    // This *should* be unreachable code as all cases as shown in the standard document have been
    // taken care of. Returning false seems to be a reasonable thing to do.
    return false;
}

/**
 * @brief Checks for total ordering abs(x) < abs(y) of two floating-point numbers.
 *
 * These numbers *may* be NaNs or infinities.
 *
 * @tparam E Exponent width
 * @tparam M Mantissa width
 * @tparam WordType The data type the underlying data is stored in
 * @param x The first/lhs floating-point number
 * @param y The second/rhs floating-point number
 * @return true iff abs(x) < abs(y)
 */
template <size_t E, size_t M, typename WordType>
bool constexpr totalOrderMag(const normalized_float<E, M, WordType>& x,
                             const normalized_float<E, M, WordType>& y)
{
    return totalOrder(abs(x), abs(y));
}
} // namespace aarith