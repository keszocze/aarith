#pragma once

#include <aarith/core/bit_cast.hpp>
#include <aarith/core/traits.hpp>

#include <cstdint>
#include <cstring>
#include <type_traits>

namespace aarith {

template <class F> constexpr size_t get_mantissa_width()
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

template <class F> constexpr size_t get_exponent_width()
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

/**
 * @brief Namespace to prevent people from accidentally using this trait
 */
namespace float_extraction_helper {
template <typename F> struct bit_cast_to_type_trait;
template <> struct bit_cast_to_type_trait<float>
{
    using type = uint32_t;
};
template <> struct bit_cast_to_type_trait<double>
{
    using type = uint64_t;
};

} // namespace float_extraction_helper

/**
 * @brief Extracts the sign of a float or double
 * @tparam F The type working on (float or double)
 * @param num The number whose sign is extracted
 * @return The sign of the provided number
 */
template <class F> constexpr uint8_t extract_sign(F num)
{

    static_assert(std::is_same_v<F, float> || std::is_same_v<F, double>);

    constexpr auto exponent_width = get_exponent_width<F>();
    constexpr auto mantissa_width = get_mantissa_width<F>();

    using int_type = typename float_extraction_helper::bit_cast_to_type_trait<F>::type;

    const int_type inum = bit_cast<int_type, F>(num);

    auto sign = inum >> (exponent_width + mantissa_width);
    return static_cast<uint8_t>(sign);
}

template <class F> inline constexpr auto extract_exponent(F num) -> size_t
{
    constexpr auto exponent_width = get_exponent_width<F>();
    constexpr auto mantissa_width = get_mantissa_width<F>();

    using int_type = typename float_extraction_helper::bit_cast_to_type_trait<F>::type;

    const int_type inum = bit_cast<int_type, F>(num);
    constexpr int_type one(1U);

    const auto exponent = (inum >> mantissa_width) & ((one << exponent_width) - one);

    return static_cast<unsigned int>(exponent);
}

template <class F>
inline constexpr auto extract_mantissa(F num) ->
    typename float_extraction_helper::bit_cast_to_type_trait<F>::type
{
    constexpr auto mantissa_width = get_mantissa_width<F>();

    using int_type = typename float_extraction_helper::bit_cast_to_type_trait<F>::type;

    int_type inum = bit_cast<int_type, F>(num);

    constexpr int_type one(1U);

    const auto mantissa = (inum & ((one << mantissa_width) - one)) | (one << mantissa_width);

    return static_cast<int_type>(mantissa);
}

} // namespace aarith
