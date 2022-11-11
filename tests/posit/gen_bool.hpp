#pragma once

#include <catch.hpp>

#include <aarith/posit.hpp>

namespace aarith {

class BoolGenerator : public Catch::Generators::IGenerator<bool>
{
public:
    explicit BoolGenerator()
        : rng{std::random_device{}()}
        , distribution(Storage::zero(), Storage::all_ones())
    {
        next();
    }

    auto get() const -> const bool& override
    {
        return current_bool;
    }

    bool next() override
    {
        current_number = distribution(rng);
        current_bool = current_number.lsb();
        return true;
    }

private:
    using Storage = uinteger<64, uint64_t>;

    std::minstd_rand rng;
    uniform_uinteger_distribution<64, uint64_t> distribution;
    Storage current_number;
    bool current_bool;
};

Catch::Generators::GeneratorWrapper<bool> random_bool()
{
    return Catch::Generators::GeneratorWrapper<bool>(
        std::unique_ptr<Catch::Generators::IGenerator<bool>>(new BoolGenerator()));
}

} // namespace aarith
