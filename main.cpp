#include <aarith/float.hpp>
#include <aarith/integer.hpp>
#include <cmath>
#include <iostream>

int main()
{
    using namespace aarith;

    using I16 = integer<16>;
    using U16 = uinteger<16>;

    I16 word;
    U16 other_word, shift;

    I16 shifted_size_t = word << size_t(45);
    I16 shifted = word << shift;
    U16 shifted_ = other_word << shift;
    (void) shifted;

    return 0;
}
