#pragma once

#include <cstddef>

namespace aarith {

template <class T> constexpr size_t size_in_words(const size_t w)
{
    const auto word_size = sizeof(T) * 8;
    return (w / word_size) + (w % word_size ? 1 : 0);
}

} // namespace aarith
