#pragma once

#include <random>
#include <stdexcept>

namespace aarith {

/**
 * Implements random number generation interface similar to std::uniform_int_distribution.
 * @note The interval returned is [min,max]
 */
template <size_t BitWidth, typename WordType = uint64_t> class uniform_uinteger_distribution
{
public:
    using input_type = uinteger<BitWidth, WordType>;
    using internal_type = uinteger<BitWidth + 1, WordType>;

    explicit uniform_uinteger_distribution(const input_type& min_ = input_type::min(),
                                           const input_type& max_ = input_type::max())
        : min(min_)
        , max(add(internal_type{max_}, internal_type::one()))
        , length(sub(max, min))
    {
        if (!(min <= max))
        {
            throw std::runtime_error("uniform_uinteger_distribution: a must be <= b");
        }
    }

    template <class Generator> auto operator()(Generator& g) -> input_type
    {
        internal_type uint;
        for (auto i = 0U; i < uint.word_count(); ++i)
        {
            uint.set_word(i, random_word(g));
        }
        // Modulo is slightly biased towards smaller numbers. Possible fix: e.g. use "Java's
        // algorithm.
        using namespace aarith::integer_operators;
        //        std::cout << min << " + (" << uint << " % " << length << ") = " << min << " + "
        //                  << (uint % length) << "\n";
        return width_cast<BitWidth>(min + (uint % length));
    }

    void reset()
    {
        random_word.reset();
    }

private:
    internal_type min;
    internal_type max;
    internal_type length;
    std::uniform_int_distribution<WordType> random_word{std::numeric_limits<WordType>::min(),
                                                        std::numeric_limits<WordType>::max()};
};

/**
 * Implements random number generation interface similar to std::uniform_int_distribution.
 */
template <size_t BitWidth, typename WordType = uint64_t> class uniform_integer_distribution
{
public:
    using result_type = integer<BitWidth, WordType>;

    template <class Generator> auto operator()(Generator& g) -> result_type
    {
        word_array<BitWidth, WordType> array;
        for (auto i = 0U; i < array.word_count(); ++i)
        {
            array.set_word(i, random_word(g));
        }
        return result_type{array};
    }

    virtual void reset()
    {
        random_word.reset();
    }

private:
    std::uniform_int_distribution<WordType> random_word{std::numeric_limits<WordType>::min(),
                                                        std::numeric_limits<WordType>::max()};
};

} // namespace aarith
