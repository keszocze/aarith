#pragma once

#include <aarith/core/bit_cast.hpp>
#include <aarith/core/traits.hpp>

#include <aarith/integer/integers.hpp>

#include <cstdint>
#include <cstring>
#include <type_traits>

namespace aarith {

template <typename F, typename = std::enable_if_t<std::is_floating_point<F>::value>>
constexpr size_t get_mantissa_width()
{
    return 0U;
}

template <> constexpr size_t get_mantissa_width<float>()
{
    return 23U;
}

template <> constexpr size_t get_mantissa_width<double>()
{
    return 52U;
}

template <typename F, typename = std::enable_if_t<std::is_floating_point<F>::value>>
constexpr size_t get_exponent_width()
{
    return 0;
}

template <> constexpr size_t get_exponent_width<float>()
{
    return 8U;
}

template <> constexpr size_t get_exponent_width<double>()
{
    return 11U;
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

struct float_disassembly
{
    uint64_t exponent;
    uint64_t mantissa;
    bool is_neg;
};

template <typename F,
          typename = std::enable_if_t<std::is_floating_point<F>::value>>
inline auto disassemble_float(F num)
-> float_disassembly
{
    constexpr auto exponent_width = get_exponent_width<F>();
    constexpr auto mantissa_width = get_mantissa_width<F>();

    using int_type = typename float_extraction_helper::bit_cast_to_type_trait<F>::type;

    const int_type inum = bit_cast<int_type, F>(num);
    constexpr int_type one(1U);

    const auto exponent = (inum >> mantissa_width) & ((one << exponent_width) - one);
    const auto mantissa = (inum & ((one << mantissa_width) - one)) | (one << mantissa_width);
    const auto sign = (inum >> (mantissa_width + exponent_width));
    
    const struct float_disassembly fd {exponent, mantissa, sign > 0};

    return fd;
}

template <typename F, typename WordType,
          typename = std::enable_if_t<std::is_floating_point<F>::value>>
inline constexpr auto extract_exponent(F num)
{
    constexpr auto exponent_width = get_exponent_width<F>();
    constexpr auto mantissa_width = get_mantissa_width<F>();

    using int_type = typename float_extraction_helper::bit_cast_to_type_trait<F>::type;

    const int_type inum = bit_cast<int_type, F>(num);
    constexpr int_type one(1U);

    const auto exponent = (inum >> mantissa_width) & ((one << exponent_width) - one);

    return uinteger<exponent_width, WordType>{exponent};
}

template <typename F, typename Wordtype,
          typename = std::enable_if_t<std::is_floating_point<F>::value>>
inline constexpr auto extract_mantissa(F num)
{
    constexpr auto mantissa_width = get_mantissa_width<F>();

    using int_type = typename float_extraction_helper::bit_cast_to_type_trait<F>::type;

    int_type inum = bit_cast<int_type, F>(num);

    constexpr int_type one(1U);

    const auto mantissa = (inum & ((one << mantissa_width) - one)) | (one << mantissa_width);

    return uinteger<mantissa_width, Wordtype>{mantissa};
}

} // namespace aarith
