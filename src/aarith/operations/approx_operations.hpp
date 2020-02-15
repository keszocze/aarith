#pragma once

#include "aarith/types/uinteger.hpp"
#include <aarith/operations/sinteger_operations.hpp>
#include <aarith/operations/uinteger_operations.hpp>
#include <cstdint>
#include <iostream>
#include <functional>

namespace aarith {

/**
 * @brief Generates an uinteger of given size with the specified number of leading ones.
 *
 * Example: generate_bitmask<uinteger<10>(3) = 1110000000
 *
 * @tparam UInteger The underlying data type
 * @param leading_ones The number of leading ones
 * @return The bit bask consisting of leading_ones ones followed by zeros
 */
template <class UInteger> constexpr auto generate_bitmask(const size_t leading_ones) -> UInteger
{
    using word_type = typename UInteger::word_type;

    const size_t bits = UInteger::width() - leading_ones;

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
    return ~mask;
}

template <class UInteger, class Function>
[[nodiscard]] UInteger approx_operation_post_masking(const UInteger& a, const UInteger b,
                                                     Function fun,
                                                     const size_t bits = UInteger::width())
{
    const UInteger result = fun(a, b);
    const UInteger mask = generate_bitmask<UInteger>(bits);

    return result & mask;
}

template <class UInteger>
[[nodiscard]] UInteger approx_add_post_masking(const UInteger& a, const UInteger b,
                                               const size_t bits = UInteger::width())
{
    return approx_operation_post_masking(a, b, &aarith::add<UInteger::width()>, bits);
}

template <class UInteger>
[[nodiscard]] UInteger approx_mul_post_masking(const UInteger& a, const UInteger b,
                                               const size_t bits = UInteger::width())
{
    return approx_operation_post_masking(a, b, &aarith::mul<UInteger::width()>, bits);
}

template <class UInteger>
[[nodiscard]] UInteger approx_sub_post_masking(const UInteger& a, const UInteger b,
                                               const size_t bits = UInteger::width())
{
    return approx_operation_post_masking(a, b, &aarith::sub<UInteger::width()>, bits);
}

template <class UInteger>
[[nodiscard]] UInteger approx_div_post_masking(const UInteger& a, const UInteger b,
                                               const size_t bits = UInteger::width())
{
    return approx_operation_post_masking(a, b, &aarith::div<UInteger>, bits);
}

template <class UInteger>
[[nodiscard]] UInteger approx_rem_post_masking(const UInteger& a, const UInteger b,
                                               const size_t bits = UInteger::width())
{
    return approx_operation_post_masking(a, b, &aarith::remainder<UInteger>, bits);
}

template <size_t W>
[[nodiscard]] uinteger<W>
approx_operation_pre_masking(const uinteger<W>& a, const uinteger<W> b,
                             const std::function<uinteger<W>(uinteger<W>, uinteger<W>)> fun,
                             size_t width = W)
{
    const uinteger<W> mask = generate_bitmask<uinteger<W>>(width);
    auto const a_masked = a & mask;
    auto const b_masked = b & mask;

    return fun(a_masked, b_masked);
}

template <size_t W, class word_type>
[[nodiscard]] uinteger<W> approx_add_pre_masking(const uinteger<W, word_type>& a,
                                                 const uinteger<W, word_type> b,
                                                 const size_t width = W)
{

    /*
     * I *hate* C++ for making me use an explicit std::function because it is too stupid to actually
     * deduce parameters from a lambda (for which I even explicitly stated the intended return type)
     */
    const std::function<uinteger<W>(uinteger<W>, uinteger<W>)> add_fun =
        [](const uinteger<W, word_type>& a_,
           const uinteger<W, word_type>& b_) -> uinteger<W, word_type> {
        return aarith::add<W>(a_, b_, false);
    };
    return approx_operation_pre_masking(a, b, add_fun, width);
}

template <class UInteger>
[[nodiscard]] UInteger approx_mul_pre_masking(const UInteger& a, const UInteger b,
                                              const size_t bits = UInteger::width())
{
    return approx_operation_pre_masking(a, b, &aarith::mul<UInteger::width()>, bits);
}

template <class UInteger>
[[nodiscard]] UInteger approx_sub_pre_masking(const UInteger& a, const UInteger b,
                                              const size_t bits = UInteger::width())
{
    return approx_operation_pre_masking(a, b, &aarith::sub<UInteger::width()>, bits);
}

template <class UInteger>
[[nodiscard]] UInteger approx_div_pre_masking(const UInteger& a, const UInteger b,
                                              const size_t bits = UInteger::width())
{
    return approx_operation_pre_masking(a, b, &aarith::div<UInteger>, bits);
}

template <class UInteger>
[[nodiscard]] UInteger approx_rem_pre_masking(const UInteger& a, const UInteger b,
                                              const size_t bits = UInteger::width())
{
    return approx_operation_pre_masking(a, b, &aarith::remainder<UInteger>, bits);
}

/**
 * @brief Multiplies the given most-significand portion of two uintegers by masking the partial
 * products used in the multiplication
 *
 * @tparam Width The width of the used uintegers
 * @param opd1 Multiplier
 * @param opd2 Multiplicand
 * @param bits Number of most-significand bits to be calculated
 * @return The result of the multiplication with double the size of the inputs
 */
template <size_t width>
auto approx_uint_bitmasking_mul(const uinteger<width>& opd1, const uinteger<width>& opd2,
                                const size_t bits) -> uinteger<2 * width>
{
    constexpr auto product_width = 2 * width;

    auto const mask = generate_bitmask<uinteger<product_width>>(bits);

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

template <size_t width, size_t lsp_width, size_t shared_bits = 0>
uinteger<width+1> FAUadder(const uinteger<width>& a, [[maybe_unused]] const uinteger<width>& b)
{

    static_assert(shared_bits <= lsp_width);
    static_assert(lsp_width < width);
    static_assert(lsp_width > 0);

    constexpr size_t lsp_index = lsp_width - 1;

    const auto a_split = split<lsp_index>(a);
    const auto b_split = split<lsp_index>(b);

    constexpr size_t msp_width = width - lsp_width;


    const uinteger<lsp_width> a_lsp = a_split.second;
    const uinteger<lsp_width> b_lsp = b_split.second;

    const uinteger<msp_width> a_msp = a_split.first;
    const uinteger<msp_width> b_msp = b_split.first;

    uinteger<lsp_width + 1> lsp_sum = expanding_add(a_lsp, b_lsp);

    uinteger<lsp_width> lsp = width_cast<lsp_width>(lsp_sum);


    // conditionally perform carry prediction
    bool predicted_carry = false;
    if constexpr (shared_bits > 0)
    {
        uinteger<shared_bits> a_shared = bit_range<lsp_index, lsp_index - (shared_bits - 1)>(a);
        uinteger<shared_bits> b_shared = bit_range<lsp_index, lsp_index - (shared_bits - 1)>(b);

        uinteger<shared_bits + 1> shared_sum = expanding_add(a_shared, b_shared);

        predicted_carry = shared_sum.msb();
    }

    // only if we did not predict a carry, we are going to use the all1 rule for error correction
    if (lsp_sum.msb() && !predicted_carry)
    {
        lsp = lsp.all_ones();
    }

    const uinteger<msp_width + 1> msp = expanding_add(a_msp,b_msp,predicted_carry);

    uinteger<width+1> result{lsp};

    const auto extended_msp = width_cast<width + 1>(msp);
    result = add(result, extended_msp << lsp_width);

    return result;
}

} // namespace aarith
