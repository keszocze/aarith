#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/float/normalized_float.hpp>

namespace aarith {

template <size_t E, size_t M, typename WordType>
bool totalOrder(const normalized_float<E, M, WordType>& x,
                const normalized_float<E, M, WordType>& y)
{
    if (x.is_nan() && !y.is_nan())
    {
        return x.is_negative();
    }

    if (!x.is_nan() && y.is_nan())
    {
        return y.is_positive();
    }

    if (x.is_nan() && y.is_nan())
    {
        if (x.is_negative() && y.is_positive())
        {
            return true;
        }
    }

    return false;
}
} // namespace aarith