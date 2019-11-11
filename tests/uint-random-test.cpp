#include "aarith/operations/exact_operations.hpp"
#include "aarith/types/integer.hpp"
#include <iostream>
#include <mpir.h>
#include <random>

using namespace aarith;

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

static constexpr size_t MaximumBitWidth = 256;

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
    return test_widths_recursive<1>(rng);
}