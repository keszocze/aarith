#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/float/float_utils.hpp>
#include <aarith/integer_no_operators.hpp>
#include <cstdint>
#include <iostream>
#include <string>
#include <time.h>
#include <type_traits>

namespace aarith {

template <size_t E, size_t M, typename WordType = uint64_t> class normalized_float
{
public:
    static_assert(M > 0, "Mantissa width has to be greater zero.");
    static_assert(E > 1, "Exponent width has to be greater one.");

    static constexpr size_t MW = M + 1;

    explicit constexpr normalized_float()
    {
        exponent = uinteger<E, WordType>(0U);
        mantissa = uinteger<MW, WordType>(0U);
        sign_neg = false;
    }

    template <typename F, typename = std::enable_if_t<std::is_floating_point<F>::value>>
    explicit constexpr normalized_float(F f)
    {
        // static_assert(get_mantissa_width<F>() < M, "Mantissa of f does not fit into explicit
        // mantissa of normalized_float.");

        auto f_exponent = extract_exponent(f);
        auto f_mantissa = extract_mantissa(f);
        constexpr auto f_exponent_width = get_exponent_width<F>();
        constexpr auto f_mantissa_width = get_mantissa_width<F>() + 1;

        sign_neg = extract_sign(f) == 1;

        constexpr auto wider_mantissa = MW >= f_mantissa_width;
        if constexpr (wider_mantissa)
        {
            mantissa = uinteger<MW, WordType>(f_mantissa);
            constexpr auto mantissa_shift = MW - f_mantissa_width;
            mantissa = mantissa << mantissa_shift;
        }
        else
        {
            constexpr auto mantissa_shift = f_mantissa_width - MW;
            f_mantissa >>= mantissa_shift;
            mantissa = uinteger<MW, WordType>(f_mantissa);
        }
        if (f_exponent > 0)
        {
            mantissa.set_msb(true);
        }

        if (f_exponent > 0)
        {
            constexpr auto wider_exponent = E >= f_exponent_width;
            if (wider_exponent)
            {
                exponent = uinteger<E, WordType>(f_exponent);
                const auto f_bias = width_cast<E>(
                    normalized_float<f_exponent_width, f_mantissa_width, WordType>::get_bias());
                const auto exponent_delta = sub(get_bias(), f_bias);
                exponent = add(exponent, exponent_delta);
            }
            else
            {
                const auto bias = get_bias();
                const auto exponent_delta =
                    normalized_float<f_exponent_width, f_mantissa_width, WordType>::get_bias().word(
                        0) -
                    bias.word(0);
                if (static_cast<decltype(f_exponent)>(exponent_delta) > f_exponent)
                {
                    exponent = uinteger<E, WordType>::all_zeroes();
                    const auto mshift =
                        static_cast<decltype(f_exponent)>(exponent_delta) - f_exponent;
                    if (MW < mshift)
                    {
                        mantissa = mantissa >> mshift;
                    }
                    else
                    {
                        mantissa = uinteger<MW, WordType>::all_zeroes();
                    }
                }
                else
                {
                    f_exponent -= static_cast<decltype(f_exponent)>(exponent_delta);
                    exponent = uinteger<E, WordType>(f_exponent);
                }
            }
        }
        else
        {
            mantissa.set_msb(false);
        }
    }

    static constexpr auto exponent_width() -> size_t
    {
        return E;
    }

    static constexpr auto mantissa_width() -> size_t
    {
        return M;
    }

    [[nodiscard]] static constexpr auto get_bias() -> uinteger<E, WordType>
    {
        const uinteger<E, WordType> one(1U);
        const uinteger<E, WordType> shifted = one << (E - 1);
        return sub(shifted, one);
    }

    /**
     * @warn The absolute value of the
     * @return
     */
    [[nodiscard]] static constexpr integer<E + 1, WordType> denorm_exponent()
    {

        const integer<E + 1, WordType> b{get_bias()};
        const integer<E + 1, WordType> neg_bias = sub(integer<E + 1, WordType>::zero(), b);

        return add(neg_bias, integer<E + 1, WordType>::one());
    }

    constexpr auto get_sign() const -> unsigned int
    {
        return (sign_neg) ? 1U : 0U;
    }

    void constexpr set_sign(unsigned int sign)
    {
        sign_neg = (sign & 1U) > 0;
    }

    auto get_exponent() const -> uinteger<E, WordType>
    {
        return exponent;
    }

    [[nodiscard]] constexpr bool is_positive() const
    {
        return !sign_neg;
    }

    [[nodiscard]] constexpr bool is_negative() const
    {
        return sign_neg;
    }

    [[nodiscard]] constexpr bool is_inf() const
    {
        return exponent == uinteger<E>::all_ones() && mantissa == uinteger<MW>::zero();
    }

    /**
     * @brief Checks whether the floating point number is not a number
     *
     * @note There is no distinction between signalling and non-signalling NaN
     *
     * @return True iff the number is NaN
     */
    [[nodiscard]] constexpr bool is_nan() const
    {
        return exponent == uinteger<E>::all_ones() && mantissa != uinteger<MW>::zero();
    }

    [[nodiscard]] constexpr auto unbiased_exponent() const -> integer<E + 1, WordType>
    {
        const integer<E + 1, WordType> signed_bias(get_bias());
        const integer<E + 1, WordType> signed_exponent(get_exponent());

        const integer<E + 1, WordType> real_exponent = sub(signed_exponent, signed_bias);
        return real_exponent;
    }

    /**
     * @brief Checks whether the floating point number is zero
     *
     * Returns true for both the positive and negative zero
     *
     * @return True iff the floating point is zero
     */
    [[nodiscard]] constexpr bool is_zero() const
    {
        return exponent.is_zero() && mantissa.is_zero();
    }

    [[nodiscard]] bool constexpr exponent_is_positive() const
    {
        return get_exponent() > get_bias();
    }

    void set_exponent(const uinteger<E, WordType>& set_to)
    {
        exponent = set_to;
    }

    auto get_full_mantissa() const -> uinteger<MW, WordType>
    {
        return mantissa;
    }

    auto get_mantissa() const -> uinteger<M, WordType>
    {
        return width_cast<M>(mantissa);
    }

    void set_mantissa(const uinteger<MW, WordType>& set_to)
    {
        mantissa = set_to;
    }

    void set_mantissa(const uinteger<M, WordType>& set_to)
    {
        mantissa = set_to;
        mantissa.set_msb(true);
    }

    auto bit(size_t index) const -> typename uinteger<M, WordType>::bit_type
    {
        if (index < MW)
        {
            return mantissa.bit(index);
        }
        else if (index < MW + E)
        {
            return exponent.bit(index - M);
        }
        else
        {
            return static_cast<typename uinteger<M, WordType>::bit_type>(get_sign());
        }
    }

    /**
     * @brief Checks whether the number is normalized
     *
     * @note The NaNs are also considered normalized!
     *
     * @return True iff the number is normalized
     */
    [[nodiscard]] constexpr bool is_normalized() const
    {
        return exponent != uinteger<E>::all_zeroes();
    }

    /**
     * @brief Expands the exponent respecting the bias of the target width
     * @tparam ES The target width of the exponent
     * @return The exponent expanded to a width of ES
     */
    template <size_t ES>[[nodiscard]] uinteger<ES, WordType> constexpr expand_exponent() const
    {
        static_assert(ES >= E, "Expanded exponent must not be shorter than the original exponent");

        using exp_type = uinteger<ES, WordType>;

        const exp_type in_bias{this->get_bias()};
        const auto out_bias = normalized_float<ES, M, WordType>::get_bias();
        auto bias_difference = sub(out_bias, in_bias);

        exp_type exponent_ = uinteger<ES, WordType>{this->get_exponent()};
        exponent_ = add(exponent_, bias_difference);

        return exponent_;
    }

    /**
     * @brief Expands the mantissa by correctly shifting the bits in the larger uinteger
     * @tparam MS The target width of the mantissa
     * @return The mantissa expended to a width of MS
     */
    template <size_t MS>[[nodiscard]] uinteger<MS, WordType> constexpr expand_mantissa() const
    {
        static_assert(MS >= M, "Expanded mantissa must not be shorter than the original mantissa");
        uinteger<MS, WordType> mantissa_{get_mantissa()};
        mantissa_ = mantissa_ << (size_t{MS} - size_t{M});
        return mantissa_;
    }

    /**
     * @brief Creates a bitstring representation of the floating point number.
     *
     * The bitstring returned may use more bits for the exponent/mantissa than the floating point
     * number it was created from. You can use this method to create valid IEEE 754 bitstrings.
     * @tparam ES The number of bits to use for the exponent
     * @tparam MS The number of bits to use for the mantissa
     * @return IEEE-754 bitstring representation of the floating point number
     */
    template <size_t ES = E, size_t MS = M>
    word_array<1 + ES + MS, WordType> constexpr as_word_array() const
    {
        using namespace aarith;

        static_assert(ES >= E);
        static_assert(MS >= M);

        auto mantissa_ = expand_mantissa<MS>();
        auto joined = concat(expand_exponent<ES>(), mantissa_);
        auto with_sign = concat(word_array<1, WordType>{this->get_sign()}, joined);

        return with_sign;
    }

    /**
     *
     * @return
     */
    [[nodiscard]] explicit constexpr operator float() const
    {
        return generic_cast<float>();
    }

    [[nodiscard]] explicit constexpr operator double() const
    {
        return generic_cast<double>();
    }

private:
    template <typename To>[[nodiscard]] constexpr To generic_cast() const
    {

        static_assert(std::is_floating_point<To>(), "Can only convert to float or double.");

        using namespace aarith;

        using uint_storage = typename float_extraction_helper::bit_cast_to_type_trait<To>::type;
        constexpr auto exp_width = get_exponent_width<To>();
        constexpr auto mant_width = get_mantissa_width<To>();

        static_assert(E <= exp_width, "Exponent width too large");
        static_assert(M <= mant_width, "Mantissa width too large");

        uinteger<1 + exp_width + mant_width, WordType> array{
            as_word_array<exp_width, mant_width>()};

        uint_storage bitstring = static_cast<uint_storage>(array);
        To result = bit_cast<To>(bitstring);
        return result;
    }

    bool sign_neg;
    uinteger<E, WordType> exponent;
    uinteger<MW, WordType> mantissa;
};

template <size_t E, size_t M, typename WordType> class is_integral<normalized_float<E, M, WordType>>
{
public:
    static constexpr bool value = false;
};

template <size_t E, size_t M, typename WordType> class is_float<normalized_float<E, M, WordType>>
{
public:
    static constexpr bool value = true;
};

template <size_t E, size_t M, typename WordType> class is_unsigned<normalized_float<E, M, WordType>>
{
public:
    static constexpr bool value = false;
};

template <size_t E, size_t M1, size_t M2, typename WordType = uint64_t>
auto equal_except_rounding(const normalized_float<E, M1, WordType> lhs,
                           const normalized_float<E, M2, WordType> rhs) -> bool
{
    if (lhs.get_sign() == rhs.get_sign() && lhs.get_exponent() == rhs.get_exponent())
    {
        if (lhs.get_full_mantissa() == rhs.get_full_mantissa())
        {
            return true;
        }
        else
        {
            constexpr auto Min = std::min(M1, M2);
            constexpr auto offset_M1 = M1 - Min;
            constexpr auto offset_M2 = M2 - Min;

            const auto m1 = lhs.get_full_mantissa();
            const auto m2 = rhs.get_full_mantissa();

            const auto bit1 = m1.bit(offset_M1);
            const auto bit2 = m2.bit(offset_M2);

            bool rounding_error = true;
            bool has_to_be_equal = bit1 == bit2;
            for (auto i = 0U; i < Min; ++i)
            {
                if (has_to_be_equal)
                {
                    if (m1.bit(i + offset_M1) != m2.bit(i + offset_M2))
                    {
                        rounding_error = false;
                        break;
                    }
                }
                else
                {
                    if (m1.bit(i + offset_M1) != m2.bit(i + offset_M2))
                    {
                        if (m1.bit(i + offset_M1) == bit1 && m2.bit(i + offset_M2) == bit2)
                        {
                            continue;
                        }
                        else
                        {
                            has_to_be_equal = true;
                        }
                    }
                    else if (i == 1)
                    {
                        has_to_be_equal = true;
                    }
                    else
                    {
                        rounding_error = false;
                        break;
                    }
                }
            }

            return rounding_error;
        }
    }
    return false;
}

template <size_t E, size_t M, typename WordType = uint64_t>
auto abs(const normalized_float<E, M, WordType> nf) -> normalized_float<E, M, WordType>
{
    auto absolute = nf;
    absolute.set_sign(0U);

    return absolute;
}

template <size_t M, typename WordType = uint64_t>
auto rshift_and_round(const uinteger<M, WordType>& m, const size_t shift_by)
    -> uinteger<M, WordType>
{
    if (shift_by == 0)
    {
        return m;
    }

    if (shift_by > M)
    {
        return uinteger<M, WordType>(0U);
    }

    auto round = 0U;

    if (m.bit(shift_by - 1) == 1)
    {
        bool on_border = true;
        for (auto c = shift_by - 1; c > 0; --c)
        {
            if (m.bit(c - 1) == 1)
            {
                on_border = false;
                break;
            }
        }
        if (on_border)
        {
            round = 0U;
        }
        else
        {
            round = 1U;
        }
    }

    return add((m >> shift_by), uinteger<M>(round));
}

template <size_t E, size_t M1, size_t M2 = M1, typename WordType = uint64_t>
auto normalize(const normalized_float<E, M1, WordType>& nf) -> normalized_float<E, M2, WordType>
{
    auto denormalized = nf;

    auto exponent = denormalized.get_exponent();
    auto mantissa = denormalized.get_full_mantissa();

    auto one_at = find_leading_one(mantissa);

    if (one_at > M1)
    {
        exponent = uinteger<E, WordType>(0U);
        // denormalized.set_sign(0);
    }
    else if (one_at >= M2)
    {
        auto shift_by = one_at - M2;
        mantissa = rshift_and_round(mantissa, shift_by);
        exponent = add(exponent, uinteger<E, WordType>(shift_by));
    }
    else
    {
        auto shift_by = M2 - one_at;
        mantissa = (mantissa << shift_by);
        exponent = sub(exponent, uinteger<E, WordType>(shift_by));
    }

    normalized_float<E, M2, WordType> normalized_float;

    normalized_float.set_sign(denormalized.get_sign());
    normalized_float.set_exponent(exponent);
    normalized_float.set_mantissa(width_cast<M2 + 1>(mantissa));

    return normalized_float;
}

template <class uint> auto find_leading_one(const uint mantissa) -> typename uint::word_type
{
    static_assert(::aarith::is_integral_v<uint>);
    static_assert(::aarith::is_unsigned_v<uint>);

    const auto width = uint::width();
    auto one_at = width;

    for (auto i = uint::word_count(); i > 0; --i)
    {
        if (mantissa.word(i - 1) == 0)
        {
            if (i == uint::word_count())
            {
                auto const modulo = width % (sizeof(typename uint::word_type) * 8);
                one_at -= modulo == 0 ? sizeof(typename uint::word_type) * 8 : modulo;
            }
            else
            {
                one_at -= sizeof(typename uint::word_type) * 8;
            }
        }
        else
        {
            break;
        }
    }

    while (one_at > 0)
    {
        --one_at;
        if (mantissa.bit(one_at) == 1)
        {
            break;
        }
    }

    if (one_at == 0 && mantissa.bit(0) == 0)
    {
        one_at = width;
    }

    return one_at;
}

} // namespace aarith
