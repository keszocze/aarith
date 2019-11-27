#pragma once

#include "aarith/types/integer.hpp"
#include <aarith/operations/exact_operations.hpp>
#include <cstdint>
#include <iostream>

namespace aarith {

template <class UInteger> auto generate_bitmask(const size_t bits) -> UInteger
{
    using word_type = typename UInteger::word_type;

    constexpr auto full_mask = static_cast<word_type>(-1);
    const auto full_mask_words = bits / static_cast<size_t>(UInteger::word_width());
    const auto remaining_bits = bits % static_cast<size_t>(UInteger::word_width());

    const typename UInteger::word_type last_word_mask = (static_cast<word_type>(1) << bits) - 1;

    UInteger mask;
    auto counter = 0U;
    for (; counter < full_mask_words; ++counter)
    {
        mask.set_word(counter, full_mask);
    }
    if (remaining_bits > 0)
    {
        mask.set_word(counter, last_word_mask);
    }
    return mask;
}

template <class UInteger, class Function>
[[nodiscard]] UInteger
approx_operation_post_masking(const UInteger& a, const UInteger b,
                              Function fun,
                              const size_t bits = UInteger::width())
{
    const UInteger result = fun(a,b);
    const UInteger mask = ~generate_bitmask<UInteger>(UInteger::width() - bits);

    return result & mask;
}

template <class UInteger>
[[nodiscard]] UInteger approx_add_post_masking(const UInteger& a, const UInteger b,
                                               const size_t bits = UInteger::width())
{
    return approx_operation_post_masking(a,b,&aarith::add<UInteger::width()>,bits);
}

template <class UInteger>
[[nodiscard]] UInteger approx_mul_post_masking(const UInteger& a, const UInteger b,
                                               const size_t bits = UInteger::width())
{
    return approx_operation_post_masking(a,b,&aarith::mul<UInteger::width()>,bits);
}

template <class UInteger>
[[nodiscard]] UInteger approx_sub_post_masking(const UInteger& a, const UInteger b,
                                               const size_t bits = UInteger::width())
{
    return approx_operation_post_masking(a,b,&aarith::sub<UInteger::width()>,bits);
}

template <class UInteger>
[[nodiscard]] UInteger approx_div_post_masking(const UInteger& a, const UInteger b,
                                               const size_t bits = UInteger::width())
{
    return approx_operation_post_masking(a,b,&aarith::div<UInteger>,bits);
}

template <class UInteger>
[[nodiscard]] UInteger approx_rem_post_masking(const UInteger& a, const UInteger b,
                                               const size_t bits = UInteger::width())
{
    return approx_operation_post_masking(a,b,&aarith::remainder<UInteger>,bits);
}

template <class UInteger>
auto approx_add_pre_masking(const UInteger& opd1, const UInteger& opd2,
                            const size_t bits = UInteger::width()) -> UInteger
{
    auto const mask = ~generate_bitmask<UInteger>(UInteger::width() - bits);

    auto const opd1_masked = opd1 & mask;
    auto const opd2_masked = opd2 & mask;

    auto const sum = add(opd1_masked, opd2_masked);
    return sum;
}

template <size_t width>
auto approx_uint_bitmasking_mul(const uinteger<width>& opd1, const uinteger<width>& opd2,
                                const size_t bits) -> uinteger<2 * width>
{
    constexpr auto product_width = 2 * width;

    auto const mask = ~generate_bitmask<uinteger<product_width>>(product_width - bits);

    uinteger<product_width> opd2_extended = width_cast<product_width, width>(opd2);

    uinteger<product_width> product;
    for (auto i = 0U; i < width; ++i)
    {
        auto const opd2_masked = opd2_extended & mask;
        product = ((opd1.bit(i) == 0) ? product : add(product, opd2_masked));
        opd2_extended <<= 1;
    }

    return product;
}

} // namespace aarith
