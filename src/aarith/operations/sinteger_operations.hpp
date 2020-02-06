#pragma once

#include <aarith/types/sinteger.hpp>
#include <aarith/types/uinteger.hpp>

namespace aarith {

/**
 * @brief Adds two signed integers of, possibly, different bit widths.
 *
 * @tparam W Width of the first summand
 * @tparam V Width of the second summand
 * @param a First summand
 * @param b Second summand
 * @param initial_carry True if there is an initial carry coming in
 * @return Sum of correct maximal bit width
 */
template <size_t W, size_t V>
[[nodiscard]] sinteger<std::max(W, V) + 1> expanding_add(const sinteger<W>& a, const sinteger<V>& b)
{
    static_assert(is_integral<sinteger<W>>::value);
    static_assert(is_integral<sinteger<V>>::value);

    constexpr size_t res_width = std::max(W, V) + 1U;

    sinteger<res_width> sum;
    typename sinteger<res_width>::word_type carry{0U};
    for (auto i = 0U; i < a.word_count(); ++i)
    {
        auto const partial_sum = a.word(i) + b.word(i) + carry;
        carry = (partial_sum < a.word(i) || partial_sum < b.word(i)) ? 1 : 0;
        sum.set_word(i, partial_sum);
    }
    return sum;
}

/**
 * @brief Adds two signed integers
 *
 * @tparam W The width of the signed integer
 * @param a First summand
 * @param b Second summand
 * @return Sum of a and b
 */
template <size_t W>[[nodiscard]] sinteger<W> add(const sinteger<W>& a, const sinteger<W>& b)
{
    sinteger<W + 1> result = expanding_add<W, W>(a, b);
    return width_cast<W>(result);
}

/**
 * @brief Computes the difference of two signed integers.
 *
 * @tparam W the bit width of the operands
 * @param a Minuend
 * @param b Subtrahend
 * @return Difference between a and b
 */
template <size_t W>[[nodiscard]] auto sub(const sinteger<W>& a, const sinteger<W>& b) -> sinteger<W>
{
    static_assert(is_integral<sinteger<W>>::value);

    sinteger<W> result;
    sinteger<W> minus_b = add(~b, sinteger<W>(1U));
    result = add(a, minus_b);
    return result;
}

/**
 * @brief Computes the absolute value of a given signed integer.
 * @tparam Width The width of the signed integer
 * @param n The signed inter to be "absolute valued"
 * @return The absolute value of the signed integer
 */
template <size_t Width>[[nodiscard]] auto abs(const sinteger<Width>& n) -> sinteger<Width>
{
    return n.is_negative() ? -n : n;
}

/**
 * @brief Computes the absolute value of a given signed integer.
 *
 * This method returns an unsigned integer. This means that the absolute value
 * will fit and no overflow will happen.
 *
 * @tparam Width The width of the signed integer
 * @param n The signed inter to be "absolute valued"
 * @return The absolute value of the signed integer
 */
template <size_t Width>[[nodiscard]] auto expanding_abs(const sinteger<Width>& n) -> uinteger<Width>
{
    uinteger<Width> abs = n.is_negative() ? -n : n;
    return abs;
}

/**
 *
 * @tparam W The width of the signed integer
 * @param n  The signed integer whose sign is to be changed
 * @return  The negative value of the signed integer
 */
template <size_t W> auto operator-(const sinteger<W>& n) -> sinteger<W>
{
    const sinteger<W> one(1U);
    return add(~n, one);
}

/**
 * @brief Arithmetic right-shift operator
 * @tparam Width The width of the signed integer
 * @param lhs The integer to be shifted
 * @param rhs The number of bits to be shifted
 * @return
 */
template <size_t Width>
auto operator>>(const sinteger<Width>& lhs, const size_t rhs) -> sinteger<Width>
{
    if (rhs >= Width)
    {
        if (lhs.is_negative())
        {
            return sinteger<Width>::all_ones();
        }
        else
        {
            return sinteger<Width>(0U);
        }
    }
    if (rhs == 0)
    {
        return lhs;
    }

    sinteger<Width> shifted;

    if (lhs.is_zero())
    {
        shifted = sinteger<Width>::all_ones();
    }

    const auto skip_words = rhs / lhs.word_width();
    const auto shift_word_right = rhs - skip_words * lhs.word_width();
    const auto shift_word_left = lhs.word_width() - shift_word_right;

    for (auto counter = skip_words; counter < lhs.word_count(); ++counter)
    {
            typename sinteger<Width>::word_type new_word;
            new_word = lhs.word(counter) >> shift_word_right;
            if (shift_word_left < lhs.word_width() && counter + 1 < lhs.word_count())
            {
                new_word = new_word | (lhs.word(counter + 1) << shift_word_left);
            }

//            if (skip_words == counter && lhs.is_negative())
//            {
//                typename sinteger<Width>::word_type mask(-1);
//                mask = mask << shift_word_left;
//                new_word = new_word | mask;
//            }

            shifted.set_word(counter - skip_words, new_word);
    }
    typename sinteger<Width>::word_type new_word;
    new_word = lhs.word(lhs.word_count() - 1) >> shift_word_right;

    if (lhs.is_negative())
    {
        typename sinteger<Width>::word_type ones(-1);
        typename sinteger<Width>::word_type mask;

        for (size_t i = lhs.word_count() - skip_words; i < lhs.word_count(); ++i) {
            shifted.set_word(i, ones);
        }

        mask = ones << shift_word_left;
        new_word = new_word | mask;
    }

    shifted.set_word(lhs.word_count() - skip_words - 1, new_word);

    return shifted;
}

} // namespace aarith
