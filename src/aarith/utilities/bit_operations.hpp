#pragma once

#include <cstddef>

namespace aarith {

template <class T> constexpr auto bit_width() -> size_t
{
    return sizeof(T) * 8;
}

template <class T> constexpr auto size_in_words(const size_t w) -> size_t
{
    return (w / bit_width<T>()) + (w % bit_width<T>() ? 1 : 0);
}

} // namespace aarith
