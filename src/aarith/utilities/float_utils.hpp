#pragma once

namespace aarith {

template<class F>
constexpr auto get_mantissa_width()
{
    static_assert(std::is_floating_point<F>(), "F has to be float or double.");
    if(sizeof(F) == 4)
    {
        return 23U;
    }
    else
    {
        return 52U;
    }
}

template<class F>
constexpr auto get_exponent_width()
{
    static_assert(std::is_floating_point<F>(), "F has to be float or double.");
    if(sizeof(F) == 4)
    {
        return 8U;
    }
    else
    {
        return 11U;
    }
}

inline auto extract_sign(float num)
-> uint8_t
{
    const auto exponent_width = get_exponent_width<float>();
    const auto mantissa_width = get_mantissa_width<float>();
    
    const unsigned int * inum = reinterpret_cast<unsigned int *>(&num);
    auto sign = *inum >> (exponent_width + mantissa_width);
    return static_cast<uint8_t>(sign);
}

inline auto extract_sign(double num)
-> uint8_t
{
    const auto exponent_width = get_exponent_width<double>();
    const auto mantissa_width = get_mantissa_width<double>();
    
    const unsigned long int * lnum = reinterpret_cast<unsigned long int *>(&num);
    auto sign = *lnum >> (exponent_width + mantissa_width);
    return static_cast<uint8_t>(sign);
}

inline auto extract_exponent(float num)
-> uint8_t
{
    const auto exponent_width = get_exponent_width<float>();
    const auto mantissa_width = get_mantissa_width<float>();

    const unsigned int * inum = reinterpret_cast<unsigned int *>(&num);
    const auto exponent = (*inum >> mantissa_width) & ((1U << exponent_width)-1);

    return static_cast<uint8_t>(exponent);
}

inline auto extract_exponent(double num)
-> uint16_t
{
    const auto exponent_width = get_exponent_width<double>();
    const auto mantissa_width = get_mantissa_width<double>();

    const unsigned long int * lnum = reinterpret_cast<unsigned long int *>(&num);
    const auto exponent = (*lnum >> mantissa_width) & ((1U << exponent_width)-1);

    return static_cast<uint16_t>(exponent);
}

inline auto extract_mantissa(float num)
-> uint32_t
{
    const auto mantissa_width = get_mantissa_width<float>();

    const unsigned int * inum = reinterpret_cast<unsigned int *>(&num);
    const auto mantissa = (*inum & ((1U << mantissa_width)-1)) | (1U << mantissa_width);
    
    return static_cast<uint32_t>(mantissa);
}

inline auto extract_mantissa(double num)
-> uint64_t
{
    constexpr auto mantissa_width = get_mantissa_width<double>();

    const unsigned long int * inum = reinterpret_cast<unsigned long int *>(&num);
    const auto mantissa = (*inum & ((1UL << mantissa_width)-1)) | (1UL << mantissa_width);
    
    return static_cast<uint64_t>(mantissa);
}

}//namespace aarith
