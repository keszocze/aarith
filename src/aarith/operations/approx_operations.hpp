#pragma once

#include <aarith/core/word_array.hpp>
#include <aarith/integer.hpp>
#include <cstdint>
#include <iostream>

namespace aarith {

/**
 * @brief Generates an uinteger of given size with the specified number of leading ones.
 *
 * Example: generate_bitmask<uinteger<10>>(3) = 1110000000
 *
 * @note There should never be a reason to call this function directly as a library user.
 *
 * @tparam Integer The underlying data type
 * @param leading_ones The number of leading ones
 * @return The bit bask consisting of leading_ones ones followed by zeros
 */
template <class Integer> auto generate_bitmask(const size_t leading_ones) -> Integer
{
    using word_type = typename Integer::word_type;

    const size_t bits = Integer::width() - leading_ones;

    constexpr auto full_mask = static_cast<word_type>(-1);
    const auto full_mask_words = bits / static_cast<size_t>(Integer::word_width());
    const auto remaining_bits = bits % static_cast<size_t>(Integer::word_width());

    const typename Integer::word_type last_word_mask = (static_cast<word_type>(1) << bits) - 1;

    Integer mask;
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

template <class Integer, class Function>
[[nodiscard]] Integer approx_operation_post_masking(const Integer& a, const Integer b, Function fun,
                                                    size_t bits)
{

    static_assert(is_integral_v<Integer>);

    /*
     * In case of signed integers we *always* want to have the signed bit correct and, therefore,
     * increase the number of correctly computed bits by one
     */
    if constexpr (!is_unsigned_v<Integer>)
    {
        bits = std::max(bits + 1, Integer::width());
    }

    const Integer result = fun(a, b);
    const Integer mask = generate_bitmask<Integer>(bits);

    return result & mask;
}

template <class Integer>
[[nodiscard]] Integer approx_add_post_masking(const Integer& a, const Integer b,
                                              const size_t bits = Integer::width())
{
    const auto fun = [](const Integer& a_, const Integer& b_) { return add(a_, b_); };
    return approx_operation_post_masking(a, b, fun, bits);
}

template <class Integer>
[[nodiscard]] Integer approx_mul_post_masking(const Integer& a, const Integer b,
                                              const size_t bits = Integer::width())
{
    const auto fun = [](const Integer& a_, const Integer& b_) { return mul(a_, b_); };
    return approx_operation_post_masking(a, b, fun, bits);
}

template <class Integer>
[[nodiscard]] Integer approx_sub_post_masking(const Integer& a, const Integer b,
                                              const size_t bits = Integer::width())
{
    const auto fun = [](const Integer& a_, const Integer& b_) { return sub(a_, b_); };
    return approx_operation_post_masking(a, b, fun, bits);
}

template <class Integer>
[[nodiscard]] Integer approx_div_post_masking(const Integer& a, const Integer b,
                                              const size_t bits = Integer::width())
{
    const auto fun = [](const Integer& a_, const Integer& b_) { return div(a_, b_); };
    return approx_operation_post_masking(a, b, fun, bits);
}

template <class Integer>
[[nodiscard]] Integer approx_rem_post_masking(const Integer& a, const Integer b,
                                              const size_t bits = Integer::width())
{
    const auto fun = [](const Integer& a_, const Integer& b_) { return rem(a_, b_); };
    return approx_operation_post_masking(a, b, fun, bits);
}

template <typename Integer, typename Function>
[[nodiscard]] Integer approx_operation_pre_masking(const Integer& a, const Integer b, Function fun,
                                                   const size_t bits = Integer::width())
{
    static_assert(is_integral_v<Integer>);

    /*
     * In case of signed integers we *always* want to have the signed bit correct and, therefore,
     * increase the number of correctly computed bits by one
     */
    if constexpr (!is_unsigned_v<Integer>)
    {
        bits = std::max(bits + 1, Integer::width());
    }

    const Integer mask = generate_bitmask<Integer>(bits);
    auto const a_masked = a & mask;
    auto const b_masked = b & mask;

    return fun(a_masked, b_masked);
}

template <class Integer>
[[nodiscard]] Integer approx_add_pre_masking(const Integer& a, const Integer b,
                                             const size_t bits = Integer::width())
{

    const auto fun = [](const Integer& a_, const Integer& b_) { return add(a_, b_); };
    return approx_operation_pre_masking(a, b, fun, bits);
}

template <class Integer>
[[nodiscard]] Integer approx_mul_pre_masking(const Integer& a, const Integer b,
                                             const size_t bits = Integer::width())
{

    const auto fun = [](const Integer& a_, const Integer& b_) { return mul(a_, b_); };
    return approx_operation_pre_masking(a, b, fun, bits);
}

template <class Integer>
[[nodiscard]] Integer approx_sub_pre_masking(const Integer& a, const Integer b,
                                             const size_t bits = Integer::width())
{
    const auto fun = [](const Integer& a_, const Integer& b_) { return sub(a_, b_); };
    return approx_operation_pre_masking(a, b, fun, bits);
}

template <class Integer>
[[nodiscard]] Integer approx_div_pre_masking(const Integer& a, const Integer b,
                                             const size_t bits = Integer::width())
{
    const auto fun = [](const Integer& a_, const Integer& b_) { return div(a_, b_); };
    return approx_operation_pre_masking(a, b, fun, bits);
}

template <class Integer>
[[nodiscard]] Integer approx_rem_pre_masking(const Integer& a, const Integer b,
                                             const size_t bits = Integer::width())
{
    const auto fun = [](const Integer& a_, const Integer& b_) { return rem(a_, b_); };
    return approx_operation_pre_masking(a, b, fun, bits);
}

/**
 * @brief Multiplies the given most-significand portion of two unsigned integers by masking the
 * partial products used in the multiplication.
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
        opd2_extended = (opd2_extended << 1);
    }

    return product;
}

/** @brief Approximately adds two unsigned integers.
 *
 * This adder does not propagate the carry from one word to the next word within the word_array that
 * stores the unsigned integer.
 *
 * This addition is neither fast nor a really good approximate adder. It is merely for educational
 * purposes to show how to use the zip_with_expand function.
 *
 * @tparam W Bit width of the first summand
 * @tparam V Bit width of the second summand
 * @param a First summand
 * @param b Second summand
 * @return Aproximated addition of a and b
 */
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
