#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/integer/integers.hpp>

namespace aarith {

/**
 * @brief Performs a runtime-checked conversion.
 *
 * @warning The conversion will throw a runtime exception of the value does
 * not fit into the desired data type!
 *
 * @tparam T The type the number is to be converted to
 * @tparam W The width of the supplied unsigned integer
 * @param a The unsigned integer to convert into the type T
 * @return
 */
template <typename T, size_t W, typename WordType = uint64_t>
constexpr T narrow_cast(const uinteger<W, WordType>& a)
{
    constexpr T max = std::numeric_limits<T>::max();

    if (a <= uinteger<W, WordType>{max})
    {
        if constexpr (sizeof(T) <= sizeof(WordType))
        {
            // the last word is sufficient to fill the desired target type, so we can simply
            // make a call to the static_cast operation
            return static_cast<T>(a.word(0));
        }
        else
        {
            constexpr size_t words_per_type = (sizeof(T)) / sizeof(WordType);

            constexpr size_t use_words =
                std::min(words_per_type, uinteger<W, WordType>::word_count());

            T result{0U};

            for (size_t i = 0; i < use_words; ++i)
            {
                result += a.word(i) << (i * uinteger<W, WordType>::word_width());
            }

            return result;
        }
    }
    else
    {
        throw std::domain_error("Can not cast the uinteger in a type of provided size");
    }
}

/**
 * @brief Performs a runtime-checked conversion.
 *
 * @warning The conversion will throw a runtime exception of the value does
 * not fit into the desired data type!
 *
 * @tparam T The type the number is to be converted to
 * @tparam W The width of the supplied signed integer
 * @param a The signed integer to convert into the type T
 * @return
 */
template <typename T, size_t W, typename WordType>
constexpr T narrow_cast(const integer<W, WordType>& a)
{
    constexpr T min = std::numeric_limits<T>::min();
    constexpr T max = std::numeric_limits<T>::max();

    using TInt = integer<sizeof(T) * 8 + 1, WordType>;
    constexpr TInt imin(min);
    constexpr TInt imax(max);

    if (a >= imin && a <= imax)
    {
        if (a >= a.zero())
        {
            // handle non-negative values like unsigned values

            const uinteger<W, WordType> ua(a);
            return narrow_cast<T>(ua);
        }
        else if (a != imin)
        {
            // negative values a where abs(a) fits into type T are handled by
            // taking the twos complement from tc and treating that like an
            // unsigned integer

            uinteger<W, WordType> ua(a);

            const auto tc = (~ua) + ua.one();
            const T converted = narrow_cast<T>(tc);
            return (-1) * converted;
        }
        else
        {
            // special case where a == imin; using the approach for a != imin
            // does not work because for our value a, abs(a) does not fit into
            // type T

            return min;
        }
    }
    else
    {
        throw std::domain_error("Can not cast the integer in a type of provided size");
    }
}

} // namespace aarith
