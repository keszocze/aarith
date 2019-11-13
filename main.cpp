#include <iostream>
#include <bitset>
#include <aarith/types/integer.hpp>
#include <aarith/operations/exact_operations.hpp>

//#include <integer.hpp>

int main() {

    using namespace aarith;
    using namespace std;

    uint64_t a =1u;
    a = a << 35;
    auto [a_upper, a_lower] = split(a);

    uinteger<64> a_int{a};
    uinteger<64> a_upper_int{a_upper};
    uinteger<64> a_lower_int{a_lower};


    cout << a_int << "\n" << a_upper_int << "\n" << a_lower_int << "\n";

    return 0;
}
