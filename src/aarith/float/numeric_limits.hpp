#pragma once

#include <aarith/float/floating_point.hpp>

namespace aarith {

/**
 * @brief Returns the min exponent as defined by std::numeric_limits
 *
 * See https://en.cppreference.com/w/cpp/types/numeric_limits/min_exponent
 *
 * @warning This only works for expnont width up to 64 bits
 *
 * @return The min exponent
 */
template <typename F> [[nodiscard]]  constexpr int min_exponent()
{
    auto ftype_min_exp = F::min_exp;
    int min_exponent_ = static_cast<int>(ftype_min_exp) + 1;
    return min_exponent_;
}

/**
 * @brief Returns the min exponent as defined by std::numeric_limits
 *
 * See https://en.cppreference.com/w/cpp/types/numeric_limits/max_exponent
 *
 * @warning This only works for expnont width up to 64 bits
 *
 * @return The min exponent
 */
template <typename F> [[nodiscard]] constexpr int max_exponent()
{
    constexpr auto ftype_max_exp = F::max_exp;
    constexpr int max_exponent_ = static_cast<int64_t>(ftype_max_exp) + 1;
    return max_exponent_;
}

} // namespace aarith

// We are only allowed to extend std with specializations
// https://en.cppreference.com/w/cpp/language/extending_std
template <size_t E, size_t M, typename WordType>
class std::numeric_limits<::aarith::floating_point<E, M, WordType>>
{
public:
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = true;
    static constexpr bool is_integer = false;
    static constexpr bool is_exact = false;
    static constexpr bool has_infinity = true;
    static constexpr bool has_quiet_NaN = true;
    static constexpr bool has_signaling_NaN = true;
    static constexpr bool is_bounded = true;
    static constexpr std::float_denorm_style has_denorm = std::denorm_present;
    static constexpr bool has_denorm_loss = false;
    static constexpr std::float_round_style round_style = std::round_to_nearest;
    static constexpr bool is_iec559 = false;
    static constexpr bool is_modulo = false;
    static constexpr int radix = 2;
    static constexpr int digits = M + 1;
    static constexpr int digits10 = ::aarith::floor<int>(
        (std::numeric_limits<::aarith::floating_point<E, M, WordType>>::digits - 1U) *
        ::aarith::log<10, 2>());

    static constexpr int max_digits10 =
        ::aarith::ceil<int>(std::numeric_limits<::aarith::floating_point<E, M, WordType>>::digits *
                                ::aarith::log<10, 2>() + // NOLINT
                            1);

    static constexpr int min_exponent =
        ::aarith::min_exponent<::aarith::floating_point<E, M, WordType>>();
    static constexpr int max_exponent =
        ::aarith::max_exponent<::aarith::floating_point<E, M, WordType>>();

     static constexpr int min_exponent10 = ::aarith::ceil<int>(double(std::numeric_limits<::aarith::floating_point<E, M, WordType>>::min_exponent) / ::aarith::log<2,10>());
     static constexpr int max_exponent10 = ::aarith::floor<int>(double(std::numeric_limits<::aarith::floating_point<E, M, WordType>>::max_exponent) / ::aarith::log<2,10>());


    static constexpr bool traps = false;

    static constexpr bool tinyness_before = false;

    /**
     * @brief Returns the minimum  positive normalized value
     *
     * For floating-point types with denormalization, min returns the minimum positive normalized
     * value. Note that this behavior may be unexpected, especially when compared to the behavior of
     * min for integral types. To find the value that has no values less than it, use
     * numeric_limits::lowest.
     * @return The minimum  positive normalized value
     */
    static constexpr ::aarith::floating_point<E, M, WordType> min() noexcept
    {
        return ::aarith::floating_point<E, M, WordType>::smallest_normalized();
    }

    static constexpr ::aarith::floating_point<E, M, WordType> lowest() noexcept
    {
        return ::aarith::floating_point<E, M, WordType>::min();
    }

    static constexpr::aarith::floating_point<E, M, WordType> max() noexcept
    {
        return ::aarith::floating_point<E, M, WordType>::max();
    }

    /**
     * @warning This is **not** the correct value!
     * @return
     */
    static constexpr ::aarith::floating_point<E, M, WordType> epsilon() noexcept
    {
        return ::aarith::floating_point<E, M, WordType>::zero();
    }

    static constexpr ::aarith::floating_point<E, M, WordType> round_error() noexcept
    {
        return ::aarith::floating_point<E, M, WordType>::round_error();
    }

    static constexpr ::aarith::floating_point<E, M, WordType> infinity() noexcept
    {
        return ::aarith::floating_point<E, M, WordType>::pos_infinity();
    }

    static constexpr ::aarith::floating_point<E, M, WordType> quiet_NaN() noexcept
    {
        return ::aarith::floating_point<E, M, WordType>::qNaN();
    }

    static constexpr ::aarith::floating_point<E, M, WordType> signaling_NaN() noexcept
    {
        return ::aarith::floating_point<E, M, WordType>::sNaN();
    }

    static constexpr ::aarith::floating_point<E, M, WordType> denorm_min() noexcept
    {
        return ::aarith::floating_point<E, M, WordType>::min();
    }
};
