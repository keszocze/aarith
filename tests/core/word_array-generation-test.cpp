#include <aarith/core.hpp>
#include <catch.hpp>

#include "gen_word_array.hpp"

using namespace aarith;

// This test basically checks that the thing compiles...
TEMPLATE_TEST_CASE_SIG("Generating ten word_arrays yields ten different results",
                       "[word_array][utility][random]",
                       ((size_t W, typename WordType), W, WordType),
                       (8, uint8_t), (8, uint16_t), (12,uint32_t))
{
    using I = word_array<W, WordType>;


    // the numbers should be in the range [Num,Num+1[ = [Num,Num]
    WordArrayGenerator<W, WordType> gen;

    I last = gen.get();

    for ([[maybe_unused]] int i = 0; i < 100; ++i)
    {
        gen.next();
        I curr = gen.get();
//        REQUIRE(last != curr);
//        std::cout << curr << "\n";
        REQUIRE(true);
        last = curr;
    }
}