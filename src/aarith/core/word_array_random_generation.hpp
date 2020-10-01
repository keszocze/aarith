#pragma once

#include <random>
#include <stdexcept>

namespace aarith {

/**
 * Implements random number generation interface similar to std::uniform_int_distribution.
 */
template <size_t BitWidth, typename WordType = uint64_t> class uniform_word_array_distribution
{
public:
    using result_type = word_array<BitWidth, WordType>;

    template <class Generator> auto operator()(Generator& g) -> result_type
    {
        result_type array;
        for (auto i = 0U; i < array.word_count(); ++i)
        {
            array.set_word(i, random_word(g));
        }
        return array;
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
