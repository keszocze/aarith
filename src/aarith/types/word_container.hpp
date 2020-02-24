#pragma once

#include "aarith/utilities/bit_operations.hpp"
#include "traits.hpp"
#include <algorithm>
#include <array>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace aarith {

template <size_t Width, class WordType = uint64_t> class word_container
{
public:
    using word_type = WordType;
    using bit_type = WordType;
    static_assert(Width > 0, " Width must be at least 1 (bit)");

    /*
     * Constructors etc.
     */

    word_container() = default;

    word_container(WordType w)
    {
        this->words[0] = w & word_mask(0);
    }

    template <class... Args> word_container(WordType w, Args... args)
    {
        set_words(w, args...);
    }

    template <class... Args> static auto from_words(Args... args) -> word_container
    {
        word_container wc;
        wc.set_words(args...);
        return wc;
    }

    template <size_t V> word_container(const word_container<V>& other)
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

        if constexpr (word_container<Width, WordType>::word_count() >
                      word_container<V>::word_count())
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

    [[nodiscard]] static constexpr auto word_mask(size_t index) noexcept -> word_type
    {

        constexpr word_type other_masks = static_cast<word_type>(-1); // all ones, e.g. no masking
        constexpr word_type last_mask =
            (width() % word_width() != 0)
                ? (static_cast<word_type>(1) << (width() % word_width())) - 1
                : static_cast<word_type>(-1);
        return (index == word_count() - 1) ? last_mask : other_masks;
    };

    [[nodiscard]] static constexpr auto width() noexcept -> size_t
    {
        return Width;
    }

    [[nodiscard]] auto word(size_t index) const -> word_type
    {
        return words[index];
    }

    /**
     * @brief Returns ths most significant bit.
     *
     * The most significant bit is the Width's one (i.e. the one you can get via bit(Width-1)). This
     * method is simply there for convenience.
     */
    [[nodiscard]] auto msb() const -> bit_type
    {
        return bit(Width - 1);
    }

    /**
     * @brief Sets the value of the most significant bit (MSB)
     * @param b The value the MSB is set to
     */
    void set_msb(const bool b)
    {
        set_bit(Width - 1, b);
    }

    /**
     * @brief Returns bit at given index.
     *
     * @note No bounds checking is performed! If your index is too large bad things will happen!
     *
     * @param index The index for which the bit is to be returned
     * @return  The bit at the indexed position
     */
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

            std::string msg = gen_oob_msg(index, true);
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

    void set_word(const size_t index, const word_type value)
    {
        if (index >= word_count())
        {
            std::string msg = gen_oob_msg(index, false);
            throw std::out_of_range(msg);
        }
        words[index] = value & word_mask(index);
    }

    // Sets the words to the given values, where the right-most argument corresponds to word 0.
    template <class... Args> void set_words(Args... args)
    {
        set_word_recursively<0>(args...);
    }

    /*
     * Constants
     */

    [[nodiscard]] static constexpr word_container all_ones()
    {
        word_container n;
        word_type ones = ~(static_cast<word_type>(0U));
        for (size_t i = 0; i < n.word_count(); ++i)
        {
            n.set_word(i, ones);
        }
        return n;
    }

    [[nodiscard]] static constexpr word_container all_zeroes()
    {
        return word_container{0U};
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
        return words.crbegin();
    }

    constexpr auto crend() const noexcept
    {
        return words.crend();
    }

private:
    template <size_t index, class... Args>
    auto set_word_recursively(word_type value, Args... args) -> size_t
    {
        static_assert(index < word_count(), "too many initializer words");
        auto const count = set_word_recursively<index + 1>(args...);
        words[count - index] = value & word_mask(count - index);
        return count;
    }

    template <size_t index> auto set_word_recursively(word_type value) -> size_t
    {
        static_assert(index < word_count(), "too many initializer words");
        words[0] = value & word_mask(0);
        return index;
    }

    std::string gen_oob_msg(size_t index, bool accessed_bit = true)
    {

        std::string msg, head, foot;
        if (accessed_bit)
        {
            head = "Trying to access bit with index ";
            foot = std::to_string(width() - 1);
        }
        else
        {
            head = "Trying to access word with index ";
            foot = std::to_string(word_count() - 1);
        }

        msg += head;
        msg += std::to_string(index);
        msg += " for word_container<";
        msg += std::to_string(width());
        msg += "> with max index ";
        msg += foot;

        return msg;
    }

    std::array<word_type, word_count()> words{{0}};
};

template <size_t DestinationWidth, size_t SourceWidth>
[[nodiscard]] auto width_cast(const word_container<SourceWidth>& source)
    -> word_container<DestinationWidth>
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

template <size_t Width>
[[nodiscard]] auto operator&(const word_container<Width>& lhs, const word_container<Width>& rhs)
    -> word_container<Width>
{
    word_container<Width> bitwise_and;
    for (auto counter = 0U; counter < lhs.word_count(); ++counter)
    {
        bitwise_and.set_word(counter, lhs.word(counter) & rhs.word(counter));
    }
    return bitwise_and;
}

template <size_t Width>
auto operator|(const word_container<Width>& lhs, const word_container<Width>& rhs)
    -> word_container<Width>
{
    word_container<Width> bitwise_or;
    for (auto counter = 0U; counter < lhs.word_count(); ++counter)
    {
        bitwise_or.set_word(counter, lhs.word(counter) | rhs.word(counter));
    }
    return bitwise_or;
}

template <size_t Width>
auto operator^(const word_container<Width>& lhs, const word_container<Width>& rhs)
    -> word_container<Width>
{
    word_container<Width> bitwise_xor;
    for (auto counter = 0U; counter < lhs.word_count(); ++counter)
    {
        bitwise_xor.set_word(counter, lhs.word(counter) ^ rhs.word(counter));
    }
    return bitwise_xor;
}

template <size_t Width>
[[nodiscard]] auto operator~(const word_container<Width>& rhs) -> word_container<Width>
{
    word_container<Width> bitwise_not;
    for (auto counter = 0U; counter < rhs.word_count(); ++counter)
    {
        bitwise_not.set_word(counter, ~rhs.word(counter));
    }
    return bitwise_not;
}

/**
 * @brief  Counts the number of bits set to zero before the first one appears (from MSB to LSB)
 * @tparam Width Width of the word_container
 * @param value The word to count the leading zeroes in
 * @return
 */
template <size_t Width> auto count_leading_zeroes(const word_container<Width>& value) -> size_t
{
    for (auto i = Width; i > 0; --i)
    {
        if (value.bit(i - 1))
        {
            return (Width - i);
        }
    }
    return Width;
}

template <size_t Width>
[[nodiscard]] auto operator<<(const word_container<Width>& lhs, const size_t rhs)
    -> word_container<Width>
{
    if (rhs >= Width)
    {
        return word_container<Width>(0U);
    }
    if (rhs == 0)
    {
        return lhs;
    }
    word_container<Width> shifted;
    const auto skip_words = rhs / lhs.word_width();
    const auto shift_word_left = rhs - skip_words * lhs.word_width();
    const auto shift_word_right = lhs.word_width() - shift_word_left;

    for (auto counter = lhs.word_count(); counter > 0; --counter)
    {
        if (counter + skip_words < lhs.word_count())
        {
            typename word_container<Width>::word_type new_word;
            new_word = lhs.word(counter) << shift_word_left;
            if (shift_word_right < lhs.word_width())
            {
                new_word = new_word | (lhs.word(counter - 1) >> shift_word_right);
            }
            shifted.set_word(counter + skip_words, new_word);
        }
    }
    typename word_container<Width>::word_type new_word;
    new_word = lhs.word(0) << shift_word_left;
    shifted.set_word(skip_words, new_word);

    return shifted;
}

template <size_t Width>
auto operator>>(const word_container<Width>& lhs, const size_t rhs) -> word_container<Width>
{
    if (rhs >= Width)
    {
        return word_container<Width>(0U);
    }
    if (rhs == 0)
    {
        return lhs;
    }

    word_container<Width> shifted;
    const auto skip_words = rhs / lhs.word_width();
    const auto shift_word_right = rhs - skip_words * lhs.word_width();
    const auto shift_word_left = lhs.word_width() - shift_word_right;

    for (auto counter = skip_words; counter < lhs.word_count(); ++counter)
    {
        typename word_container<Width>::word_type new_word;
        new_word = lhs.word(counter) >> shift_word_right;
        if (shift_word_left < lhs.word_width() && counter + 1 < lhs.word_count())
        {
            new_word = new_word | (lhs.word(counter + 1) << shift_word_left);
        }
        shifted.set_word(counter - skip_words, new_word);
    }
    typename word_container<Width>::word_type new_word;
    new_word = lhs.word(lhs.word_count() - 1) >> shift_word_right;
    shifted.set_word(lhs.word_count() - skip_words - 1, new_word);

    return shifted;
}

/**
 * @brief Extracts a range from the word container
 *
 * Note that the indexing is done
 *  - zero based starting from the LSB
 *  - is inclusive (i.e. the start and end point are part of the range)
 *
 * @tparam S Starting index (inclusive, from left to right)
 * @tparam E  Ending index (inclusive, from left to right)
 * @tparam W Width of the word container that the range is taken from
 * @param w  Word container from which the range is taken from
 * @return Range word[S,E], including the
 */
template <size_t S, size_t E, size_t W>
word_container<(S - E) + 1> bit_range(const word_container<W>& w)
{
    static_assert(S < W, "Range must start within the word");
    static_assert(E <= S, "Range must be positive (i.e. this method will not reverse the word");

    return width_cast<(S - E) + 1>(w >> E);
}

/**
 * @brief Splits the word container at the given splitting point
 * @tparam S Splitting point
 * @tparam W Width of the word wontainer to be split
 * @param w Word container that is split
 * @return Pair of <word[W-1,S+1], word[S,0]>
 */
template <size_t S, size_t W>
std::pair<word_container<W - (S + 1)>, word_container<S + 1>> split(const word_container<W>& w)
{
    static_assert(S < W - 1 && S >= 0);

    const word_container<W - (S + 1)> lhs{width_cast<W - (S + 1)>(w >> (S + 1))};

    const word_container<S + 1> rhs = width_cast<S + 1>(w);

    return std::make_pair(lhs, rhs);
}

/**
 * @brief Applies a given function to all words within the word_container
 * @tparam F "Catch-all" parameter for functions operating on the words of the word_container
 * @tparam W Bit width of the word_container to operate on
 * @param w The word_container to operate on
 * @param f Function of type word_container<W>::word_type -> word_container<W>::word_type
 * @return A new word_container with the transformed words
 */
template <class F, size_t W>
[[nodiscard]] word_container<W> map(const word_container<W>& w, const F f)
{

    word_container<W> result;
    for (size_t i = 0; i < w.word_count(); ++i)
    {
        result.set_word(i, f(w.word(i)));
    }
    return result;
}

/**
 * @brief Applies a function to all words of two word_containers and zips them to a new
 * word_container
 *
 * If the word_containers have a different number of words, the result will be of size min(W,V);
 *
 * @tparam F "Catch-all" parameter for functions operating on the words of the word_container
 * @tparam W Bit width of the first word_container to operate on
 * @tparam V Bit width of the secnd word_container to operate on
 * @param w The first word_container to operate on
 * @param v The second word_container to operate on
 * @param f Function of type (word_container<W>::word_type, word_container<W>::word_type) ->
 * word_container<W>
 * @return The newly created, zipped word_container
 */
template <class F, size_t W, size_t V>
[[nodiscard]] word_container<std::min(W, V)> zip_with(const word_container<W>& w,
                                                      const word_container<V>& v, F f)
{
    constexpr size_t L = std::min(W, V);
    word_container<L> result;

    for (size_t i = 0; i < result.word_count(); ++i)
    {
        result.set_word(i, f(w.word(i), v.word(i)));
    }
    return result;
}

/**
 * @brief Applies a function to all words of two word_containers and zips them to a new
 * word_container carrying along a state
 *
 * The state that is carry will be parsed to the next function call on the two words of the container and may be
 * updated in the process (e.g. generating a carry that propagates through the container).
 *
 * If the word_containers have a different number of words, the result will be of size min(W,V);
 *
 * @tparam F "Catch-all" parameter for functions operating on the words of the word_container
 * @tparam W Bit width of the first word_container to operate on
 * @tparam V Bit width of the secnd word_container to operate on
 * @param w The first word_container to operate on
 * @param v The second word_container to operate on
 * @param f Function of type (word_container<W>::word_type, word_container<W>::word_type, word_container<W>::word_type) ->
 * std::pari<word_container<W>,word_container<W>::word_type>
 * @param initial_state The initial state to be passed into the operation
 * @return The newly created, zipped word_container
 */
template <class F, size_t W, size_t V>
[[nodiscard]] word_container<std::min(W, V)>
zip_with_state(const word_container<W>& w, const word_container<V>& v, const F f,
               const typename word_container<std::min(W, V)>::word_type initial_state =
                   typename word_container<std::min(W, V)>::word_type())
{
    constexpr size_t L = std::min(W, V);
    word_container<L> result;

    using wt = typename word_container<L>::word_type;

    wt state = initial_state;

    for (size_t i = 0; i < result.word_count(); ++i)
    {
        const auto fun_res = f(w.word(i), v.word(i), state);
        state = fun_res.second;
        wt new_word = fun_res.first;
        std::cout << "setting word " << i << " to " << new_word << "\n";
        result.set_word(i, new_word);
    }
    return result;
}

/**
 * @brief Zips two word containers that might have a different number of words, expanding missing
 * words with zeroes.
 *
 * If the word_containers have a different number of words, the "missing" words are filled up with
 * zeroes such that this method will return a word_container that has the max width of both input
 * word_containers.
 *
 * @tparam F "Catch all" type for the zip with function
 * @tparam W Width of the first word_container
 * @tparam V Width of the second word_container
 * @param w First word_container
 * @param v Second word_container
 * @param f The zip_with function of type (word_container<W>::word_type,
 * word_container<W>::word_type) -> word_container<W>
 * @return
 */
template <class F, size_t W, size_t V>
[[nodiscard]] word_container<std::max(W, V)> zip_with_expand(const word_container<W>& w,
                                                             const word_container<V>& v, const F f)
{
    constexpr size_t wc = word_container<W>::word_count();
    constexpr size_t vc = word_container<V>::word_count();

    // simply return the "standard" zip_with when the word widths are identical
    if constexpr (wc == vc)
    {
        return zip_with(w, v, f);
    }
    else
    {

        constexpr size_t max = std::max(W, V);

        word_container<max> w_{w};
        word_container<max> v_{v};

        return zip_with(w_, v_, f);
    }
}

/**
 * @brief Zips two word containers that might have a different number of words, expanding missing
 * words with zeroes carrying along a state.
 *
 * The state that is carry will be parsed to the next function call on the two words of the container and may be
 * updated in the process (e.g. generating a carry that propagates through the container).
 *
 *
 * If the word_containers have a different number of words, the "missing" words are filled up with
 * zeroes such that this method will return a word_container that has the max width of both input
 * word_containers.
 *
 * @tparam F "Catch all" type for the zip with function
 * @tparam W Width of the first word_container
 * @tparam V Width of the second word_container
 * @param w First word_container
 * @param v Second word_container
 * @param f Function of type (word_container<W>::word_type, word_container<W>::word_type, word_container<W>::word_type) ->
 * std::pari<word_container<W>,word_container<W>::word_type>
 * @param initial_state The initial state to be passed into the operation
 * @return
 */
template <class F, size_t W, size_t V>
[[nodiscard]] word_container<std::max(W, V)>
zip_with_state_expand(const word_container<W>& w, const word_container<V>& v, const F f,
                      const typename word_container<std::max(W, V)>::word_type initial_state =
                          typename word_container<std::max(W, V)>::word_type())
{
    constexpr size_t wc = word_container<W>::word_count();
    constexpr size_t vc = word_container<V>::word_count();

    // simply return the "standard" zip_with when the word widths are identical
    if constexpr (wc == vc)
    {
        return zip_with_state(w, v, f);
    }
    else
    {

        constexpr size_t max = std::max(W, V);

        word_container<max> w_{w};
        word_container<max> v_{v};

        return zip_with_state(w_, v_, f, initial_state);
    }
}

/**
 *
 * @brief Reduces the vector using a given two-ary function.
 *
 * This is also known as a fold.
 *
 * @tparam R Return type of the reducing function
 * @tparam F "Catch all" type for the reducing function
 * @tparam W The width of the word_container
 * @param w The word_container
 * @param f The reducing function of type (word_container<W>::word_type, R) -> R
 * @param initial_value The initial value that is fed into f
 * @return The value from reducing the word_container using f and the given initial value
 */
template <class R, class F, size_t W>
[[nodiscard]] R reduce(const word_container<W>& w, const F f, const R initial_value)
{
    R result = initial_value;

    for (size_t i = 0; i < w.word_count(); ++i)
    {
        result = f(w.word(i), result);
    }

    return result;
}

/**
 * @brief zips and reduces two word_containers
 *
 * @tparam R The return type of the reducing function
 * @tparam F "Catch all" type for the reducing function
 * @tparam W The width of the first word_container
 * @tparam V The width of the second word_container
 * @param w The first word_container
 * @param v The second word_container
 * @param f The reducing function of type (word_container<W>::word_type,
 * word_container<V>__word_type, R) -> R
 * @param initial_value The initial value that is fed into f
 * @return The value from zipping and reducing the two word_containers
 */
template <class R, class F, size_t W, size_t V>
[[nodiscard]] R zip_reduce(const word_container<W>& w, const word_container<V>& v, const F f,
                           const R initial_value)
{
    R result = initial_value;

    constexpr size_t L = std::min(W, V);

    for (size_t i = 0; i < word_container<L>::word_count(); ++i)
    {
        result = f(w.word(i), v.word(i), result);
    }

    return result;
}

/**
 * @brief zips and reduces two word_containers that might have a different word count
 *
 * This method expands missing words by using zeros instead. This means that no values are skipped.
 *
 * @tparam R The return type of the reducing function
 * @tparam F "Catch all" type for the reducing function
 * @tparam W The width of the first word_container
 * @tparam V The width of the second word_container
 * @param w The first word_container
 * @param v The second word_container
 * @param f The reducing function of type (word_container<W>::word_type,
 * word_container<V>__word_type, R) -> R
 * @param initial_value The initial value that is fed into f
 * @return The value from zipping and reducing the two word_containers
 */
template <class R, class F, size_t W, size_t V>
[[nodiscard]] R zip_reduce_expand(const word_container<W>& w, const word_container<V>& v, const F f,
                                  const R initial_value)
{

    R result = initial_value;

    constexpr size_t wc = w.word_count();
    constexpr size_t vc = v.word_count();

    if constexpr (wc == vc)
    {
        return zip_reduce(w, v, f, initial_value);
    }
    else
    {

        constexpr size_t max = std::max(W, V);

        word_container<max> w_{w};
        word_container<max> v_{v};

        return zip_reduce(w_, v_, f, initial_value);
        ;
    }
}

} // namespace aarith
