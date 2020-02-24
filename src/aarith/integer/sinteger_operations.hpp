#pragma once

#include <aarith/integer/sinteger.hpp>
#include <aarith/integer/uinteger.hpp>
#include <aarith/integer/uinteger_operations.hpp>

namespace aarith {


    template <size_t Width>
    [[nodiscard]] auto operator&(const sinteger<Width>& lhs, const sinteger<Width>& rhs)
    -> sinteger<Width>
    {
        word_array<Width> lhs_w{lhs};
        word_array<Width> rhs_w{rhs};

        word_array<Width> result = lhs_w & rhs_w;

        return sinteger<Width>{result};
    }

    template <size_t Width>
    [[nodiscard]] auto operator|(const sinteger<Width>& lhs, const sinteger<Width>& rhs)
    -> sinteger<Width>
    {
        word_array<Width> lhs_w{lhs};
        word_array<Width> rhs_w{rhs};

        word_array<Width> result = lhs_w | rhs_w;

        return sinteger<Width>{result};
    }

    template <size_t Width>[[nodiscard]] auto operator~(const sinteger<Width>& rhs) -> sinteger<Width>
    {
        word_array<Width> rhs_w{rhs};
        word_array<Width> result = ~rhs_w;
        return sinteger<Width>{result};
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
 * @brief Subtracts two signed integers of, possibly, different bit widths.
 *
 * @tparam W Width of the minuend
 * @tparam V Width of the subtrahend
 * @param a Minuend
 * @param b Subtrahend
 * @return Difference of correct bit width
 */
template <size_t W, size_t V>
[[nodiscard]] auto expanding_sub(const sinteger<W>& a, const sinteger<V>& b)
    -> sinteger<std::max(W, V)>
{
    constexpr size_t res_width = std::max(W, V);
    uinteger<res_width> result{sub(width_cast<res_width>(a), width_cast<res_width>(b))};

    return result;
}

/**
 * @brief Multiplies two signed integers.
 *
 *
 * This implements the Booth multiplication algorithm with extension to correctly handle the most
 * negative number. See https://en.wikipedia.org/wiki/Booth%27s_multiplication_algorithm for
 * details.
 *
 * @tparam W The bit width of the first multiplicant
 * @tparam V The bit width of the second multiplicant
 * @param a First multiplicant
 * @param b Second multiplicant
 * @return Product of a and b
 */
template <size_t W, size_t V>
[[nodiscard]] auto expanding_mul(const sinteger<W>& m, const sinteger<V>& r) -> sinteger<V + W>
{

    //    std::cout << "m\t" << to_binary(m) << "\n";
    //    std::cout << "r\t" << to_binary(r) << "\n";

    constexpr size_t K = W + V + 2;

    sinteger<K> A{width_cast<W + 1>(m)};
    sinteger<K> S = -A;

    A = A << V + 1;
    S = S << V + 1;

    sinteger<K> P{r};
    P = P << 1;

    //    std::cout << "A\t" << to_binary(A) << "\n";
    //    std::cout << "S\t" << to_binary(S) << "\n";
    //    std::cout << "P\t" << to_binary(P) << "\n\n";

    for (size_t i = 0; i < V; ++i)
    {

        bool last_bit = P.bit(0);
        bool snd_last_bit = P.bit(1);

        //        std::cout << "P" << i << "\t" << to_binary(P) << "\n";

        if (snd_last_bit && !last_bit)
        {
            //            std::cout << "P = P + S\n";
            P = add(P, S);
            //            std::cout << "P" << i << "\t" << to_binary(P) << "\n";
        }
        if (!snd_last_bit && last_bit)
        {
            //            std::cout << "P = P + A\n";
            P = add(P, A);
            //            std::cout << "P" << i << "\t" << to_binary(P) << "\n";
        }

        P = P >> 1;

        //        std::cout << "P" << i << "\t" << to_binary(P) << "\n\n";
    }

    return width_cast<W + V>(P >> 1);
}

/**
 * @brief Multiplies two signed integers.
 *
 * @note No Type conversion is performed. If the bit widths do not match, the code will not compile!
 *
 * This implements the Booth multiplication algorithm with extension to correctly handle the most
 * negative number. See https://en.wikipedia.org/wiki/Booth%27s_multiplication_algorithm for
 * details. The result is then cropped to fit the initial bit width
 *
 * @tparam W The bit width of the multiplicants
 * @param a First multiplicant
 * @param b Second multiplicant
 * @return Product of a and b
 */
template <size_t W>[[nodiscard]] sinteger<W> mul(const sinteger<W>& a, const sinteger<W>& b)
{
    return width_cast<W>(expanding_mul(a, b));
}

/**
 * @brief Computes the absolute value of a given signed integer.
 *
 * @warn There is a potential loss of precision as abs(sinteger::min) > sinteger::max
 *
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
 * @brief Implements the restoring division algorithm.
 *
 * @note sinteger<W>::min/sinteger<W>(-1) will return <sinteger<W>::min,0>, i.e. some weird overflow happens
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
[[nodiscard]] std::pair<sinteger<W>, sinteger<W>> restoring_division(const sinteger<W>& numerator,
                                                                     const sinteger<V>& denominator)

{



    using SInteger = sinteger<W>;
//    using LargeSInteger = sinteger<2 * W>;

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

    sinteger<W+1> Q(div.first);
    sinteger<W+1> remainder(div.second);



    if (negate)
    {
        Q=-Q;
    }

    sinteger<W> Q_cast = width_cast<W>(Q);
    sinteger<W> remainder_cast = width_cast<W>(remainder);

    return std::make_pair(Q_cast, remainder_cast);
}

/**
 * @brief Computes the remainder of the division of one sinteger by another sinteger
 * *
 * @tparam W Width of the numbers being used in the division
 * @param numerator The number that is to be divided
 * @param denominator The number that divides the other number
 * @return The remainder of the division operation
 */
template <size_t W>
[[nodiscard]] auto remainder(const sinteger<W>& numerator, const sinteger<W>& denominator) -> sinteger<W>
{
    return restoring_division(numerator, denominator).second;
}


/**
 * @brief Divides one sinteger by another sinteger
 *
 * @note sinteger<W>::min/sinteger<W>(-1) will return <sinteger<W>::min,0>, i.e. some weird overflow happens
 *
 * @tparam W Width of the numbers being used in the division
 * @param numerator The number that is to be divided
 * @param denominator The number that divides the other number
 * @return The quotient of the division operation
 */
template <size_t W>
[[nodiscard]] auto div(const sinteger<W>& numerator, const sinteger<W>& denominator) -> sinteger<W>
{
    return restoring_division(numerator, denominator).first;
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

        shifted.set_word(counter - skip_words, new_word);
    }
    typename sinteger<Width>::word_type new_word;
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

} // namespace aarith
