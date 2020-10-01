#pragma once

#include <random>

namespace aarith {

/**
 * Implements random number generation interface similar to std::uniform_int_distribution.
 */
template <size_t I, size_t F, template <size_t, typename> typename BaseInt, typename WordType = uint64_t> class uniform_fixed_point_distribution
{
public:
    using result_type = fixed<I,F,BaseInt, WordType>;

    template <class Generator> auto operator()(Generator& g) -> result_type
    {
        auto data = random_array(g);
        return result_type::from_bitstring(data);
    }

    virtual void reset()
    {
        random_array.reset();
    }

private:
    uniform_word_array_distribution<I+F, WordType> random_array{};
};

}