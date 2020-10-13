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

    using IntegerExp = uinteger<E, WordType>;
    using IntegerMant = uinteger<MW, WordType>;

    static constexpr IntegerExp bias = uinteger<E - 1, WordType>::all_ones();

    explicit constexpr normalized_float()
    {
        exponent = IntegerExp::zero();
        mantissa = IntegerMant::zero();
        sign_neg = false;
    }

    template <typename F, typename = std::enable_if_t<std::is_floating_point<F>::value>>
    explicit normalized_float(const F f)
    {

        auto extracted_exp = extract_exponent<F, WordType>(f);
        auto extracted_mantissa = extract_mantissa<F, WordType>(f);
        constexpr size_t ext_exp_width = get_exponent_width<F>();
        constexpr size_t ext_mant_width = get_mantissa_width<F>();

        sign_neg = (f < 0);

        // if the mantissa of the normalized float can store at least as many bit as the
        // mantissa of the supplied native data type, it has to be shifted by the
        // difference in widths (which may be zero!)
        if constexpr (ext_mant_width <= M)
        {
            // this is basically a width cast of the extracted mantissa to fit into stored one
            mantissa = extracted_mantissa;
            mantissa <<= (M - ext_mant_width);
        }
        else
        {
            // the extracted mantissa does *not* necessarily fit in the stored mantissa. we perform
            // a width cast that potentially loses quite some precision
            // we first move the bits to the right in order not to lose them
            extracted_mantissa >>= (ext_mant_width - M);
            mantissa = width_cast<M>(extracted_mantissa);
        }

        if constexpr (ext_exp_width < E)
        {
            // a wider exponent means a larger bias, so we have to add the difference between two
            // biases to the old exponent to get the old value
            // TODO handle denormalized numbers?
            constexpr IntegerExp smaller_bias = uinteger<ext_exp_width - 1, WordType>::all_ones();
            constexpr IntegerExp diff = sub(bias, smaller_bias);
            exponent = add(IntegerExp{extracted_exp}, diff);
        }
        else if (ext_exp_width > E)
        {
            // a smaller exponent means a lower bias, so we subtract the difference between the two
            // biases from the original bias
            // in case of over- or underflow we set the value to infinity or 0
            // TODO handle denormalized numbers?
            using OExp = uinteger<ext_exp_width + 1, WordType>;
            constexpr OExp bigger_bias = uinteger<ext_exp_width - 1, WordType>::all_ones();
            constexpr OExp smaller_bias = bias;
            constexpr OExp diff = sub(bigger_bias, smaller_bias);
            const auto exp = sub(OExp(extracted_exp), diff);
            const bool overflow = exp >= OExp(IntegerExp::all_ones()) && exp.bit(ext_exp_width) == 0;
            const bool underflow = exp.bit(ext_exp_width) == 1;
            if (underflow)
            {
                mantissa = mantissa.all_zeroes();
                exponent = exponent.all_zeroes();
            }
            else if (overflow)
            {
                mantissa = mantissa.all_zeroes();
                exponent = exponent.all_ones();
            }
            else
            {
                exponent = width_cast<E>(exp);
            }
        }
        else
        {
            // assume same size
            exponent = extracted_exp;
        }

        if (!is_special())
        {
            mantissa.set_msb(true);
        }
    }

    [[nodiscard]] static constexpr normalized_float zero()
    {
        return normalized_float{};
    }

    [[nodiscard]] static constexpr normalized_float one()
    {
        normalized_float num{};
        auto exp = word_array<E, WordType>::all_ones();
        exp.set_msb(false);
        num.set_exponent(exp);

        auto mant = uinteger<MW, WordType>::all_zeroes();
        mant.set_msb(true);
        num.set_full_mantissa(mant);

        return num;
    }

    [[nodiscard]] static constexpr normalized_float pos_infinity()
    {
        normalized_float pos_inf{};
        pos_inf.set_exponent(word_array<E, WordType>::all_ones());
        return pos_inf;
    }

    [[nodiscard]] static constexpr normalized_float neg_infinity()
    {
        normalized_float neg_inf{};
        neg_inf.set_exponent(word_array<E, WordType>::all_ones());
        neg_inf.set_sign(-1);
        return neg_inf;
    }

    [[nodiscard]] static constexpr normalized_float NaN()
    {
        normalized_float nan{};
        nan.set_exponent(word_array<E, WordType>::all_ones());
        auto nan_mantissa = word_array<M, WordType>::all_zeroes();
        nan_mantissa.set_msb(1);

        nan.set_mantissa(nan_mantissa);
        return nan;
    }

    static constexpr auto exponent_width() -> size_t
    {
        return E;
    }

    static constexpr auto mantissa_width() -> size_t
    {
        return M;
    }

    /**
     * @warn The absolute value of the
     * @return
     */
    [[nodiscard]] static constexpr integer<E + 1, WordType> denorm_exponent()
    {

        const integer<E + 1, WordType> b{bias};
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
        return exponent == uinteger<E>::all_ones() &&
               width_cast<M>(mantissa) == uinteger<M>::zero();
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
        return exponent == IntegerExp::all_ones() && width_cast<M>(mantissa) != uinteger<M>::zero();
    }

    [[nodiscard]] constexpr auto unbiased_exponent() const -> integer<E + 1, WordType>
    {
        const integer<E + 1, WordType> signed_bias{bias};
        const integer<E + 1, WordType> signed_exponent{get_exponent()};

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

    void set_full_mantissa(const uinteger<MW, WordType>& set_to)
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
        return exponent != IntegerExp::all_zeroes();
    }

    /**
     * @brief Returns if the number is denormalized or NaN/Inf
     * @return
     */
    [[nodiscard]] constexpr bool is_special() const
    {
        const bool denormalized = (exponent == IntegerExp::all_zeroes());
        const bool exception = (exponent == IntegerExp::all_ones());
        const bool result = (denormalized || exception);
        return result;
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

        const exp_type in_bias{this->bias};
        const auto out_bias = normalized_float<ES, M, WordType>::bias;
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
