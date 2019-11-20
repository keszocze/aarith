#include <iostream>
#include <bitset>
#include <aarith/types/integer.hpp>
#include <aarith/operations/exact_operations.hpp>
#include <aarith/utilities/string_utils.hpp>


int main() {

    using namespace aarith;

    static constexpr uint8_t number_a1 = 20;
    static constexpr uint8_t number_b1 = 100;

    static constexpr uint8_t num_res = number_a1 - number_b1;

    static constexpr size_t TestWidth1 = 8;

    const uinteger<TestWidth1> a1{number_a1};
    const uinteger<TestWidth1> b1{number_b1};

    auto const result = exact_uint_sub(a1, b1);

    std::cout << result << "\t" << result.word_mask(0) << "\t" << uinteger<64>{result.word(0)} << "\t" << num_res << "\n";


    return 0;
}
