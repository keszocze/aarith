#pragma once

#include "aarith/types/integer.hpp"
#include "aarith/types/traits.hpp"
#include <cstdint>

namespace aarith {

template <size_t Width> bool operator==(const uinteger<Width>& a, const uinteger<Width>& b)
{
    for (auto i = 0U; i < a.word_count(); ++i)
    {
        if (a.word(i) != b.word(i))
        {
            return false;
        }
    }

    return true;
}

template <size_t Width> bool operator<(const uinteger<Width>& a, const uinteger<Width>& b)
{
    for (auto i = 0U; i < a.word_count(); ++i)
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
    return false;
}

template <size_t Width> bool operator!=(const uinteger<Width>& a, const uinteger<Width>& b)
{
    // we do not care about speed and simply call the equality function....
    return !(a == b);
}

template <size_t Width> bool operator<=(const uinteger<Width>& a, const uinteger<Width>& b)
{
    return (a < b) || (a == b);
}

template <size_t Width> bool operator>=(const uinteger<Width>& a, const uinteger<Width>& b)
{
    return b <= a;
}

template <size_t Width> bool operator>(const uinteger<Width>& a, const uinteger<Width>& b)
{
    return b < a;
}

} // namespace aarith