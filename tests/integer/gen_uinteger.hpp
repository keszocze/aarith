#pragma once

#include <aarith/integer_no_operators.hpp>
#include <catch.hpp>

namespace aarith {

template <size_t BitWidth, typename WordType = uint64_t>
class UIntegerGenerator : public Catch::Generators::IGenerator<uinteger<BitWidth, WordType>>
{
public:
    using I = uinteger<BitWidth, WordType>;


    explicit UIntegerGenerator(const I& min_val= I::min(), const I& max_val=I::max())
        : rng{std::random_device{}()}
        , random_number(min_val, max_val)
        , current_number(random_number(rng))
    {
    }

    auto get() const -> const uinteger<BitWidth, WordType>& override
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
    uniform_uinteger_distribution<BitWidth, WordType> random_number;
    uinteger<BitWidth, WordType> current_number;
};

template <size_t BitWidth, typename WordType = uint64_t>
auto random_uinteger(const uinteger<BitWidth, WordType>& min_val,
                     const uinteger<BitWidth, WordType>& max_val)
    -> Catch::Generators::GeneratorWrapper<uinteger<BitWidth, WordType>>
{
    return Catch::Generators::GeneratorWrapper<uinteger<BitWidth, WordType>>(
        std::unique_ptr<Catch::Generators::IGenerator<uinteger<BitWidth, WordType>>>(
            new UIntegerGenerator<BitWidth, WordType>(min_val, max_val)));
}

template <size_t BitWidth, typename WordType = uint64_t>
auto random_uinteger() {
    using I = uinteger<BitWidth, WordType>;
    return random_uinteger<BitWidth,WordType>(I::min(), I::max());
}

} // namespace aarith