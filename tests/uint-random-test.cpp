#include "aarith/operations/exact_operations.hpp"
#include "aarith/types/integer.hpp"
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
    mpz_init_set_str(a_mpir, to_string(lhs).c_str(), 2);
    mpz_init_set_str(b_mpir, to_string(rhs).c_str(), 2);
    mpz_add(a_mpir, a_mpir, b_mpir);
    mpz_init_set_str(mask_mpir, std::string(BitWidth, '1').c_str(), 2);
    mpz_and(a_mpir, a_mpir, mask_mpir);
    mpz_init_set_str(result_mpir, to_string(result).c_str(), 2);
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
        GENERATE(take(8, random_uinteger<BitWidth>(1, uinteger<BitWidth>::word_count())));
    auto const rhs =
        GENERATE(take(8, random_uinteger<BitWidth>(1, uinteger<BitWidth>::word_count())));
    auto const result = exact_uint_add(lhs, rhs);

    SECTION(to_string(lhs) + " + " + to_string(rhs) + " = " + to_string(result))
    {
        REQUIRE(is_uint_add_correct(lhs, rhs, result));
    }
}

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

SCENARIO("Adding randomly generated uintegers", "[uinteger][arithmetic][random]")
{
    //test_recursively<MinimumBitWidth>();
    test_uint_add<16>();
}

/*
template <size_t BitWidth, class Rng> class uinteger_random_generator
{
public:
    auto generate(Rng& rng) -> uinteger<BitWidth>
    {
        uinteger<BitWidth> a;
        auto const word_count = random_word_count(rng);
        for (auto i = 0U; i < word_count; ++i)
        {
            a.set_word(i, random_word(rng));
        }
        return a;
    }

private:
    std::uniform_int_distribution<size_t> random_word_count{1, uinteger<BitWidth>::word_count()};
    std::uniform_int_distribution<uint64_t> random_word{1, std::numeric_limits<uint64_t>::max()};
};


static constexpr auto numbers_per_width(size_t) -> size_t
{
    return 10;
}

// Actual testing takes place here.
template <size_t BitWidth, class Rng> int test_width(Rng& rng)
{
    bool success{true};
    for (auto i = 0U; i < numbers_per_width(BitWidth); ++i)
    {
        uinteger_random_generator<BitWidth, Rng> random_uinteger;
        auto const a = random_uinteger.generate(rng);
        auto const b = random_uinteger.generate(rng);
        auto const result = exact_uint_add(a, b);

        // Test result against MPIR, a well-tested arbitrary precision library.
        mpz_t a_mpir, b_mpir, result_mpir;
        mpz_init_set_str(a_mpir, to_string(a).c_str(), 2);
        mpz_init_set_str(b_mpir, to_string(b).c_str(), 2);
        mpz_add(a_mpir, a_mpir, b_mpir);
        mpz_init_set_str(result_mpir, to_string(result).c_str(), 2);
        success = (mpz_cmp(a_mpir, result_mpir) == 0) && success;
    }
    return success;
}

template <size_t BitWidth, class Rng> int test_widths_recursive(Rng& rng)
{
    auto const returnValue = test_width<BitWidth>(rng);
    if constexpr (BitWidth < MaximumBitWidth)
    {
        return test_widths_recursive<BitWidth + 1>(rng) && returnValue;
    }
    else
    {
        return returnValue;
    }
}

int main()
{
    std::random_device dev;
    std::mt19937_64 rng(dev());
    return test_widths_recursive<MinimumBitWidth>(rng);
}
*/