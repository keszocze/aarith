#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/core/word_array.hpp>
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace aarith {

template <size_t Width, class WordType = uint64_t>
class uinteger : public word_array<Width, WordType>
{
public:
    constexpr uinteger() = default;

    constexpr explicit uinteger(WordType n)
        : word_array<Width, WordType>(n)
    {
    }

    // Constructor for unsigned ints that are *not* the WordType
    template <typename Val, typename = std::enable_if_t<::aarith::is_unsigned_int<Val> &&
                                                        (sizeof(Val) * 8) <= Width>>
    constexpr explicit uinteger(Val n)
        : word_array<Width, WordType>(0U)
    {

        constexpr size_t size_of_val = sizeof(Val) * 8;
        constexpr size_t word_size = uinteger<Width, WordType>::word_width();

        if constexpr (size_of_val <= word_size)
        {
            this->set_word(0, n);
        }
        else
        {
            size_t word_index = 0;
            while (n != 0)
            {
                WordType part_n = static_cast<WordType>(n);
                this->set_word(word_index, part_n);
                n = n >> word_size;
                ++word_index;
            }
        }
    }

    template <class... Args>
    constexpr uinteger(WordType fst, Args... args)
        : word_array<Width, WordType>(fst, args...)
    {
    }

    template <size_t V>
    constexpr uinteger<Width, WordType>(const uinteger<V, WordType>& other)
        : word_array<Width, WordType>(width_cast<Width, V, WordType>(other))
    {
    }

    template <size_t V>
    constexpr uinteger<Width, WordType>(const word_array<V, WordType>& other)
        : word_array<Width, WordType>(other)
    {
    }

    template <class... Args> static constexpr auto from_words(Args... args) -> uinteger
    {
        uinteger n;
        n.set_words(args...);
        return n;
    }

    /*
     * Constants
     */

    [[nodiscard]] static constexpr uinteger min()
    {
        return uinteger::all_zeroes();
    }

    [[nodiscard]] static constexpr uinteger max()
    {
        return uinteger::all_ones();
    }

    [[nodiscard]] static constexpr uinteger zero()
    {
        return uinteger::all_zeroes();
    }

    [[nodiscard]] static constexpr uinteger one()
    {
        uinteger n = uinteger::all_zeroes();
        n.set_bit(0);
        return n;
    }

    /**
     * @brief Returns whether the number is negative.
     * @return Always returns false
     */
    [[nodiscard]] bool constexpr is_negative() const
    {
        return false;
    }

    /*
     * Conversion operators
     */

private:
    template <typename T> constexpr T generic_cast() const
    {
        if constexpr (sizeof(T) <= sizeof(WordType))
        {
            // the last word is sufficient to fill the desired target type, so we can simply
            // make a call to the static_cast operation
            return static_cast<T>(this->word(0));
        }
        else
        {
            constexpr size_t words_per_type = (sizeof(T)) / sizeof(WordType);

            constexpr size_t use_words =
                std::min(words_per_type, uinteger<Width, WordType>::word_count());

            uint16_t result = 0;

            for (size_t i = 0; i < use_words; ++i)
            {
                result += this->word(i) << (i * uinteger<Width, WordType>::word_width());
            }

            return result;
        }
    }

public:
    /**
     * @brief Converts to an uint8_t
     *
     * Note that there will be a possible loss of precision as this method simply cuts
     * of the "overflowing" bits.
     *
     * @return An uint8_t storing the value of this uinteger
     */
    explicit constexpr operator uint8_t() const
    {
        // we can safely return this as the smallest WordType is uint8_t
        return static_cast<uint8_t>(this->word(0));
    }

    /**
     * @brief Converts to an uint16_t
     *
     * Note that there will be a possible loss of precision as this method simply cuts
     * of the "overflowing" bits.
     *
     * @return An uint16_t storing the value of this uinteger
     */
    explicit constexpr operator uint16_t() const
    {
        return generic_cast<uint16_t>();
    }

    /**
     * @brief Converts to an uint32_t
     *
     * Note that there will be a possible loss of precision as this method simply cuts
     * of the "overflowing" bits.
     *
     * @return An uint32_t storing the value of this uinteger
     */
    explicit constexpr operator uint32_t() const
    {
        return generic_cast<uint32_t>();
    }

    /**
     * @brief Converts to an uint64_t
     *
     * Note that there will be a possible loss of precision as this method simply cuts
     * of the "overflowing" bits.
     *
     * @return An uint64_t storing the value of this uinteger
     */
    explicit constexpr operator uint64_t() const
    {
        return generic_cast<uint64_t>();
    }
};

template <size_t DestinationWidth, size_t SourceWidth, typename WordType>
[[nodiscard]] auto constexpr width_cast(const uinteger<SourceWidth, WordType>& source)
    -> uinteger<DestinationWidth, WordType>
{
    if constexpr (DestinationWidth == SourceWidth)
    {
        return source;
    }
    else
    {
        word_array<DestinationWidth, WordType> result =
            width_cast<DestinationWidth, SourceWidth, WordType>(
                static_cast<word_array<SourceWidth, WordType>>(source));
        return uinteger<DestinationWidth, WordType>{result};
    }
}
/*
 * Traits
 */
template <size_t Width, typename WordType> class is_integral<uinteger<Width, WordType>>
{
public:
    static constexpr bool value = true;
};

template <size_t Width, typename WordType> class is_unsigned<uinteger<Width, WordType>>
{
public:
    static constexpr bool value = true;
};

template <size_t Width, typename WordType> class is_word_array<uinteger<Width, WordType>>
{
public:
    static constexpr bool value = true;
};

template <size_t Width, class WordType = uint64_t>
class integer : public word_array<Width, WordType>
{
public:
    constexpr integer() = default;

    explicit constexpr integer(WordType n)
        : word_array<Width, WordType>(n)
    {
    }

    template <typename T, typename = std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>>>
    explicit constexpr integer(T t)
        : word_array<Width, WordType>(static_cast<WordType>(t))
    {
        if (t < 0)
        {
            auto const ones = static_cast<WordType>(-1);
            for (size_t i = 1; i < integer<Width>::word_count(); i++)
            {
                this->set_word(i, ones);
            }
        }
    }

    template <typename T, class... Args,
              typename = std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>>>
    explicit constexpr integer(T t, Args... args)
        : word_array<Width, WordType>(static_cast<WordType>(t), args...)
    {
    }

    template <class... Args>
    constexpr integer(WordType fst, Args... args)
        : word_array<Width, WordType>(fst, args...)
    {
    }

    template <size_t V>
    constexpr integer<Width, WordType>(const integer<V, WordType>& other)
        : word_array<Width, WordType>(width_cast<Width, V, WordType>(other))
    {
    }

    template <size_t V>
    constexpr integer<Width, WordType>(const word_array<V, WordType>& other)
        : word_array<Width, WordType>(width_cast<Width, V, WordType>(other))
    {
    }

    [[nodiscard]] static constexpr integer min()
    {
        integer min;
        min.set_bit(Width - 1, true);
        return min;
    }

    [[nodiscard]] static constexpr integer max()
    {
        integer max = integer::all_ones();
        max.set_bit(Width - 1, false);
        return max;
    }

    [[nodiscard]] static constexpr integer zero()
    {
        integer zero = integer::all_zeroes();
        return zero;
    }

    [[nodiscard]] static constexpr integer one()
    {
        integer one(1);
        return one;
    }

    /**
     * @brief Returns minus one
     * @return minus one
     */
    [[nodiscard]] static constexpr integer minus_one()
    {
        integer minus_one = integer::all_ones();
        return minus_one;
    }

    /**
     * @brief Returns whether the number is negative.
     * @return Whether the number is negative
     */
    [[nodiscard]] constexpr bool is_negative() const
    {
        return (this->msb() == 1);
    }
};

template <size_t Width, typename WordType> class is_integral<integer<Width, WordType>>
{
public:
    static constexpr bool value = true;
};

template <size_t Width, typename WordType> class is_unsigned<integer<Width, WordType>>
{
public:
    static constexpr bool value = false;
};

template <size_t Width, typename WordType> class is_word_array<integer<Width, WordType>>
{
public:
    static constexpr bool value = true;
};

/**
 *
 * @brief Expands/shrinks the bit width of the integer
 *
 * The value of the integer remains unchanged if the bit width is increased.
 *
 * @note Reducing the bit width performs a hard truncation. This means that the sign of the integer
 * might change as a result of this operation. This might be surprising in some situations.
 *
 * @tparam DestinationWidth The width to which the input is expanded/shrunk
 * @tparam SourceWidth The input width of the integer
 * @param source The integer that whose width is changed
 * @return integer with specified bit width
 */
template <size_t DestinationWidth, size_t SourceWidth>
[[nodiscard]] constexpr auto width_cast(const integer<SourceWidth>& source)
    -> integer<DestinationWidth>
{
    word_array<DestinationWidth> result =
        width_cast<DestinationWidth, SourceWidth>(static_cast<word_array<SourceWidth>>(source));
    if constexpr (DestinationWidth > SourceWidth)
    {
        const bool is_negative = source.is_negative();

        // TODO find a quicker way to correctly expand the width
        if (is_negative)
        {
            for (size_t i = SourceWidth; i < DestinationWidth; ++i)
            {
                result.set_bit(i);
            }
        }
        return integer<DestinationWidth>{result};
    }
    else
    {
        return integer<DestinationWidth>{result};
    }
}

} // namespace aarith

#include <aarith/core/number_utils.hpp>

// We are only allowed to extend std with specializations
// https://en.cppreference.com/w/cpp/language/extending_std
template <size_t W, typename WordType> class std::numeric_limits<::aarith::uinteger<W, WordType>>
{
public:
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = false;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;
    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    static constexpr bool is_bounded = true;
    static constexpr std::float_denorm_style has_denorm = std::denorm_absent;
    static constexpr bool has_denorm_loss = false;

    // TODO do we need to take that into account somewhere?
    static constexpr std::float_round_style round_style = std::round_toward_zero;
    static constexpr bool is_iec559 = false;
    static constexpr bool is_modulo = true;
    static constexpr int radix = 2;
    static constexpr int digits = W; // TODO what happens if W > max_int?
    static constexpr int digits10 =
        ::aarith::ceil<int>(std::numeric_limits<::aarith::uinteger<W>>::digits *
                            ::aarith::log<10, 2>()) -
        1;

    // weird decision but https://en.cppreference.com/w/cpp/types/numeric_limits/max_digits10 says
    // so
    static constexpr int max_digits10 = 0;

    static constexpr int min_exponent = 0;
    static constexpr int min_exponent10 = 0;
    static constexpr int max_exponent = 0;
    static constexpr int max_exponent10 = 0;

    // Is this value set how it is supposed to? At least division by zero throws an exception, so
    // this value is, most likely, correct.
    static constexpr bool traps = true;

    static constexpr bool tinyness_before = false;

    static constexpr ::aarith::uinteger<W, WordType> min() noexcept
    {
        return ::aarith::uinteger<W, WordType>::zero();
    }

    static constexpr ::aarith::uinteger<W, WordType> lowest() noexcept
    {
        return ::aarith::uinteger<W, WordType>::zero();
    }

    static constexpr ::aarith::uinteger<W, WordType> max() noexcept
    {
        return ::aarith::uinteger<W, WordType>::max();
    }

    static constexpr ::aarith::uinteger<W, WordType> epsilon() noexcept
    {
        return ::aarith::uinteger<W, WordType>::zero();
    }

    static constexpr ::aarith::uinteger<W, WordType> round_error() noexcept
    {
        return ::aarith::uinteger<W, WordType>::zero();
    }

    static constexpr ::aarith::uinteger<W, WordType> infinity() noexcept
    {
        return ::aarith::uinteger<W, WordType>::zero();
    }

    static constexpr ::aarith::uinteger<W, WordType> quiet_NaN() noexcept
    {
        return ::aarith::uinteger<W, WordType>::zero();
    }

    static constexpr ::aarith::uinteger<W, WordType> signaling_NaN() noexcept
    {
        return ::aarith::uinteger<W, WordType>::zero();
    }

    static constexpr ::aarith::uinteger<W, WordType> denorm_min() noexcept
    {
        return ::aarith::uinteger<W, WordType>::min();
    }
};

template <size_t W, typename WordType> class std::numeric_limits<::aarith::integer<W, WordType>>
{
public:
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = true;
    static constexpr bool is_integer = true;
    static constexpr bool is_exact = true;
    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = false;
    static constexpr bool has_signaling_NaN = false;
    static constexpr bool is_bounded = true;
    static constexpr std::float_denorm_style has_denorm = std::denorm_absent;
    static constexpr bool has_denorm_loss = false;

    // TODO do we need to take that into account somewhere?
    static constexpr std::float_round_style round_style = std::round_toward_zero;
    static constexpr bool is_iec559 = false;
    static constexpr bool is_modulo = false;
    static constexpr int radix = 2;
    static constexpr int digits = W - 1; // TODO what happens if W > max_int?
    static constexpr int digits10 =
        ::aarith::ceil<int>(std::numeric_limits<::aarith::integer<W>>::digits *
                            ::aarith::log<10, 2>()) -
        1;

    // weird decision but https://en.cppreference.com/w/cpp/types/numeric_limits/max_digits10 says
    // so
    static constexpr int max_digits10 = 0;

    static constexpr int min_exponent = 0;
    static constexpr int min_exponent10 = 0;
    static constexpr int max_exponent = 0;
    static constexpr int max_exponent10 = 0;

    // Is this value set how it is supposed to? At least division by zero throws an exception, so
    // this value is, most likely, correct.
    static constexpr bool traps = true;

    static constexpr bool tinyness_before = false;

    static constexpr ::aarith::integer<W, WordType> min() noexcept
    {
        return ::aarith::integer<W, WordType>::min();
    }

    static constexpr ::aarith::integer<W, WordType> lowest() noexcept
    {
        return ::aarith::integer<W, WordType>::min();
    }

    static constexpr ::aarith::integer<W, WordType> max() noexcept
    {
        return ::aarith::integer<W, WordType>::max();
    }

    static constexpr ::aarith::integer<W, WordType> epsilon() noexcept
    {
        return ::aarith::integer<W, WordType>::zero();
    }

    static constexpr ::aarith::integer<W, WordType> round_error() noexcept
    {
        return ::aarith::integer<W, WordType>::zero();
    }

    static constexpr ::aarith::integer<W, WordType> infinity() noexcept
    {
        return ::aarith::integer<W, WordType>::zero();
    }

    static constexpr ::aarith::integer<W, WordType> quiet_NaN() noexcept
    {
        return ::aarith::integer<W, WordType>::zero();
    }

    static constexpr ::aarith::integer<W, WordType> signaling_NaN() noexcept
    {
        return ::aarith::integer<W, WordType>::zero();
    }

    static constexpr ::aarith::integer<W, WordType> denorm_min() noexcept
    {
        return ::aarith::integer<W, WordType>::zero();
    }
};
