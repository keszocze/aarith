
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
    const double number_b = 0.25;

    const normfloat<8,24> nf(number_a);
    const normfloat<8,24> nb(number_b);

    std::array<float, 6> farr = {0.5f, 0.25f, 0.125f, -0.5f, -0.25f, -0.125f};
    std::array<normfloat<8, 24>, 6> nfarr;
    for(auto i = 0U; i < nfarr.size(); ++i)
    {
        nfarr[i] = normfloat<8, 24>(farr[i]);
    }

    std::cout << to_binary(nf)
              << std::endl
              << to_hex(nf)
              << std::endl
              << to_compute_string(nf)
              << std::endl
              << to_sci_string(nf)
              << std::endl
              << std::endl;

    const normfloat<2,2> nfs(number_a);

    std::cout << to_binary(nfs)
              << std::endl
              << to_hex(nfs)
              << std::endl
              << to_compute_string(nfs)
              << std::endl
              << to_sci_string(nfs)
              << std::endl
              << std::endl;

    const normfloat<64,64> nfc(number_a);

    std::cout << to_binary(nfc)
              << std::endl
              << to_hex(nfc)
              << std::endl
              << to_compute_string(nfc)
              << std::endl
              << to_sci_string(nfc)
              << std::endl
              << std::endl;

    const normfloat<93,204> nfl(number_a);

    std::cout << to_binary(nfl)
              << std::endl
              << to_hex(nfl)
              << std::endl
              << to_compute_string(nfl)
              << std::endl
              << to_sci_string(nfl)
              << std::endl
              << std::endl;

    const auto addresult = exact_nfloat_add(nf, nb);
    std::cout << "sum " << addresult << std::endl;


    const auto subresult = exact_nfloat_sub(nf, nb);
    std::cout << "sum " << subresult << std::endl;
    
    for(auto i = 0; i < nfarr.size(); ++i)
    {
        for(auto j = 0; j < nfarr.size(); ++j)
        {
            const auto sum = exact_nfloat_add(nfarr[i], nfarr[j]);
            const auto fsum = farr[i] + farr[j];
            std::cout << nfarr[i] << " + " << nfarr[j] << " = " << sum << ((normfloat<8, 24>(fsum) == sum)?(""):(" (wrong)")) << "\n";
            if(! (normfloat<8, 24>(fsum) == sum))
            {
                std::cout << to_binary(sum) << "\n"
                          << to_binary(normfloat<8, 24>(fsum)) << "\n";
            }
            const auto negsum = exact_nfloat_sub(nfarr[i], nfarr[j]);
            const auto fnegsum = farr[i] - farr[j];
            std::cout << nfarr[i] << " - " << nfarr[j] << " = " << negsum << ((normfloat<8, 24>(fnegsum) == negsum)?(""):(" (wrong)")) << "\n";
            if(! (normfloat<8, 24>(fnegsum) == negsum))
            {
                std::cout << to_binary(negsum) << "\n"
                          << to_binary(normfloat<8, 24>(fnegsum)) << "\n";
            }
            std::cout << "\n";
        }
    }

    std::cout << to_binary(normfloat<8, 24>(0.0f)) << "\n";

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
