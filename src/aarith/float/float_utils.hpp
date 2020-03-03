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

/**
 * To avoid undefined behaviour when type punning, we are using memcpy.
 * This is rather annoying but seems to be the way to go. The implementation
 * is stolen from this talk:
 * https://www.youtube.com/watch?v=_qzMpk-22cc
 *
 * @todo replace with std::bit_cast when switching to C++20
 *
 * @tparam To The type to convert to
 * @tparam From The type to convert from
 * @param src The source of the bits
 * @return The new type filled with the bits of src
 */
template <typename To, typename From,
          typename =
              std::enable_if_t<(sizeof(To) == sizeof(From)) && std::is_trivially_copyable_v<From> &&
                               std::is_trivially_copyable_v<To>>>
To bit_cast(const From& src) noexcept
{
    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
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
template <class F> uint8_t extract_sign(F num)
{

    static_assert(std::is_same_v<F, float> || std::is_same_v<F, double>);

    constexpr auto exponent_width = get_exponent_width<F>();
    constexpr auto mantissa_width = get_mantissa_width<F>();

    using int_type = typename float_extraction_helper::bit_cast_to_type_trait<F>::type;

    const int_type inum = bit_cast<int_type, F>(num);

    auto sign = inum >> (exponent_width + mantissa_width);
    return static_cast<uint8_t>(sign);
}

// inline auto extract_sign(float num) -> uint8_t
//{
//    constexpr auto exponent_width = get_exponent_width<float>();
//    constexpr auto mantissa_width = get_mantissa_width<float>();
//
//    uint32_t inum = get_float_bits<uint32_t>(num);
//
//    auto sign = inum >> (exponent_width + mantissa_width);
//    return static_cast<uint8_t>(sign);
//}
//
// inline auto extract_sign(double num) -> uint8_t
//{
//    constexpr auto exponent_width = get_exponent_width<double>();
//    constexpr auto mantissa_width = get_mantissa_width<double>();
//
//    uint64_t lnum = get_float_bits<uint64_t>(num);
//
//    auto sign = lnum >> (exponent_width + mantissa_width);
//    return static_cast<uint8_t>(sign);
//}

inline auto extract_exponent(float num) -> unsigned int
{
    constexpr auto exponent_width = get_exponent_width<float>();
    constexpr auto mantissa_width = get_mantissa_width<float>();

    uint32_t inum = bit_cast<uint32_t>(num);

    const auto exponent = (inum >> mantissa_width) & ((1U << exponent_width) - 1);

    return static_cast<unsigned int>(exponent);
}

inline auto extract_exponent(double num) -> unsigned int
{
    constexpr auto exponent_width = get_exponent_width<double>();
    constexpr auto mantissa_width = get_mantissa_width<double>();

    uint64_t lnum = bit_cast<uint64_t>(num);

    const auto exponent = (lnum >> mantissa_width) & ((1U << exponent_width) - 1);

    return static_cast<unsigned int>(exponent);
}

inline auto extract_mantissa(float num) -> uint32_t
{
    constexpr auto mantissa_width = get_mantissa_width<float>();

    uint32_t inum = bit_cast<uint32_t>(num);

    const auto mantissa = (inum & ((1U << mantissa_width) - 1)) | (1U << mantissa_width);

    return static_cast<uint32_t>(mantissa);
}

inline auto extract_mantissa(double num) -> uint64_t
{
    constexpr auto mantissa_width = get_mantissa_width<double>();

    uint64_t lnum = bit_cast<uint64_t>(num);

    const auto mantissa = (lnum & ((1ULL << mantissa_width) - 1)) | (1ULL << mantissa_width);

    return static_cast<uint64_t>(mantissa);
}

} // namespace aarith
