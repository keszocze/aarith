#include <iostream>
#include <bitset>
#include <aarith/types/integer.hpp>
#include <aarith/operations/exact_operations.hpp>
#include <aarith/utilities/string_utils.hpp>

//#include <integer.hpp>

int main() {

    using namespace aarith;
    using namespace std;

    const uinteger<64> n =uinteger<64>::from_words(static_cast<uint64_t>(-1));
    cout << n << "\n";
    cout << prepend_zero_word(n) << "\n";
    return 0;
}
