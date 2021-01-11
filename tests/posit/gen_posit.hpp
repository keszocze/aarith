#pragma once

#include <aarith/posit.hpp>
#include <catch.hpp>

namespace aarith {

template <typename PositType>
class PositGenerator : public Catch::Generators::IGenerator<PositType>
{
public:
    using Storage = typename PositType::storage_type;

    explicit PositGenerator()
        : rng{std::random_device{}()}
        , random_number(Storage::zero(), Storage::all_ones())
        , current_number(random_number(rng))
    {
        current_posit = PositType::from(current_number);
    }

    auto get() const -> const PositType& override
    {
        return current_posit;
    }

    bool next() override
    {
        current_number = random_number(rng);
        current_posit = PositType::from(current_number);
        return true;
    }

private:
    static constexpr size_t N = PositType::width();
    using WT = typename PositType::word_type;

    std::minstd_rand rng;
    uniform_uinteger_distribution<N, WT> random_number;
    Storage current_number;
    PositType current_posit;
};

template <typename PositType>
Catch::Generators::GeneratorWrapper<PositType> random_posit()
{
    return Catch::Generators::GeneratorWrapper<PositType>(
        std::unique_ptr<Catch::Generators::IGenerator<PositType>>(
            new PositGenerator<PositType>()));
}

} // namespace aarith
