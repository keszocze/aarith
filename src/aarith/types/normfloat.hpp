#pragma once

#include <aarith/utilities/bit_operations.hpp>
#include <aarith/utilities/float_utils.hpp>
#include <aarith/types/integer.hpp>
#include <aarith/types/traits.hpp>
#include <cstdint>
#include <iostream>
#include <type_traits>
#include <string>


namespace aarith {

template <size_t E, size_t M>
class normfloat
{
public:
    static_assert(M > 0, "Mantissa width has to be greater zero.");
    static_assert(E > 1, "Exponent width has to be greater one.");
    
    template<class F>
    explicit normfloat(F f)
    {
        static_assert(!std::is_integral<F>::value, "Only floating point numbers are supported");
        //static_assert(get_mantissa_width<F>() < M, "Mantissa of f does not fit into explicit mantissa of normfloat.");

        auto f_exponent = extract_exponent(f);
        auto f_mantissa = extract_mantissa(f);
        constexpr auto f_exponent_width = get_exponent_width<F>();
        constexpr auto f_mantissa_width = get_mantissa_width<F>() + 1;

        sign_neg = extract_sign(f) == 1;

        constexpr auto wider_mantissa = M >= f_mantissa_width;
        if(wider_mantissa)
        {
            mantissa = uinteger<M>(f_mantissa);
            const auto mantissa_shift = M - f_mantissa_width;
            mantissa = mantissa << (mantissa_shift);
        }
        else
        {
            const auto mantissa_shift = f_mantissa_width - M;
            f_mantissa >>= mantissa_shift;
            mantissa = uinteger<M>(f_mantissa);
        }

        constexpr auto wider_exponent = E >= f_exponent_width;
        if(wider_exponent)
        {
            exponent = uinteger<E>(f_exponent);
            const uinteger<E> f_bias((1U << (f_exponent_width-1))-1UL);
            const auto exponent_delta = get_bias() - f_bias;
            exponent = exponent + exponent_delta;

        }
        else
        {
            const auto bias = get_bias();
            const auto exponent_delta = ((1U << (f_exponent_width-1))-1U) - bias.word(0);
            f_exponent -= exponent_delta;
            exponent = uinteger<E>(f_exponent);
        }
    }

    static constexpr auto exponent_width()
    -> size_t
    {
        return E;
    }

    static constexpr auto mantissa_width()
    -> size_t
    {
        return M;
    }

    static constexpr auto get_bias()
    -> uinteger<E>
    {
        const uinteger<E> one(1U);
        const uinteger<E> shifted = one << (E - 1);
        return shifted - one;
    }

    auto get_sign() const
    -> unsigned
    {
        return (sign_neg)?1U:0U;
    }

    void set_sign(uint8_t sign)
    {
        sign_neg = sign > 0;
    }

    auto get_exponent() const
    -> uinteger<E>
    {
        return exponent;
    }

    void set_exponent(const uinteger<E> &set_to)
    {
        exponent = set_to;
    }

    auto get_mantissa() const
    -> uinteger<M>
    {
        return mantissa;
    }

    void set_mantissa(const uinteger<M> &set_to)
    {
        mantissa = set_to;
    }

    auto bit(size_t index) const
    -> typename uinteger<M>::bit_type
    {
        if(index < M)
        {
            return mantissa.bit(index);
        }
        else if(index < M + E)
        {
            return exponent.bit(index - M);
        }
        else
        {
            return static_cast<typename uinteger<M>::bit_type>(get_sign());
        }
    }

    auto is_normalized() const
    -> bool
    {
        return bit(M) == 1;
    }

private:
    bool sign_neg;
    uinteger<E> exponent;
    uinteger<M> mantissa;

};

template <size_t E, size_t M> class is_integral<normfloat<E, M>>
{
public:
    static constexpr bool value = false;
};

template <size_t E, size_t M> class is_float<normfloat<E, M>>
{
public:
    static constexpr bool value = true;
};

template <size_t E, size_t M> class is_unsigned<normfloat<E, M>>
{
public:
    static constexpr bool value = false;
};

template<size_t E, size_t M>
auto operator<(const normfloat<E, M> lhs, const normfloat<E, M> rhs)
-> bool
{
    if(lhs.get_sign() > rhs.get_sign())
    {
        return true;
    }
    else if(lhs.get_exponent() == rhs.get_exponent())
    {
        if(lhs.get_mantissa() == rhs.get_mantissa())
        {
            return false;
        } 
        else if(lhs.get_mantissa() < rhs.get_mantissa())
        {
            return lhs.get_sign() == 0;
        }
        else
        {
            return lhs.get_sign() == 1;
        }
    }
    else if(lhs.get_exponent() < rhs.get_exponent())
    {
        return lhs.get_sign() == 0;
    }
    else
    {
        return lhs.get_sign() == 1;
    }
}

template<size_t E, size_t M>
auto operator==(const normfloat<E, M> lhs, const normfloat<E, M> rhs)
-> bool
{
    return lhs.get_sign() == rhs.get_sign() && lhs.get_exponent() == rhs.get_exponent() && lhs.get_mantissa == rhs.get_mantissa();
}

template<size_t E, size_t M>
auto operator>(const normfloat<E, M> lhs, const normfloat<E, M> rhs)
-> bool
{
    return rhs < lhs;
}

template<size_t e, size_t m>
auto operator>=(const normfloat<e, m> lhs, const normfloat<e, m> rhs)
-> bool
{
    return rhs < lhs || lhs == rhs;
}

template<size_t e, size_t m>
auto operator<=(const normfloat<e, m> lhs, const normfloat<e, m> rhs)
-> bool
{
    return lhs < rhs || lhs == rhs;
}


template<size_t E, size_t M>
auto abs(const normfloat<E, M> nf)
-> normfloat<E, M>
{
    auto absolute = nf;
    absolute.set_sign(0U);

    return absolute;
}

template<size_t E, size_t M>
auto normalize(const normfloat<E, M> &nf)
-> normfloat<E, M>
{
    auto normalized = nf;

    auto exponent = normalized.get_exponent();
    auto mantissa = normalized.get_mantissa();

    auto shift_by = find_leading_one(mantissa);

    mantissa <<= shift_by;
    exponent += uinteger<E>(shift_by);

    normalized.set_exponent(exponent);
    normalized.set_mantissa(mantissa);

    return normalized;
}

template<class uint>
auto find_leading_one(const uint mantissa)
-> typename uint::word_type
{
    static_assert(is_integral<uint>::value);
    static_assert(is_unsigned<uint>::value);

    const auto width = uint::width();
    auto zero_at = width;
    
    for(auto i = uint::word_count(); i > 0; --i)
    {
        if(mantissa.word(i-1) == 0)
        {
            if(i == uint::word_count())
            {
                auto const modulo = width % (sizeof(typename uint::word_type)*8);
                zero_at -= modulo == 0 ? sizeof(typename uint::word_type)*8 : modulo;
            }
            else
            {
                zero_at -= sizeof(typename uint::word_type)*8;
            }
        }
        else
        {
            break;
        }
    }

    while(zero_at > 0)
    {
        --zero_at;
        if(mantissa.bit(zero_at) == 1)
        {
            break;
        }
    }

    std::cout << "mantissa: " << mantissa << std::endl;
    std::cout << "zero at: " << zero_at << std::endl;
    
    return zero_at;
}

} // namespace aarith
