#pragma once

#include <cstdint>
#include <limits>
#include <type_traits>

namespace aarith {

/**
 * @brief Type trait to check if a type is a word array
 *
 * @see `word_array`
 *
 * @tparam Type The type to check
 */
template <class Type> class is_word_array
{
public:
    /**
     * By default, no type is an aarith word array
     */
    static constexpr bool value = false;
};

/**
 * @brief Test for a type being an word array
 *
 * Helper for the `is_word_array` type trait
 *
 * @tparam Type Type to check for being an aarith word array
 */
template <class Type> inline constexpr bool is_word_array_v = is_word_array<Type>::value;


/**
 * @brief Type trait to check if a type is an aarith integer
 * @tparam Type The Type to check
 */
template <class Type> class is_integral
{
public:
    /**
     * By default, mo type is an aarith integer
     */
    static constexpr bool value = false;
};

/**
 * @brief Test for a type being an aarith integer
 *
 * Helper for the `is_integral` type trait
 * @tparam Type The type to check
 *
 */
template <class Type> inline constexpr bool is_integral_v = is_integral<Type>::value;


/**
 * @brief Type trait to check if a type is an unsigned aarith type
 *
 * @tparam Type The type to check
 */
template <class Type> class is_unsigned
{
public:
    /**
     * By default, no type is an unsigned type
     */
    static constexpr bool value = false;
};

/**
 * @brief Test for an aarith type to be unsigned
 *
 * @tparam Type The type to check
 */
template <class Type> inline constexpr bool is_unsigned_v = is_unsigned<Type>::value;


/**
 * @brief Type trait to check if a type is a signed aarith type
 * @tparam Type The type to check
 */
template <class Type> class is_signed
{
public:
    /**
     * By default, no type is an signed aarith type
     */
    static constexpr bool value = false;
};

/**
 * @brief Test for an aarith type to be signed
 * @tparam Type The type to check
 */
template <class Type> inline constexpr bool is_signed_v = is_signed<Type>::value;

/**
 * @brief Type trait to check if a type is an aarith floating_point number
 *
 * @note This does *not* return true for the native data types such as float or double!
 *
 * @tparam Type The type to check
 */
template <class Type> class is_float
{
public:

    /**
     * By default, a type is not of type aarith floating_point
     */
    static constexpr bool value = false;
};

template <typename Type> class is_fixed_point
{
public:
    static constexpr bool value = false;
};

template <typename Type> inline constexpr bool is_fixed_point_v = is_fixed_point<Type>::value;

/**
 * @brief Tests if a type is an aarith floating_point
 *
 * Helper for the `is_float` type trait
 *
 * @tparam Type The type to check for being an aarith `floating_point`
 */
template <class Type> inline constexpr bool is_float_v = is_float<Type>::value;

/**
 * @brief Type trait to check if two types use the same word type to store the data
 *
 * @tparam A First type
 * @tparam B Second type
 */
template <typename A, typename B>
inline constexpr bool same_word_type = std::is_same_v<typename A::word_type, typename B::word_type>;

/**
 * @brief Type trait to check if two types have the same 'signedness'
 *
 * It returns true if and only if both types are signed or unsigned
 *
 * @tparam A First type
 * @tparam B Second type
 */
template <typename A, typename B>
inline constexpr bool same_signedness = (is_unsigned_v<A> == is_unsigned_v<B>);

/**
 * @brief Type trait to check a type for being an unsigned integer
 *
 * It seems that the type traits of C++ have no reasonable concept of "unsigned integer" so we have
 * to add this ourselves.
 *
 * @tparam T The type to check for "unsigned int'nes"
 */
template <typename T>
inline constexpr bool is_unsigned_int =
    std::is_same_v<T, std::size_t> || std::is_same_v<T, uint64_t> || std::is_same_v<T, uint32_t> ||
    std::is_same_v<T, uint16_t> || std::is_same_v<T, uint8_t>;

} // namespace aarith
