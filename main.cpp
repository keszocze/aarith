#include <aarith/integer.hpp>
#include <bitset>
#include <iostream>

int main()
{
    using namespace aarith;

//    for (int i = 0; i < 50; ++i)
//    {
//
//        uint8_t u8 = 42;
//        uinteger<8> aarith_u8{u8};
//        std::cout << "uint8_t=" << u8 << " (as int=" << int{u8}
//                  << ") aarith::uinteger<8>=" << aarith_u8 << "\n";
//    }


    integer<8> larger = integer<4>{3};

    integer<16> smaller = integer<32>{2};
    integer<8> usmaller = uinteger<32>{1337};
//
//    std::cout << smaller << "\n";
//    std::cout << usmaller << "\n";

    int res = 1;
    for (int i=1;i<20;++i) {
        res *= i;
        std::cout << i << ": " << res << "\n";
    }



    return 0;

}
