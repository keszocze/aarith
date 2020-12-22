#pragma once

#include <aarith/float.hpp>
#include <catch.hpp>

namespace aarith {

template <size_t E, size_t M, FloatGenerationModes Mode = FloatGenerationModes::NonSpecial,
          typename WordType = uint64_t>
class FloatGenerator : public Catch::Generators::IGenerator<floating_point<E, M, WordType>>
{
public:
    using F = floating_point<E, M, WordType>;

    explicit FloatGenerator()
        : rng{std::random_device{}()}
        , random_number()
        , current_number(random_number(rng))
    {
    }

    auto get() const -> const F& override
    {
        return current_number;
    }

    bool next() override
    {
        current_number = random_number(rng);
        return true;
    }

private:
    std::minstd_rand rng;
    floating_point_distribution<E, M, Mode, WordType> random_number;
    F current_number;
};

template <size_t E, size_t M, FloatGenerationModes Mode = FloatGenerationModes::NonSpecial,
          typename WordType = uint64_t>
auto random_float() -> Catch::Generators::GeneratorWrapper<floating_point<E, M, WordType>>
{
    return Catch::Generators::GeneratorWrapper<floating_point<E, M, WordType>>(
        std::unique_ptr<Catch::Generators::IGenerator<floating_point<E, M, WordType>>>(
            new FloatGenerator<E, M, Mode, WordType>()));
}

} // namespace aarith