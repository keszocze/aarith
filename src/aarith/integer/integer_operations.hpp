#pragma once

#include <aarith/integer/integer.hpp>
#include <aarith/integer/uinteger.hpp>
#include <aarith/integer/uinteger_operations.hpp>

namespace aarith {

/**
 * @brief Arithmetic right-shift operator
 * @tparam Width The width of the signed integer
 * @param lhs The integer to be shifted
 * @param rhs The number of bits to be shifted
 * @return
 */
template <size_t Width>
auto operator>>(const integer<Width>& lhs, const size_t rhs) -> integer<Width>
{
    if (rhs >= Width)
    {
        if (lhs.is_negative())
        {
            return integer<Width>::all_ones();
        }
        else
        {
            return integer<Width>(0U);
        }
    }
    if (rhs == 0)
    {
        return lhs;
    }

    integer<Width> shifted;

    if (lhs.is_zero())
    {
        shifted = integer<Width>::all_ones();
    }

    const auto skip_words = rhs / lhs.word_width();
    const auto shift_word_right = rhs - skip_words * lhs.word_width();
    const auto shift_word_left = lhs.word_width() - shift_word_right;

    for (auto counter = skip_words; counter < lhs.word_count(); ++counter)
    {
        typename integer<Width>::word_type new_word;
        new_word = lhs.word(counter) >> shift_word_right;
        if (shift_word_left < lhs.word_width() && counter + 1 < lhs.word_count())
        {
            new_word = new_word | (lhs.word(counter + 1) << shift_word_left);
        }

        shifted.set_word(counter - skip_words, new_word);
    }
    typename integer<Width>::word_type new_word;
    new_word = lhs.word(lhs.word_count() - 1) >> shift_word_right;

    shifted.set_word(lhs.word_count() - skip_words - 1, new_word);

    if (lhs.is_negative())
    {
        for (size_t i = (Width - 1); i >= (Width - rhs); --i)
        {
            shifted.set_bit(i);
        }
    }

    return shifted;
}

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
[[nodiscard]] integer<std::max(W, V) + 1> expanding_add(const integer<W>& a, const integer<V>& b,
                                                        const bool initial_carry = false)
{
    static_assert(is_integral<integer<W>>::value);
    static_assert(is_integral<integer<V>>::value);

    constexpr size_t res_width = std::max(W, V) + 1U;

    integer<res_width> sum;
    integer<res_width> a_ = width_cast<res_width>(a);
    integer<res_width> b_ = width_cast<res_width>(b);

    using word_type = typename uinteger<res_width>::word_type;
    word_type carry = initial_carry ? 1U : 0U;
    for (auto i = 0U; i < a_.word_count(); ++i)
    {
        word_type word_a{a_.word(i)};
        word_type word_b{b_.word(i)};

        word_type partial_sum = word_a + word_b;
        word_type new_carry = (partial_sum < word_a || partial_sum < word_b) ? 1 : 0;
        partial_sum = partial_sum + carry;
        carry = new_carry || partial_sum < word_a || partial_sum < word_b;
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
template <size_t W>[[nodiscard]] integer<W> add(const integer<W>& a, const integer<W>& b)
{
    integer<W + 1> result = expanding_add<W, W>(a, b);
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
template <size_t W>[[nodiscard]] auto sub(const integer<W>& a, const integer<W>& b) -> integer<W>
{
    static_assert(is_integral<integer<W>>::value);

    integer<W> result;
    integer<W> minus_b = add(~b, integer<W>(1U));
    result = add(a, minus_b);
    return result;
}

/**
 * @brief Subtracts two signed integers of, possibly, different bit widths.
 *
 * @tparam W Width of the minuend
 * @tparam V Width of the subtrahend
 * @param a Minuend
 * @param b Subtrahend
 * @return Difference of correct bit width
 */
template <size_t W, size_t V>
[[nodiscard]] auto expanding_sub(const integer<W>& a, const integer<V>& b)
    -> integer<std::max(W, V)>
{
    constexpr size_t res_width = std::max(W, V);
    uinteger<res_width> result{sub(width_cast<res_width>(a), width_cast<res_width>(b))};

    return result;
}

/**
 * @brief Multiplies two signed integers.
 *
 *
 * This implements the Booth multiplication algorithm with extension to correctly handle the
 * most negative number. See https://en.wikipedia.org/wiki/Booth%27s_multiplication_algorithm
 * for details.
 *
 * @tparam W The bit width of the first multiplicant
 * @tparam V The bit width of the second multiplicant
 * @param a First multiplicant
 * @param b Second multiplicant
 * @return Product of a and b
 */
template <size_t W, size_t V>
[[nodiscard]] auto expanding_mul(const integer<W>& m, const integer<V>& r) -> integer<V + W>
{

    constexpr size_t K = W + V + 2;

    integer<K> A{width_cast<W + 1>(m)};
    integer<K> S = -A;

    A = A << V + 1;
    S = S << V + 1;

    integer<K> P{r};
    P = P << 1;

    for (size_t i = 0; i < V; ++i)
    {

        bool last_bit = P.bit(0);
        bool snd_last_bit = P.bit(1);

        if (snd_last_bit && !last_bit)
        {
            P = add(P, S);
        }
        if (!snd_last_bit && last_bit)
        {
            P = add(P, A);
        }

        P = P >> 1;
    }

    return width_cast<W + V>(P >> 1);
}

/**
 * @brief Multiplies two signed integers.
 *
 * @note No Type conversion is performed. If the bit widths do not match, the code will not
 * compile! Use @see expanding_mul for that.
 *
 * This implements the Booth multiplication algorithm with extension to correctly handle the
 * most negative number. See https://en.wikipedia.org/wiki/Booth%27s_multiplication_algorithm
 * for details. The result is then cropped to fit the initial bit width
 *
 * @tparam W The bit width of the multiplicants
 * @param a First multiplicant
 * @param b Second multiplicant
 * @return Product of a and b
 */
template <size_t W>[[nodiscard]] integer<W> mul(const integer<W>& a, const integer<W>& b)
{
    return width_cast<W>(expanding_mul(a, b));
}

/**
 * @brief Computes the absolute value of a given signed integer.
 *
 * @warn There is a potential loss of precision as abs(integer::min) > integer::max
 *
 * @tparam Width The width of the signed integer
 * @param n The signed inter to be "absolute valued"
 * @return The absolute value of the signed integer
 */
template <size_t Width>[[nodiscard]] auto abs(const integer<Width>& n) -> integer<Width>
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
template <size_t Width>[[nodiscard]] auto expanding_abs(const integer<Width>& n) -> uinteger<Width>
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
template <size_t W> auto operator-(const integer<W>& n) -> integer<W>
{
    const integer<W> one(1U);
    return add(~n, one);
}

/**
 * @brief Implements the restoring division algorithm.
 *
 * @note integer<W>::min/integer<W>(-1) will return <integer<W>::min,0>, i.e. some weird
 * overflow happens
 *
 * @see https://en.wikipedia.org/wiki/Division_algorithm#Restoring_division
 *
 * @param numerator The number that is to be divided
 * @param denominator The number that divides the other number
 * @tparam W Width of the numbers used in division.
 *
 * @return Pair of (quotient, remainder)
 *
 */
template <std::size_t W, std::size_t V>
[[nodiscard]] std::pair<integer<W>, integer<W>> restoring_division(const integer<W>& numerator,
                                                                   const integer<V>& denominator)

{

    using SInteger = integer<W>;
    //    using LargeSInteger = integer<2 * W>;

    // Cover some special cases in order to speed everything up
    if (denominator.is_zero())
    {
        throw std::runtime_error("Attempted division by zero");
    }
    if (numerator.is_zero())
    {
        return std::make_pair(SInteger::zero(), SInteger::zero());
    }
    if (denominator == SInteger::one())
    {
        return std::make_pair(numerator, SInteger::zero());
    }

    if (numerator == denominator)
    {
        return std::make_pair(SInteger::one(), SInteger::zero());
    }

    const bool negate = numerator.is_negative() ^ denominator.is_negative();

    const uinteger<W> N = expanding_abs(numerator);
    const uinteger<W> D = expanding_abs(denominator);

    if (N < D)
    {
        return std::make_pair(SInteger::zero(), numerator);
    }

    const auto div = restoring_division(N, D);

    integer<W + 1> Q(div.first);
    integer<W + 1> remainder(div.second);

    if (negate)
    {
        Q = -Q;
    }

    integer<W> Q_cast = width_cast<W>(Q);
    integer<W> remainder_cast = width_cast<W>(remainder);

    return std::make_pair(Q_cast, remainder_cast);
}

/**
 * @brief Computes the remainder of the division of one integer by another integer
 * *
 * @tparam W Width of the numbers being used in the division
 * @param numerator The number that is to be divided
 * @param denominator The number that divides the other number
 * @return The remainder of the division operation
 */
template <size_t W>
[[nodiscard]] auto remainder(const integer<W>& numerator, const integer<W>& denominator)
    -> integer<W>
{
    return restoring_division(numerator, denominator).second;
}

/**
 * @brief Divides one integer by another integer
 *
 * @note integer<W>::min/integer<W>(-1) will return <integer<W>::min,0>, i.e. some weird
 * overflow happens
 *
 * @tparam W Width of the numbers being used in the division
 * @param numerator The number that is to be divided
 * @param denominator The number that divides the other number
 * @return The quotient of the division operation
 */
template <size_t W>
[[nodiscard]] auto div(const integer<W>& numerator, const integer<W>& denominator) -> integer<W>
{
    return restoring_division(numerator, denominator).first;
}

/**
 * @brief Adds two signed integers of, possibly, different bit widths.
 *
 * This is an implementation using a more functional style of programming. It is not particularly
 * fast and only here for eudcational purposes. You can use method as a means to understand how to
 * work on an integer
 *
 * @tparam W Width of the first summand
 * @tparam V Width of the second summand
 * @param a First summand
 * @param b Second summand
 * @param initial_carry True if there is an initial carry coming in
 * @return Sum of correct maximal bit width
 */
template <class IntA, class IntB>
[[nodiscard]] auto fun_add_expand(const IntA& a, const IntB& b, const bool initial_carry = false)
    -> decltype(width_cast<std::max(IntA::width(), IntB::width()) + 1U>(a))
// IntC
{
    static_assert(is_integral<IntA>::value);
    static_assert(is_integral<IntB>::value);
    static_assert(is_unsigned<IntA>::value == is_unsigned<IntB>::value);
    static_assert(std::is_same<typename IntA::word_type, typename IntB::word_type>::value);

    constexpr size_t res_width = std::max(IntA::width(), IntB::width()) + 1U;
    using word_type = typename IntA::word_type;
    word_type carry = initial_carry ? 1U : 0U;

    const auto a_expanded = width_cast<res_width>(a);
    const auto b_expanded = width_cast<res_width>(b);

    const auto f = [carry = word_type(carry)](word_type ain, word_type bin) mutable {
        word_type partial_sum = ain + bin;
        word_type new_carry = (partial_sum < ain || partial_sum < bin) ? 1U : 0U;

        partial_sum = partial_sum + carry;
        carry = new_carry || (partial_sum < ain || partial_sum < bin) ? 1U : 0U;

        return partial_sum;
    };

    const auto result = width_cast<res_width>(zip_with(a_expanded, b_expanded, f));
    return result;
}

/**
 * @brief Adds two signed integers of, possibly, different bit widths.
 *
 * @see fun_add_expand
 *
 * @tparam W Width of the first summand
 * @tparam V Width of the second summand
 * @param a First summand
 * @param b Second summand
 * @param initial_carry True if there is an initial carry coming in
 * @return Sum of correct maximal bit width
 */
template <class Int>
[[nodiscard]] auto fun_add(const Int& a, const Int& b, const bool initial_carry = false) -> Int
{
    return width_cast<Int::width()>(fun_add_expand(a, b, initial_carry));
}

} // namespace aarith
