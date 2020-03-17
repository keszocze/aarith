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
 * @note If exponent equals std::numeric_limits<size_t>::max(), this method throws an exception, unless base equals zero
 *
 * @param base
 * @param exponent
 * @return The base to the power of the exponent
 */
constexpr size_t pow(const size_t base, const size_t exponent)
{
    if (exponent == std::numeric_limits<size_t>::max()) {
        if (base == 0U) {
            return 0U;
        }
        else {
            throw std::runtime_error("Attempted exponentiation by std::numeric_limits<size_t>::max()");
        }
    }
    size_t result = 1;
    for (size_t i = 0U; i <= exponent; ++i)
    {
        result = result * base;
    }
    return result;
}

} // namespace aarith