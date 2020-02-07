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
class uinteger : public word_container<Width, WordType>
{
public:
    uinteger() = default;

    explicit uinteger(WordType n)
        : word_container<Width, WordType>(n)
    {
    }

    template <class... Args>
    uinteger(WordType fst, Args... args)
        : word_container<Width>(fst, args...)
    {
    }

    template <size_t V>
    uinteger<Width, WordType>(const uinteger<V, WordType>& other)
        : word_container<Width>(static_cast<const word_container<V, WordType>&>(other))
    {
    }

    template <size_t V>
    uinteger<Width, WordType>(const word_container<V, WordType>& other)
        : word_container<Width>(other)
    {
    }

    template <class... Args> static auto from_words(Args... args) -> uinteger
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
        uinteger n=uinteger::all_zeroes();
        n.set_bit(0);
        return n;
    }

    auto operator<<=(const size_t shift_by) -> uinteger&
    {
        return *this = *this << shift_by;
    }

    auto operator>>=(const size_t shift_by) -> uinteger&
    {
        return *this = *this >> shift_by;
    }

    auto operator+=(const uinteger<Width> addend) -> uinteger&
    {
        return *this = *this + addend;
    }
};

template <size_t Width>
[[nodiscard]] auto operator<<(const uinteger<Width>& lhs, const size_t rhs) -> uinteger<Width>
{

    word_container<Width> tmp{lhs};

    return uinteger<Width>{tmp << rhs};
}

template <size_t Width>
auto operator>>(const uinteger<Width>& lhs, const size_t rhs) -> uinteger<Width>
{

    word_container<Width> tmp{lhs};
    return uinteger<Width>{tmp >> rhs};
}

template <size_t Width>
[[nodiscard]] auto operator&(const uinteger<Width>& lhs, const uinteger<Width>& rhs)
    -> uinteger<Width>
{
    word_container<Width> lhs_w{lhs};
    word_container<Width> rhs_w{rhs};

    word_container<Width> result = lhs_w & rhs_w;

    return uinteger<Width>{result};
}

template <size_t Width>
[[nodiscard]] auto operator|(const uinteger<Width>& lhs, const uinteger<Width>& rhs)
    -> uinteger<Width>
{
    word_container<Width> lhs_w{lhs};
    word_container<Width> rhs_w{rhs};

    word_container<Width> result = lhs_w | rhs_w;

    return uinteger<Width>{result};
}

template <size_t Width>[[nodiscard]] auto operator~(const uinteger<Width>& rhs) -> uinteger<Width>
{
    word_container<Width> rhs_w{rhs};
    word_container<Width> result = ~rhs_w;
    return uinteger<Width>{result};
}

template <size_t DestinationWidth, size_t SourceWidth>
[[nodiscard]] auto width_cast(const uinteger<SourceWidth>& source) -> uinteger<DestinationWidth>
{
    word_container<SourceWidth> in{source};

    word_container<DestinationWidth> result = width_cast<DestinationWidth>(in);

    return uinteger<DestinationWidth>{result};
}
/*
 * Traits
 */
template <size_t Width> class is_integral<uinteger<Width>>
{
public:
    static constexpr bool value = true;
};

template <size_t Width> class is_unsigned<uinteger<Width>>
{
public:
    static constexpr bool value = true;
};

} // namespace aarith

// We are only allowed to extend std with specializations
// https://en.cppreference.com/w/cpp/language/extending_std
template <size_t W> class std::numeric_limits<aarith::uinteger<W>>
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
    static constexpr std::float_round_style round_style =
        std::round_toward_zero;
    static constexpr bool is_iec559 = false;
    static constexpr bool is_modulo = true;
    static constexpr int radix = 2;
    static constexpr int digits = W; // TODO what happens if W > max_int?
    static constexpr int digits10 = std::numeric_limits<aarith::uinteger<W>>::digits *
                                    std::log10(std::numeric_limits<aarith::uinteger<W>>::radix);

    // weird decision but https://en.cppreference.com/w/cpp/types/numeric_limits/max_digits10 says so
    static constexpr int max_digits10 = 0;

    static constexpr int min_exponent = 0;
    static constexpr int min_exponent10 = 0;
    static constexpr int max_exponent = 0;
    static constexpr int max_exponent10 = 0;

    // Is this value set how it is supposed to? At least division by zero throws an exception, so
    // this value is, most likely, correct.
    static constexpr bool traps = true;

    static constexpr bool tinyness_before = false;

    static constexpr aarith::uinteger<W> min() noexcept
    {
        return aarith::uinteger<W>::zero();
    }

    static constexpr aarith::uinteger<W> lowest() noexcept
    {
        return aarith::uinteger<W>::zero();
    }

    static constexpr aarith::uinteger<W> max() noexcept
    {
        return aarith::uinteger<W>::max();
    }

    static constexpr aarith::uinteger<W> epsilon() noexcept
    {
        return aarith::uinteger<W>::zero();
    }

    static constexpr aarith::uinteger<W> round_error() noexcept
    {
        return aarith::uinteger<W>::zero();
    }

    static constexpr aarith::uinteger<W> infinity() noexcept
    {
        return aarith::uinteger<W>::zero();
    }

    static constexpr aarith::uinteger<W> quiet_NaN() noexcept
    {
        return aarith::uinteger<W>::zero();
    }

    static constexpr aarith::uinteger<W> signaling_NaN() noexcept
    {
        return aarith::uinteger<W>::zero();
    }

    static constexpr aarith::uinteger<W> denorm_min() noexcept
    {
        return aarith::uinteger<W>::min();
    }
};