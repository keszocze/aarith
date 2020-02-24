#pragma once

#include "normfloat.hpp"
#include <cstdint>

namespace aarith {

template<size_t E, size_t M>
auto operator<(const normfloat<E, M> lhs, const normfloat<E, M> rhs)
-> bool
{
    if(lhs.get_sign() > rhs.get_sign())
    {
        return true;
    }
    else if(lhs.get_exponent() == rhs.get_exponent())
    {
        if(lhs.get_mantissa() == rhs.get_mantissa())
        {
            return false;
        } 
        else if(lhs.get_mantissa() < rhs.get_mantissa())
        {
            return lhs.get_sign() == 0;
        }
        else
        {
            return lhs.get_sign() == 1;
        }
    }
    else if(lhs.get_exponent() < rhs.get_exponent())
    {
        return lhs.get_sign() == 0;
    }
    else
    {
        return lhs.get_sign() == 1;
    }
}

template<size_t E, size_t M>
auto operator==(const normfloat<E, M> lhs, const normfloat<E, M> rhs)
-> bool
{
    return lhs.get_sign() == rhs.get_sign() && lhs.get_exponent() == rhs.get_exponent() && lhs.get_mantissa() == rhs.get_mantissa();
}

template<size_t E, size_t M>
auto operator>(const normfloat<E, M> lhs, const normfloat<E, M> rhs)
-> bool
{
    return rhs < lhs;
}

template<size_t e, size_t m>
auto operator>=(const normfloat<e, m> lhs, const normfloat<e, m> rhs)
-> bool
{
    return rhs < lhs || lhs == rhs;
}

template<size_t e, size_t m>
auto operator<=(const normfloat<e, m> lhs, const normfloat<e, m> rhs)
-> bool
{
    return lhs < rhs || lhs == rhs;
}

} // namespace aarith
