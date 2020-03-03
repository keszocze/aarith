#include <aarith/float.hpp>
#include <catch.hpp>

using namespace aarith;

SCENARIO("Adding two floating-point numbers exactly", "[normalized_float][arithmetic][addition]")
{
    GIVEN("Single precision floats (E = 8, M = 23+1)")
    {
        static constexpr size_t E = 8;
        static constexpr size_t M = 24;

        WHEN("The second operand is -a.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = -number_a;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("The result should be 0")
            {
                REQUIRE(result == normalized_float<E, M>(0.f));
            }
        }
        WHEN("The second operand is 0.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = 0;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("The result should be a")
            {
                REQUIRE(result == a);
            }
        }
        WHEN("The first operand is negative, and the second operand is 0.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = 0;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("The result should be a")
            {
                REQUIRE(result == a);
            }
        }
        WHEN("The first operand is 0.")
        {
            static constexpr float number_a = 0;
            static constexpr float number_b = 93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("The result should be b")
            {
                REQUIRE(result == b);
            }
        }
        WHEN("The first operand is 0, and the second operand is negative.")
        {
            static constexpr float number_a = 0;
            static constexpr float number_b = -93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("The result should be b")
            {
                REQUIRE(result == b);
            }
        }
        WHEN("The absolute of the first operand is higher than the abolute of the first operand, both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 213.22154f;
            static constexpr float number_b = 93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the abolute of the first operand, operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154f;
            static constexpr float number_b = 93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the abolute of the first operand, operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 213.22154f;
            static constexpr float number_b = -93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the abolute of the first operand, both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154f;
            static constexpr float number_b = -93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the abolute of the first operand, both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = 213.22154f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the abolute of the first operand, operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = 213.22154f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the abolute of the first operand, operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = -213.22154f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the abolute of the first operand, both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = -213.22154f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a + number_b)));
            }
        }
    }
}

SCENARIO("Subtracting two floating-point numbers exactly", "[normalized_float][arithmetic][subtraction]")
{
    GIVEN("Single precision floats (E = 8, M = 23+1)")
    {
        static constexpr size_t E = 8;
        static constexpr size_t M = 24;

        WHEN("Both operands are the same.")
        {
            static constexpr float number_a = 93.211546f;
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
            static constexpr float number_a = 93.211546f;
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
            static constexpr float number_a = -93.211546f;
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
            static constexpr float number_b = 93.211546f;
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
            static constexpr float number_b = -93.211546f;
            static constexpr float res = -number_b;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("The result should be -b")
            {
                REQUIRE(result == normalized_float<E, M>(res));
            }
        }
        WHEN("The absolute of the first operand is higher than the abolute of the first operand, both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 213.22154f;
            static constexpr float number_b = 93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the abolute of the first operand, operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154f;
            static constexpr float number_b = 93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the abolute of the first operand, operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 213.22154f;
            static constexpr float number_b = -93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the abolute of the first operand, both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154f;
            static constexpr float number_b = -93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the abolute of the first operand, both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = 213.22154f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the abolute of the first operand, operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = 213.22154f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the abolute of the first operand, operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = -213.22154f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the abolute of the first operand, both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = -213.22154f;
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

SCENARIO("Exact multiplication of two floating-point numbers", "[normalized_float][arithmetic][multiplication]")
{
    GIVEN("Single precision floats (E = 8, M = 23+1)")
    {
        static constexpr size_t E = 8;
        static constexpr size_t M = 24;

        WHEN("The absolute of the first operand is higher than the abolute of the first operand, both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 213.22154f;
            static constexpr float number_b = 93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the abolute of the first operand, operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154f;
            static constexpr float number_b = 93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the abolute of the first operand, operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 213.22154f;
            static constexpr float number_b = -93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the abolute of the first operand, both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154f;
            static constexpr float number_b = -93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the abolute of the first operand, both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = 213.22154f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the abolute of the first operand, operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = 213.22154f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the abolute of the first operand, operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = -213.22154f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the abolute of the first operand, both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = -213.22154f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
    }
}

SCENARIO("Exact division of two floating-point numbers", "[normalized_float][arithmetic][division]")
{
    GIVEN("Single precision floats (E = 8, M = 23+1)")
    {
        static constexpr size_t E = 8;
        static constexpr size_t M = 24;

        WHEN("The absolute of the first operand is higher than the abolute of the first operand, both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 213.22154f;
            static constexpr float number_b = 93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the abolute of the first operand, operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154f;
            static constexpr float number_b = 93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the abolute of the first operand, operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 213.22154f;
            static constexpr float number_b = -93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the abolute of the first operand, both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154f;
            static constexpr float number_b = -93.211546f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the abolute of the first operand, both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = 213.22154f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the abolute of the first operand, operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = 213.22154f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the abolute of the first operand, operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = -213.22154f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the abolute of the first operand, both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = -213.22154f;
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a / number_b)));
            }
        }
    }
}
