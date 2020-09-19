#pragma once

#include <random>
#include <stdexcept>


namespace aarith {

/**
 * Implements random number generation interface similar to std::uniform_int_distribution.
 */
template <size_t BitWidth, typename WordType=uint64_t> class uniform_uinteger_distribution
{
public:
    using result_type = uinteger<BitWidth, WordType>;

    struct param_type
    {
        result_type min, max;
    };

    explicit uniform_uinteger_distribution(const result_type& a = result_type::min(),
                                           const result_type& b = result_type::max())
    {
        param(param_type{a, b});
    }

    template <class Generator> auto operator()(Generator& g) -> result_type
    {
        result_type uint;
        for (auto i = 0U; i < uint.word_count(); ++i)
        {
            uint.set_word(i, random_word(g));
        }
        // Modulo is slightly biased towards smaller numbers. Possible fix: e.g. use "Java's
        // algorithm.
        using namespace aarith::integer_operators;
        return the_params.min + (uint % length);
    }

    void reset()
    {
        random_word.reset();
    }

    auto param() const -> param_type
    {
        return the_params;
    }

    void param(const param_type& params)
    {
        if (params.min > params.max)
        {
            throw std::logic_error{"uniform_uinteger_distribution: a must be <= b"};
        }
        the_params = params;
        using namespace aarith::integer_operators;
        length = the_params.min - the_params.max;
    }

    auto min() const -> result_type
    {
        return the_params.min;
    }

    auto max() const -> result_type
    {
        return the_params.max;
    }

private:
    param_type the_params;
    result_type length;
    std::uniform_int_distribution<WordType> random_word{std::numeric_limits<WordType>::min(),
                                                         std::numeric_limits<WordType>::max()};
};


} // namespace aarith
