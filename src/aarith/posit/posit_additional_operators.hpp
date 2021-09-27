#pragma once

#include <aarith/posit_no_operators.hpp>
#include <cstdlib>

namespace aarith {
namespace posit_operators {

/**
 * @brief Unary plus.
 *
 * Returns the posit as-is. Sometimes it can be useful to be explicit
 * about the sign of variables or literals. In these cases, an unary plus
 * is useful.
 *
 * If this posit represents NaR, the result is also NaR.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>& operator+(const posit<N, ES, WT>& self)
{
    return self;
}

/**
 * @brief Sum of two posits.
 *
 * If either operand is NaR, the result is also NaR. If neither operands
 * are NaR, the result is guaranteed not to be NaR.
 *
 * @param rhs The posit to add to this posit.
 * @return The sum of this and rhs.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> operator+(const posit<N, ES, WT>& lhs,
                                                   const posit<N, ES, WT>& rhs)
{
    const auto [sum, rbit] = add(lhs, rhs);
    return sum;
}

/**
 * @brief Add rhs to this.
 *
 * @param rhs The posit to add to this posit.
 */
template <size_t N, size_t ES, typename WT>
posit<N, ES, WT>& operator+=(posit<N, ES, WT>& lhs, const posit<N, ES, WT>& rhs)
{
    lhs = lhs + rhs;
    return lhs;
}

/**
 * @brief Pre-Increment.
 *
 * @return A reference to the incremented value.
 */
template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT>& operator++(posit<N, ES, WT>& self)
{
    // ++x
    self = self + self.one();
    return self;
}

/**
 * @brief Post-Increment.
 *
 * @return A copy of the value before it was incremented.
 */
template <size_t N, size_t ES, typename WT>
constexpr posit<N, ES, WT> operator++(posit<N, ES, WT>& self, int)
{
    // x++
    const auto copy = self;
    self += self.one();
    return copy;
}

/**
 * @brief Unary minus.
 *
 * If this posit represents NaR, the result is also NaR. Also, as posits
 * only have one unique representation of zero, calling this operator on a
 * posit that represents zero again returns zero.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> operator-(const posit<N, ES, WT>& self)
{
    return negate(self);
}

/**
 * @brief Posit subtraction.
 *
 * If either operand is NaR, the result is also NaR. If neither operands
 * are NaR, the result is guaranteed not to be NaR.
 *
 * @param rhs The posit to add to this posit.
 * @return The sum of this and rhs.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> operator-(const posit<N, ES, WT>& lhs,
                                                   const posit<N, ES, WT>& rhs)
{
    return lhs + (-rhs);
}

/**
 * @brief Subtract rhs from this.
 *
 * @param rhs The posit to subtract.
 */
template <size_t N, size_t ES, typename WT>
posit<N, ES, WT>& operator-=(posit<N, ES, WT>& lhs, const posit<N, ES, WT>& rhs)
{
    lhs = lhs - rhs;
    return lhs;
}

/**
 * @brief Pre-Decrement
 *
 * @return A reference to the decremented value.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT>& operator--(posit<N, ES, WT>& self)
{
    // --x
    self = self - self.one();
    return self;
}

/**
 * @brief Post-Decrement.
 *
 * @return A copy of the value before it was decremented.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> operator--(posit<N, ES, WT>& self, int)
{
    // x--
    const auto copy = self;
    self = self - self.one();
    return copy;
}

/**
 * @brief Return the product of this multiplied with rhs.
 *
 * If either operand is NaR, the result is also NaR. If neither operands
 * are NaR, the result is guaranteed not to be NaR.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> operator*(const posit<N, ES, WT>& lhs,
                                                   const posit<N, ES, WT>& rhs)
{
    const auto [product, rbit] = mul(lhs, rhs);
    return product;
}

/**
 * @brief Set this to this multiplied with rhs.
 */
template <size_t N, size_t ES, typename WT>
posit<N, ES, WT>& operator*=(posit<N, ES, WT>& lhs, const posit<N, ES, WT>& rhs)
{
    lhs = lhs * rhs;
    return lhs;
}

/**
 * @brief Return this divided by other.
 *
 * If either operand is NaR, the result is also NaR. If neither operands
 * are NaR, the result is guaranteed not to be NaR.
 */
template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr posit<N, ES, WT> operator/(const posit<N, ES, WT>& lhs,
                                                   const posit<N, ES, WT>& rhs)
{
    const auto [quot, rbit] = div(lhs, rhs);
    return quot;
}

/**
 * @brief Set this to this divided by other.
 */
template <size_t N, size_t ES, typename WT>
posit<N, ES, WT>& operator/=(posit<N, ES, WT>& lhs, const posit<N, ES, WT>& rhs)
{
    lhs = lhs / rhs;
    return lhs;
}

} // namespace posit_operators
} // namespace aarith
