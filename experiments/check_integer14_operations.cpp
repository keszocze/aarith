#include "int_correctness_check_fun.hpp"


template <typename WordType>
void helper() {
    using namespace std;
    using namespace aarith;

    constexpr size_t Width = 14;
    using UB = uint16_t;
    using B = int16_t;

    using UI = uinteger<Width, WordType>;
    using I = integer<Width, WordType>;

    check_int_operation<uinteger, Width, UB, false,WordType>("addition", &::aarith::add<UI>, native_add);
    check_int_operation<uinteger, Width, UB, false, WordType>("subtraction", &::aarith::sub<UI>, native_sub);
    check_int_operation<uinteger, Width, UB, false, WordType>("mul", &::aarith::mul<UI>, native_mul);
    check_int_operation<uinteger, Width, UB, false, WordType>(
        "mul_karazuba", &::aarith::karazuba<Width, WordType>, native_mul);
    check_int_operation<uinteger, Width, UB, true, WordType>("division", &::aarith::div<UI>, native_div);
    check_int_operation<uinteger, Width, UB, true, WordType>("modulo", &::aarith::remainder<UI>, native_mod);

    check_int_operation<integer, Width, B, false, WordType>("addition", &::aarith::add<I>, native_add);
    check_int_operation<integer, Width, B, false, WordType>("subtraction", &::aarith::sub<I>, native_sub);
    check_int_operation<integer, Width, B, false, WordType>("mul", &::aarith::mul<I>, native_mul);
    check_int_operation<integer, Width, B, false, WordType>("mul_naive", &::aarith::naive_mul<Width, WordType>, native_mul);
    check_int_operation<integer, Width, B, false, WordType>("mul_inplace", &::aarith::inplace_mul<Width, WordType>, native_mul);
    check_int_operation<integer, Width, B, true, WordType>("division", &::aarith::div<I>, native_div);
    check_int_operation<integer, Width, B, true, WordType>("modulo", &::aarith::remainder<I>, native_mod);
}


int main()
{


    helper<uint16_t>();
    helper<uint32_t>();
    helper<uint64_t>();

    return 0;
}