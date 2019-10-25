#pragma once

#include <cstddef>
#include <tuple>

namespace aarith {

template <class T> constexpr size_t size_in_words(const size_t w)
{
    const auto word_size = sizeof(T) * 8;
    return (w / word_size) + (w % word_size ? 1 : 0);
}

constexpr std::pair<uint32_t , uint32_t > split(const uint64_t n)
{
            uint64_t bitmask{1};
            bitmask = (bitmask << 32) - 1;

            return {n >> 32, n & bitmask };
}

constexpr uint64_t unsplit(const uint32_t upper, const uint32_t lower)
{
    uint64_t result{upper};

    result <<= 32;
    result += lower;

    return result;
}

} // namespace aarith
