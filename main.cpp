#include <aarith/float.hpp>
#include <aarith/integer.hpp>
#include <iostream>
#include <numeric>
#include <vector>


int main()
{
    using namespace aarith;
    uint64_t a = 10, b=20;
    uinteger<64> a_=10, b_=20;
    std::cout << "a+b=" << (a+b) << "\n";
    std::cout << "a_+b_=" << (a_+b_) << "\n";

    return 0;
}
