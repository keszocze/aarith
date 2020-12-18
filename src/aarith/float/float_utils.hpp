#pragma once

#include <aarith/core/bit_cast.hpp>
#include <aarith/core/traits.hpp>

#include <aarith/integer/integers.hpp>

#include <cstdint>
#include <cstring>
#include <type_traits>

namespace aarith {

/**
 * @brief Tests whether a floating-point number is negative.
 *
 * isSignMinus(x) is true if and only if x has negative sign. isSignMinus applies to zeros and NaNs
 * as well.
 *
 * @tparam F The floating-point type
 * @param f The floating-point number to test
 * @return True iff the number is negative
 */
template <typename F> bool constexpr isSignMinus(const F& f)
{
    return f.is_negative();
}

/**
 * @brief Tests whether a floating-point number is normal.
 *
 * isNormal(f) is true if and only if f is normal (not zero, subnormal, infinite, or NaN)
 *
 * @tparam F The floating-point type
 * @param f The floating-point number to test
 * @return True iff the number is normal
 */
template <typename F> bool constexpr isNormal(const F& f)
{
    return f.is_normalized();
}

/**
 * @brief Tests whether a floating-point number is finite.
 *
 * isFinite(f) is true if and only if f is zero, subnormal or normal (not infinite or NaN).
 *
 * @tparam F The floating-point type
 * @param f The floating-point number to test
 * @return True iff the number is finite
 */
template <typename F> bool constexpr isFinite(const F& f)
{
    return f.is_finite();
}

/**
 * @brief Tests whether a floating-point number is zero (ignoring the sign).
 *
 * @tparam F The floating-point type
 * @param f The floating-point number to test
 * @return True iff the number is +/- zero
 */
template <typename F> bool constexpr isZero(const F& f)
{
    return f.is_zero();
}

/**
 * @brief Tests whether a floating-point number is zero (ignoring the sign).
 *
 * @note Zero is *not* considered subnormal!
 *
 * @tparam F The floating-point type
 * @param f The floating-point number to test
 * @return True iff the number is +/- zero
 */
template <typename F> bool constexpr isSubnormal(const F& f)
{
    return f.is_subnormal();
}

/**
 * @brief Tests whether a floating-point number is infinite (ignoring the sign)
 *
 * @tparam F The floating-point type
 * @param f The floating-point number to test
 * @return True iff the number is infinite
 */
template <typename F> bool constexpr isInfinite(const F& f)
{
    return f.is_inf();
}

/**
 * @brief Tests whether a floating-point number is NaN
 *
 * @note This method does not distinguish between signalling and quiet NaNs
 *
 * @tparam F The floating-point type
 * @param f The floating-point number to test
 * @return True iff the number is NaN
 */
template <typename F> bool constexpr isNaN(const F& f)
{
    return f.is_nan();
}

/**
 * @brief Tests whether a floating-point number is a signaling NaN
 *
 * @tparam F The floating-point type
 * @param f The floating-point number to test
 * @return True iff the number is a signaling NaN
 */
template <typename F> bool constexpr isSignaling(const F& f)
{
    return f.is_sNaN();
}

/**
 * @brief Tests whether a floating-point number is a quiet NaN
 *
 * @note This method is not required by the IEEE 754 standard
 *
 * @tparam F The floating-point type
 * @param f The floating-point number to test
 * @return True iff the number is a quiet NaN
 */
template <typename F> bool constexpr isQuiet(const F& f)
{
    return f.is_qNaN();
}

/**
 * @brief Enumeration of the different types of floating-points
 *
 * These are defined in Section 5.7.2. of the 2019 standard
 */
enum class IEEEClass
{
    signalingNaN,
    quietNaN,
    negativeInfinity,
    negativeNormal,
    negativeSubnormal,
    negativeZero,
    positiveZero,
    positiveSubnormal,
    positiveNormal,
    positiveInfinity,
    UNCLASSIFIED // error case that should never be reached!
};

/**
 * @brief Determines the class (e.g. NaN, positive subnormal) of a floating-point number.
 *
 * This method corresponds to the "class" method described in Section 5.7.2 of the 2019 standard.
 * The function name had to be changed as C++ does not allow to name functions "class".
 *
 * @tparam F The floating-point type
 * @param f The floating-point number to test
 * @return The class of the floating-point number
 */
template <typename F> constexpr IEEEClass fp_class(const F& f)
{

    if (f.is_qNaN())
    {
        return IEEEClass::quietNaN;
    }

    if (f.is_sNaN())
    {
        return IEEEClass::signalingNaN;
    }

    if (f.is_neg_inf())
    {
        return IEEEClass::negativeInfinity;
    }

    if (f.is_pos_inf())
    {
        return IEEEClass::positiveInfinity;
    }

    if (f.is_neg_zero())
    {
        return IEEEClass::negativeZero;
    }

    if (f.is_pos_zero())
    {
        return IEEEClass::positiveZero;
    }

    if (f.is_normalized())
    {
        if (f.is_positive())
        {
            return IEEEClass::positiveNormal;
        }
        else
        {
            return IEEEClass::negativeNormal;
        }
    }

    if (f.is_subnormal())
    {
        if (f.is_positive())
        {
            return IEEEClass::positiveSubnormal;
        }
        else
        {
            return IEEEClass::negativeSubnormal;
        }
    }

    // we should never reach this!
    return IEEEClass::UNCLASSIFIED;
}

/**
 * @brief The possible radices used to store floating-point numbers.
 *
 * Aarith uses base two only.
 */
enum class Radix
{
    Two,
    Ten
};

/**
 * @brief Returns the radix of the floating-point number.
 *
 * Aarith only supports base two, hence Radix::Two is the only return value.
 *
 * @tparam F The floating-point type
 * @param f The floating-point number to test
 * @return
 */
template <typename F> Radix constexpr radix([[maybe_unused]] const F& f)
{
    return Radix::Two;
}

/**
 * @brief Tests whether a floating-point number canoncial
 *
 * Aarith does not support non-canonical numbers hence this method always returns true.
 *
 * @tparam F The floating-point type
 * @param f The floating-point number to test
 * @return True
 */
template <typename F> bool constexpr isCanonical([[maybe_unused]] const F& f)
{
    return true;
}

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
    static constexpr size_t width = 32;
};
template <> struct bit_cast_to_type_trait<double>
{
    using type = uint64_t;
    static constexpr size_t width = 64;
};

} // namespace float_extraction_helper

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
