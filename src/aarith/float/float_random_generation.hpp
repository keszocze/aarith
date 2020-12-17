#pragma once

#include <aarith/integer.hpp>

#include <random>
#include <stdexcept>

namespace aarith {

/**
 * @brief Different modes for generating normalized_floats
 */
enum class FloatGenerationModes
{
    /**
     * @brief Generates normalized numbers only.
     *
     * This does *not* include zero, NaN and +/- infinity.
     *
     * @warning This mode does *not* generate the number zero!
     */
    NormalizedOnly,

    /**
     * @brief Generates denormalized numbers only.
     *
     * This does *not* include NaN and +/- infinity.
     *
     * @note This mode can generate the number zero
     */
    DenormalizedOnly,

    /**
     * @brief Generates normalized numbers and NaN and +/- infinity
     */
    NormalizedAndSpecial,

    /**
     * @brief Generates normalized and denormalized numbers excluding NaN and +/- infinity
     */
    NonSpecial,

    /**
     * @brief Generates fully random exponents.
     *
     * The generated numbers include
     *  - normalized numbers
     *  - denormalized numbers
     *  - Not a number
     *  - +/- infinity
     */
    FullyRandom
};

/**
 * Implements random number generation interface. There is absolutely no guarantee on the type of
 * the distribution whatsoever!
 */
template <size_t E, size_t M, FloatGenerationModes Mode = FloatGenerationModes::NonSpecial,
          typename WordType = uint64_t>
class normalized_float_distribution
{
public:
    using F = normalized_float<E, M, WordType>;
    using IntExp = typename F::IntegerExp;
    using IntMant = typename F::IntegerMant;

    explicit normalized_float_distribution()
    {
    }

    template <typename Generator> auto operator()(Generator& g) -> F
    {

        IntExp e{IntExp::zero()};

        if constexpr (Mode == FloatGenerationModes::FullyRandom)
        {
            e = random_exp(g);
        }
        else if constexpr (Mode == FloatGenerationModes::NormalizedOnly)
        {
            e = random_norm_non_special_exp(g);
        }
        else if constexpr (Mode == FloatGenerationModes::NonSpecial)
        {
            e = random_non_special_exp(g);
        }
        else if constexpr (Mode == FloatGenerationModes::NormalizedAndSpecial)
        {
            e = random_norm_exp(g);
        }
        else
        {
            // Mode == FloatGenerationModes::DenormalizedOnly
            // nothing needs to be done here as e already is zero
        }

        IntMant m = random_mant(g);
        bool sign = random_sign(g);

        return F{sign, e, m};
    }

private:
    static constexpr IntExp exp_almost_max = sub(IntExp::max(), IntExp::one());

    uniform_uinteger_distribution<E, WordType> random_norm_non_special_exp{IntExp::one(),
                                                                           exp_almost_max};
    uniform_uinteger_distribution<E, WordType> random_non_special_exp{IntExp::zero(),
                                                                      exp_almost_max};
    uniform_uinteger_distribution<E, WordType> random_norm_exp{IntExp::one(), IntExp::max()};
    uniform_uinteger_distribution<E, WordType> random_exp{IntExp::zero(), IntExp::max()};
    uniform_uinteger_distribution<M, WordType> random_mant;
    std::bernoulli_distribution random_sign;
};
} // namespace aarith