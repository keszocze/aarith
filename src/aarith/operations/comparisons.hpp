#pragma once

#include "aarith/types/integer.hpp"
#include "aarith/types/normfloat.hpp"
#include "aarith/types/traits.hpp"
#include <cstdint>

namespace aarith {

template <size_t W, size_t V> bool operator==(const uinteger<W>& a, const uinteger<V>& b)
{
    const auto min_count = std::min(a.word_count(), b.word_count());
    const auto max_count = std::max(a.word_count(), b.word_count());

    for (auto i = 0U; i < min_count; ++i)
    {
        if (a.word(i) != b.word(i))
        {
            return false;
        }
    }

    if constexpr (W > V)
    {
        for (size_t i = min_count; i < max_count; ++i)
        {
            if (a.word(i) != 0U)
            {
                return false;
            }
        }
    }
    else
    {
        for (size_t i = min_count; i < max_count; ++i)
        {
            if (b.word(i) != 0U)
            {
                return false;
            }
        }
    }

    return true;
}

template <size_t W, size_t V> bool operator<(const uinteger<W>& a, const uinteger<V>& b)
{

    const auto min_count = std::min(a.word_count(), b.word_count());
    const auto max_count = std::max(a.word_count(), b.word_count());

    for (auto i = 0U; i < min_count; ++i)
    {
        auto const word_a = a.word(a.word_count() - i - 1);
        auto const word_b = b.word(b.word_count() - i - 1);
        if (word_a < word_b)
        {
            return true;
        }
        else if (word_a > word_b)
        {
            return false;
        }
    }

    if constexpr (W > V)
    {
        for (size_t i = min_count; i < max_count; ++i)
        {
            auto const word_a = a.word(a.word_count()- i - 1);
            if (word_a > 0U) {
                return false;
            }
        }
    }
    else
    {
        for (size_t i = min_count; i < max_count; ++i)
        {
            auto const word_b = b.word(b.word_count()- i - 1);
            if (word_b > 0U) {
                return true;
            }
        }
    }

    return false;
}

template <size_t W, size_t V> bool operator!=(const uinteger<W>& a, const uinteger<V>& b)
{
    // we do not care about speed and simply call the equality function....
    return !(a == b);
}

template <size_t W, size_t V> bool operator<=(const uinteger<W>& a, const uinteger<V>& b)
{
    return (a < b) || (a == b);
}

template <size_t W, size_t V> bool operator>=(const uinteger<W>& a, const uinteger<V>& b)
{
    return b <= a;
}

template <size_t W, size_t V> bool operator>(const uinteger<W>& a, const uinteger<V>& b)
{
    return b < a;
}

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
