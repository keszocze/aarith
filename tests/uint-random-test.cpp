#include "aarith/operations/exact_operations.hpp"
#include "aarith/types/uinteger.hpp"
#include "aarith/utilities/string_utils.hpp"
#include <catch.hpp>
#include <iostream>
#include <mpir.h>
#include <random>

using namespace aarith;

template <size_t BitWidth>
class UIntegerGenerator : public Catch::Generators::IGenerator<uinteger<BitWidth>>
{
public:
    explicit UIntegerGenerator(size_t min_length, size_t max_length)
        : rng{std::random_device{}()}
        , random_word_count{min_length, max_length}
    {
    }

    auto get() const -> const uinteger<BitWidth>& override
    {
        return current_number;
    }

    bool next() override
    {
        auto const word_count = random_word_count(rng);
        for (auto i = 0U; i < word_count; ++i)
        {
            current_number.set_word(i, random_word(rng));
        }
        return true;
    }

private:
    std::minstd_rand rng;
    std::uniform_int_distribution<size_t> random_word_count;
    std::uniform_int_distribution<typename uinteger<BitWidth>::word_type> random_word{
        1, std::numeric_limits<uint64_t>::max()};
    uinteger<BitWidth> current_number;
};

template <size_t BitWidth>
auto random_uinteger(size_t min_length, size_t max_length)
    -> Catch::Generators::GeneratorWrapper<uinteger<BitWidth>>
{
    return Catch::Generators::GeneratorWrapper<uinteger<BitWidth>>(
        std::unique_ptr<Catch::Generators::IGenerator<uinteger<BitWidth>>>(
            new UIntegerGenerator<BitWidth>(min_length, max_length)));
}

template <size_t BitWidth>
bool is_uint_add_correct(const uinteger<BitWidth>& lhs, const uinteger<BitWidth>& rhs,
                         const uinteger<BitWidth>& result)
{
    mpz_t a_mpir, b_mpir, result_mpir, mask_mpir;
    mpz_init_set_str(a_mpir, to_binary(lhs).c_str(), 2);
    mpz_init_set_str(b_mpir, to_binary(rhs).c_str(), 2);
    mpz_add(a_mpir, a_mpir, b_mpir);
    mpz_init_set_str(mask_mpir, std::string(BitWidth, '1').c_str(), 2);
    mpz_and(a_mpir, a_mpir, mask_mpir);
    mpz_init_set_str(result_mpir, to_binary(result).c_str(), 2);
    const bool success = mpz_cmp(a_mpir, result_mpir) == 0;
    mpz_clear(a_mpir);
    mpz_clear(b_mpir);
    mpz_clear(result_mpir);
    mpz_clear(mask_mpir);
    return success;
}

template <size_t BitWidth> void test_uint_add()
{
    auto const lhs =
        GENERATE(take(4, random_uinteger<BitWidth>(1, uinteger<BitWidth>::word_count())));
    auto const rhs =
        GENERATE(take(4, random_uinteger<BitWidth>(1, uinteger<BitWidth>::word_count())));
    auto const result = add(lhs, rhs);
    REQUIRE(is_uint_add_correct(lhs, rhs, result));
}

/*
static constexpr size_t MinimumBitWidth = 16;
static constexpr size_t MaximumBitWidth = 16;
static constexpr size_t BitWidthStep = 6;

template <size_t BitWidth> void test_recursively()
{
    test_uint_add<BitWidth>();
    if constexpr (BitWidth < MaximumBitWidth)
    {
        test_recursively<BitWidth + BitWidthStep>();
    }
}
*/

#define TestBitWidths uinteger<16>, uinteger<21>, uinteger<24>, uinteger<32>, uinteger<192>

TEMPLATE_TEST_CASE("Adding randomly generated uintegers", "[uinteger][arithmetic][random]",
                   TestBitWidths)
{
    // test_recursively<MinimumBitWidth>();
    WHEN("Adding uintegers of width " + std::to_string(TestType::width()))
    {
        test_uint_add<TestType::width()>();
    }
}
