#pragma once

#include <aarith/core/word_array.hpp>
#include <cstdint>
#include <cstdlib>

namespace aarith {
/**
 * Represents a posit number that can represent real values. Based on
 * the description by John L. Gustafson ("Posit Arithmetic", 10
 * October 2017.)
 *
 * @tparam NBits The total width in bits of the given Posit.
 * @tparam ExponentSize The maximum width in bits of the exponent for
 * a given Posit.
 */
template <size_t NBits, size_t ExponentSize, class WordType = uint64_t>
class posit : public word_array<NBits, WordType>
{
public:
    /**
     * Construct this posit initalized to zero.
     */
    constexpr posit() = default;

    /**
     * Construct this posit with given bits.
     *
     * @param n Bits used to initalize the underlying type.
     */
    constexpr explicit posit(WordType n)
        : word_array<NBits, WordType>(n)
    {
    }

private:
};
} // namespace aarith
