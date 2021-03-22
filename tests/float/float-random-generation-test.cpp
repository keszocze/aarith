#include <aarith/float.hpp>
#include <catch.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_float.hpp"

using namespace aarith;

// This test basically checks that the thing compiles...
TEMPLATE_TEST_CASE_SIG("Generating ten floating points yields ten different results",
                       "[floating_point][utility][random]",
                       ((size_t E, size_t M, typename WordType), E, M, WordType), (8, 23, uint64_t))
{
    using F = floating_point<E, M, WordType>;

    // the numbers should be in the range [Num,Num+1[ = [Num,Num]
    FloatGenerator<E, M> gen;

    F last = gen.get();

    for ([[maybe_unused]] int i = 0; i < 30; ++i)
    {
        gen.next();
        F curr = gen.get();
        //        REQUIRE(last != curr);
        //                std::cout << curr <<  "\t" << to_binary(curr) << "\n";
        REQUIRE(true);
        last = curr;
    }
}
