#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/float/float_utils.hpp>
#include <aarith/integer/uinteger.hpp>
#include <cstdint>
#include <iostream>
#include <string>
#include <time.h>
#include <type_traits>

namespace aarith {

template <size_t E, size_t M> class normalized_float
{
public:
    static_assert(M > 0, "Mantissa width has to be greater zero.");
    static_assert(E > 1, "Exponent width has to be greater one.");

    explicit normalized_float()
    {
        exponent = uinteger<E>(0U);
        mantissa = uinteger<M>(0U);
        sign_neg = false;
    }

    template <class F> explicit normalized_float(F f)
    {
        static_assert(std::is_floating_point<F>::value,
                      "Only floating point numbers are supported");
        // static_assert(get_mantissa_width<F>() < M, "Mantissa of f does not fit into explicit
        // mantissa of normalized_float.");

        auto f_exponent = extract_exponent(f);
        auto f_mantissa = extract_mantissa(f);
        constexpr auto f_exponent_width = get_exponent_width<F>();
        constexpr auto f_mantissa_width = get_mantissa_width<F>() + 1;

        sign_neg = extract_sign(f) == 1;

        constexpr auto wider_mantissa = M >= f_mantissa_width;
        if (wider_mantissa)
        {
            mantissa = uinteger<M>(f_mantissa);
            constexpr auto mantissa_shift = M - f_mantissa_width;
            mantissa = mantissa << mantissa_shift;
        }
        else
        {
            constexpr auto mantissa_shift = f_mantissa_width - M;
            f_mantissa >>= mantissa_shift;
            mantissa = uinteger<M>(f_mantissa);
        }

        if (f_exponent > 0)
        {
            constexpr auto wider_exponent = E >= f_exponent_width;
            if (wider_exponent)
            {
                exponent = uinteger<E>(f_exponent);
                const auto f_bias =
                    width_cast<E>(normalized_float<f_exponent_width, f_mantissa_width>::get_bias());
                const auto exponent_delta = sub(get_bias(), f_bias);
                exponent = add(exponent, exponent_delta);
            }
            else
            {
                const auto bias = get_bias();
                const auto exponent_delta =
                    normalized_float<f_exponent_width, f_mantissa_width>::get_bias().word(0) -
                    bias.word(0);
                f_exponent -= static_cast<decltype(f_exponent)>(exponent_delta);
                exponent = uinteger<E>(f_exponent);
            }
        }
        else
        {
            mantissa.set_bit(M - 1, false);
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

    static constexpr auto get_bias() -> uinteger<E>
    {
        const uinteger<E> one(1U);
        const uinteger<E> shifted = one << (E - 1);
        return sub(shifted, one);
    }

    auto get_sign() const -> unsigned int
    {
        return (sign_neg) ? 1U : 0U;
    }

    void set_sign(unsigned int sign)
    {
        sign_neg = (sign & 1U) > 0;
    }

    auto get_exponent() const -> uinteger<E>
    {
        return exponent;
    }

    void set_exponent(const uinteger<E>& set_to)
    {
        exponent = set_to;
    }

    auto get_mantissa() const -> uinteger<M>
    {
        return mantissa;
    }

    void set_mantissa(const uinteger<M>& set_to)
    {
        mantissa = set_to;
    }

    auto bit(size_t index) const -> typename uinteger<M>::bit_type
    {
        if (index < M)
        {
            return mantissa.bit(index);
        }
        else if (index < M + E)
        {
            return exponent.bit(index - M);
        }
        else
        {
            return static_cast<typename uinteger<M>::bit_type>(get_sign());
        }
    }

    auto is_normalized() const -> bool
    {
        return bit(M) == 1;
    }

private:
    bool sign_neg;
    uinteger<E> exponent;
    uinteger<M> mantissa;
};

template <size_t E, size_t M> class is_integral<normalized_float<E, M>>
{
public:
    static constexpr bool value = false;
};

template <size_t E, size_t M> class is_float<normalized_float<E, M>>
{
public:
    static constexpr bool value = true;
};

template <size_t E, size_t M> class is_unsigned<normalized_float<E, M>>
{
public:
    static constexpr bool value = false;
};

template <size_t E, size_t M1, size_t M2>
auto equal_except_rounding(const normalized_float<E, M1> lhs, const normalized_float<E, M2> rhs)
    -> bool
{
    if (lhs.get_sign() == rhs.get_sign() && lhs.get_exponent() == rhs.get_exponent())
    {
        if (lhs.get_mantissa() == rhs.get_mantissa())
        {
            return true;
        }
        else
        {
            constexpr auto Min = std::min(M1, M2);
            constexpr auto offset_M1 = M1 - Min;
            constexpr auto offset_M2 = M2 - Min;

            const auto m1 = lhs.get_mantissa();
            const auto m2 = rhs.get_mantissa();

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

template <size_t E, size_t M> auto abs(const normalized_float<E, M> nf) -> normalized_float<E, M>
{
    auto absolute = nf;
    absolute.set_sign(0U);

    return absolute;
}

template <size_t M>
auto rshift_and_round(const uinteger<M>& m, const size_t shift_by) -> uinteger<M>
{
    if (shift_by == 0)
    {
        return m;
    }

    if (shift_by > M)
    {
        return uinteger<M>(0U);
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
            srand(static_cast<unsigned int>(time(NULL)));
            round = rand() % 2;
        }
        else
        {
            round = 1U;
        }
    }

    return add((m >> shift_by), uinteger<M>(round));
}

template <size_t E, size_t M1, size_t M2 = M1>
auto normalize(const normalized_float<E, M1>& nf) -> normalized_float<E, M2>
{
    auto denormalized = nf;

    auto exponent = denormalized.get_exponent();
    auto mantissa = denormalized.get_mantissa();

    auto one_at = find_leading_one(mantissa);

    if (one_at == M1)
    {
        exponent = uinteger<E>(0U);
        denormalized.set_sign(0);
    }
    else if (one_at >= M2)
    {
        auto shift_by = one_at + 1 - M2;
        mantissa = rshift_and_round(mantissa, shift_by);
        exponent = add(exponent, uinteger<E>(shift_by));
    }
    else
    {
        auto shift_by = M2 - one_at - 1;
        mantissa <<= shift_by;
        exponent = sub(exponent, uinteger<E>(shift_by));
    }

    normalized_float<E, M2> normalized_float;

    normalized_float.set_sign(denormalized.get_sign());
    normalized_float.set_exponent(exponent);
    normalized_float.set_mantissa(width_cast<M2>(mantissa));

    return normalized_float;
}

template <class uint> auto find_leading_one(const uint mantissa) -> typename uint::word_type
{
    static_assert(is_integral<uint>::value);
    static_assert(is_unsigned<uint>::value);

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
