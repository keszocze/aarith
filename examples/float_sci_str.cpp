#include <aarith/float.hpp>
#include <aarith/float/string_utils.hpp>
#include <cmath>

using namespace aarith;

int main()
{
    using nsingle = floating_point<8, 23>;
    using ndouble = floating_point<11, 52>;
    using nddouble = floating_point<22, 104>;
    const float base = 3.1415f;
    float ref = base;
    double dref = base;
    nsingle sgl(ref);
    ndouble dbl(ref);
    nddouble ddbl(ref);

    do{
        sgl = nsingle(ref);
        std::cout << "float:\n" << ref << "\n"
                  << "nfloat - single precision:\n" << to_sci_string(sgl) << "\n\n";
        
        ref *= base;
    }
    while(!sgl.is_inf());
    
    ref = base;
    do{
        sgl = nsingle(ref);
        std::cout << "float:\n" << ref << "\n"
                  << "nfloat - single precision:\n" << to_sci_string(sgl) << "\n\n";
        
        ref /= base;
    }
    while(!sgl.is_zero());
    
    dref = base;
    do{
        dbl = ndouble(dref);
        std::cout << "float:\n" << dref << "\n"
                  << "nfloat - double precision:\n" << to_sci_string(dbl) << "\n\n";
        
        dref *= base;
    }
    while(!dbl.is_inf());
    
    dref = base;
    do{
        dbl = ndouble(dref);
        std::cout << "float:\n" << dref << "\n"
                  << "nfloat - double precision:\n" << to_sci_string(dbl) << "\n\n";
        
        dref /= base;
    }
    while(!dbl.is_zero());
    
    dref = base;
    do{
        ddbl = nddouble(dref);
        std::cout << "float:\n" << dref << "\n"
                  << "nfloat - twice double precision:\n" << to_sci_string(ddbl) << "\n\n";
        
        dref *= base;
    }
    while(!ddbl.is_inf());
    
    dref = base;
    do{
        ddbl = nddouble(dref);
        std::cout << "float:\n" << dref << "\n"
                  << "nfloat - twice double precision:\n" << to_sci_string(ddbl) << "\n"
                  << "nfloat - twice double precision:\n" << to_binary(ddbl) << "\n\n";
        
        dref /= base;
    }
    while(!ddbl.is_zero());
    

    return 0;
}
