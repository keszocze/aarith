#pragma once

#include <aarith/fixed_point.hpp>
#include <catch.hpp>

namespace aarith {

template <size_t I, size_t F, template <size_t, typename> class BaseInt,
          typename WordType = uint64_t>
class FixedPointGenerator : public Catch::Generators::IGenerator<fixed<I, F, BaseInt, WordType>>
{
public:
    using Fixed = fixed<I, F, BaseInt, WordType>;

    explicit FixedPointGenerator()
        : rng{std::random_device{}()}
        , random_number()
        , current_number(random_number(rng))
    {
    }

    auto get() const -> const Fixed& override
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
    uniform_fixed_point_distribution<I, F, BaseInt, WordType> random_number;
    Fixed current_number;
};

template <size_t I, size_t F, template <size_t, typename> class BaseInt,
          typename WordType = uint64_t>
auto random_fixed_point() -> Catch::Generators::GeneratorWrapper<fixed<I, F, BaseInt, WordType>>
{
    return Catch::Generators::GeneratorWrapper<fixed<I, F, BaseInt, WordType>>(
        std::unique_ptr<Catch::Generators::IGenerator<fixed<I, F, BaseInt, WordType>>>(
            new FixedPointGenerator<I, F, BaseInt, WordType>()));
}

} // namespace aarith