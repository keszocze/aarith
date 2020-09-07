#pragma once

#include <random>
#include <stdexcept>

#include <aarith/integer.hpp>

namespace aarith {

/**
 * Implements random number generation interface similar to std::uniform_int_distribution.
 */
template <size_t BitWidth> class uniform_uinteger_distribution
{
public:
    using result_type = uinteger<BitWidth>;

    struct param_type
    {
        uinteger<BitWidth> a, b;
    };

    uniform_uinteger_distribution()
        : uniform_uinteger_distribution{uinteger<BitWidth>::zero()}
    {
    }

    explicit uniform_uinteger_distribution(const uinteger<BitWidth>& a,
                                           const uinteger<BitWidth>& b = uinteger<BitWidth>::max())
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
        return the_params.a + (uint % length);
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
        if (params.a > params.b)
        {
            throw std::logic_error{"uniform_uinteger_distribution: a must be <= b"};
        }
        the_params = params;
        using namespace aarith::integer_operators;
        length = the_params.b - the_params.a;
    }

    auto a() const -> result_type
    {
        return the_params.a;
    }

    auto b() const -> result_type
    {
        return the_params.b;
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
    uinteger<BitWidth> length;
    using word_type = typename uinteger<BitWidth>::word_type;
    std::uniform_int_distribution<word_type> random_word{std::numeric_limits<word_type>::min(),
                                                         std::numeric_limits<word_type>::max()};
};

} // namespace aarith