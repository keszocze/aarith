#pragma once

namespace aarith {

template <unsigned Radix, unsigned Value> constexpr double log();

template <> constexpr double log<2, 10>()
{
    return 3.32192809489;
}

template <> constexpr double log<10, 2>()
{
    return 0.30102999566;
}

template <class Result> constexpr Result ceil(double num)
{
    return (static_cast<double>(static_cast<Result>(num)) == num)
               ? static_cast<Result>(num)
               : static_cast<Result>(num) + ((num > 0) ? 1 : 0);
}

/**
 * @brief Exponentiation function
 *
 * @note This function does not make any attempts to be fast or to prevent overflows!
 *
 * @note If exponent equals std::numeric_limits<size_t>::max(), this method throws an exception,
 * unless base equals zero
 *
 * @param base
 * @param exponent
 * @return The base to the power of the exponent
 */
constexpr size_t pow(const size_t base, const size_t exponent)
{
    if (exponent == std::numeric_limits<size_t>::max())
    {
        if (base == 0U)
        {
            return 0U;
        }
        else
        {
            throw std::runtime_error(
                "Attempted exponentiation by std::numeric_limits<size_t>::max()");
        }
    }
    size_t result = 1;
    for (size_t i = 0U; i < exponent; ++i)
    {
        result = result * base;
    }
    return result;
}

/**
 * @brief Computes the position of the first set bit (i.e. a bit set to one) in the size_t from LSB
 * to MSB starting with index 1
 *
 * The method returns 0 when no bit is set to one.
 *
 * @note Be aware that this method starts counting at 1 (instead of the more commonly used 0).
 *
 * @example first_set_bit(4)=first_set_bit(0..00100)=3
 *
 * TODO remove this method when clang supports constexpr for std::log2 and std::floor
 *
 * @param n The size_t whose first set bit should be found
 * @return The index of the first set bit in n
 */
[[nodiscard]] constexpr size_t first_set_bit(const size_t n)
{
    size_t first_bit = 0UL;
    size_t tmp = n;
    while (tmp)
    {
        ++first_bit;
        tmp = tmp >> 1;
    }
    return first_bit;
}

/**
 * @brief Rounds down to the next power of two
 * @param n The number to round
 * @return The largest number m=2^k for some k such that m <= n
 *
 * TODO remove this method when clang supports constexpr for std::log2 and std::floor
 */
[[nodiscard]] constexpr size_t floor_to_pow(const size_t n)
{
    const size_t first_bit = first_set_bit(n);
    if (first_bit > 0)
    {
        return 1UL << (first_bit - 1);
    }
    else
    {
        return 0UL;
    }
}

} // namespace aarith
