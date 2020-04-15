#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/integer/integers.hpp>

namespace aarith {

template <size_t W, typename T> constexpr uint8_t to_uint8_t(const uinteger<W, T>& a)
{

    constexpr uint8_t max = std::numeric_limits<uint8_t>::max();

    if (a <= uinteger<W, T>{max})
    {
        // this works fine as the word type is at least a uint8_t
        return static_cast<uint8_t>(a.word(0));
    }
    else
    {
        // TODO die Nummer mit in die Fehlermeldung packen
        throw std::domain_error("The number does not fit into a uin8_t");
    }
}

} // namespace aarith