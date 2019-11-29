#pragma once


#include <algorithm>


namespace aarith
{
    template <size_t Width, class WordType=uint64_t>
    class word_container {
    public:
        using word_type = WordType;
        using bit_type = WordType;
        static_assert(Width > 0, " Width must be at least 1 (bit)");


        /*
         * Constructrs etc.
         */

        word_container() = default;

        template <class T>
        explicit word_container(T n)
        : words{{0}}
        {
            static_assert(std::is_integral<T>::value, "Only integral values are supported");
            static_assert(!std::is_signed<T>::value, "Only unsigned numbers are supported");
            static_assert(sizeof(T) * 8 <= sizeof(word_type) * 8,
                          "Only up to 64 bit integers are supported");
            //        static_assert(sizeof(T) * 8 <= Width, "Data type can not fit provided number");

            words[0] = n;
        }

        template <class... Args> word_container(Args... args)
        {
            set_words(args...);
        }

        template <class... Args> static auto from_words(Args... args) -> word_container
        {
            word_container wc;
            wc.set_words(args...);
            return wc;
        }

        template <size_t V> word_container(const word_container<V> & other)
        {
            static_assert(V <= Width, "Can not create a word_container from larger container");

            for (auto i = 0U; i < other.word_count(); ++i)
            {
                set_word(i, other.word(i));
            }
        }
        template <size_t V> word_container<Width> operator=(const word_container<V>& other)
        {
            static_assert(V <= Width, "Can not create a word_container from larger container");

            if constexpr (V == Width)
            {
                if (&other == this)
                {
                    return *this;
                }
            }

            for (size_t i = 0U; i < other.word_count(); ++i)
            {
                set_word(i, other.word(i));
            }

            if constexpr (this->word_count() > other.word_count())
            {
                for (size_t i = other.word_count(); i < this->word_count(); ++i)
                {
                    set_word(i, 0U);
                }
            }
            return *this;
        }



        /*
         * Getters
         */

        [[nodiscard]] static constexpr auto word_width() noexcept -> size_t
        {
            return sizeof(word_type) * 8;
        }

        [[nodiscard]] static constexpr auto word_count() noexcept -> size_t
        {
            return size_in_words<word_type>(Width);
        }

        [[nodiscard]] static constexpr auto width() noexcept -> size_t
        {
            return Width;
        }

        [[nodiscard]] auto word(size_t index) const -> word_type
        {
            return words[index];
        }

        auto bit(size_t index) const -> bit_type
        {
            auto const the_word = word(index / word_width());
            auto const masked_bit = the_word & (static_cast<word_type>(1) << (index % word_width()));
            return static_cast<bit_type>(masked_bit > 0 ? 1 : 0);
        }


        template <size_t Count> auto bits(size_t index) const -> word_container<Count>
        {
            word_container<Count> result;
            for (auto i = 0U; i < Count; ++i)
            {
                result.set_bit(i, bit(index + i));
            }
            return result;
        }


        /*
         * Setters
         */



        void set_bit(size_t index, bool value = true)
        {
            if (index >= width())
            {
                // TODO generate the string elsewhere
                std::string msg;
                msg += "Trying to access bit with index ";
                msg += std::to_string(index);
                msg += " for uinteger<";
                msg += std::to_string(width());
                msg += "> with max index ";
                msg += std::to_string(width() - 1);
                throw std::out_of_range(msg);
            }
            const size_t word_index = index / word_width();
            const size_t inner_word_index = index % word_width();
            word_type mask = (1ULL << inner_word_index);

            if (value)
            {
                words[word_index] |= mask;
            }
            else
            {
                words[word_index] &= ~mask;
            }
        }

        static constexpr auto word_index(size_t bit_index) -> size_t
        {
            return bit_index / word_width();
        }


        void set_bit(size_t index, bit_type value)
        {
            auto const the_word = word(word_index(index));
            auto const masked_word = the_word & ~(static_cast<word_type>(1) << (index % word_width()));
            set_word(word_index(index),
                     masked_word | (static_cast<word_type>(value & 1) << (index % word_width())));
        }

        void set_word(size_t index, word_type value)
        {
            if (index >= word_count())
            {
                // TODO generate the string elsewhere
                std::string msg;
                msg += "Trying to access word with index ";
                msg += std::to_string(index);
                msg += " for uinteger<";
                msg += std::to_string(width());
                msg += "> with max index ";
                msg += std::to_string(word_count() - 1);

                throw std::out_of_range(msg);
            }
            words[index] = value & word_mask(index);
        }

        void set_wordss(const word_container& other)
        {
            for (size_t i=0U; i < word_count(); ++i) {
                words[i]=other.word(i);
            }
        }

        // Sets the words to the given values, where the right-most argument corresponds to word 0.
        template <class... Args> void set_words(Args... args)
        {
            set_word_recursively<0>(args...);
        }




        /*
         * Utility stuff
         */

        [[nodiscard]] bool is_zero() const noexcept
        {
            return std::all_of(words.begin(), words.end(), [](const word_type& w) {
                word_type zero = 0U;
                return w == zero;
            });
        }

        [[nodiscard]] explicit operator bool() const noexcept
        {
            return std::any_of(words.begin(), words.end(), [](const word_type& w) {
                word_type zero = 0U;
                return w != zero;
            });
        }


        [[nodiscard]] static constexpr auto word_mask(size_t index) noexcept -> word_type
        {

            constexpr word_type other_masks = static_cast<word_type>(-1); // all ones, e.g. no masking
            constexpr word_type last_mask =
                    (width() % word_width() != 0)
                    ? (static_cast<word_type>(1) << (width() % word_width())) - 1
                    : static_cast<word_type>(-1);
            return (index == word_count() - 1) ? last_mask : other_masks;
        };

        constexpr auto begin() const noexcept
        {
            return words.begin();
        }

        constexpr auto end() const noexcept
        {
            return words.end();
        }

        constexpr auto cbegin() const noexcept
        {
            return words.cbegin();
        }

        constexpr auto cend() const noexcept
        {
            return words.cend();
        }

        constexpr auto rbegin() const noexcept
        {
            return words.rbegin();
        }

        constexpr auto rend() const noexcept
        {
            return words.rend();
        }

        constexpr auto crbegin() const noexcept
        {
            return words.bcregin();
        }

        constexpr auto crend() const noexcept
        {
            return words.crend();
        }

    protected:
        template <size_t index, class... Args>
        auto set_word_recursively(word_type value, Args... args) -> size_t
        {
            static_assert(index < word_count(), "too many initializer words");
            auto const count = set_word_recursively<index + 1>(args...);
            words[count - index] = value & word_mask(index);
            return count;
        }

        template <size_t index> auto set_word_recursively(word_type value) -> size_t
        {
            static_assert(index < word_count(), "too many initializer words");
            words[0] = value & word_mask(index);
            return index;
        }

        std::array<word_type, word_count()> words{{0}};
    };



    template <size_t DestinationWidth, size_t SourceWidth>
    [[nodiscard]] auto width_cast(const word_container<SourceWidth>& source) -> word_container<DestinationWidth>
    {
        word_container<DestinationWidth> word_container;
        if constexpr (DestinationWidth >= SourceWidth)
        {
            for (auto i = 0U; i < source.word_count(); ++i)
            {
                word_container.set_word(i, source.word(i));
            }
        }
        else
        {
            for (auto i = 0U; i < word_container.word_count(); ++i)
            {
                word_container.set_word(i, source.word(i));
            }
        }
        return word_container;
    }
}