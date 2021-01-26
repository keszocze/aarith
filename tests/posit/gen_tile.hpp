#pragma once

#include <aarith/posit.hpp>
#include <catch.hpp>

#include "gen_posit.hpp"
#include "gen_bool.hpp"

namespace aarith {

template <typename TileType>
class TileGenerator : public Catch::Generators::IGenerator<TileType>
{
public:
    explicit TileGenerator()
    {
        next();
    }

    auto get() const -> const TileType& override
    {
        return current_tile;
    }

    bool next() override
    {
        if (!posit_generator.next())
        {
            return false;
        }

        if (!bool_generator.next())
        {
            return false;
        }

        const auto& p = posit_generator.get();
        const auto& u = bool_generator.get();
        current_tile = TileType::from(p, u);

        return true;
    }

private:
    using Posit = typename TileType::posit_type;

    PositGenerator<Posit> posit_generator;
    BoolGenerator bool_generator;
    TileType current_tile;
};

template <typename TileType>
Catch::Generators::GeneratorWrapper<TileType> random_tile()
{
    return Catch::Generators::GeneratorWrapper<TileType>(
        std::unique_ptr<Catch::Generators::IGenerator<TileType>>(
            new TileGenerator<TileType>()));
}

} // namespace aarith
