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
 * @brief Computes the absolute value of a given signed integer.
 * @tparam Width The width of the signed integer
 * @param n The signed inter to be "absolute valued"
 * @return The absolute value of the signed integer 
 */
template <size_t Width> [[nodiscard]] auto abs(const sinteger<Width>& n) -> sinteger<Width>
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
template <size_t Width> [[nodiscard]] auto expanding_abs(const sinteger<Width>& n) -> uinteger<Width>
{
    uinteger<Width> abs = n.is_negative() ? -n : n;
    return abs;
}

template <size_t W> auto operator-(const sinteger<W>& n) -> sinteger<W>
{
    const sinteger<W> one(1U);
    return add(~n, one);
}

} // namespace aarith
