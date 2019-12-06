#pragma once

#include "aarith/types/integer.hpp"

#include <limits>

namespace aarith {

template <class Type> class is_integral
{
public:
    static constexpr bool value = false;
};

template <class Type> class is_unsigned
{
public:
    static constexpr bool value = false;
};

} // namespace aarith

// We are only allowed to extend std with  specializations
// https://en.cppreference.com/w/cpp/language/extending_std
template <size_t W>
class std::numeric_limits<aarith::uinteger<W>>
{
    static constexpr bool is_exact = true;
    static constexpr bool is_bounded = true;
    static constexpr bool is_signed = false;
}