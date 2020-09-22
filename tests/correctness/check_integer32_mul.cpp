#include "int_correctness_check_fun.hpp"

int main()
{
    using namespace std;
    using namespace aarith;

    constexpr size_t Width = 32;
    using B = int32_t;
    using WordType = uint64_t;

    check_int_operation<integer, Width, B, false, WordType>("mul_booth", &::aarith::naive_mul<Width, WordType>, native_mul);
    check_int_operation<integer, Width, B, false, WordType>("mul_booth", &::aarith::booth_mul<Width, WordType>, native_mul);
    check_int_operation<integer, Width, B, false, WordType>("mul_booth", &::aarith::booth_inplace_mul<Width, WordType>, native_mul);

    return 0;
}