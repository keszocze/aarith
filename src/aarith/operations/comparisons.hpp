#pragma once

#include <cstdint>
#include "aarith/types/traits.hpp"


namespace aarith
{
    template <class UInteger> bool operator== (const UInteger& a, const UInteger& b)
    {
        static_assert(is_integral<UInteger>::value);
        static_assert(is_unsigned<UInteger>::value);

        for (auto i=0U; i < a.word_count(); ++i)
        {
            if (a.word(i) != b.word(i))
            {
                return false;
            }
        }

        return true;
    }


    template <class UInteger> bool operator< (const UInteger& a, const UInteger& b)
    {
        static_assert(is_integral<UInteger>::value);
        static_assert(is_unsigned<UInteger>::value);

        for (auto i=0U; i < a.word_count(); ++i)
        {
            if (a.word(i) >= b.word(i))
            {
                return false;
            }
        }

        return true;
    }


    template <class UInteger> bool operator!= (const UInteger& a, const UInteger& b)
    {
        // we do not care about speed and simply call the equality function....
        return !(a == b);
    }

    template <class UInteger> bool operator<= (const UInteger& a, const UInteger& b)
    {
        return (a < b) || (a == b);
    }


    template <class UInteger> bool operator>= (const UInteger& a, const UInteger& b)
    {
        return (b <= a);
    }



    template <class UInteger> bool operator> (const UInteger& a, const UInteger& b)
    {
        return (b < a);
    }




}