#pragma once

#include <aarith/float/normalized_float.hpp>
#include <cstdint>

namespace aarith {

template <size_t E, size_t M>
auto operator<(const normalized_float<E, M> lhs, const normalized_float<E, M> rhs) -> bool
{

    if (lhs.is_nan() || rhs.is_nan())
    {
        return false;
    }

    // positive and negative zero are to be treated equally
    if (lhs.is_zero() && rhs.is_zero())
    {
        return false;
    }

    if (lhs.is_negative() && rhs.is_positive())
    {
        return true;
    }
    else if (lhs.get_exponent() == rhs.get_exponent())
    {
        if (lhs.get_full_mantissa() == rhs.get_full_mantissa())
        {
            return false;
        }
        else if (lhs.get_full_mantissa() < rhs.get_full_mantissa())
        {
            return lhs.get_sign() == 0;
        }
        else
        {
            return lhs.get_sign() == 1;
        }
    }
    else if (lhs.get_exponent() < rhs.get_exponent())
    {
        return lhs.get_sign() == 0;
    }
    else
    {
        return lhs.get_sign() == 1;
    }
}

/**
 * @brief Compares to floating point numbers bit by bit
 * @tparam E Exponent width
 * @tparam M Mantissa width
 * @param lhs
 * @param rhs
 * @return True iff the floats match in every single bit
 */
template <size_t E, size_t M>
bool bitwise_equality(const normalized_float<E, M> lhs, const normalized_float<E, M> rhs)
{
    const bool equal_sign = (lhs.get_sign() == rhs.get_sign());
    const bool equal_exponent = (lhs.get_exponent() == rhs.get_exponent());
    const bool equal_mantissa = (lhs.get_full_mantissa() == rhs.get_full_mantissa());
    //    std::cout << equal_sign << "\t" << equal_exponent << "\t" << equal_mantissa << "\n";
    return equal_sign && equal_exponent && equal_mantissa;
}

template <size_t E, size_t M>
auto operator==(const normalized_float<E, M> lhs, const normalized_float<E, M> rhs) -> bool
{

    if (lhs.is_nan() || rhs.is_nan())
    {
        return false;
    }

    // positive and negative zero should be treated equal
    if (lhs.is_zero() && rhs.is_zero())
    {
        return true;
    }

    return bitwise_equality(lhs, rhs);
}

template <size_t E, size_t M>
auto operator!=(const normalized_float<E, M> lhs, const normalized_float<E, M> rhs) -> bool
{
    return !(lhs == rhs);
}

template <size_t E, size_t M>
auto operator>(const normalized_float<E, M> lhs, const normalized_float<E, M> rhs) -> bool
{
    return rhs < lhs;
}

template <size_t e, size_t m>
auto operator>=(const normalized_float<e, m> lhs, const normalized_float<e, m> rhs) -> bool
{
    return rhs < lhs || lhs == rhs;
}

template <size_t e, size_t m>
auto operator<=(const normalized_float<e, m> lhs, const normalized_float<e, m> rhs) -> bool
{
    return lhs < rhs || lhs == rhs;
}

} // namespace aarith
