#pragma once

#include <aarith/core.hpp>
#include <catch.hpp>

namespace aarith {

template <size_t BitWidth, typename WordType = uint64_t>
class WordArrayGenerator : public Catch::Generators::IGenerator<word_array<BitWidth, WordType>>
{
public:
    using I = word_array<BitWidth, WordType>;

    explicit WordArrayGenerator()
        : rng{std::random_device{}()}
        , random_array()
        , current_array(random_array(rng))
    {
    }

    auto get() const -> const I& override
    {
        return current_array;
    }

    bool next() override
    {
        current_array = random_array(rng);
        return true;
    }

private:
    std::minstd_rand rng;
    uniform_word_array_distribution<BitWidth, WordType> random_array;
    I current_array;
};

template <size_t BitWidth, typename WordType = uint64_t>
auto random_word_array() -> Catch::Generators::GeneratorWrapper<word_array<BitWidth, WordType>>
{
    return Catch::Generators::GeneratorWrapper<word_array<BitWidth, WordType>>(
        std::unique_ptr<Catch::Generators::IGenerator<word_array<BitWidth, WordType>>>(
            new WordArrayGenerator<BitWidth, WordType>()));
}

} // namespace aarith