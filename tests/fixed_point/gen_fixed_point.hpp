#pragma once

#include <aarith/fixed_point.hpp>
#include <catch.hpp>

namespace aarith {

template <size_t I, size_t F, template <size_t, size_t, typename> typename FixedType,
          typename WordType = uint64_t>
class FixedPointGenerator : public Catch::Generators::IGenerator<FixedType<I, F, WordType>>
{
public:
    using Fixed = FixedType<I, F, WordType>;

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
    uniform_fixed_point_distribution<I, F, FixedType, WordType> random_number;
    Fixed current_number;
};

template <size_t I, size_t F, template <size_t, size_t, typename> typename FixedType,
          typename WordType = uint64_t>
auto random_fixed_point() -> Catch::Generators::GeneratorWrapper<FixedType<I, F, WordType>>
{
    return Catch::Generators::GeneratorWrapper<FixedType<I, F, WordType>>(
        std::unique_ptr<Catch::Generators::IGenerator<FixedType<I, F, WordType>>>(
            new FixedPointGenerator<I, F, FixedType, WordType>()));
}

} // namespace aarith