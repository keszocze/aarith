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

template <size_t Width>
[[nodiscard]] auto operator<<(const uinteger<Width>& lhs, const size_t rhs) -> uinteger<Width>
{
    if (rhs >= Width)
    {
        return uinteger<Width>(0U);
    }
    if (rhs == 0)
    {
        return lhs;
    }

    word_container<Width> tmp{lhs};
    uinteger<Width> shifted{tmp << rhs};

    return shifted;
}

template <size_t Width>
auto operator>>(const uinteger<Width>& lhs, const size_t rhs) -> uinteger<Width>
{
    if (rhs >= Width)
    {
        return uinteger<Width>(0U);
    }
    if (rhs == 0)
    {
        return lhs;
    }

    word_container<Width> tmp{lhs};
    uinteger<Width> shifted{tmp >> rhs};
    return shifted;

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

} // namespace aarith
