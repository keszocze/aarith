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
