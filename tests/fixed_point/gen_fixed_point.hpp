#pragma once

#include <aarith/fixed_point.hpp>
#include <catch.hpp>

namespace aarith {

// TODO SFINAE it!
template <typename FixedPoint>
class FixedPointGenerator : public Catch::Generators::IGenerator<FixedPoint>
{
public:

    explicit FixedPointGenerator()
        : rng{std::random_device{}()}
        , random_number()
        , current_number(random_number(rng))
    {
    }

    auto get() const -> const FixedPoint& override
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
    uniform_fixed_point_distribution<FixedPoint> random_number;
    FixedPoint current_number;
};


template <typename FixedPoint>
auto random_fixed_point() -> Catch::Generators::GeneratorWrapper<FixedPoint>
{
    return Catch::Generators::GeneratorWrapper<FixedPoint>(
        std::unique_ptr<Catch::Generators::IGenerator<FixedPoint>>(
            new FixedPointGenerator<FixedPoint>()));
}




} // namespace aarith