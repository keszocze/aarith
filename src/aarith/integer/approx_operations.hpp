#pragma once

#include <aarith/core/word_array.hpp>
#include <aarith/core/word_array_operations.hpp>
#include <aarith/integer/integers.hpp>
#include <aarith/integer/integer_operations.hpp>
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
template <class Integer> auto generate_bitmask(size_t leading_ones) -> Integer
{
    using word_type = typename Integer::word_type;

    if (leading_ones > Integer::width())
    {
        leading_ones = Integer::width();
    }

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

    static_assert(::aarith::is_integral_v<Integer>);

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

template <size_t W>
[[nodiscard]] auto approx_expanding_add_post_masking(const uinteger<W> a, const uinteger<W> b,
                                                     const size_t bits = W)
-> uinteger<W+1>
{
    const auto result = expanding_add(a,b);
    const auto mask = generate_bitmask<uinteger<result.width()>>(bits);

    return result & mask;
}

template <class Integer>
[[nodiscard]] Integer approx_sub_post_masking(const Integer& a, const Integer b,
                                              const size_t bits = Integer::width())
{
    const auto fun = [](const Integer& a_, const Integer& b_) { return sub(a_, b_); };
    return approx_operation_post_masking(a, b, fun, bits);
}

template <size_t W>
[[nodiscard]] auto approx_expanding_sub_post_masking(const uinteger<W> a, const uinteger<W> b,
                                                     const size_t bits = W)
-> uinteger<W+1>
{
    const auto result = expanding_sub(a,b);
    const auto mask = generate_bitmask<uinteger<result.width()>>(bits);

    return result & mask;
}

template <class Integer>
[[nodiscard]] Integer approx_mul_post_masking(const Integer& a, const Integer b,
                                              const size_t bits = Integer::width())
{
    const auto fun = [](const Integer& a_, const Integer& b_) { return mul(a_, b_); };
    return approx_operation_post_masking(a, b, fun, bits);
}

template <size_t W>
[[nodiscard]] auto approx_expanding_mul_post_masking(const uinteger<W>& a, const uinteger<W> b,
                                                     const size_t bits = 2*W)
-> uinteger<2*W>
{
    const auto result = schoolbook_expanding_mul(a, b);
    const auto mask = generate_bitmask<uinteger<result.width()>>(bits);

    return result & mask;
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
    static_assert(::aarith::is_integral_v<Integer>);

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
template <size_t Width, typename WordType>
auto approx_uint_bitmasking_mul(const uinteger<Width, WordType>& opd1,
                                const uinteger<Width, WordType>& opd2, const size_t bits)
-> uinteger<2 * Width, WordType>
{
    constexpr auto product_width = 2 * Width;

    auto const mask = generate_bitmask<uinteger<product_width, WordType>>(bits);

    uinteger<product_width, WordType> opd2_extended = width_cast<product_width, Width>(opd2);

    uinteger<product_width, WordType> product;
    for (auto i = 0U; i < Width; ++i)
    {
        auto const opd2_masked = opd2_extended & mask;
        product = ((opd1.bit(i) == 0) ? product : add(product, opd2_masked));
        opd2_extended = (opd2_extended << 1);
    }

    return product;
}

template <size_t width, size_t lsp_width, size_t shared_bits = 0>
uinteger<width+1> FAUadder(const uinteger<width>& a, const uinteger<width>& b)
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
template <size_t W, size_t V, typename WordType>
[[nodiscard]] uinteger<std::max(W, V), WordType> trivial_approx_add(const uinteger<W, WordType> a,
                                                                    const uinteger<V, WordType> b)
{

    constexpr auto word_adder = [](const WordType a_, const WordType b_) { return a_ + b_; };

    const uinteger<std::max(W, V), WordType> result =
        zip_with_expand<decltype(word_adder), W, V, WordType>(a, b, word_adder);


    return result;
}

template <size_t width, size_t lsp_width, size_t shared_bits = 0>
uinteger<width+1> FAU_sub(const uinteger<width>& a, [[maybe_unused]] const uinteger<width>& b)
{
    auto b_inv = ~width_cast<width+1>(b);
    const auto one = uinteger<width+1>(1U);
    b_inv = add(b_inv, one);

    const auto a_ext = width_cast<width+1>(a);
    return width_cast<width+1>(FAUadder<width+1, lsp_width, shared_bits>(a_ext, b_inv));
}

} // namespace aarith
