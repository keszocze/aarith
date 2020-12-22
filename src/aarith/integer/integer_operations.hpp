#pragma once
#include <aarith/core/traits.hpp>
#include <aarith/integer/integers.hpp>
#include <type_traits>

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
[[nodiscard]] constexpr auto expanding_add(const I& a, const T& b, const bool initial_carry)
{

    static_assert(::aarith::is_integral_v<I>);
    static_assert(::aarith::is_integral_v<T>);

    // TODO do we need this assertion?
    static_assert(::aarith::same_signedness<I, T>);

    constexpr size_t res_width = std::max(I::width(), T::width()) + 1U;

    auto a_ = width_cast<res_width>(a);
    auto b_ = width_cast<res_width>(b);

    using word_type = typename decltype(a_)::word_type; // weird but works O_0

    decltype(a_) sum{0U};

    word_type carry = initial_carry ? word_type{1U} : word_type{0U};

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

template <typename I, typename T> [[nodiscard]] constexpr auto expanding_add(const I& a, const T& b)
{

    return expanding_add(a, b, false);
}

/**
 * @brief Computes the difference of two integers.
 *
 * @tparam I The integer type used in the subtraction
 * @param a Minuend
 * @param b Subtrahend
 * @return Difference between a and b
 */
template <typename I> [[nodiscard]] constexpr auto sub(const I& a, const I& b) -> I
{
    static_assert(::aarith::is_integral_v<I>);

    constexpr size_t W = I::width();

    if constexpr (W <= I::word_width())
    {
        const auto result = I{static_cast<typename I::word_type>(a.word(0) - b.word(0))};
        return result;
    }
    else
    {
        auto result = expanding_add(a, ~b, true);
        return width_cast<W>(result);
    }
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
// template <typename I, typename T>[[nodiscard]] constexpr auto expanding_sub(const I& a, const T&
// b, const bool initial_borrow = false)
template <typename I, typename T> [[nodiscard]] constexpr auto expanding_sub(const I& a, const T& b)
{

    static_assert(::aarith::same_signedness<I, T>);
    static_assert(::aarith::same_word_type<I, T>);

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
template <typename I> [[nodiscard]] I constexpr add(const I& a, const I& b)
{
    constexpr size_t W = I::width();

    // if the number completely fits into the word, we can simply use the default implementation
    // of the addition on the basis WordType
    if constexpr (W <= I::word_width())
    {
        const auto result = I{static_cast<typename I::word_type>(a.word(0) + b.word(0))};
        return result;
    }
    else
    {
        const auto result = expanding_add<I, I>(a, b);
        return width_cast<W>(result);
    }
}

/**
 * @brief Multiplies two unsigned integers expanding the bit width so that the result fits.
 *
 * This implements the simplest multiplication algorithm (binary "long multiplication") that adds up
 * the partial products everywhere where the first multiplicand has a 1 bit. The simplicity, of
 * course, comes at the cost of performance.
 *
 * @tparam W The bit width of the first multiplicand
 * @tparam V The bit width of the second multiplicand
 * @param a First multiplicand
 * @param b Second multiplicand
 * @return Product of a and b
 */
template <std::size_t W, std::size_t V, typename WordType>
[[nodiscard]] constexpr uinteger<W + V, WordType>
schoolbook_expanding_mul(const uinteger<W, WordType>& a, const uinteger<V, WordType>& b)
{

    constexpr std::size_t res_width = W + V;
    uinteger<res_width, WordType> result{0U};

    if constexpr (res_width <= uinteger<W, WordType>::word_width())
    {
        auto result_uint = a.word(0) * b.word(0);
        result.set_word(0, result_uint);
    }
    else
    {
        static_assert(::aarith::is_integral_v<uinteger<res_width, WordType>>);
        static_assert(::aarith::is_unsigned_v<uinteger<res_width, WordType>>);

        const auto leading_zeroes = V - count_leading_zeroes(b);
        uinteger<res_width, WordType> a_ = width_cast<res_width>(a);

        for (auto bit_index = 0U; bit_index < leading_zeroes; ++bit_index)
        {
            if (b.bit(bit_index))
            {
                result = add(result, a_ << bit_index);
            }
        }
    }
    return result;
}

/**
 * @brief Multiplies two integers.
 *
 * @note No Type conversion is performed. If the bit widths do not match, the code will not
 * compile! Use @see booth_expanding_mul for that.
 *
 * The result is then cropped to fit the initial bit width
 *
 * @tparam I The integer type to operate on
 * @param a First multiplicand
 * @param b Second multiplicand
 * @return Product of a and b
 */
template <typename I, typename = std::enable_if_t<is_integral_v<I> && is_unsigned_v<I>>>
[[nodiscard]] constexpr I schoolbook_mul(const I& a, const I& b)
{

    constexpr size_t W = I::width();

    // if the number completely fits into the word, we can simply use the default implementation
    // of the multiplication on the basis WordType
    if constexpr (W <= I::word_width())
    {
        const I result{static_cast<typename I::word_type>(a.word(0) * b.word(0))};
        return result;
    }
    else
    {
        return width_cast<W>(schoolbook_expanding_mul(a, b));
    }
}

/**
 * @brief Multiplies two unsigned integers using the Karazuba algorithm expanding the bit width so
 * that the result fits.
 *
 * This implements the karazuba multiplication algorithm (divide and conquer).
 *
 * @tparam W The bit width of the first multiplicant
 * @tparam V The bit width of the second multiplicant
 * @param a First multiplicant
 * @param b Second multiplicant
 * @return Product of a and b
 */
template <std::size_t W, std::size_t V, typename WordType>
[[nodiscard]] constexpr uinteger<W + V, WordType> expanding_karazuba(const uinteger<W, WordType>& a,
                                                                     const uinteger<V, WordType>& b)
{

    constexpr std::size_t res_width = W + V;
    if constexpr (res_width <= uinteger<W, WordType>::word_width())
    {
        auto result_uint = a.word(0) * b.word(0);
        const uinteger<res_width, WordType> result(result_uint);
        return result;
    }
    else if constexpr (W == V)
    {
        if (a.is_zero() || b.is_zero())
        {
            return uinteger<res_width, WordType>::zero();
        }

        // floor to the next value with power of 2
        // std::log2 and std::floor  not constexpr and did not compile with clang
        constexpr size_t floor_pow_two = floor_to_pow(W);

        constexpr size_t karazuba_width =
            (floor_pow_two == W) ? (floor_pow_two >> 1) : (floor_pow_two);

        const auto a_split = split<karazuba_width - 1>(a);
        const auto b_split = split<karazuba_width - 1>(b);

        const auto ah = uinteger<W - karazuba_width, WordType>(a_split.first);
        const auto al = uinteger<karazuba_width, WordType>(a_split.second);
        const auto bh = uinteger<W - karazuba_width, WordType>(b_split.first);
        const auto bl = uinteger<karazuba_width, WordType>(b_split.second);

        const auto p1 = expanding_karazuba<W - karazuba_width, W - karazuba_width>(ah, bh);
        const auto p2 = expanding_karazuba<karazuba_width, karazuba_width>(al, bl);
        const auto s1 = expanding_add(ah, al);
        const auto s2 = expanding_add(bh, bl);

        // prevent infinite call loop
        // TODO (keszocze) find a better way to do this
        uinteger<2 * (karazuba_width + 1), WordType> p3;
        if (s1.bit(s1.width() - 1) == 1 || s2.bit(s2.width() - 1) == 1)
        {
            p3 = expanding_karazuba(s1, s2);
        }
        else
        {
            const auto ps1 = width_cast<karazuba_width>(s1);
            const auto ps2 = width_cast<karazuba_width>(s2);
            const auto p3t = expanding_karazuba(ps1, ps2);
            p3 = p3t;
        }

        constexpr auto full_shift = 2 * karazuba_width;
        const auto k1 = width_cast<res_width>(p1) << full_shift;
        const auto k2 = width_cast<res_width>(expanding_sub(p3, expanding_add(p1, p2)))
                        << karazuba_width;
        const auto product = expanding_add(k1, expanding_add(k2, p2));

        return width_cast<res_width>(product);
    }
    else
    {
        constexpr std::size_t max_width = std::max(W, V);

        const auto a_ = width_cast<max_width>(a);
        const auto b_ = width_cast<max_width>(b);

        const auto res = expanding_karazuba(a_, b_);

        return width_cast<res_width>(res);
    }
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
template <std::size_t W, std::size_t V, typename WordType>
[[nodiscard]] constexpr std::pair<uinteger<W, WordType>, uinteger<W, WordType>>
restoring_division(const uinteger<W, WordType>& numerator, const uinteger<V, WordType>& denominator)
{
    using UInteger = uinteger<W, WordType>;
    using LargeUInteger = uinteger<2 * W, WordType>;

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

    const uinteger<W, WordType> remainder_ = width_cast<W>(R >> n);

    return std::make_pair(Q, remainder_);
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
template <typename I>
[[nodiscard]] constexpr auto remainder(const I& numerator, const I& denominator) -> I
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
template <typename I>
[[nodiscard]] constexpr auto div(const I& numerator, const I& denominator) -> I
{
    return restoring_division(numerator, denominator).first;
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
template <size_t Width, typename WordType>
[[nodiscard]] constexpr auto abs(const integer<Width, WordType>& n) -> integer<Width, WordType>
{
    return n.is_negative() ? negate(n) : n;
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
template <size_t Width, typename WordType>
[[nodiscard]] constexpr auto expanding_abs(const integer<Width, WordType>& n)
    -> uinteger<Width, WordType>
{
    uinteger<Width, WordType> abs = n.is_negative() ? negate(n) : n;
    return abs;
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
[[nodiscard]] constexpr auto fun_add_expand(const IntA& a, const IntB& b,
                                            const bool initial_carry = false)
{
    static_assert(::aarith::is_integral_v<IntA>);
    static_assert(::aarith::is_integral_v<IntB>);

    static_assert(::aarith::same_signedness<IntA, IntB>);
    static_assert(::aarith::same_word_type<IntA, IntB>);

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
[[nodiscard]] constexpr auto fun_add(const I& a, const I& b, const bool initial_carry = false) -> I
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
template <size_t Width, typename WordType>
auto constexpr operator>>=(integer<Width, WordType>& lhs, const size_t rhs)
    -> integer<Width, WordType>
{
    arithmetic_right_shift(lhs, rhs);
    return lhs;
}

template <size_t Width, typename WordType, typename U,
          typename = std::enable_if_t<is_unsigned_v<U>>>
auto constexpr operator>>=(integer<Width, WordType>& lhs, const U& rhs) -> integer<Width, WordType>&
{
    return arithmetic_right_shift(lhs, static_cast<size_t>(rhs));
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
template <size_t Width, typename WordType>
auto constexpr operator>>(const integer<Width, WordType>& lhs, const size_t rhs)
    -> integer<Width, WordType>
{
    integer<Width, WordType> shifted{lhs};
    shifted >>= rhs;
    return shifted;
}

template <size_t Width, typename WordType, typename U,
          typename = std::enable_if_t<is_unsigned_v<U>>>
auto constexpr operator>>(const integer<Width, WordType>& lhs, const U& rhs)
    -> integer<Width, WordType>
{
    integer<Width, WordType> shifted{lhs};
    shifted >>= static_cast<size_t>(rhs);
    return shifted;
}

template <typename I, typename = std::enable_if_t<is_integral_v<I>>> I& operator--(I& a)
{
    I val = sub(a, I::one());
    a.set_bits(val);
    return a;
}

template <typename I, typename = std::enable_if_t<is_integral_v<I>>> I operator--(I& a, int)
{
    I cpy{a};
    --a;
    return cpy;
}

template <typename I, typename = std::enable_if_t<is_integral_v<I>>> I& operator++(I& a)
{
    I val = add(a, I::one());
    a.set_bits(val);
    return a;
}

template <typename I, typename = std::enable_if_t<is_integral_v<I>>> I operator++(I& a, int)
{
    I cpy{a};
    ++a;
    return cpy;
}

/**
 * @brief Naively multiplies two signed integers.
 *
 * @tparam W The bit width of the first multiplicand
 * @tparam V The bit width of the second multiplicand
 * @param a First multiplicand
 * @param b Second multiplicand
 * @return Product of a and b
 */
template <size_t W, size_t V, typename WordType>
[[nodiscard]] constexpr auto naive_expanding_mul(const integer<W, WordType>& m,
                                                 const integer<V, WordType>& r)
{
    const bool m_neg = m.is_negative();
    const bool r_neg = r.is_negative();

    const uinteger<W, WordType> m_ = m_neg ? expanding_abs(m) : uinteger<W, WordType>{m};
    const uinteger<V, WordType> r_ = r_neg ? expanding_abs(r) : uinteger<V, WordType>{r};

    const integer<W + V, WordType> result = schoolbook_expanding_mul(m_, r_);

    return m_neg ^ r_neg ? -result : result;
}

/**
 * @brief Naively multiplies two integers.
 *
 * @note No Type conversion is performed. If the bit widths do not match, the code will not
 * compile! Use @see booth_expanding_mul for that.
 *
 * The result is then cropped to fit the initial bit width
 *
 * @tparam I The integer type to operate on
 * @param a First multiplicand
 * @param b Second multiplicand
 * @return Product of a and b
 */
template <size_t W, typename WordType>
[[nodiscard]] constexpr integer<W, WordType> naive_mul(const integer<W, WordType>& a,
                                                       const integer<W, WordType>& b)
{

    using I = integer<W, WordType>;

    // if the number completely fits into the word, we can simply use the default implementation
    // of the multiplication on the basis WordType
    if constexpr (W <= I::word_width())
    {
        const I result{static_cast<typename I::word_type>(a.word(0) * b.word(0))};
        return result;
    }
    else
    {
        return width_cast<W>(naive_expanding_mul(a, b));
    }
}

/**
 * @brief Multiplies two signed integers.
 *
 *
 * This implements the Booth multiplication algorithm with extension to correctly handle the
 * most negative number. See https://en.wikipedia.org/wiki/Booth%27s_multiplication_algorithm
 * for details.
 *
 * @tparam x The bit width of the first multiplicant
 * @tparam y The bit width of the second multiplicant
 * @param m Multiplicand
 * @param r Multiplier
 * @return Product of m and r
 */
template <size_t x, size_t y, typename WordType>
[[nodiscard]] constexpr auto booth_expanding_mul(const integer<x, WordType>& m,
                                                 const integer<y, WordType>& r)
    -> integer<y + x, WordType>
{

    if (m.is_zero() || r.is_zero())
    {
        return integer<y + x, WordType>::zero();
    }

    constexpr size_t K = x + y + 2;

    integer<x + 1, WordType> expanded_m = width_cast<x + 1>(m);

    //    std::cout << "expanded m: " << to_binary(expanded_m) << "\n";

    uinteger<K, WordType> A{static_cast<word_array<x + 1, WordType>>(expanded_m)};
    uinteger<K, WordType> S{static_cast<word_array<x + 1, WordType>>(-expanded_m)};

    A = A << y + 1;
    S = S << y + 1;

    uinteger<K, WordType> P{static_cast<word_array<x, WordType>>(r)};
    P = P << 1;

    //    std::cout << "A: " << to_binary(A) << "\n";
    //    std::cout << "S: " << to_binary(S) << "\n";
    //    std::cout << "P: " << to_binary(P) << "\n\n";
    for (size_t i = 0; i < y; ++i)
    {

        bool last_bit = P.bit(0);
        bool snd_last_bit = P.bit(1);

        //        std::cout << "P_pre_" << i << ": " << to_binary(P) << "\n";

        if (snd_last_bit && !last_bit)
        {
            P = add(P, S);
        }
        if (!snd_last_bit && last_bit)
        {
            P = add(P, A);
        }

        const bool prefix_minus = P.msb();
        P = P >> 1;
        if (prefix_minus)
        {
            P.set_msb(true);
        }

        //        std::cout << "P_pos_" << i << ": " << to_binary(P) << "\n";
    }

    auto result = width_cast<x + y>(P >> 1);

    return integer<x + y, WordType>{result};
}

/**
 * @brief Multiplies two integers.
 *
 * @note No Type conversion is performed. If the bit widths do not match, the code will not
 * compile! Use @see booth_expanding_mul for that.
 *
 * The result is then cropped to fit the initial bit width
 *
 * @tparam I The integer type to operate on
 * @param a First multiplicand
 * @param b Second multiplicand
 * @return Product of a and b
 */
template <size_t W, typename WordType>
[[nodiscard]] constexpr integer<W, WordType> booth_mul(const integer<W, WordType>& a,
                                                       const integer<W, WordType>& b)
{
    using I = integer<W, WordType>;

    // if the number completely fits into the word, we can simply use the default implementation
    // of the multiplication on the basis WordType
    if constexpr (W <= I::word_width())
    {
        const I result{static_cast<typename I::word_type>(a.word(0) * b.word(0))};
        return result;
    }
    else
    {
        return width_cast<W>(booth_expanding_mul(a, b));
    }
}

/**
 * @brief Multiplies two signed integers.
 *
 *
 * This implements the Booth multiplication algorithm with extension to correctly handle the
 * most negative number. See https://en.wikipedia.org/wiki/Booth%27s_multiplication_algorithm
 * for details.
 *
 * @tparam x The bit width of the first multiplicant
 * @tparam y The bit width of the second multiplicant
 * @param m Multiplicand
 * @param r Multiplier
 * @return Product of m and r
 */
template <size_t x, size_t y, typename WordType>
[[nodiscard]] constexpr auto booth_inplace_expanding_mul(const integer<x, WordType>& m,
                                                         const integer<y, WordType>& r)
    -> integer<y + x, WordType>
{

    if (m.is_zero() || r.is_zero())
    {
        return integer<y + x, WordType>::zero();
    }

    constexpr size_t K = x + y + 2;

    integer<x + 1, WordType> expanded_m = width_cast<x + 1>(m);

    uinteger<K, WordType> A{static_cast<word_array<x + 1, WordType>>(expanded_m)};
    uinteger<K, WordType> S{static_cast<word_array<x + 1, WordType>>(-expanded_m)};

    A <<= y + 1;
    S <<= y + 1;

    uinteger<K, WordType> P{static_cast<word_array<x, WordType>>(r)};
    P <<= 1;

    for (size_t i = 0; i < y; ++i)
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

        const bool prefix_minus = P.msb();
        P >>= 1;
        if (prefix_minus)
        {
            P.set_msb(true);
        }
    }

    P >>= 1;
    auto result = width_cast<x + y>(P);

    return integer<x + y, WordType>{result};
}

/**
 * @brief Multiplies two integers.
 *
 * @note No Type conversion is performed. If the bit widths do not match, the code will not
 * compile! Use @see booth_expanding_mul for that.
 *
 * The result is then cropped to fit the initial bit width
 *
 * @tparam I The integer type to operate on
 * @param a First multiplicand
 * @param b Second multiplicand
 * @return Product of a and b
 */
template <size_t W, typename WordType>
[[nodiscard]] constexpr integer<W, WordType> booth_inplace_mul(const integer<W, WordType>& a,
                                                               const integer<W, WordType>& b)
{

    using I = integer<W, WordType>;

    // if the number completely fits into the word, we can simply use the default implementation
    // of the multiplication on the basis WordType
    if constexpr (W <= I::word_width())
    {
        const I result{static_cast<typename I::word_type>(a.word(0) * b.word(0))};
        return result;
    }
    else
    {
        return width_cast<W>(booth_inplace_expanding_mul(a, b));
    }
}

/**
 * @brief Negates the value
 * @tparam W The width of the signed integer
 * @param n  The signed integer whose sign is to be changed
 * @return  The negative value of the signed integer
 */
template <size_t W, typename WordType>
constexpr auto negate(const integer<W, WordType>& n) -> integer<W, WordType>
{
    const integer<W, WordType> one(1U);
    return add(~n, one);
}

/**
 * @brief Computes the sign of the integer.
 *
 * For the number zero, the function returns a signum of 0,  -1 for negative numbers and +1 for
 * positive numbers.
 *
 * @tparam W The width of the integer
 * @tparam WordType The word type to store the data in
 * @param n The integer
 * @return The sign of the integer
 */
template <size_t W, typename WordType> [[nodiscard]] constexpr int8_t signum(integer<W, WordType> n)
{
    if (n.is_zero())
    {
        return 0;
    }
    if (n.is_negative())
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

/**
 * @brief Computes the sign of the unsigned integer.
 *
 * For the number zero, the function returns a signum of 0 and a 1 for every other number.
 *
 * @tparam W The width of the unsigned integer
 * @param n The integer
 * @return The sign of the integer
 */
template <size_t W, typename WordType>
[[nodiscard]] constexpr int8_t signum(uinteger<W, WordType> n)
{
    return n.is_zero() ? 0 : 1;
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
template <std::size_t W, std::size_t V, typename WordType>
[[nodiscard]] constexpr std::pair<integer<W, WordType>, integer<W, WordType>>
restoring_division(const integer<W, WordType>& numerator, const integer<V, WordType>& denominator)
{

    using Integer = integer<W, WordType>;
    using UInteger = uinteger<W, WordType>;
    using IntOneBitMore = integer<W + 1, WordType>;
    //    using LargeSInteger = integer<2 * W>;

    // Cover some special cases in order to speed everything up
    if (denominator.is_zero())
    {
        throw std::runtime_error("Attempted division by zero");
    }
    if (numerator.is_zero())
    {
        return std::make_pair(Integer::zero(), Integer::zero());
    }
    if (denominator == Integer::one())
    {
        return std::make_pair(numerator, Integer::zero());
    }

    if (numerator == denominator)
    {
        return std::make_pair(Integer::one(), Integer::zero());
    }

    const bool to_negate = numerator.is_negative() ^ denominator.is_negative();

    const UInteger N = expanding_abs(numerator);
    const UInteger D = expanding_abs(denominator);

    if (N < D)
    {
        return std::make_pair(Integer::zero(), numerator);
    }

    const auto div_ = restoring_division(N, D);

    IntOneBitMore Q(div_.first);
    IntOneBitMore remainder_(div_.second);

    if (to_negate)
    {
        Q = negate(Q);
    }

    Integer Q_cast = width_cast<W>(Q);
    Integer remainder_cast = width_cast<W>(remainder_);

    if (numerator.is_negative())
    {
        remainder_cast = -remainder_cast;
    }

    return std::make_pair(Q_cast, remainder_cast);
}

template <typename I, typename = std::enable_if_t<is_integral_v<I>>>
constexpr I mul(const I& a, const I& b)
{
    if constexpr (is_unsigned_v<I>)
    {
        return schoolbook_mul(a, b);
    }
    else
    {
        return naive_mul(a, b);
    }
}

template <typename I, typename = std::enable_if_t<is_integral_v<I>>>
auto constexpr expanding_mul(const I& a, const I& b)
{
    if constexpr (is_unsigned_v<I>)
    {
        return schoolbook_expanding_mul(a, b);
    }
    else
    {
        return naive_expanding_mul(a, b);
    }
}

/**
 * @brief Exponentiation function
 *
 * @note This function does not make any attempts to be fast or to prevent overflows!
 *
 * @note If exponent equals std::numeric_limits<size_t>::max(), this method throws an exception,
 * unless base equals zero
 * @tparam W Bit width of the integer
 * @param base
 * @param exponent
 * @return The base to the power of the exponent
 */
template <typename IntegerType> IntegerType pow(const IntegerType& base, const size_t exponent)
{

    if (exponent == std::numeric_limits<size_t>::max())
    {
        if (base.is_zero())
        {
            return IntegerType::zero();
        }
        else
        {
            throw std::runtime_error(
                "Attempted exponentiation by std::numeric_limits<size_t>::max()");
        }
    }

    IntegerType result = IntegerType::one();

    for (size_t i = 0U; i <= exponent; ++i)
    {
        result = mul(result, base);
    }
    return result;
}

/**
 *
 * @brief Exponentiation function
 *
 * @note This function does not make any attempts to be fast or to prevent overflows!
 *
 * @note If exponent equals std::numeric_limits<IntegerType>::max(), this method throws an
 * exception, unless base equals zero
 *
 * @tparam IntegerType The type of integer used in the computation
 * @param base
 * @param exponent
 * @return The base to the power of the exponent
 */
template <typename IntegerType>
IntegerType pow(const IntegerType& base, const IntegerType& exponent)
{

    static_assert(aarith::is_integral_v<IntegerType>,
                  "Exponentiation is only supported for aarith integers");

    if (exponent == IntegerType::max())
    {
        if (base == IntegerType::zero())
        {
            return IntegerType::zero();
        }
        else
        {
            throw std::runtime_error(
                "Attempted exponentiation by std::numeric_limits<IntegerType<W>>::max()");
        }
    }

    IntegerType result = IntegerType::one();
    IntegerType iter = IntegerType::zero();
    while (iter < exponent)
    {
        result = mul(result, base);
        iter = add(iter, IntegerType::one());
    }

    return result;
}

/**
 * @brief Multiplies two unsigned integers using the Karazuba algorithm
 *
 * This implements the karazuba multiplication algorithm (divide and conquer).
 *
 * @tparam W The bit width of the multiplicants
 * @param a First multiplicant
 * @param b Second multiplicant
 * @return Product of a and b
 */
template <std::size_t W, typename WordType>
[[nodiscard]] constexpr uinteger<W, WordType> karazuba(const uinteger<W, WordType>& a,
                                                       const uinteger<W, WordType>& b)
{
    return width_cast<W>(expanding_karazuba(a, b));
}

/**
 * @brief Computes the distance (i.e. the absolute difference) between two integers
 * @tparam Integer The integer type to operate on
 * @param a First integer
 * @param b Second integer
 * @return The distance between the two integers
 */
template <typename Integer>
[[nodiscard]] constexpr Integer distance(const Integer& a, const Integer& b)
{
    return (a <= b) ? sub(b, a) : sub(a, b);
}

/**
 * @brief Left-shift assignment operator
 * @tparam W The word_container type to work on
 * @param lhs The word_container to be shifted
 * @param rhs The number of bits to shift
 * @return The shifted word_container
 */
template <typename W, typename I,
          typename = std::enable_if_t<is_word_array_v<W> && is_integral_v<I> && is_unsigned_v<I>>>
constexpr auto operator<<=(W& lhs, const I rhs) -> W
{
    const size_t shift = static_cast<size_t>(rhs);
    lhs <<= shift;
    return lhs;
}

/**
 * @brief Left-shift assignment operator
 * @tparam W The word_container type to work on
 * @param lhs The word_container to be shifted
 * @param rhs The number of bits to shift
 * @return The shifted word_container
 */
template <typename W, typename I,
          typename = std::enable_if_t<is_word_array_v<W> && is_integral_v<I> && is_unsigned_v<I>>>
constexpr auto operator<<(const W& lhs, const I rhs) -> W
{
    W cpy{lhs};
    return (cpy <<= rhs);
}

/**
 * @brief Left-shift assignment operator
 * @tparam W The word_container type to work on
 * @param lhs The word_container to be shifted
 * @param rhs The number of bits to shift
 * @return The shifted word_container
 */
template <typename W, typename I,
          typename = std::enable_if_t<is_word_array_v<W> && is_integral_v<I> && is_unsigned_v<I>>>
constexpr auto operator>>=(W& lhs, const I rhs) -> W
{
    const size_t shift = static_cast<size_t>(rhs);
    lhs >>= shift;
    return lhs;
}

/**
 * @brief Left-shift assignment operator
 * @tparam W The word_container type to work on
 * @param lhs The word_container to be shifted
 * @param rhs The number of bits to shift
 * @return The shifted word_container
 */
template <typename W, typename I,
          typename = std::enable_if_t<is_word_array_v<W> && is_integral_v<I> && is_unsigned_v<I>>>
constexpr auto operator>>(const W& lhs, const I rhs) -> W
{
    W cpy{lhs};
    return (cpy >>= rhs);
}

/**
 * Convenience namespace to include when code should be written the "normal" way. There is one
 * caveat though: No automatic type conversion will take place!
 */
namespace integer_operators {

// template <typename Val, typename = std::enable_if_t<::aarith::is_unsigned_int<Val> &&
//                                                    (sizeof(Val) * 8) <= Width>>

template <typename I, typename = std::enable_if_t<is_integral<I>::value>>
auto constexpr operator-(const I& num) -> I
{
    return negate(num);
}

template <typename I, typename = std::enable_if_t<is_integral<I>::value>>
auto constexpr operator+(const I& lhs, const I& rhs) -> I
{
    return add(lhs, rhs);
}

template <typename I, typename = std::enable_if_t<is_integral<I>::value>>
auto constexpr operator-(const I& lhs, const I& rhs) -> I
{
    return sub(lhs, rhs);
}

template <typename I, typename = std::enable_if_t<is_integral_v<I>>>
auto constexpr operator*(const I& lhs, const I& rhs) -> I
{
    return mul(lhs, rhs);
}

template <typename I, typename = std::enable_if_t<is_integral<I>::value>>
auto constexpr operator/(const I& lhs, const I& rhs) -> I
{
    return div(lhs, rhs);
}

template <typename I, typename = std::enable_if_t<is_integral<I>::value>>
auto constexpr operator%(const I& lhs, const I& rhs) -> I
{
    return remainder(lhs, rhs);
}

} // namespace integer_operators

} // namespace aarith
