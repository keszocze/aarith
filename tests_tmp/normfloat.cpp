
#include <cstdint>
#include <iostream>
#include "aarith/operations/approx_operations.hpp"
#include "aarith/operations/exact_operations.hpp"
#include "aarith/types/normfloat.hpp"
#include "aarith/types/integer.hpp"
#include <aarith/operations/comparisons.hpp>
#include <aarith/utilities/string_utils.hpp>


int main(){

    using namespace aarith;
    
    const double number_a = 0.5;

    const normfloat<8,24> nf(number_a);

    std::cout << to_binary(nf);
    std::cout << std::endl;
    std::cout << to_hex(nf);
    std::cout << std::endl;
    std::cout << to_compute_string(nf);
    std::cout << std::endl;
    std::cout << to_sci_string(nf);
    std::cout << std::endl;
    std::cout << std::endl;

    const normfloat<2,2> nfs(number_a);

    std::cout << to_binary(nfs);
    std::cout << std::endl;
    std::cout << to_hex(nfs);
    std::cout << std::endl;
    std::cout << to_compute_string(nfs);
    std::cout << std::endl;
    std::cout << to_sci_string(nfs);
    std::cout << std::endl;
    std::cout << std::endl;

    const normfloat<64,64> nfc(number_a);

    std::cout << to_binary(nfc);
    std::cout << std::endl;
    std::cout << to_hex(nfc);
    std::cout << std::endl;
    std::cout << to_compute_string(nfc);
    std::cout << std::endl;
    std::cout << to_sci_string(nfc);
    std::cout << std::endl;
    std::cout << std::endl;

    const normfloat<93,204> nfl(number_a);

    std::cout << to_binary(nfl);
    std::cout << std::endl;
    std::cout << to_hex(nfl);
    std::cout << std::endl;
    std::cout << to_compute_string(nfl);
    std::cout << std::endl;
    std::cout << to_sci_string(nfl);
    std::cout << std::endl;
    std::cout << std::endl;

    const auto addresult = exact_nfloat_add(nf, nf);
    std::cout << addresult << std::endl;

    /*
    //auto bcdtest = uinteger<128>::from_words(8999999999999999999U, 1234567890123456789U);
    auto bcdtest = uinteger<128>::from_words(1U, 1234567890123456789U);
    std::cout << bcdtest << " == " << std::endl;
    print_uint(std::cout, bcdtest);
    std::cout << std::endl;

    auto shiftrtest = uinteger<32>(4U);
    std::cout << shiftrtest << " " << (shiftrtest >> 1) << " " << (shiftrtest >> 2) << std::endl;

    auto shiftrtest2 = uinteger<192>::from_words(0xC000000000000000, 0U, 0U);
    std::cout << shiftrtest2 << std::endl;
    for(auto i = 0U; i < 192; ++i)
    {
        std::cout << to_binary(shiftrtest2 >> i, 64);
        std::cout << std::endl;
    }
    */

    return 0;
}
