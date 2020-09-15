#pragma once

#include <aarith/float/normalized_float.hpp>
#include <cstdint>

namespace aarith {

template <size_t E, size_t M>
auto operator<(const normalized_float<E, M> lhs, const normalized_float<E, M> rhs) -> bool
{
    if (lhs.get_sign() > rhs.get_sign())
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

template <size_t E, size_t M>
auto operator==(const normalized_float<E, M> lhs, const normalized_float<E, M> rhs) -> bool
{
    return lhs.get_sign() == rhs.get_sign() && lhs.get_exponent() == rhs.get_exponent() &&
           lhs.get_full_mantissa() == rhs.get_full_mantissa();
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
