#include "int_correctness_check_fun.hpp"

int main()
{
    using namespace std;
    using namespace aarith;

    constexpr size_t Width = 24;
    using UB = uint32_t;
    using B = int32_t;

    using UI = uinteger<Width, uint64_t>;
    using I = integer<Width, uint64_t>;

    check_int_operation<uinteger, Width, UB, false>("addition", &::aarith::add<UI>, native_add);
    check_int_operation<uinteger, Width, UB, false>("subtraction", &::aarith::sub<UI>, native_sub);
    check_int_operation<uinteger, Width, UB, false>("mul", &::aarith::mul<UI>, native_mul);
    check_int_operation<uinteger, Width, UB, false>(
        "mul_karazuba", &::aarith::karazuba<Width, uint64_t>, native_mul);
    check_int_operation<uinteger, Width, UB, true>("division", &::aarith::div<UI>, native_div);
    check_int_operation<uinteger, Width, UB, true>("modulo", &::aarith::remainder<UI>, native_mod);

    check_int_operation<integer, Width, B, false>("addition", &::aarith::add<I>, native_add);
    check_int_operation<integer, Width, B, false>("subtraction", &::aarith::sub<I>, native_sub);
    check_int_operation<integer, Width, B, false>("mul", &::aarith::mul<I>, native_mul);
    check_int_operation<integer, Width, B, true>("division", &::aarith::div<I>, native_div);
    check_int_operation<integer, Width, B, true>("modulo", &::aarith::remainder<I>, native_mod);

    return 0;
}