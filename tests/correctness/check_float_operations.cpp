#include <aarith/float.hpp>
#include <catch.hpp>

using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Arithmetic should match the native data types",
                       "[floating_point][arithmetic][constexpr][checking]",
                       ((size_t E, size_t M, typename native), E, M, native), (8, 23, float),
                       (11, 52, double))
{
    using F = floating_point<E, M>;

    native a_float{-1337.35f};
    native b_float{420815.0f};

    constexpr native delta = {0.123f};
    constexpr native delta2 = {3.33f};

    for (size_t i = 0; i < std::numeric_limits<size_t>::max() - 1; ++i)
    {
        F a{a_float};
        F b{b_float};

        F res = a + b;
        native res_float = a_float + b_float;

        F res_float_{res_float};
        native res_ = static_cast<native>(res);

        if (res_float_ != res)
        {

            std::cout << a << " + " << b << " = " << res << "\n";
            std::cout << to_binary(a) << " + " << to_binary(b) << " = " << to_binary(res) << "\n";
            std::cout << tcs(a) << "\t+\t" << tcs(b) << " = " << tcs(res) << "\n";

            std::cout << a_float << " + " << b_float << " = " << res_float << "\n";
            std::cout << to_binary(res) << " vs. " << to_binary(res_float_) << "\n";
        }

        CHECK(res_float_ == res);
        REQUIRE(res_ == res_float);

        a_float += delta;
        b_float += delta2;
    }
}

SCENARIO("Adding two floating-point numbers exactly", "[floating_point][arithmetic][addition]")
{
    GIVEN("Single precision floats (E = 8, M = 23)")
    {
        static constexpr size_t E = 8;
        static constexpr size_t M = 23;

        WHEN("The second operand is -a.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = -number_a;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            std::cout << "result: " << to_binary(result) << std::endl;
            std::cout << "zero: " << to_binary(floating_point<E, M>(0.f)) << std::endl;
            THEN("The result should be 0")
            {
                REQUIRE(result == floating_point<E, M>(0.f));
            }
        }
        WHEN("The second operand is 0.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = 0;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("The result should be a")
            {
                std::cout << to_binary(result) << " vs. " << to_binary(a) << "\n";
                REQUIRE(result == a);
            }
        }
        WHEN("The first operand is negative, and the second operand is 0.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = 0;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("The result should be a")
            {
                REQUIRE(result == a);
            }
        }
        WHEN("The first operand is 0.")
        {
            static constexpr float number_a = 0;
            static constexpr float number_b = 93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("The result should be b")
            {
                REQUIRE(result == b);
            }
        }
        WHEN("The first operand is 0, and the second operand is negative.")
        {
            static constexpr float number_a = 0;
            static constexpr float number_b = -93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("The result should be b")
            {
                REQUIRE(result == b);
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 213.22154f;
            static constexpr float number_b = 93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154f;
            static constexpr float number_b = 93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 213.22154f;
            static constexpr float number_b = -93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154f;
            static constexpr float number_b = -93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = 213.22154f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = 213.22154f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = -213.22154f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = -213.22154f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
        WHEN("When one operand is a > 1 and the other is 0 < b < 1.")
        {
            static constexpr float number_a = 2.75f;
            static constexpr float number_b = 0.5f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = add(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a + number_b)));
            }
        }
    }
}

SCENARIO("Subtracting two floating-point numbers exactly",
         "[floating_point][arithmetic][subtraction]")
{
    GIVEN("Single precision floats (E = 8, M = 23)")
    {
        static constexpr size_t E = 8;
        static constexpr size_t M = 23;

        WHEN("Both operands are the same.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = number_a;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = sub(a, b);

            THEN("The result should be 0")
            {
                REQUIRE(result == floating_point<E, M>(0.f));
            }
        }
        WHEN("The second operand is 0.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = 0;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = sub(a, b);

            THEN("The result should be a")
            {
                REQUIRE(result == a);
            }
        }
        WHEN("The first operand is negative, and the second operand is 0.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = 0;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = sub(a, b);

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
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = sub(a, b);

            THEN("The result should be -b")
            {
                REQUIRE(result == floating_point<E, M>(res));
            }
        }
        WHEN("The first operand is 0, and the second operand is negative.")
        {
            static constexpr float number_a = 0;
            static constexpr float number_b = -93.211546f;
            static constexpr float res = -number_b;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = sub(a, b);

            THEN("The result should be -b")
            {
                REQUIRE(result == floating_point<E, M>(res));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 213.22154f;
            static constexpr float number_b = 93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154f;
            static constexpr float number_b = 93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 213.22154f;
            static constexpr float number_b = -93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154f;
            static constexpr float number_b = -93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = 213.22154f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = 213.22154f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = -213.22154f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a - number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = -213.22154f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = sub(a, b);

            THEN("It should be the correct sum.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a - number_b)));
            }
        }
    }
}

SCENARIO("Exact multiplication of two floating-point numbers",
         "[floating_point][arithmetic][multiplication]")
{
    GIVEN("Single precision floats (E = 8, M = 23)")
    {
        static constexpr size_t E = 8;
        static constexpr size_t M = 23;

        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 213.22154f;
            static constexpr float number_b = 93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result_float{number_a * number_b};
            const floating_point<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                std::cout << number_a << "\t" << number_b << "\n";
                std::cout << (number_a * number_b) << "\n";
                std::cout << result_float << "\n";
                std::cout << result << "\n";
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154f;
            static constexpr float number_b = 93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 213.22154f;
            static constexpr float number_b = -93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154f;
            static constexpr float number_b = -93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = 213.22154f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = 213.22154f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = -213.22154f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = -213.22154f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a * number_b)));
            }
        }
        WHEN("When one operand is a > 1 and the other is 0 < b < 1.")
        {
            static constexpr float number_a = 2.75f;
            static constexpr float number_b = 0.5f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a * number_b)));
            }
        }
    }
}

SCENARIO("Exact division of two floating-point numbers", "[floating_point][arithmetic][division]")
{
    GIVEN("Single precision floats (E = 8, M = 23)")
    {
        static constexpr size_t E = 8;
        static constexpr size_t M = 23;

        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 213.22154f;
            static constexpr float number_b = 93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154f;
            static constexpr float number_b = 93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 213.22154f;
            static constexpr float number_b = -93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -213.22154f;
            static constexpr float number_b = -93.211546f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = 213.22154f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = 213.22154f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
        {
            static constexpr float number_a = 93.211546f;
            static constexpr float number_b = -213.22154f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a / number_b)));
            }
        }
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
        {
            static constexpr float number_a = -93.211546f;
            static constexpr float number_b = -213.22154f;
            const floating_point<E, M> a{number_a};
            const floating_point<E, M> b{number_b};
            const floating_point<E, M> result = div(a, b);

            THEN("It should be the correct product.")
            {
                REQUIRE(equal_except_rounding(result, floating_point<E, M>(number_a / number_b)));
            }
        }
    }
}

SCENARIO("IEEE-754 arithmetic conversion: float, double",
         "[floating_point][conversion][ieee-754][casting]")
{
    GIVEN("A float number")
    {
        WHEN("The number is 0.")
        {
            float a = 0.f;

            THEN("The floating_point should convert back to the original number.")
            {
                REQUIRE(static_cast<float>(floating_point<8, 23>(a)) == a);
            }
        }
    }
    GIVEN("A double number")
    {
        WHEN("The number is 0.")
        {
            double a = 0.;

            THEN("The floating_point should convert back to the original number.")
            {
                REQUIRE(static_cast<double>(floating_point<11, 52>(a)) == a);
            }
        }
    }
    GIVEN("A random float number")
    {
        float a = GENERATE(take(10, random(float(1.0), std::numeric_limits<float>::max())));

        THEN("The floating_point should convert back to the original number.")
        {
            REQUIRE(static_cast<float>(floating_point<8, 23>(a)) == a);
        }
    }
    GIVEN("A random double number")
    {
        double a = GENERATE(take(10, random(double(1.0), std::numeric_limits<double>::max())));

        THEN("The floating_point should convert back to the original number.")
        {
            REQUIRE(static_cast<double>(floating_point<11, 52>(a)) == a);
        }
    }
}
