#pragma once

#include <cstdint>
#include <iostream>
#include "aarith/types/integer.hpp"

namespace aarith {

template <class UInteger> auto generate_bitmask(const uint32_t bits) -> UInteger
{
    UInteger mask;

    constexpr typename UInteger::word_type full_mask =
        static_cast<typename UInteger::word_type>(-1);
    const auto full_mask_words = bits / static_cast<uint32_t>(UInteger::word_width());
    const auto remaining_bits =
        bits - full_mask_words * static_cast<uint32_t>(UInteger::word_width());

    const typename UInteger::word_type last_word_mask =
        (static_cast<typename UInteger::word_type>(1) << bits) - 1;

    auto counter = 0U;
    while (counter < full_mask_words)
    {
        mask.set_word(counter, full_mask);
        ++counter;
    }

    if (remaining_bits > 0)
    {
        mask.set_word(counter, last_word_mask);
    }

    return mask;
}

template <class UInteger>
auto approx_uint_bitmasking_add(const UInteger& opd1, const UInteger& opd2, const uint32_t bits)
    -> UInteger
{
    UInteger sum, mask;

    mask = generate_bitmask<UInteger>(UInteger::width() - bits);
    mask = ~mask;

    UInteger opd1_masked = opd1 & mask;
    UInteger opd2_masked = opd2 & mask;

    sum = exact_uint_add(opd1_masked, opd2_masked);

    return sum;
}

template <size_t width>
auto approx_uint_bitmasking_mul(const uinteger<width>& opd1, const uinteger<width>& opd2, const uint32_t bits)
    -> uinteger<2*width>
{
    constexpr auto product_width = 2 * width;
    uinteger<product_width> product, mask;

    mask = generate_bitmask<uinteger<product_width>>(product_width - bits);
    mask = ~mask;

    uinteger<product_width> opd2_extended = width_cast<product_width, width>(opd2);

    for (auto counter = 0U; counter < width; ++counter)
    {
        auto opd2_masked = opd2_extended & mask;
        product = ((opd1.bit(counter) == 0) ? (product) : (exact_uint_add(product, opd2_masked)));
        opd2_extended = opd2_extended << 1;

        std::cout << "iteration " << counter << ": product = " << product.word(0) << std::endl;
    }

    return product;
}

} // namespace aarith
