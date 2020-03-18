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
 * @brief Computes the position of the first set bit (i.e. a bit set to one) in the size_t from MSB
 * to LSB
 *
 * @param n The size_t whose first set bit should be found
 * @return The index of the first set bit in n
 */
[[nodiscard]] constexpr size_t first_set_bit(const size_t n)
{
    size_t first_bit = 0UL;
    while (n >> first_bit)
    {
        ++first_bit;
    }
    return first_bit;
}

} // namespace aarith