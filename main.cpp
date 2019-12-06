#include <aarith/operations/exact_operations.hpp>
#include <aarith/types/integer.hpp>
#include <bitset>
#include <iostream>

int main()
{
    using namespace aarith;
    std::cout << (std::numeric_limits<uinteger<13>>::is_exact ? "T" : "F") << std::endl;
    return 0;
}
