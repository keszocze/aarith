#pragma once

#include <aarith/core.hpp>
#include <aarith/float/float_utils.hpp>
#include <aarith/integer_no_operators.hpp>

#include <bitset>
#include <cassert>
#include <cstdint>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace aarith {

template <size_t E, size_t M, typename WordType = uint64_t> class floating_point; // NOLINT

using half_precision = floating_point<5, 10, uint64_t>;        // NOLINT
using single_precision = floating_point<8, 23, uint64_t>;      // NOLINT
using double_precision = floating_point<11, 52, uint64_t>;     // NOLINT
using quadruple_precision = floating_point<15, 112, uint64_t>; // NOLINT
using bfloat16 = floating_point<8, 7, uint64_t>;               // NOLINT
using tensorfloat32 = floating_point<8, 10, uint64_t>;         // NOLINT

/**
 * @brief Expands the mantissa by correctly shifting the bits in the larger uinteger
 * @tparam MS The target width of the mantissa
 * @return The mantissa expended to a width of MS
 */
template <size_t MS, size_t E, size_t M, typename WordType>
[[nodiscard]] uinteger<MS, WordType> constexpr expand_mantissa(
    const floating_point<E, M, WordType>& f)
{
    static_assert(MS >= M, "Expanded mantissa must not be shorter than the original mantissa");
    uinteger<MS, WordType> mantissa_{f.get_mantissa()};
    mantissa_ <<= (size_t{MS} - size_t{M});
    return mantissa_;
}

/**
 * @brief Creates a bitstring representation of the floating point number.
 *
 * You can use this method to create valid IEEE 754 bitstrings.
 *
 * @tparam E The bit-width of the input's exponent
 * @tparam M The bit-width of the input's mantissa
 * @tparam WordType The word type used to store the actual data
 * @param f The floating-point numbers whose bitstring is to be created
 * @return IEEE-754 bitstring representation of the floating point number
 */
template <size_t E, size_t M, typename WordType>
[[nodiscard]] word_array<1 + E + M, WordType> as_word_array(const floating_point<E, M, WordType>& f)
{
    word_array<E + M> exp_mantissa = concat(f.get_exponent(), f.get_mantissa());
    auto full_float = concat(word_array<1, WordType>{f.get_sign()}, exp_mantissa);
    return full_float;
}

/**
 * @brief Creates a bitstring representation of the floating point number.
 *
 * The bitstring returned may use more bits for the exponent/mantissa than the floating point
 * number it was created from. You can use this method to create valid IEEE 754 bitstrings.
 *
 * @tparam ET The target bit-width for the exponent
 * @tparam MT The target bit-width for the mantissa
 * @tparam E The bit-width of the input's exponent
 * @tparam M The bit-width of the input's mantissa
 * @tparam WordType
 * @param f
 * @return
 */
template <size_t ET, size_t MT, size_t E, size_t M, typename WordType>
[[nodiscard]] constexpr floating_point<ET, MT, WordType>
width_cast(const floating_point<E, M, WordType>& f)
{

    if constexpr (ET == E && MT == M)
    {
        return f;
    }

    using R = floating_point<ET, MT, WordType>;

    static_assert(ET >= E);
    static_assert(MT >= M);

    using m_type = uinteger<MT, WordType>;
    using e_type = uinteger<ET, WordType>;

    m_type mantissa_;
    e_type exponent_;

    if (f.is_denormalized())
    {
        //############### Expand the mantissa
        [[maybe_unused]] size_t shift_amount = 0;
        if constexpr (MT == M)
        {
            mantissa_ = f.get_mantissa();
        }
        else
        {
            auto new_mantissa = f.get_mantissa();
            shift_amount = count_leading_zeroes(new_mantissa) + 1;
            new_mantissa = (new_mantissa << shift_amount);
            mantissa_ = m_type{new_mantissa};
            mantissa_ <<= (size_t{MT} - size_t{M + 1});
        }

        if constexpr (ET == E)
        {
            exponent_ = f.get_exponent();
        }
        else
        {
            using IntegerUnbiasedExp = integer<ET + 1, WordType>;
            IntegerUnbiasedExp exp_f = f.unbiased_exponent();
            exp_f = exp_f - IntegerUnbiasedExp{shift_amount};

            const IntegerUnbiasedExp out_bias = floating_point<ET, M, WordType>::bias;
            IntegerUnbiasedExp biased_exp = exp_f + out_bias;
            exponent_ = width_cast<ET>(biased_exp);
        }
    }
    else
    {

        //############### Expand the mantissa
        if constexpr (M == MT)
        {
            mantissa_ = f.get_mantissa();
        }
        else
        {

            mantissa_ = f.get_mantissa();
            mantissa_ <<= (size_t{MT} - size_t{M});
        }

        //############### Expand the exponent
        if constexpr (E == ET)
        {
            exponent_ = f.get_exponent();
        }
        else
        {
            if (f.is_zero())
            {
                exponent_ = e_type::zero();
            }
            else if (f.get_exponent() == uinteger<E, WordType>::all_ones())
            {
                exponent_ = e_type::all_ones();
            }
            else
            {
                const e_type in_bias{f.bias};
                const auto out_bias = floating_point<ET, M, WordType>::bias;
                auto bias_difference = sub(out_bias, in_bias);
                exponent_ = add(e_type(f.get_exponent()), bias_difference);
            }
        }
    }
    auto exp_mantissa = concat(exponent_, mantissa_);
    auto full_bitstring = concat(uinteger<1, WordType>{f.is_negative()}, exp_mantissa);

    R res(full_bitstring);
    return res;
}

template <size_t E, size_t M, typename WordType> class floating_point
{
public:
    static_assert(M > 0, "Mantissa width has to be greater zero.");
    static_assert(E > 1, "Exponent width has to be greater one.");

    static constexpr size_t MW = M + 1;
    static constexpr size_t Width = M + E + 1;

    using IntegerExp = uinteger<E, WordType>;
    using IntegerUnbiasedExp = integer<E + 1, WordType>;
    using IntegerMant = uinteger<MW, WordType>;
    using IntegerFrac = uinteger<M, WordType>;

    static constexpr IntegerExp bias = uinteger<E - 1, WordType>::all_ones();
    static constexpr IntegerUnbiasedExp max_exp = uinteger<E - 1, WordType>::all_ones();
    static constexpr IntegerUnbiasedExp min_exp = []() {
        const IntegerExp bias_ = IntegerExp{uinteger<E - 1, WordType>::all_ones()};
        IntegerUnbiasedExp min_exp_{bias_};
        IntegerUnbiasedExp one = IntegerUnbiasedExp::one();
        min_exp_ = negate(sub(min_exp_, one));
        return min_exp_;
    }();

    explicit constexpr floating_point()
        : sign_neg(false)
        , exponent(IntegerExp::zero())
        , mantissa(IntegerMant::zero())
    {
    }

    explicit constexpr floating_point(const word_array<1 + E + M>& w)
        : floating_point((w.msb() != 0U), bit_range<(E + M) - 1, M>(w), bit_range<M - 1, 0>(w))
    {
        // the equality check ensures that we have a bool (instead of an uint) so that the compiler
        // knows which constructor to call
    }

    explicit constexpr floating_point(const bool is_neg, IntegerExp exp,
                                      word_array<M, WordType> frac)
        : sign_neg(is_neg)
        , exponent(exp)
        , mantissa((exponent == IntegerExp::zero()) ? IntegerMant{frac}
                                                    : msb_one(IntegerMant{frac}))
    {
        /*
         * How this constructors works:
         * frac has only M bits but IntegerMant stores MW (i.e., M+1) bits. Putting frac in an
         * IntegerMant adds an additional bit to the left that is pre-filled with the value 0. In
         * case of denormalized numbers, this is fine. In the other case, the most significant bit
         * hast to be manually set to 1, as is done by the msb_one function.
         */
    }

    explicit constexpr floating_point(const bool is_neg, IntegerExp exp,
                                      word_array<MW, WordType> mant)
        : sign_neg(is_neg)
        , exponent(exp)
        , mantissa(mant)
    {
    }

    explicit constexpr floating_point(const unsigned int is_neg, IntegerExp exp,
                                      word_array<M, WordType> frac)
        : floating_point((is_neg != 0U), exp, frac)
    {
        // the equality check ensures that we have a bool (instead of an uint) so that the compiler
        // knows which constructor to call
    }

    explicit constexpr floating_point(const unsigned int is_neg, const IntegerExp& exp,
                                      const word_array<MW, WordType>& mant)
        : sign_neg(is_neg) // NOLINT
        , exponent(exp)
        , mantissa(mant)
    {
    }

    constexpr floating_point(const floating_point<E, M, WordType>& f)
        : sign_neg(f.sign_neg)
        , exponent(f.exponent)
        , mantissa(f.mantissa)
    {
    }

    template <size_t E_, size_t M_, typename = std::enable_if_t<(M > M_) && (E > E_)>>
    constexpr explicit floating_point(const floating_point<E_, M_, WordType>& f)
        : floating_point(width_cast<E, M, E_, M_, WordType>(f))
    {
    }

    explicit floating_point(std::string_view other)
    {

        if (other.length() != 1 + E + M)
        {
            throw std::invalid_argument(
                std::string("Length of bit string (") + std::to_string(other.length()) +
                std::string(") does not match the number of bits in the aarith::floating_point (") +
                std::to_string(1 + E + M) + std::string("you are trying to create"));
        }

        switch (other.front())
        {
        case '1': {
            this->sign_neg = true;
            break;
        }
        case '0': {
            this->sign_neg = false;
            break;
        }
        default:
            throw std::invalid_argument(std::string("Unexpected character at sign position: '") +
                                        std::string(other) +
                                        std::string("': expecting '1' and '0' only"));
        }
        try
        {
            this->exponent = aarith::word_array<E, WordType>{other.substr(1, E)};
        }
        catch (const std::invalid_argument& e)
        {
            throw std::invalid_argument(std::string("Unexpected character in exponent: ") +
                                        std ::string(e.what()));
        }

        aarith::word_array<M, WordType> mant;
        try
        {
            mant = aarith::word_array<M, WordType>{other.substr(E + 1, M)};
        }
        catch (const std::invalid_argument& e)
        {
            throw std::invalid_argument(std::string("Unexpected character in mantissa: ") +
                                        std ::string(e.what()));
        }
        {
        }

        if (exponent == IntegerExp::zero())
        {
            this->mantissa = IntegerMant{mant};
        }
        else
        {
            this->mantissa = msb_one(IntegerMant{mant});
        }
    }

    template <typename F, typename = std::enable_if_t<std::is_floating_point<F>::value>>
    explicit floating_point(const F f)
    {
        const float_disassembly value_disassembled = disassemble_float<F>(f);
        constexpr size_t ext_exp_width = get_exponent_width<F>();
        constexpr size_t ext_mant_width = get_mantissa_width<F>();
        uinteger<ext_exp_width, WordType> extracted_exp{value_disassembled.exponent};
        uinteger<ext_mant_width, WordType> extracted_mantissa{value_disassembled.mantissa};

        // does not correctly insert -0
        // sign_neg = (f < 0);
        sign_neg = value_disassembled.is_neg; // NOLINT

        if (f == static_cast<F>(0.))
        {
            return;
        }

        using E_ = decltype(extracted_exp);
        using M_ = decltype(extracted_mantissa);

        sign_neg = std::signbit(f);

        // if the mantissa of the float can store at least as many bit as the
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
            // a width cast that potentially loses quite some accuracy
            // we first move the bits to the right in order not to lose too many bits
            extracted_mantissa >>= (ext_mant_width - M);
            mantissa = width_cast<M>(extracted_mantissa); // cuts off from the left
        }

        if constexpr (ext_exp_width < E)
        {
            // a wider exponent means a larger bias, so we have to add the difference between two
            // biases to the old exponent to get the old value

            // we need to make sure that inf/NaN remains the same
            if (extracted_exp == E_::all_ones())
            {
                exponent = uinteger<E>::all_ones();
            }
            // the other special case is for zero and denormalized numbers: the exponent has to
            // remain zeroes only as well
            // else if (extracted_exp == E_::all_zeroes())
            //{
            //    //then F is denormalized, but may be normalized in a bigger format
            //    exponent = sub(uinteger<E>::all_zeroes();
            //}
            else
            {
                // no special case left -> we can adjust the exponent
                constexpr IntegerExp smaller_bias =
                    uinteger<ext_exp_width - 1, WordType>::all_ones();
                constexpr IntegerExp diff = sub(bias, smaller_bias);

                if (extracted_exp == E_::all_zeroes() && extracted_mantissa != M_::all_zeroes())
                {
                    const auto one_at = first_set_bit(mantissa);
                    if (one_at)
                    {
                        exponent = add(IntegerExp{extracted_exp}, diff);
                        auto shift_by = M - *one_at;
                        if (exponent <= uinteger<sizeof(decltype(shift_by)) * 8>(shift_by))
                        {
                            // shift_by -= 1;
                            mantissa = (mantissa << (exponent.word(0) - 1));
                            exponent = exponent.all_zeroes();
                        }
                        else
                        {
                            mantissa = (mantissa << shift_by);
                            exponent = sub(exponent, uinteger<E, WordType>(shift_by - 1));
                        }
                    }
                    else
                    {
                        exponent = exponent.all_zeroes();
                    }
                }
                else
                {
                    exponent = add(IntegerExp{extracted_exp}, diff);
                }
            }
        }
        else if (ext_exp_width > E)
        {
            // a smaller exponent means a lower bias, so we subtract the difference between the two
            // biases from the original bias
            // in case of over- or underflow we set the value to infinity or 0

            // we need to make sure that inf/NaN remains the same
            if (extracted_exp == E_::all_ones())
            {
                exponent = uinteger<E>::all_ones();
            }
            // the other special case is for zero and denormalized numbers: the exponent has to
            // remain zeroes only as well
            else if (extracted_exp == E_::all_zeroes())
            {
                exponent = uinteger<E>::all_zeroes();
            }
            else
            {
                using OExp = uinteger<ext_exp_width + 1, WordType>;
                constexpr OExp bigger_bias = uinteger<ext_exp_width - 1, WordType>::all_ones();
                constexpr OExp smaller_bias = bias;
                constexpr OExp diff = sub(bigger_bias, smaller_bias);
                const auto exp = sub(OExp(extracted_exp), diff);
                const bool overflow =
                    exp >= OExp(IntegerExp::all_ones()) && exp.bit(ext_exp_width) == 0;
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

    /**
     *
     * @return The value zero
     */
    [[nodiscard]] static constexpr floating_point zero()
    {
        return floating_point{};
    }

    /**
     *
     * @return The value negative zero
     */
    [[nodiscard]] static constexpr floating_point neg_zero()
    {
        return floating_point(true, IntegerExp::all_zeroes(), IntegerMant::all_zeroes());
    }

    /**
     *
     * @return The value one
     */
    [[nodiscard]] static constexpr floating_point one()
    {
        constexpr word_array<E, WordType> exp{word_array<E - 1, WordType>::all_ones()};
        return floating_point(false, exp, IntegerMant::msb_one());
    }

    /**
     *
     * @return The value one
     */
    [[nodiscard]] static constexpr floating_point neg_one()
    {
        constexpr word_array<E, WordType> exp{word_array<E - 1, WordType>::all_ones()};
        return floating_point(true, exp, IntegerMant::msb_one());
    }

    /**
     *
     * @return positive infinity
     */
    [[nodiscard]] static constexpr floating_point pos_infinity()
    {
        constexpr floating_point pos_inf(false, IntegerExp::all_ones(), IntegerMant::all_zeroes());
        return pos_inf;
    }

    /**
     *
     * @return negative infinity
     */
    [[nodiscard]] static constexpr floating_point neg_infinity()
    {
        constexpr floating_point neg_inf(true, IntegerExp::all_ones(), IntegerMant::all_zeroes());
        return neg_inf;
    }

    /**
     *
     * @return The smallest finite value
     */
    [[nodiscard]] static constexpr floating_point min()
    {
        word_array<E, WordType> exp = word_array<E, WordType>::all_ones();
        exp.set_bit(0, false);
        return floating_point(true, exp, IntegerMant::all_ones());
    }

    /**
     *
     * @return The largest finite value
     */
    [[nodiscard]] static constexpr floating_point max()
    {
        word_array<E, WordType> exp = word_array<E, WordType>::all_ones();
        exp.set_bit(0, false);
        return floating_point(false, exp, IntegerMant::all_ones());
    }

    /**
     *
     * @return Smallest positive normalized value
     */
    [[nodiscard]] static constexpr floating_point smallest_normalized()
    {
        constexpr floating_point small_normalized(false, IntegerExp::one(),
                                                  IntegerFrac::all_zeroes());
        return small_normalized;
    }

    /**
     *
     * @return Smallest positive denormalized value
     */
    [[nodiscard]] static constexpr floating_point smallest_denormalized()
    {
        constexpr floating_point small_denorm(false, IntegerExp::all_zeroes(), IntegerMant::one());
        return small_denorm;
    }

    /**
     *
     * @return The maximal rounding error (assuming round-to-nearest)
     */
    [[nodiscard]] static constexpr floating_point round_error()
    {
        IntegerMant m = IntegerMant::all_zeroes();
        m.set_msb(true);

        IntegerExp e = IntegerExp::all_ones();
        e.set_bit(0, false);
        e.set_msb(false);

        const floating_point rounding_error(false, e, m);
        return rounding_error;
    }

    /**
     * @brief Creates a quiet NaN value
     * @param payload The payload to store in the NaN
     * @return The bit representation of the quiet NaN containing the payload
     */
    [[nodiscard]] static constexpr floating_point
    qNaN(const IntegerFrac& payload = IntegerFrac::msb_one())
    {
        IntegerFrac payload_{payload};
        payload_.set_msb(true);
        return floating_point(false, IntegerExp::all_ones(), IntegerMant{payload_});
    }

    /**
     * @brief Creates a signalling NaN value
     * @param payload The payload to store in the NaN (must not be zero)
     * @return The bit representation of the signalling NaN containing the payload
     */
    [[nodiscard]] static constexpr floating_point
    sNaN(const IntegerFrac& payload = IntegerFrac::one())
    {

        IntegerFrac payload_{payload};
        payload_.set_msb(false);
        assert(!payload_.is_zero() && "NaN must have a payload");
        return floating_point(false, IntegerExp::all_ones(), payload_);
    }

    /**
     * @brief Returns a floating point number indicating not a number (NaN).
     * @return A non-signalling not a number value
     */
    [[nodiscard]] static constexpr floating_point NaN()
    {
        return qNaN();
    }

    static constexpr auto exponent_width() -> size_t
    {
        return E;
    }

    static constexpr auto mantissa_width() -> size_t
    {
        return M;
    }

    [[nodiscard]] constexpr auto get_sign() const -> unsigned int
    {
        return (sign_neg) ? 1U : 0U;
    }

    void constexpr set_sign(unsigned int sign)
    {
        sign_neg = (sign & 1U) > 0;
    }

    [[nodiscard]] auto get_exponent() const -> uinteger<E, WordType>
    {
        return exponent;
    }

    /**
     * @brief Tests whether the floating-point number is positive.
     *
     * This returns true for zeros and NaNs as well.
     *
     * @return True iff the sign bit is not set
     */
    [[nodiscard]] constexpr bool is_positive() const
    {
        return !sign_neg;
    }

    /**
     * @brief Tests whether the floating-point number is negative.
     *
     * This returns true for zeros and NaNs as well.
     *
     * @return True iff the sign bit is set
     */
    [[nodiscard]] constexpr bool is_negative() const
    {
        return sign_neg;
    }

    /**
     * @brief Returns whether the number is finite
     *
     * @note NaNs are *not* considered finite
     *
     * @return True iff the number is finite
     */
    [[nodiscard]] constexpr bool is_finite() const
    {
        return (exponent != uinteger<E>::all_ones());
    }

    [[nodiscard]] constexpr bool is_inf() const
    {
        return exponent == uinteger<E>::all_ones() &&
               width_cast<M>(mantissa) == uinteger<M>::zero();
    }

    [[nodiscard]] constexpr bool is_pos_inf() const
    {
        return !sign_neg && exponent == uinteger<E>::all_ones() &&
               width_cast<M>(mantissa) == uinteger<M>::zero();
    }

    [[nodiscard]] constexpr bool is_neg_inf() const
    {
        return sign_neg && exponent == uinteger<E>::all_ones() &&
               width_cast<M>(mantissa) == uinteger<M>::zero();
    }

    /**
     * @brief Checks whether the floating point number is NaN (not a number)
     *
     * @note There is no distinction between signalling and non-signalling NaN
     *
     * @return True iff the number is NaN
     */
    [[nodiscard]] constexpr bool is_nan() const
    {
        const bool exp_ones = exponent == IntegerExp ::all_ones();
        const bool mant_zero = get_mantissa().is_zero();
        return exp_ones && !mant_zero;
    }

    /**
     * @brief Checks if the number is a quiet NaN
     * @return True iff the number is a quiet NaN
     */
    [[nodiscard]] constexpr bool is_qNaN() const
    {
        const bool exp_all_ones = exponent == IntegerExp ::all_ones();
        const bool first_bit_set = width_cast<M>(mantissa).msb();
        return exp_all_ones && first_bit_set;
    }

    /**
     * @brief Checks if the number is a signalling NaN
     * @return True iff the number is a signalling NaN
     */
    [[nodiscard]] constexpr bool is_sNaN() const
    {
        const bool exp_all_ones = exponent == IntegerExp ::all_ones();
        const auto fraction = width_cast<M>(mantissa);
        const bool first_bit_unset = !fraction.msb();
        const bool not_zero = fraction != IntegerFrac::zero();
        return exp_all_ones && first_bit_unset && not_zero;
    }

    [[nodiscard]] constexpr auto unbiased_exponent() const -> integer<E + 1, WordType>
    {
        if (!this->is_normalized())
        {
            return min_exp;
        }

        const IntegerUnbiasedExp signed_bias{bias};
        const IntegerUnbiasedExp signed_exponent{get_exponent()};

        const IntegerUnbiasedExp unbiased_exp = sub(signed_exponent, signed_bias);
        return unbiased_exp;
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

    /**
     * @brief Checks whether the floating point number is positive zero
     *
     *
     * @return True iff the floating point is positive zero
     */
    [[nodiscard]] constexpr bool is_pos_zero() const
    {
        return !sign_neg && exponent.is_zero() && mantissa.is_zero();
    }

    /**
     * @brief Checks whether the floating point number is negative zero
     *
     *
     * @return True iff the floating point is negative zero
     */
    [[nodiscard]] constexpr bool is_neg_zero() const
    {
        return sign_neg && exponent.is_zero() && mantissa.is_zero();
    }

    void set_exponent(const uinteger<E, WordType>& set_to)
    {
        exponent = set_to;
    }

    [[nodiscard]] auto constexpr get_full_mantissa() const -> uinteger<MW, WordType>
    {
        return mantissa;
    }

    [[nodiscard]] auto constexpr get_mantissa() const -> uinteger<M, WordType>
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
        if (exponent != IntegerExp::all_zeroes())
        {
            mantissa.set_msb(true);
        }
    }

    [[nodiscard]] auto bit(size_t index) const -> typename uinteger<M, WordType>::bit_type
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
     * @brief Checks whether the number is normal
     *
     * This is true if and only if the floating-point number  is normal (not zero, subnormal,
     * infinite, or NaN).
     *
     * @return True iff the number is normalized
     */
    [[nodiscard]] constexpr bool is_normalized() const
    {
        const bool denormalized = (exponent == IntegerExp::all_zeroes());
        const bool exception = (exponent == IntegerExp::all_ones());
        const bool result = !denormalized && !exception;
        return result;
    }

    /**
     * @brief Returns whether the number is denormalized
     *
     * @note Denormalized numbers do *not* include: NaN, +/- inf and, surprisingly, zero.
     *
     * @return True iff the number is denormalized
     */
    [[nodiscard]] constexpr bool is_denormalized() const
    {
        const bool denormalized = (exponent == IntegerExp::all_zeroes());
        const bool exception = (exponent == IntegerExp::all_ones());
        const bool rest_is_null = (mantissa == IntegerMant::all_zeroes());
        const bool result = denormalized && !exception && !rest_is_null;
        return result;
    }

    /**
     * @brief Tests if the number is subnormal
     *
     * @note Zero is *not* considered subnormal!
     *
     * @return True iff the number is subnormal
     */
    [[nodiscard]] constexpr bool is_subnormal() const
    {
        const bool denormalized = exponent.is_zero();
        const bool not_zero = !mantissa.is_zero();
        const bool result = denormalized && not_zero;
        return result;
    }

    /**
     * @brief Returns whether the number is denormalized or NaN/Inf
     * @return True iff the number is denornmalized, infinite or a NaN
     */
    [[nodiscard]] constexpr bool is_special() const
    {
        return !is_normalized();
    }

    /**
     * @brief Casts the normalized float to the native float type.
     *
     * @note The cast is only possible when there will be no loss of precision
     *
     * @return The value converted to float format
     */
    [[nodiscard]] explicit constexpr operator float() const
    {
        return generic_cast<float>();
    }

    /**
     * @brief Casts the normalized float to the native double type.
     *
     * @note The cast is only possible when there will be no loss of precision
     *
     * @return The value converted to double format
     */
    [[nodiscard]] explicit constexpr operator double() const
    {
        return generic_cast<double>();
    }

    template <size_t ETarget, size_t MTarget>
    [[nodiscard]] explicit constexpr operator floating_point<ETarget, MTarget, WordType>() const
    {
        return width_cast<ETarget, MTarget>(*this);
    }

    floating_point<E, M, WordType>& operator=(const floating_point<E, M, WordType>& f)
    {
        if (this == &f)
        {
            return *this;
        }
        this->sign_neg = f.sign_neg;
        this->exponent = f.exponent;
        this->mantissa = f.mantissa;
        return *this;
    }

    [[nodiscard]] floating_point<E, M> make_quiet_nan() const
    {
        auto nan_mantissa = mantissa;
        nan_mantissa.set_bit(M - 1, static_cast<WordType>(1U));
        const auto nan_exponent = uinteger<E>::all_ones();
        floating_point<E, M> nan{sign_neg, nan_exponent, nan_mantissa};
        return nan;
    }

private:
    /**
     * @brief Casts the number to float or double.
     *
     * @note The cast is only possible when there will be no loss of precision.
     *
     * @tparam To Either float or double
     * @return Float/Double representation of the number
     */
    template <typename To> [[nodiscard]] constexpr To generic_cast() const
    {

        static_assert(std::is_floating_point<To>(), "Can only convert to float or double.");

        using uint_storage = typename float_extraction_helper::bit_cast_to_type_trait<To>::type;
        constexpr auto exp_width = get_exponent_width<To>();
        constexpr auto mant_width = get_mantissa_width<To>();

        static_assert(E <= exp_width, "Exponent width too large");
        static_assert(M <= mant_width, "Mantissa width too large");

        auto resized = width_cast<exp_width, mant_width, E, M, WordType>(*this);
        auto as_array = as_word_array(resized);
        uinteger<1 + exp_width + mant_width, WordType> array{as_array};

        uint_storage bitstring = static_cast<uint_storage>(array);
        To result = bit_cast<To>(bitstring);
        return result;
    }

    bool sign_neg{false};
    uinteger<E, WordType> exponent;
    uinteger<MW, WordType> mantissa;
};

template <size_t E, size_t M, typename WordType> class is_integral<floating_point<E, M, WordType>>
{
public:
    static constexpr bool value = false;
};

template <size_t E, size_t M, typename WordType> class is_float<floating_point<E, M, WordType>>
{
public:
    static constexpr bool value = true;
};

template <size_t E, size_t M, typename WordType> class is_unsigned<floating_point<E, M, WordType>>
{
public:
    static constexpr bool value = false;
};

template <size_t E, size_t M1, size_t M2, typename WordType = uint64_t>
auto bit_equal(const floating_point<E, M1, WordType> lhs, const floating_point<E, M2, WordType> rhs)
    -> bool
{
    return lhs.get_sign() == rhs.get_sign() && lhs.get_exponent() == rhs.get_exponent() &&
           lhs.get_mantissa() == rhs.get_mantissa();
}

/**
 * @brief Checks if two floating-point numbers are equal except for differences stemming from
 * different rounding modes.
 *
 * @note Two NaNs are compared to be equal by this method!
 *
 * @tparam E
 * @tparam M1
 * @tparam M2
 * @tparam WordType
 * @param lhs
 * @param rhs
 * @return
 */
template <size_t E, size_t M1, size_t M2, typename WordType = uint64_t>
auto equal_except_rounding(const floating_point<E, M1, WordType> lhs,
                           const floating_point<E, M2, WordType> rhs) -> bool
{

    if (lhs.is_nan() || rhs.is_nan() || (lhs.get_sign() != rhs.get_sign()) ||
        (lhs.get_exponent() != rhs.get_exponent()))
    {
        return false;
    }

    if (lhs.get_full_mantissa() == rhs.get_full_mantissa())
    {
        return true;
    }

    constexpr auto Min = std::min(M1, M2);
    constexpr auto offset_M1 = M1 - Min;
    constexpr auto offset_M2 = M2 - Min;

    const auto m1 = lhs.get_full_mantissa();
    const auto m2 = rhs.get_full_mantissa();

    auto bit1 = m1.bit(offset_M1);
    auto bit2 = m2.bit(offset_M2);

    bool rounding_error = true;
    bool has_to_be_equal = false; // bit1 == bit2;
    bool initial_zeroes = true;
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
            if (initial_zeroes && m1.bit(i + offset_M1) == 0 && m2.bit(i + offset_M2) == 0)
            {
                continue;
            }
            else if (m1.bit(i + offset_M1) != m2.bit(i + offset_M2))
            {
                if (initial_zeroes)
                {
                    bit1 = m1.bit(i + offset_M1);
                    bit2 = m2.bit(i + offset_M2);
                    initial_zeroes = false;
                }
                if (m1.bit(i + offset_M1) == bit1 && m2.bit(i + offset_M2) == bit2)
                {
                    continue;
                }
                else
                {
                    has_to_be_equal = true;
                }
            }
            else if (i > 0)
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

/**
 * @brief Computes the absolute value of the floating point number
 *
 * Quoting the standard: "copies a floating-point operand x to a destination in the same format,
 * setting the sign bit to 0 (positive)"
 *
 * @note This method ignores NaN values in the sense that they are also copied and the sign bit set
 * to zero.
 *
 * @tparam E Width of exponent
 * @tparam M Width of mantissa
 * @tparam WordType The word type used to internally store the data
 * @return The absolute value of the provided number
 */
template <size_t E, size_t M, typename WordType = uint64_t>
auto constexpr abs(const floating_point<E, M, WordType> nf) -> floating_point<E, M, WordType>
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
auto normalize(const floating_point<E, M1, WordType>& num) -> floating_point<E, M2, WordType>
{
    auto exponent = width_cast<E + 1>(num.get_exponent());
    auto mantissa = num.get_full_mantissa();

    const auto one_at = first_set_bit(mantissa);

    // i.e. there was no one in the mantissa at all
    if (!one_at)
    {
        // if mantissa is zero
        exponent = exponent.all_zeroes();
    }
    else if (*one_at >= M2)
    {
        auto shift_by = *one_at - M2;
        mantissa = rshift_and_round(mantissa, shift_by);
        if (exponent == exponent.all_zeroes())
        {
            exponent = add(exponent, uinteger<E + 1, WordType>(shift_by + 1));
        }
        else
        {
            exponent = add(exponent, uinteger<E + 1, WordType>(shift_by));
        }
    }
    else
    {
        auto shift_by = M2 - *one_at;
        if (exponent != exponent.all_zeroes())
        {
            if (exponent <= uinteger<E + 1>(shift_by))
            {
                // shift_by -= 1;
                mantissa = (mantissa << (exponent.word(0) - 1));
                exponent = exponent.all_zeroes();
            }
            else
            {
                mantissa = (mantissa << shift_by);
                exponent = sub(exponent, uinteger<E + 1, WordType>(shift_by));
            }
        }
    }

    floating_point<E, M2, WordType> normalized(num.get_sign(), width_cast<E>(exponent),
                                               width_cast<M2 + 1>(mantissa));

    if (normalized.is_nan() || exponent.bit(E) == 1)
    {
        auto inf = floating_point<E, M2, WordType>::pos_infinity();
        inf.set_sign(normalized.get_sign());
        return inf;
    }

    return normalized;
}

// ironically, defining the functions below makes the implementation conform more to the standard

/**
 * @brief Indicates whether this floating-point implementation conforms to the IEEE 754 (1985)
 * standard
 * @return false
 */
constexpr bool is754version1985()
{
    return false;
}

/**
 * @brief Indicates whether this floating-point implementation conforms to the IEEE 754 (2008)
 * standard
 * @return false
 */
constexpr bool is754version2008()
{
    return false;
}

/**
 * @brief Indicates whether this floating-point implementation conforms to the IEEE 754 (2019)
 * standard
 * @return false
 */
constexpr bool is754version2019()
{
    return false;
}

} // namespace aarith
