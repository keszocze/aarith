#pragma once

#include <limits>

namespace aarith {

template <class Type> class is_word_array
{
public:
    static constexpr bool value = false;
};

template <class Type> inline constexpr bool is_word_array_v = is_word_array<Type>::value;

template <class Type> class is_integral
{
public:
    static constexpr bool value = false;
};

template <class Type> inline constexpr bool is_integral_v = is_integral<Type>::value;

template <class Type> class is_unsigned
{
public:
    static constexpr bool value = false;
};

template <class Type> inline constexpr bool is_unsigned_v = is_unsigned<Type>::value;

template <class Type> class is_float
{
public:
    static constexpr bool value = false;
};

template <class Type> inline constexpr bool is_float_v = is_float<Type>::value;

} // namespace aarith
