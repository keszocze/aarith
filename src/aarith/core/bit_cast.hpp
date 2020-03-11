#pragma once

#include <cstring>
#include <type_traits>

namespace aarith {
/**
 * To avoid undefined behaviour when type punning, we are using memcpy.
 * This is rather annoying but seems to be the way to go. The implementation
 * is stolen from this talk:
 * https://www.youtube.com/watch?v=_qzMpk-22cc
 *
 * @todo replace with std::bit_cast when switching to C++20
 *
 * @tparam To The type to convert to
 * @tparam From The type to convert from
 * @param src The source of the bits
 * @return The new type filled with the bits of src
 */
template <typename To, typename From,
          typename =
              std::enable_if_t<(sizeof(To) == sizeof(From)) && std::is_trivially_copyable_v<From> &&
                               std::is_trivially_copyable_v<To>>>

To bit_cast(const From& src) noexcept
{
    To dst;
    std::memcpy(&dst, &src, sizeof(To));
    return dst;
}

} // namespace aarith