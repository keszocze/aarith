#pragma once

#include "aarith/types/integer.hpp"
#include "aarith/types/random.hpp"

#include <catch.hpp>


using namespace aarith;

template <size_t BitWidth>
class UIntegerGenerator : public Catch::Generators::IGenerator<uinteger<BitWidth>>
{
public:
    explicit UIntegerGenerator(size_t, size_t)
        : rng{std::random_device{}()}
    {
    }

    auto get() const -> const uinteger<BitWidth>& override
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
    uinteger<BitWidth> current_number;
    uniform_uinteger_distribution<BitWidth> random_number;
};

template <size_t BitWidth>
auto random_uinteger(size_t min_length, size_t max_length)
    -> Catch::Generators::GeneratorWrapper<uinteger<BitWidth>>
{
    return Catch::Generators::GeneratorWrapper<uinteger<BitWidth>>(
        std::unique_ptr<Catch::Generators::IGenerator<uinteger<BitWidth>>>(
            new UIntegerGenerator<BitWidth>(min_length, max_length)));
}
