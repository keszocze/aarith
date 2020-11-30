#include <aarith/float.hpp>

#include "gen_float.hpp"
#include "../test-signature-ranges.hpp"

#include <bitset>
#include <catch.hpp>


using namespace aarith;

template <typename N>
// it is not really "full range" but at least it works
auto full_native_range()
{
    return Catch::Generators::random<N>(std::numeric_limits<N>::lowest() / 100.0f,
                                        std::numeric_limits<N>::max() / 100.0f);
}



TEMPLATE_TEST_CASE_SIG("Floating point subtraction matches its native counterparts",
                       "[normalized_float][arithmetic][subtraction]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{

    using F = normalized_float<E, M>;

    Native a_native = GENERATE(take(15, full_native_range<Native>()));
    Native b_native = GENERATE(take(15, full_native_range<Native>()));
    F a{a_native};
    F b{b_native};

    F res = a - b;
    Native res_native = a_native - b_native;

    F res_native_{res_native};
    Native res_ = static_cast<Native>(res);

    if (!equal_except_rounding(res_native_, res))
    {
        std::cout << "a - b\n"
                  << to_binary(F(a)) << " - \n"
                  << to_binary(F(b)) << "\n"
                  << to_binary(res) << "(normalized_float) !=\n"
                  << to_binary(F(res_native_)) << "(float)\n\n";
    }

    CHECK(equal_except_rounding(res_native_, res));
    REQUIRE(equal_except_rounding(F{res_}, F{res_native}));
}


SCENARIO("Subtracting two floating-point numbers (hand picked examples)",
         "[normalized_float][arithmetic][subtraction]")
{
    GIVEN("Single precision floats (E = 8, M = 23)")
    {
        static constexpr size_t E = 8;
        static constexpr size_t M = 23;

        WHEN("Both operands are the same.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = number_a;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("The result should be 0")
            {
                REQUIRE(result == normalized_float<E, M>(0.f));
            }
        }
        WHEN("The second operand is 0.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = 0;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("The result should be a")
            {
                REQUIRE(result == a);
            }
        }
        WHEN("The first operand is negative, and the second operand is 0.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = 0;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("The result should be a")
            {
                REQUIRE(result == a);
            }
        }
        WHEN("The first operand is 0.")
        {
            static constexpr float number_a = 0;
            static constexpr float number_b = 93.211546F;
            static constexpr float res = -number_b;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("The result should be -b")
            {
                REQUIRE(result == normalized_float<E, M>(res));
            }
        }
        WHEN("The first operand is 0, and the second operand is negative.")
        {
            static constexpr float number_a = 0;
            static constexpr float number_b = -93.211546F;
            static constexpr float res = -number_b;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("The result should be -b")
            {
                REQUIRE(result == normalized_float<E, M>(res));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 213.22154F;
            static constexpr float number_b = 93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154F;
            static constexpr float number_b = 93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 213.22154F;
            static constexpr float number_b = -93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154F;
            static constexpr float number_b = -93.211546F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = 213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = 213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 93.211546F;
            static constexpr float number_b = -213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546F;
            static constexpr float number_b = -213.22154F;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
    }
}