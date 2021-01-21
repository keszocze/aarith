#pragma once

#include <random>

namespace aarith {

/**
 * Implements random number generation interface similar to std::uniform_int_distribution.
 */
template <typename FixedPoint> class uniform_fixed_point_distribution
{
public:
    template <class Generator> auto operator()(Generator& g) -> FixedPoint
    {
        auto data = random_array(g);
        return FixedPoint::from_bitstring(data);
    }

    virtual void reset()
    {
        random_array.reset();
    }

private:
    static constexpr auto S = FixedPoint::width;
    using word_type = typename FixedPoint::word_type;
    uniform_word_array_distribution<S, word_type> random_array{};
};

} // namespace aarith