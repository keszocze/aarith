#pragma once

#include <aarith/integer/integer.hpp>
#include <aarith/integer/uinteger.hpp>
#include <aarith/integer/uinteger_operations.hpp>

namespace aarith {

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





} // namespace aarith
