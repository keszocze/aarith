#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/core/word_array.hpp>

namespace aarith {

template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
constexpr W& operator&=(W& a, const W& b)
{
    for (auto counter = 0U; counter < a.word_count(); ++counter)
    {
        a.set_word(counter, a.word(counter) & b.word(counter));
    }
    return a;
}

template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
[[nodiscard]] constexpr auto operator&(const W& lhs, const W& rhs) -> W
{
    W bitwise_and{lhs};
    bitwise_and &= rhs;
    return bitwise_and;
}

template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
constexpr W& operator|=(W& a, const W& b)
{
    for (auto counter = 0U; counter < a.word_count(); ++counter)
    {
        a.set_word(counter, a.word(counter) | b.word(counter));
    }
    return a;
}

template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
auto constexpr operator|(const W& lhs, const W& rhs) -> W
{
    W bitwise_or{lhs};
    bitwise_or |= rhs;
    return bitwise_or;
}

template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
constexpr W& operator^=(W& a, const W& b)
{
    for (auto counter = 0U; counter < a.word_count(); ++counter)
    {
        a.set_word(counter, a.word(counter) ^ b.word(counter));
    }
    return a;
}

template <typename W, typename = std::enable_if_t<is_word_array_v<W>>>
constexpr auto operator^(const W& lhs, const W& rhs) -> W
{
    W bitwise_xor{lhs};
    bitwise_xor ^= rhs;
    return bitwise_xor;
}

template <typename W> [[nodiscard]] constexpr auto operator~(const W& rhs) -> W
{
    static_assert(::aarith::is_word_array_v<W>);
    W bitwise_not;
    for (auto counter = 0U; counter < rhs.word_count(); ++counter)
    {
        bitwise_not.set_word(counter, ~rhs.word(counter));
    }
    return bitwise_not;
}

} // namespace aarith