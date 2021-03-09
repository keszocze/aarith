#include <aarith/float.hpp>
#include <aarith/float/string_utils.hpp>
#include <cmath>

using namespace aarith;

template <class F, class FP> 
void compare_to_float(F base)
{
    F ref = base;
    FP sgl;
    do{
        sgl = FP(ref);
        std::cout 
            << "float:\n" 
            << ref << "\n"
            << to_binary(FP(ref)) << "\n"
            << "nfloat - single precision:\n" 
            << to_sci_string(sgl) 
            << "\n\n";
        
        ref *= base;
    }
    while(!sgl.is_inf());
    
    ref = base;
    do{
        sgl = FP(ref);
        std::cout 
            << "float:\n" 
            << ref << "\n"
            << to_binary(FP(ref)) << "\n"
            << "nfloat - single precision:\n" 
            << to_sci_string(sgl) 
            << "\n\n";
        
        ref /= base;
    }
    while(!sgl.is_zero());
    
    
}

int main()
{
    using nsingle = floating_point<8, 23>;
    using nbetweensd = floating_point<10, 42>;
    using ndouble = floating_point<11, 52>;
    using nddouble = floating_point<22, 104>;
    const float base = 3.1415f;

    //compare_to_float<float, nsingle>(base);
    compare_to_float<float, nbetweensd>(base);
    //compare_to_float<double, ndouble>(static_cast<double>(base));
    //compare_to_float<double, nddouble>(static_cast<double>(base));
    

    return 0;
}
