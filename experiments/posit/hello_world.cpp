/**
 * Hello World
 *
 * Count up from -maxpos to maxpos. Not very interesting, but enough for trying
 * out the build.
 */

#include <aarith/posit.hpp>
#include <iostream>

using namespace aarith;

int main()
{
    posit8 p = posit8::min();

    while (!p.is_nar())
    {
        std::cout << p << std::endl;
        p = p.incremented();
    }
}
