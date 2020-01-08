#pragma once

#include "aarith/utilities/bit_operations.hpp"
#include "traits.hpp"
#include <aarith/types/word_container.hpp>
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
class sinteger : public word_container<Width, WordType>
{
public:
    sinteger() = default;

    explicit sinteger(WordType n)
        : word_container<Width, WordType>(n)
    {
    }

    template <typename T, typename = std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>>>
    explicit sinteger(T t)
        : word_container<Width, WordType>(static_cast<WordType>(t))
    {
        if (t < 0) {
            const WordType ones(-1);
            for (size_t i = 1; i < sinteger<Width>::word_count(); i++)
            {
                this->set_word(i,ones);
            }
        }
    }

    template <typename T, class... Args,
              typename = std::enable_if_t<std::is_integral_v<T> && std::is_signed_v<T>>>
    explicit sinteger(T t, Args... args)
        : word_container<Width, WordType>(static_cast<WordType>(t), args...)
    {
    }

    template <class... Args>
    sinteger(WordType fst, Args... args)
        : word_container<Width>(fst, args...)
    {
    }

    template <size_t V>
    sinteger<Width, WordType>(const sinteger<V, WordType>& other)
        : word_container<Width>(static_cast<const word_container<V, WordType>&>(other))
    {
    }

    template <size_t V>
    sinteger<Width, WordType>(const word_container<V, WordType>& other)
        : word_container<Width>(other)
    {
    }

    auto operator<<=(const size_t shift_by) -> sinteger&
    {
        return *this = *this << shift_by;
    }

    auto operator>>=(const size_t shift_by) -> sinteger&
    {
        return *this = *this >> shift_by;
    }

    auto operator+=(const sinteger<Width> addend) -> sinteger&
    {
        return *this = *this + addend;
    }

    [[nodiscard]] static constexpr sinteger min()
    {
        sinteger min;
        min.set_bit(Width - 1, true);
        return min;
    }

    [[nodiscard]] static constexpr sinteger max()
    {
        sinteger max = sinteger::all_ones();
        max.set_bit(Width - 1, false);
        return max;
    }

    [[nodiscard]] static constexpr sinteger zero()
    {
        sinteger zero = sinteger::all_zeroes();
        return zero;
    }

    /**
     * @brief Returns whether the number is negative.
     * @return Whether the number is negative
     */
    [[nodiscard]] bool is_negative() const
    {
        return (this->msb() == 1);
    }
};

template <size_t Width> class is_integral<sinteger<Width>>
{
public:
    static constexpr bool value = true;
};

template <size_t Width> class is_unsigned<sinteger<Width>>
{
public:
    static constexpr bool value = false;
};

template <size_t Width>
[[nodiscard]] auto operator&(const sinteger<Width>& lhs, const sinteger<Width>& rhs)
    -> sinteger<Width>
{
    word_container<Width> lhs_w{lhs};
    word_container<Width> rhs_w{rhs};

    word_container<Width> result = lhs_w & rhs_w;

    return sinteger<Width>{result};
}

template <size_t Width>
[[nodiscard]] auto operator|(const sinteger<Width>& lhs, const sinteger<Width>& rhs)
    -> sinteger<Width>
{
    word_container<Width> lhs_w{lhs};
    word_container<Width> rhs_w{rhs};

    word_container<Width> result = lhs_w | rhs_w;

    return sinteger<Width>{result};
}

template <size_t Width>[[nodiscard]] auto operator~(const sinteger<Width>& rhs) -> sinteger<Width>
{
    word_container<Width> rhs_w{rhs};
    word_container<Width> result = ~rhs_w;
    return sinteger<Width>{result};
}

template <size_t DestinationWidth, size_t SourceWidth>
[[nodiscard]] auto width_cast(const sinteger<SourceWidth>& source) -> sinteger<DestinationWidth>
{
    word_container<SourceWidth> in{source};

    word_container<DestinationWidth> result = width_cast<DestinationWidth>(in);

    return sinteger<DestinationWidth>{result};
}

} // namespace aarith

// We are only allowed to extend std with specializations
// https://en.cppreference.com/w/cpp/language/extending_std
template <size_t W> class std::numeric_limits<aarith::sinteger<W>>
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
    static constexpr int digits10 = std::numeric_limits<aarith::sinteger<W>>::digits *
                                    std::log10(std::numeric_limits<aarith::sinteger<W>>::radix);

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

    static constexpr aarith::sinteger<W> min() noexcept
    {
        return aarith::sinteger<W>::min();
    }

    static constexpr aarith::sinteger<W> lowest() noexcept
    {
        return aarith::sinteger<W>::min();
    }

    static constexpr aarith::sinteger<W> max() noexcept
    {
        return aarith::sinteger<W>::max();
    }

    static constexpr aarith::sinteger<W> epsilon() noexcept
    {
        return aarith::sinteger<W>::zero();
    }

    static constexpr aarith::sinteger<W> round_error() noexcept
    {
        return aarith::sinteger<W>::zero();
    }

    static constexpr aarith::sinteger<W> infinity() noexcept
    {
        return aarith::sinteger<W>::zero();
    }

    static constexpr aarith::sinteger<W> quiet_NaN() noexcept
    {
        return aarith::sinteger<W>::zero();
    }

    static constexpr aarith::sinteger<W> signaling_NaN() noexcept
    {
        return aarith::sinteger<W>::zero();
    }

    static constexpr aarith::sinteger<W> denorm_min() noexcept
    {
        return aarith::sinteger<W>::zero();
    }
};