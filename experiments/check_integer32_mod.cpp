#include "int_correctness_check_fun.hpp"

int main()
{
    using namespace std;
    using namespace aarith;

    constexpr size_t Width = 32;
    using UB = uint32_t;
    using B = int32_t;
    using WordType = uint64_t;

    using UI = uinteger<Width, WordType>;
    using I = integer<Width, WordType>;

    check_int_operation<integer, Width, B, true, WordType>("modulo", &::aarith::remainder<I>, native_mod);

    return 0;
}