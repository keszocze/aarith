#pragma once

#include <aarith/core/traits.hpp>
#include <aarith/core/word_array.hpp>

namespace aarith {

/**
 * @brief Applies a given function to all words within the word_array
 * @tparam F "Catch-all" parameter for functions operating on the words of the word_array
 * @tparam W Bit width of the word_array to operate on
 * @param w The word_array to operate on
 * @param f Function of type word_array<W>::word_type -> word_array<W>::word_type
 * @return A new word_array with the transformed words
 */
template <class F, size_t W, typename WordType>
[[nodiscard]] constexpr word_array<W, WordType> map(const word_array<W, WordType>& w, const F f)
{

    word_array<W, WordType> result;
    for (size_t i = 0; i < w.word_count(); ++i)
    {
        result.set_word(i, f(w.word(i)));
    }
    return result;
}

/**
 *
 * @brief Reduces the vector using a given two-ary function.
 *
 * This is also known as a fold.
 *
 * @tparam R Return type of the reducing function
 * @tparam F "Catch all" type for the reducing function
 * @tparam W The width of the word_array
 * @param w The word_array
 * @param f The reducing function of type (word_array<W>::word_type, R) -> R
 * @param initial_value The initial value that is fed into f
 * @return The value from reducing the word_array using f and the given initial value
 */
template <class R, class F, size_t W, typename WordType>
[[nodiscard]] R constexpr reduce(const word_array<W, WordType>& w, const F f, const R initial_value)
{
    R result = initial_value;

    for (size_t i = 0; i < w.word_count(); ++i)
    {
        result = static_cast<R>(f(w.word(i), result));
    }

    return result;
}

/**
 * @brief Applies a function to all words of two word_arrays and zips them to a new
 * word_array
 *
 * If the word_arrays have a different number of words, the result will be of size min(W,V);
 *
 * @tparam F "Catch-all" parameter for functions operating on the words of the word_array
 * @tparam W Bit width of the first word_array to operate on
 * @tparam V Bit width of the second word_array to operate on
 * @param w The first word_array to operate on
 * @param v The second word_array to operate on
 * @param f Function of type (word_array<W>::word_type, word_array<W>::word_type) ->
 * word_array<W>
 * @return The newly created, zipped word_array
 */
template <class F, size_t W, size_t V, typename WordType>
[[nodiscard]] constexpr word_array<std::min(W, V), WordType>
zip_with(const word_array<W, WordType>& w, const word_array<V, WordType>& v, F f)
{
    constexpr size_t L = std::min(W, V);
    word_array<L, WordType> result;

    for (size_t i = 0; i < result.word_count(); ++i)
    {
        result.set_word(i, f(w.word(i), v.word(i)));
    }
    return result;
}

/**
 * @brief Applies a function to all words of two word_arrays and zips them to a new
 * word_array carrying along a state
 *
 * The state that is carry will be parsed to the next function call on the two words of the
 * container and may be updated in the process (e.g. generating a carry that propagates through the
 * container).
 *
 * If the word_arrays have a different number of words, the result will be of size min(W,V);
 *
 * @tparam F "Catch-all" parameter for functions operating on the words of the word_array
 * @tparam W Bit width of the first word_array to operate on
 * @tparam V Bit width of the secnd word_array to operate on
 * @param w The first word_array to operate on
 * @param v The second word_array to operate on
 * @param f Function of type (word_array<W>::word_type, word_array<W>::word_type,
 * word_array<W>::word_type) -> std::pari<word_array<W>,word_array<W>::word_type>
 * @param initial_state The initial state to be passed into the operation
 * @return The newly created, zipped word_array
 */
template <class F, size_t W, size_t V, typename WordType>
[[nodiscard]] constexpr word_array<std::min(W, V), WordType>
zip_with_state(const word_array<W, WordType>& w, const word_array<V, WordType>& v, const F f,
               const typename word_array<std::min(W, V), WordType>::word_type initial_state =
                   typename word_array<std::min(W, V), WordType>::word_type())
{
    constexpr size_t L = std::min(W, V);
    word_array<L, WordType> result;

    using wt = typename word_array<L, WordType>::word_type;

    wt state = initial_state;

    for (size_t i = 0; i < result.word_count(); ++i)
    {
        const auto fun_res = f(w.word(i), v.word(i), state);
        state = fun_res.second;
        wt new_word = fun_res.first;
        result.set_word(i, new_word);
    }
    return result;
}

/**
 * @brief Zips two word containers that might have a different number of words, expanding missing
 * words with zeroes.
 *
 * If the word_arrays have a different number of words, the "missing" words are filled up with
 * zeroes such that this method will return a word_array that has the max width of both input
 * word_arrays.
 *
 * @tparam F "Catch all" type for the zip with function
 * @tparam W Width of the first word_array
 * @tparam V Width of the second word_array
 * @param w First word_array
 * @param v Second word_array
 * @param f The zip_with function of type (word_array<W>::word_type,
 * word_array<W>::word_type) -> word_array<W>
 * @return
 */
template <class F, size_t W, size_t V, typename WordType>
[[nodiscard]] constexpr word_array<std::max(W, V), WordType>
zip_with_expand(const word_array<W, WordType>& w, const word_array<V, WordType>& v, const F f)
{
    constexpr size_t wc = word_array<W, WordType>::word_count();
    constexpr size_t vc = word_array<V, WordType>::word_count();

    // simply return the "standard" zip_with when the word widths are identical
    if constexpr (wc == vc)
    {
        return zip_with(w, v, f);
    }
    else
    {

        constexpr size_t max = std::max(W, V);

        word_array<max, WordType> w_{w};
        word_array<max, WordType> v_{v};

        return zip_with(w_, v_, f);
    }
}

/**
 * @brief Zips two word containers that might have a different number of words, expanding missing
 * words with zeroes carrying along a state.
 *
 * The state that is carry will be parsed to the next function call on the two words of the
 * container and may be updated in the process (e.g. generating a carry that propagates through the
 * container).
 *
 *
 * If the word_arrays have a different number of words, the "missing" words are filled up with
 * zeroes such that this method will return a word_array that has the max width of both input
 * word_arrays.
 *
 * @tparam F "Catch all" type for the zip with function
 * @tparam W Width of the first word_array
 * @tparam V Width of the second word_array
 * @param w First word_array
 * @param v Second word_array
 * @param f Function of type (word_array<W>::word_type, word_array<W>::word_type,
 * word_array<W>::word_type) -> std::pari<word_array<W>,word_array<W>::word_type>
 * @param initial_state The initial state to be passed into the operation
 * @return
 */
template <class F, size_t W, size_t V, typename WordType>
[[nodiscard]] constexpr word_array<std::max(W, V), WordType>
zip_with_state_expand(const word_array<W, WordType>& w, const word_array<V, WordType>& v, const F f,
                      const typename word_array<std::max(W, V), WordType>::word_type initial_state =
                          typename word_array<std::max(W, V), WordType>::word_type())
{
    constexpr size_t wc = word_array<W, WordType>::word_count();
    constexpr size_t vc = word_array<V, WordType>::word_count();

    // simply return the "standard" zip_with when the word widths are identical
    if constexpr (wc == vc)
    {
        return zip_with_state(w, v, f);
    }
    else
    {

        constexpr size_t max = std::max(W, V);

        word_array<max, WordType> w_{w};
        word_array<max, WordType> v_{v};

        return zip_with_state(w_, v_, f, initial_state);
    }
}

/**
 * @brief zips and reduces two word_arrays
 *
 * @tparam R The return type of the reducing function
 * @tparam F "Catch all" type for the reducing function
 * @tparam W The width of the first word_array
 * @tparam V The width of the second word_array
 * @param w The first word_array
 * @param v The second word_array
 * @param f The reducing function of type (word_array<W>::word_type,
 * word_array<V>__word_type, R) -> R
 * @param initial_value The initial value that is fed into f
 * @return The value from zipping and reducing the two word_arrays
 */
template <class R, class F, size_t W, size_t V, typename WordType>
[[nodiscard]] R constexpr zip_reduce(const word_array<W, WordType>& w,
                                     const word_array<V, WordType>& v, const F f,
                                     const R initial_value)
{
    R result = initial_value;

    constexpr size_t L = std::min(W, V);

    for (size_t i = 0; i < word_array<L, WordType>::word_count(); ++i)
    {
        result = f(w.word(i), v.word(i), result);
    }

    return result;
}

/**
 * @brief zips and reduces two word_arrays that might have a different word count
 *
 * This method expands missing words by using zeros instead. This means that no values are skipped.
 *
 * @tparam R The return type of the reducing function
 * @tparam F "Catch all" type for the reducing function
 * @tparam W The width of the first word_array
 * @tparam V The width of the second word_array
 * @param w The first word_array
 * @param v The second word_array
 * @param f The reducing function of type (word_array<W>::word_type,
 * word_array<V>__word_type, R) -> R
 * @param initial_value The initial value that is fed into f
 * @return The value from zipping and reducing the two word_arrays
 */
template <class R, class F, size_t W, size_t V, typename WordType>
[[nodiscard]] R zip_reduce_expand(const word_array<W, WordType>& w,
                                  const word_array<V, WordType>& v, const F f,
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

        word_array<max, WordType> w_{w};
        word_array<max, WordType> v_{v};

        return zip_reduce(w_, v_, f, initial_value);
    }
}

} // namespace aarith