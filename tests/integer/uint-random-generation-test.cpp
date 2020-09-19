#include <aarith/integer_no_operators.hpp>
#include <catch.hpp>

#include "gen_uinteger.hpp"
#include "test_util.hpp"

using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Generating exactly one number works as intended",
                       "[integer][unsigned][utility][random]",
                       ((size_t W, typename WordType, size_t Num), W, WordType, Num),
                       (8, uint8_t, 15), (8, uint16_t, 15), (12,uint32_t,5000))
{
    using I = uinteger<W, WordType>;

    const I val{Num};

    // the numbers should be in the range [Num,Num+1[ = [Num,Num]
    UIntegerGenerator<W, WordType> gen(val, I{Num + 1U});

    REQUIRE(gen.get() == val);

    for ([[maybe_unused]] int i = 0; i < 10; ++i)
    {
        gen.next();
        REQUIRE(gen.get() == val);
    }
}

TEMPLATE_TEST_CASE_SIG("Generating exactly one number with more than one word works as intended",
                       "[integer][unsigned][utility][random]",
                       ((size_t W, typename WordType), W, WordType),
                       (17, uint8_t), (40, uint16_t), (70,uint32_t), (150, uint64_t))
{
    using I = uinteger<W, WordType>;

    const I val=I::from_words(1U, 0U,0U);

    // the numbers should be in the range [Num,Num+1[ = [Num,Num]
    UIntegerGenerator<W, WordType> gen(val, add(val, I::one()));


    REQUIRE(gen.get() == val);

    for ([[maybe_unused]] int i = 0; i < 10; ++i)
    {
        gen.next();
        REQUIRE(gen.get() == val);
    }
}

TEMPLATE_TEST_CASE_SIG("The generated numbers are in the given range",
                       "[integer][unsigned][utility][random]",
                       ((size_t W, typename WordType, size_t L, size_t U), W, WordType, L, U),
                       (8, uint8_t, 15, 40), (8, uint16_t, 15, 40), (12,uint32_t,5000, 16000))
{
    using I = uinteger<W, WordType>;

    const I lower{L};
    const I upper{U};

    // the numbers should be in the range [Num,Num+1[ = [Num,Num]
    UIntegerGenerator<W, WordType> gen(lower, upper);

    REQUIRE(gen.get() >= lower);
    REQUIRE(gen.get() < upper);

    for ([[maybe_unused]] int i = 0; i < 1000; ++i)
    {
        gen.next();
        REQUIRE(gen.get() >= lower);
        REQUIRE(gen.get() < upper);
    }
}

TEMPLATE_TEST_CASE_SIG("Generating two numbers with constraints works",
                       "[integer][unsigned][utility][random]",
                       ((size_t W, typename WordType, size_t L, size_t U), W, WordType, L, U),
                       (8, uint8_t, 15, 40), (8, uint16_t, 15, 40), (12,uint32_t,5000, 16000))
{
    using I = uinteger<W, WordType>;

    const I lower{L};
    const I upper{U};
    const I lower_{U+10};
    const I upper_{U+100};

    // the numbers should be in the range [Num,Num+1[ = [Num,Num]
    UIntegerGenerator<W, WordType> gen(lower, upper);
    UIntegerGenerator<W, WordType> gen_(lower_, upper_);

    REQUIRE(gen.get() >= lower);
    REQUIRE(gen.get() < upper);

    for ([[maybe_unused]] int i = 0; i < 1000; ++i)
    {
        gen.next();
        gen_.next();
        const auto a = gen.get();
        const auto a_ = gen_.get();
        REQUIRE(a >= lower);
        REQUIRE(a < upper);
        REQUIRE(a_ >= lower_);
        REQUIRE(a_ < upper_);
        REQUIRE(a < a_);
    }
}
