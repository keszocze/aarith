#include "int_correctness_check_fun.hpp"

int main()
{
    using namespace std;
    using namespace aarith;

    constexpr size_t Width = 32;
    using B = int32_t;
    using WordType = uint64_t;
    using I = integer<Width, uint64_t>;

    check_int_operation<integer, Width, B, true, WordType>("division", &::aarith::div<I>,
                                                           native_div);

    return 0;
}