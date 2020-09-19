#include "../test-signature-ranges.hpp"
#include "gen_uinteger.hpp"
#include <aarith/integer_no_operators.hpp>

#include <catch.hpp>
#include <iostream>
#include <mpir.h>

using namespace aarith;

//template<typename I>
//bool operator==(const I integer, const mpz_t & m) {
//    mpz_t tmp;
//    mpz_init_set_str(tmp, to_binary(integer).c_str(), 2);
//    const bool success = (mpz_cmp(tmp, m) == 0);
//    mpz_clear(tmp);
//    return success;
//}

template <size_t BitWidth, typename WordType>
bool is_uint_add_correct(const uinteger<BitWidth, WordType>& lhs, const uinteger<BitWidth, WordType>& rhs,
                         const uinteger<BitWidth, WordType>& result)
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

template <size_t BitWidth, typename WordType>
bool is_uint_mul_correct(const uinteger<BitWidth, WordType>& lhs, const uinteger<BitWidth, WordType>& rhs,
                         const uinteger<BitWidth, WordType>& result)
{
    mpz_t a_mpir, b_mpir, result_mpir, mask_mpir;
    mpz_init_set_str(a_mpir, to_binary(lhs).c_str(), 2);
    mpz_init_set_str(b_mpir, to_binary(rhs).c_str(), 2);
    mpz_mul(a_mpir, a_mpir, b_mpir);
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

template <size_t BitWidth, typename WordType> void test_uint_add()
{
    auto const lhs =
        GENERATE(take(4, random_uinteger<BitWidth, WordType>()));
    auto const rhs =
        GENERATE(take(4, random_uinteger<BitWidth, WordType>()));
    auto const result = add(lhs, rhs);
    REQUIRE(is_uint_add_correct(lhs, rhs, result));
}

TEMPLATE_TEST_CASE_SIG("Large number addition match those of MPIR",
                       "[integer][unsigned][arithmetic][addition][mpir]", AARITH_TEST_SIGNATURE,
                       AARITH_TEST_TEMPLATE_LARGE_PARAM_RANGE)
{
    using I = uinteger<W, WordType>;
    const I lhs =
        GENERATE(take(4, random_uinteger<W, WordType>()));
    const I rhs =
        GENERATE(take(4, random_uinteger<W, WordType>()));
    const I result = add(lhs, rhs);
    REQUIRE(is_uint_add_correct<W, WordType>(lhs, rhs, result));

}

//TEMPLATE_TEST_CASE_SIG("Large number multiplication match those of MPIR",
//                       "[integer][signed][arithmetic][multiplication][mpir]", AARITH_INT_TEST_SIGNATURE,
//                       AARITH_INT_TEST_TEMPLATE_LARGE_PARAM_RANGE)
//{
//    using I = uinteger<W, WordType>;
//    const I lhs =
//        GENERATE(take(4, random_uinteger<W, WordType>()));
//    const I rhs =
//        GENERATE(take(4, random_uinteger<W, WordType>()));
//    const I result_school = schoolbook_mul(lhs, rhs);
//    const I result_kara = karazuba(lhs, rhs);
//    REQUIRE(is_uint_add_correct<W, WordType>(lhs, rhs, result_school));
//    REQUIRE(is_uint_add_correct<W, WordType>(lhs, rhs, result_kara));
//
//}