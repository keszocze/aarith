
#include <cstdint>
#include <iostream>
#include "aarith/operations/approx_operations.hpp"
#include "aarith/operations/exact_operations.hpp"
#include "aarith/types/integer.hpp"
#include <aarith/operations/comparisons.hpp>


int main(){

    using namespace aarith;
    
    static constexpr size_t TestWidth = 8;
    static_assert(uinteger<TestWidth>::word_count() == 1);
    static constexpr uint8_t number_a = 170U;
    static constexpr uint8_t number_b = 255U;

    const uinteger<TestWidth> a{number_a};
    const uinteger<TestWidth> b{number_b};
    auto const result = approx_uint_bitmasking_mul(a, b, 16U);

    return 0;
}
