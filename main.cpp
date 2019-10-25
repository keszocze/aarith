#include <iostream>

#include <aarith/types/integer.hpp>
#include <aarith/utilities/bit_operations.hpp>
#include <bitset>


int main() {

    uint64_t n = 1u <<30;
    n += 0b10101 << 5;
    n = n << 10;

    std::cout << std::bitset<64>(n) << "\n";

    auto [upper,lower] = aarith::split(n);

    std::cout << std::bitset<32>(upper) << std::bitset<32>(lower) << "\n";


    return 0;
}