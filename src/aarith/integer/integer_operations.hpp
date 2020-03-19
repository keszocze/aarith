#pragma once

#include <aarith/integer.hpp>

namespace aarith {

/**
 * @brief Adds two unsigned integers of, possibly, different bit widths.
 *
 * @tparam I Integer type of the first summand
 * @tparam T Integer type of the second summand
 * @param a First summand
 * @param b Second summand
 * @param initial_carry True if there is an initial carry coming in
 * @return Sum of a and b with bit width max(I::width,T::width)+1
 */
template <typename I, typename T>
[[nodiscard]] constexpr auto expanding_add(const I& a, const T& b, const bool initial_carry = false)
{

    static_assert(is_integral_v<I>);
    static_assert(is_integral_v<T>);

    // TODO do we need this assertion?
    static_assert(aarith::same_sign<I, T>);

    constexpr size_t res_width = std::max(I::width(), T::width()) + 1U;

    auto a_ = width_cast<res_width>(a);
    auto b_ = width_cast<res_width>(b);

    using word_type = typename decltype(a_)::word_type; // weird but works O_0

    decltype(a_) sum{0U};

    word_type carry = initial_carry ? 1U : 0U;

    for (auto i = 0U; i < sum.word_count(); ++i)
    {

        word_type word_a{a_.word(i)};
        word_type word_b{b_.word(i)};

        word_type partial_sum = word_a + word_b;
        word_type new_carry = (partial_sum < word_a || partial_sum < word_b) ? 1U : 0U;
        partial_sum += carry;
        carry = (new_carry || partial_sum < word_a || partial_sum < word_b) ? 1U : 0U;
        sum.set_word(i, partial_sum);
    }
    return sum;
}

/**
 * @brief Computes the difference of two integers.
 *
 * @tparam I The integer type used in the subtraction
 * @param a Minuend
 * @param b Subtrahend
 * @return Difference between a and b
 */
template <typename I>[[nodiscard]] constexpr auto sub(const I& a, const I& b) -> I
{
    static_assert(is_integral_v<I>);

    auto result = expanding_add(a, ~b, true);
    return width_cast<I::width()>(result);
}

/**
 * @brief Subtracts two integers of, possibly, different bit widths.
 *
 * Expanding does not, in contrast to @see expanding_add, ensure that no underflow will happen. It
 * simply makes sure that the resulting bit width is the larger of both input bit widths.
 *
 * @tparam W Width of the minuend
 * @tparam V Width of the subtrahend
 * @param a Minuend
 * @param b Subtrahend
 * @return Difference of correct bit width
 */
template <typename I, typename T>[[nodiscard]] constexpr auto expanding_sub(const I& a, const T& b)
{

    // TODO do we need this assertion?
    static_assert(aarith::same_sign<I, T>);
    static_assert(aarith::same_word_type<I, T>);

    constexpr size_t res_width = std::max(I::width(), T::width());
    const auto result{sub(width_cast<res_width>(a), width_cast<res_width>(b))};

    return result;
}

/**
 * @brief Adds two integers
 *
 * @tparam I The integer type used for the addition
 * @param a First summand
 * @param b Second summand
 * @return Sum of a and b
 */
template <typename I>[[nodiscard]] I constexpr add(const I& a, const I& b)
{
    constexpr size_t W = I::width();
    const auto result = expanding_add<I, I>(a, b);
    return width_cast<W>(result);
}

/**
 * @brief Multiplies two unsigned integers expanding the bit width so that the result fits.
 *
 * This implements the simplest multiplication algorithm (binary "long multiplication") that adds up
 * the partial products everywhere where the first multiplicand has a 1 bit. The simplicity, of
 * course, comes at the cost of performance.
 *
 * @tparam W The bit width of the first multiplicant
 * @tparam V The bit width of the second multiplicant
 * @param a First multiplicant
 * @param b Second multiplicant
 * @return Product of a and b
 */
template <std::size_t W, std::size_t V>
[[nodiscard]] constexpr uinteger<W + V> expanding_mul(const uinteger<W>& a, const uinteger<V>& b)
{

    constexpr std::size_t res_width = W + V;
    uinteger<res_width> result{0U};
    if constexpr (res_width <= 64)
    {
        uint64_t result_uint64 = a.word(0) * b.word(0);
        result.set_word(0, result_uint64);
    }
    else
    {
        static_assert(is_integral_v<uinteger<res_width>>);
        static_assert(is_unsigned_v<uinteger<res_width>>);

        const auto leading_zeroes = V - count_leading_zeroes(b);
        uinteger<res_width> a_ = width_cast<res_width>(a);

        auto bit_index = 0U;
        while (bit_index < leading_zeroes)
        {
            if (b.bit(bit_index))
            {
                result = add(result, a_ << bit_index);
            }
            ++bit_index;
        }
    }
    return result;
}

/**
 * @brief Multiplies two integers.
 *
 * @note No Type conversion is performed. If the bit widths do not match, the code will not
 * compile! Use @see expanding_mul for that.
 *
 * The result is then cropped to fit the initial bit width
 *
 * @tparam I The integer type to operate on
 * @param a First multiplicant
 * @param b Second multiplicant
 * @return Product of a and b
 */
template <typename I>[[nodiscard]] constexpr I mul(const I& a, const I& b)
{
    return width_cast<I::width()>(expanding_mul(a, b));
}

/**
 * @brief Implements the restoring division algorithm.
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
[[nodiscard]] std::pair<uinteger<W>, uinteger<W>>

restoring_division(const uinteger<W>& numerator, const uinteger<V>& denominator)
{
    using UInteger = uinteger<W>;
    using LargeUInteger = uinteger<2 * W>;

    if (denominator.is_zero())
    {
        throw std::runtime_error("Attempted division by zero");
    }

    // Cover some special cases in order to speed everything up
    if (numerator == denominator)
    {
        return std::make_pair(UInteger::one(), UInteger::zero());
    }
    if (numerator.is_zero())
    {
        return std::make_pair(UInteger::zero(), UInteger::zero());
    }
    if (numerator < denominator)
    {
        return std::make_pair(UInteger::zero(), numerator);
    }
    if (denominator == UInteger::one())
    {
        return std::make_pair(numerator, UInteger::zero());
    }

    // Perform restoring division in all other cases
    const auto n = numerator.width();
    const LargeUInteger D = (width_cast<2 * W>(denominator) << n);
    LargeUInteger R = width_cast<2 * W>(numerator);
    UInteger Q = UInteger::zero();

    for (size_t i = 0; i < n; ++i)
    {
        const auto bit = (n - 1) - i;
        const LargeUInteger TwoR = (R << 1);
        if (TwoR >= D)
        {
            R = sub(TwoR, D);
            Q.set_bit(bit, true);
        }
        else
        {
            R = TwoR;
            Q.set_bit(bit, false);
        }
    }

    const uinteger<W> remainder = width_cast<W>(R >> n);

    return std::make_pair(Q, remainder);
}

/**
 * @brief Computes the remainder of the division of one integer by another integer
 *
 * @note For signed integers, weird under-/overflows for ::min() may occur
 *
 * @tparam I Integer type to work on
 * @param numerator The number that is to be divided
 * @param denominator The number that divides the other number
 * @return The remainder of the division operation
 */
template <typename I>[[nodiscard]] auto remainder(const I& numerator, const I& denominator) -> I
{
    return restoring_division(numerator, denominator).second;
}

/**
 * @brief Divides one integer by another integer
 *
 * @note integer<W>::min/integer<W>(-1) will return <integer<W>::min,0>, i.e. some weird
 * overflow happens for signed integers
 *
 * @tparam I Integer type to work on
 * @param numerator The number that is to be divided
 * @param denominator The number that divides the other number
 * @return The quotient of the division operation
 */
template <typename I>[[nodiscard]] auto div(const I& numerator, const I& denominator) -> I
{
    return restoring_division(numerator, denominator).first;
}

/**
 * @brief Adds two signed integers of, possibly, different bit widths.
 *
 * This is an implementation using a more functional style of programming. It is not particularly
 * fast and only here for educational purposes. You can use method as a means to understand how to
 * work on an integer.
 *
 * @tparam IntA The integer type of the first summand
 * @tparam IntB The integer type of the second summand
 * @param a First summand
 * @param b Second summand
 * @param initial_carry True if there is an initial carry coming in
 * @return Sum of correct maximal bit width
 */
template <class IntA, class IntB>
[[nodiscard]] auto fun_add_expand(const IntA& a, const IntB& b, const bool initial_carry = false)
{
    static_assert(is_integral_v<IntA>);
    static_assert(is_integral_v<IntB>);

    // TODO do we need this assertion?
    static_assert(same_sign<IntA, IntB>);
    static_assert(same_word_type<IntA, IntB>);

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
 * @brief Adds two integers of, possibly, different bit widths.
 *
 * @see fun_add_expand
 *
 * @tparam I Integer type used in the addition
 * @param a First summand
 * @param b Second summand
 * @param initial_carry True if there is an initial carry coming in
 * @return Sum of a and b
 */
template <typename I>
[[nodiscard]] auto fun_add(const I& a, const I& b, const bool initial_carry = false) -> I
{
    return width_cast<I::width()>(fun_add_expand(a, b, initial_carry));
}

/**
 * @brief Arithmetic right-shift operator
 *
 * This shift preserves the signedness of the integer.
 *
 * @tparam Width The width of the signed integer
 * @param lhs The integer to be shifted
 * @param rhs The number of bits to be shifted
 * @return The shifted integer
 */
template <size_t Width>
auto constexpr operator>>(const integer<Width>& lhs, const size_t rhs) -> integer<Width>
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
[[nodiscard]] constexpr auto expanding_mul(const integer<W>& m, const integer<V>& r) -> integer<V + W>
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
[[nodiscard]] std::pair<integer<W>, integer<W>>

restoring_division(const integer<W>& numerator, const integer<V>& denominator)
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
 * Convenience namespace to include when code should be written the "normal" way. There is one
 * caveat though: No automatic type conversion will take place!
 */
namespace aarith::arithmetic_operators {

template <typename I> auto operator+(const I& lhs, const I& rhs) -> I
{
    return add(lhs, rhs);
}

template <typename I> auto operator-(const I& lhs, const I& rhs) -> I
{
    return sub(lhs, rhs);
}

template <typename I> auto operator*(const I& lhs, const I& rhs) -> I
{
    return mul(lhs, rhs);
}

template <typename I> auto operator/(const I& lhs, const I& rhs) -> I
{
    return div(lhs, rhs);
}

template <typename I> auto operator%(const I& lhs, const I& rhs) -> I
{
    return remainder(lhs, rhs);
}

} // namespace aarith::arithmetic_operators

} // namespace aarith
