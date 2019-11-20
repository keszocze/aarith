#pragma once

#include <cstdint>
#include <type_traits>
#include <sstream>
#include <string>

namespace aarith{

template<class I>
auto single_hex(const I num)
-> char
{
    static_assert(std::is_integral<I>(), "Can only convert integers.");
    I mask = 15U;

    auto const hex = num & mask;

    if(hex < 10)
    {
        return static_cast<char>(hex) + '0';
    }
    else
    {
        return static_cast<char>(hex - 10) + 'A';
    }
}

template<class I>
auto to_hex(std::ostream &stream, const I &num, unsigned int cull)
-> std::ostream&
{
    to_hex(stream, num, cull, false);
    return stream;
}

template<class I>
auto to_hex(std::ostream &stream, const I &num, unsigned int cull, bool remove_leading_zeroes, bool show_zero)
-> std::ostream&
{
    static_assert(std::is_integral<I>(), "Can only convert integers.");

    auto const max_cull = sizeof(I)*8-4;
    if(cull > max_cull)
    {
        cull = max_cull;
    }
    cull = (cull/4)*4;

    for(auto counter = sizeof(I)*8 - cull; counter > 0; counter -= 4)
    {
        auto hex_char = single_hex(num >> counter - 4);
        if(!(remove_leading_zeroes && hex_char == '0'))
        {
            stream << hex_char;
            remove_leading_zeroes = false;
        }
    }

    if(remove_leading_zeroes && show_zero)
    {
        stream << 0;
    }

    return stream;
}

} //namespace aarith
