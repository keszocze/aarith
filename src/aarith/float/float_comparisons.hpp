#pragma once

#include <aarith/float/floating_point.hpp>
#include <cstdint>

namespace aarith {

template <size_t E, size_t M>
auto constexpr operator<(const floating_point<E, M> lhs, const floating_point<E, M> rhs) -> bool
{

    if (lhs.is_nan() || rhs.is_nan())
    {
        return false;
    }

    // positive and negative zero are to be treated equally
    if (lhs.is_zero() && rhs.is_zero())
    {
        return false;
    }

    if (lhs.is_negative() != rhs.is_negative())
    {
        return lhs.is_negative() && rhs.is_positive();
    }
    // now the sign is equal

    if (lhs.get_exponent() == rhs.get_exponent())
    {
        if (lhs.get_full_mantissa() == rhs.get_full_mantissa())
        {
            return false;
        }
        else if (lhs.get_full_mantissa() < rhs.get_full_mantissa())
        {
            return lhs.get_sign() == 0;
        }
        else
        {
            return lhs.get_sign() == 1;
        }
    }
    else if (lhs.get_exponent() < rhs.get_exponent())
    {
        return lhs.get_sign() == 0;
    }
    else
    {
        return lhs.get_sign() == 1;
    }
}

/**
 * @brief Compares to floating point numbers bit by bit
 * @tparam E Exponent width
 * @tparam M Mantissa width
 * @param lhs
 * @param rhs
 * @return True iff the floats match in every single bit
 */
template <size_t E, size_t M>
bool constexpr bitwise_equality(const floating_point<E, M> lhs, const floating_point<E, M> rhs)
{
    const bool equal_sign = (lhs.get_sign() == rhs.get_sign());
    const bool equal_exponent = (lhs.get_exponent() == rhs.get_exponent());
    const bool equal_mantissa = (lhs.get_full_mantissa() == rhs.get_full_mantissa());
    //    std::cout << equal_sign << "\t" << equal_exponent << "\t" << equal_mantissa << "\n";
    return equal_sign && equal_exponent && equal_mantissa;
}

template <size_t E, size_t M>
auto constexpr operator==(const floating_point<E, M> lhs, const floating_point<E, M> rhs) -> bool
{

    if (lhs.is_nan() || rhs.is_nan())
    {
        return false;
    }

    // positive and negative zero should be treated equal
    if (lhs.is_zero() && rhs.is_zero())
    {
        return true;
    }

    return bitwise_equality(lhs, rhs);
}

template <size_t E, size_t M, size_t E_, size_t M_,
          typename = std::enable_if_t<(E != E_) || (M != M_)>>
bool constexpr logical_equality(const floating_point<E, M> lhs, const floating_point<E_, M_> rhs)
{

    bool equal_mantissa = false;

    if constexpr (M > M_)
    {
        return logical_equality(rhs, lhs);
    }
    else if constexpr (M == M_)
    {
        equal_mantissa = lhs.get_full_mantissa() == rhs.get_full_mantissa();
    }
    else
    {
        const auto val = bit_range<M_, M_ - M>(rhs.get_full_mantissa());
        const auto zero = bit_range<(M_ - M) - 1, 0>(rhs.get_full_mantissa());
        equal_mantissa = (zero == zero.all_zeroes()) && (val == lhs.get_full_mantissa());
    }

    const bool equal_sign = (lhs.get_sign() == rhs.get_sign());

    const bool equal_exponent = (lhs.unbiased_exponent() == rhs.unbiased_exponent());
    //    std::cout << equal_sign << "\t" << equal_exponent << "\t" << equal_mantissa << "\n";
    return equal_sign && equal_exponent && equal_mantissa;
}

template <size_t E, size_t M, size_t E_, size_t M_,
          typename = std::enable_if_t<(E != E_) || (M != M_)>>
auto constexpr operator==(const floating_point<E, M> lhs, const floating_point<E_, M_> rhs) -> bool
{
    //    std::cout << "spec float op==\n";
    if (lhs.is_nan() || rhs.is_nan())
    {
        return false;
    }

    // positive and negative zero should be treated equal
    if (lhs.is_zero() && rhs.is_zero())
    {
        return true;
    }

    if (lhs.is_inf() && rhs.is_inf())
    {
        return lhs.is_negative() == rhs.is_negative();
    }

    const bool logic_eq = logical_equality(lhs, rhs);
    return logic_eq;
}

template <size_t E, size_t M, size_t E_, size_t M_,
          typename = std::enable_if_t<(E != E_) || (M != M_)>>
bool constexpr operator<(const floating_point<E, M> lhs, const floating_point<E_, M_> rhs)
{
    if (lhs.is_nan() || rhs.is_nan())
    {
        return false;
    }

    // positive and negative zero are to be treated equally
    if (lhs.is_zero() && rhs.is_zero())
    {
        return false;
    }

    if (lhs.is_negative() != rhs.is_negative())
    {
        return lhs.is_negative() && rhs.is_positive();
    }
    // now the sign is equal

    if (lhs.unbiased_exponent() == rhs.unbiased_exponent())
    {
        if (lhs.get_full_mantissa() == rhs.get_full_mantissa())
        {
            return false;
        }
        else if (lhs.get_full_mantissa() < rhs.get_full_mantissa())
        {
            return lhs.get_sign() == 0;
        }
        else
        {
            return lhs.get_sign() == 1;
        }
    }
    else if (lhs.unbiased_exponent() < rhs.unbiased_exponent())
    {
        return lhs.get_sign() == 0;
    }
    else
    {
        return lhs.get_sign() == 1;
    }
}

template <size_t E, size_t M, size_t E_, size_t M_>
auto constexpr operator!=(const floating_point<E, M> lhs, const floating_point<E_, M_> rhs) -> bool
{
    return !(lhs == rhs);
}

template <size_t E, size_t M>
auto constexpr operator>(const floating_point<E, M> lhs, const floating_point<E, M> rhs) -> bool
{
    return rhs < lhs;
}

template <size_t e, size_t m>
auto constexpr operator>=(const floating_point<e, m> lhs, const floating_point<e, m> rhs) -> bool
{
    return rhs < lhs || lhs == rhs;
}

template <size_t e, size_t m>
auto constexpr operator<=(const floating_point<e, m> lhs, const floating_point<e, m> rhs) -> bool
{
    return lhs < rhs || lhs == rhs;
}

} // namespace aarith
