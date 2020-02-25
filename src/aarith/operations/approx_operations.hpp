#pragma once

#include <aarith/integer/uinteger.hpp>
#include <aarith/integer/uinteger_operations.hpp>
#include <cstdint>
#include <iostream>

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
template <class UInteger> auto generate_bitmask(const size_t leading_ones) -> UInteger
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

template <class UInteger, class Function>
[[nodiscard]] UInteger approx_operation_pre_masking(const UInteger& a, const UInteger b,
                                                    Function fun,
                                                    const size_t bits = UInteger::width())
{
    const UInteger mask = generate_bitmask<UInteger>(bits);
    auto const a_masked = a & mask;
    auto const b_masked = b & mask;

    return fun(a_masked, b_masked);
}

template <class UInteger>
[[nodiscard]] UInteger approx_add_pre_masking(const UInteger& a, const UInteger b,
                                              const size_t bits = UInteger::width())
{
    return approx_operation_pre_masking(a, b, &aarith::add<UInteger::width()>, bits);
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

template <size_t W, size_t V>
[[nodiscard]] uinteger<std::max(W, V)> trivial_approx_add(const uinteger<W>& a,
                                                          const uinteger<V>& b)
{

    const auto word_adder = [](const typename uinteger<W>::word_type a_,
                               const typename uinteger<V>::word_type b_) { return a_ + b_; };

    uinteger<std::max(W, V)> result = zip_with_expand<decltype(word_adder), W, V>(a, b, word_adder);

    return result;
}

} // namespace aarith
