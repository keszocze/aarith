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
    [[nodiscard]] auto operator<<(const sinteger<Width>& lhs, const size_t rhs) -> sinteger<Width>
    {
        if (rhs >= Width)
        {
            return sinteger<Width>(0U);
        }
        if (rhs == 0)
        {
            return lhs;
        }
        sinteger<Width> shifted;
        const auto skip_words = rhs / lhs.word_width();
        const auto shift_word_left = rhs - skip_words * lhs.word_width();
        const auto shift_word_right = lhs.word_width() - shift_word_left;

        for (auto counter = lhs.word_count(); counter > 0; --counter)
        {
            if (counter + skip_words < lhs.word_count())
            {
                typename sinteger<Width>::word_type new_word;
                new_word = lhs.word(counter) << shift_word_left;
                if (shift_word_right < lhs.word_width())
                {
                    new_word = new_word | (lhs.word(counter - 1) >> shift_word_right);
                }
                shifted.set_word(counter + skip_words, new_word);
            }
        }
        typename sinteger<Width>::word_type new_word;
        new_word = lhs.word(0) << shift_word_left;
        shifted.set_word(skip_words, new_word);

        return shifted;
    }

    template <size_t Width>
    auto operator>>(const sinteger<Width>& lhs, const size_t rhs) -> sinteger<Width>
    {
        if (rhs >= Width)
        {
            return sinteger<Width>(0U);
        }
        if (rhs == 0)
        {
            return lhs;
        }

        sinteger<Width> shifted;
        const auto skip_words = rhs / lhs.word_width();
        const auto shift_word_right = rhs - skip_words * lhs.word_width();
        const auto shift_word_left = lhs.word_width() - shift_word_right;

        for (auto counter = 0U; counter < lhs.word_count(); ++counter)
        {
            if (skip_words <= counter)
            {
                typename sinteger<Width>::word_type new_word;
                new_word = lhs.word(counter) >> shift_word_right;
                if (shift_word_left < lhs.word_width() && counter + 1 < lhs.word_count())
                {
                    new_word = new_word | (lhs.word(counter + 1) << shift_word_left);
                }
                shifted.set_word(counter - skip_words, new_word);
            }
        }
        typename sinteger<Width>::word_type new_word;
        new_word = lhs.word(lhs.word_count() - 1) >> shift_word_right;
        shifted.set_word(lhs.word_count() - skip_words - 1, new_word);

        return shifted;
    }

    template <size_t Width> auto abs_two_complement(const sinteger<Width>& value) -> sinteger<Width>
    {
        if (value.bit(Width - 1) == 1)
        {
            const sinteger<Width> one(1U);
            return ~value + one;
        }
        return value;
    }

    template <size_t Width>
    [[nodiscard]] auto operator&(const sinteger<Width>& lhs, const sinteger<Width>& rhs)
    -> sinteger<Width>
    {
        word_container<Width> lhs_w = static_cast<const word_container<Width>&>(lhs);
        word_container<Width> rhs_w = static_cast<const word_container<Width>&>(rhs);

        word_container<Width> result = lhs_w & rhs_w;

        return static_cast<sinteger<Width>>(result);
    }

    template <size_t Width>
    [[nodiscard]] auto operator|(const sinteger<Width>& lhs, const sinteger<Width>& rhs)
    -> sinteger<Width>
    {
        word_container<Width> lhs_w = static_cast<const word_container<Width>&>(lhs);
        word_container<Width> rhs_w = static_cast<const word_container<Width>&>(rhs);

        word_container<Width> result = lhs_w | rhs_w;

        return static_cast<sinteger<Width>>(result);
    }

    template <size_t Width>[[nodiscard]] auto operator~(const sinteger<Width>& rhs) -> sinteger<Width>
    {
        word_container<Width> rhs_w = static_cast<const word_container<Width>&>(rhs);
        word_container<Width> result = ~rhs_w;
        return static_cast<sinteger<Width>>(result);
    }

    template <size_t DestinationWidth, size_t SourceWidth>
    [[nodiscard]] auto width_cast(const sinteger<SourceWidth>& source) -> sinteger<DestinationWidth>
    {
        word_container<SourceWidth> in = static_cast<const word_container<SourceWidth>&>(source);

        word_container<DestinationWidth> result = width_cast<DestinationWidth>(in);

        return static_cast<sinteger<DestinationWidth>>(result);
    }

} // namespace aarith
