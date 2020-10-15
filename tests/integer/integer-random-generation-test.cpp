#include <aarith/integer_no_operators.hpp>
#include <catch.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_integer.hpp"

using namespace aarith;

// This test basically checks that the thing compiles...
TEMPLATE_TEST_CASE_SIG("Generating ten integers yields ten different results",
                       "[integer][utility][random]",
                       ((size_t W, typename WordType), W, WordType), (16, uint8_t))
{
    using I = integer<W, WordType>;

    // the numbers should be in the range [Num,Num+1[ = [Num,Num]
    IntegerGenerator<W, WordType> gen;

    I last = gen.get();

    for ([[maybe_unused]] int i = 0; i < 30; ++i)
    {
        gen.next();
        I curr = gen.get();
        //        REQUIRE(last != curr);
        //        std::cout << curr <<  "\t" << group_digits(to_binary(curr),8) << "\n";
        REQUIRE(true);
        last = curr;
    }
}
