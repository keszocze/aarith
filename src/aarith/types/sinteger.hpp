#pragma once

#include "aarith/utilities/bit_operations.hpp"
#include "traits.hpp"
#include <aarith/types/word_container.hpp>
#include <algorithm>
#include <array>
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
