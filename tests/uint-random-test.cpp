#include <aarith/integer.hpp>
#include "gen_uinteger.hpp"

#include <catch.hpp>
#include <iostream>
#include <mpir.h>

using namespace aarith;


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
