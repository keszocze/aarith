#pragma once

#include <aarith/core/traits.hpp>

#include <cstdint>
#include <cstring>
#include <type_traits>

namespace aarith {

template <class F> constexpr auto get_mantissa_width()
{
    static_assert(std::is_floating_point<F>(), "F has to be float or double.");
    if constexpr (sizeof(F) == 4)
    {
        return 23U;
    }
    else
    {
        return 52U;
    }
}

template <class F> constexpr auto get_exponent_width()
{
    static_assert(std::is_floating_point<F>(), "F has to be float or double.");
    if constexpr (sizeof(F) == 4)
    {
        return 8U;
    }
    else
    {
        return 11U;
    }
}

template <class I, class F> I get_float_bits(const F f)
{
    /*
     * We have to make sure that only the correct types are cast
     */
    static_assert((std::is_same_v<I, uint32_t> && std::is_same_v<F, float>) ||
                  (std::is_same_v<I, uint64_t> && std::is_same_v<F, double>));

    I value;
    std::memcpy(&value, &f, sizeof(I));
    return value;
}

inline auto extract_sign(float num) -> uint8_t
{
    constexpr auto exponent_width = get_exponent_width<float>();
    constexpr auto mantissa_width = get_mantissa_width<float>();

    uint32_t inum = get_float_bits<uint32_t>(num);

    auto sign = inum >> (exponent_width + mantissa_width);
    return static_cast<uint8_t>(sign);
}

inline auto extract_sign(double num) -> uint8_t
{
    constexpr auto exponent_width = get_exponent_width<double>();
    constexpr auto mantissa_width = get_mantissa_width<double>();

    uint64_t lnum = get_float_bits<uint64_t>(num);

    auto sign = lnum >> (exponent_width + mantissa_width);
    return static_cast<uint8_t>(sign);
}

inline auto extract_exponent(float num) -> unsigned int
{
    constexpr auto exponent_width = get_exponent_width<float>();
    constexpr auto mantissa_width = get_mantissa_width<float>();

    uint32_t inum = get_float_bits<uint32_t>(num);

    const auto exponent = (inum >> mantissa_width) & ((1U << exponent_width) - 1);

    return static_cast<unsigned int>(exponent);
}

inline auto extract_exponent(double num) -> unsigned int
{
    constexpr auto exponent_width = get_exponent_width<double>();
    constexpr auto mantissa_width = get_mantissa_width<double>();

    uint64_t lnum = get_float_bits<uint64_t>(num);

    const auto exponent = (lnum >> mantissa_width) & ((1U << exponent_width) - 1);

    return static_cast<unsigned int>(exponent);
}

inline auto extract_mantissa(float num) -> uint32_t
{
    constexpr auto mantissa_width = get_mantissa_width<float>();

    uint32_t inum = get_float_bits<uint32_t>(num);

    const auto mantissa = (inum & ((1U << mantissa_width) - 1)) | (1U << mantissa_width);

    return static_cast<uint32_t>(mantissa);
}

inline auto extract_mantissa(double num) -> uint64_t
{
    constexpr auto mantissa_width = get_mantissa_width<double>();

    uint64_t lnum = get_float_bits<uint64_t>(num);

    const auto mantissa = (lnum & ((1ULL << mantissa_width) - 1)) | (1ULL << mantissa_width);

    return static_cast<uint64_t>(mantissa);
}

} // namespace aarith
