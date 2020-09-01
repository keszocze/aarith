#include <aarith/float.hpp>
#include <catch.hpp>

using namespace aarith;

SCENARIO("Adding two floating-point numbers exactly", "[normalized_float][arithmetic][addition][burp]")
{
    GIVEN("Single precision floats (E = 8, M = 23)")
    {
        static constexpr size_t E = 8;
        static constexpr size_t M = 23;

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
                std::cout << to_binary(result) << " vs. " << to_binary(a) << "\n";
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
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
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
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
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
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
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
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
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
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
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
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
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
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
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
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
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

SCENARIO("Subtracting two floating-point numbers exactly",
         "[normalized_float][arithmetic][subtraction]")
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
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
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
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
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
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
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
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
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
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
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
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
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
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
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
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
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

SCENARIO("Exact multiplication of two floating-point numbers",
         "[normalized_float][arithmetic][multiplication]")
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
            const normalized_float<E, M> a{number_a};
            const normalized_float<E, M> b{number_b};
            const normalized_float<E, M> result_float{number_a * number_b};
            const normalized_float<E, M> result = mul(a, b);

            THEN("It should be the correct product.")
            {
                std::cout << number_a << "\t" << number_b << "\n";
                std::cout << (number_a * number_b) << "\n";
                std::cout << result_float << "\n";
                std::cout << result << "\n";
                REQUIRE(equal_except_rounding(result, normalized_float<E, M>(number_a * number_b)));
            }
        }
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
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
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
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
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
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
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
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
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
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
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
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
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
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
    GIVEN("Single precision floats (E = 8, M = 23)")
    {
        static constexpr size_t E = 8;
        static constexpr size_t M = 23;

        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
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
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
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
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
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
        WHEN("The absolute of the first operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
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
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are positive and the result is smaller infinity.")
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
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 1 is negative, and the result is not infinity.")
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
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "operand 2 is negative, and the result is not infinity.")
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
        WHEN("The absolute of the second operand is higher than the absolute of the first operand, "
             "both operands are negative, and the result is not infinity.")
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

SCENARIO("IEEE-754 arithmetic comparison: float",
         "[normalized_float][arithmetic][multiplication][ieee-754]")
{

    GIVEN("0.5 and 0.25") {
        float half = 0.5;
        float quarter = 1.5;
        float threequarter = -0.875;
        float zero = 0.0;

        using afloat = normalized_float<8,23>;

        afloat ahalf{half};
        afloat aquarter{quarter};
        aquarter.set_exponent(uinteger<8>::zero());
        afloat athreequarter{threequarter};
        afloat azero{zero};

        std::cout << half << " " << ahalf << "\t" << quarter << " " << aquarter << "\n";
        std::cout << to_binary(ahalf) << "\t" << to_binary(aquarter) << "\n";
        std::cout << to_compute_string(ahalf) << "\t" << to_compute_string(aquarter) << "\n";
        std::cout << tcs(ahalf) << "\t" << tcs(aquarter)<< "\t" << tcs(athreequarter) << "\t" << tcs(azero) << "\n";
//        std::cout << ahalf.get_real_exponent() << "\t" << aquarter.unbiased_exponent() << "\n";
//        std::cout << to_binary(ahalf.get_real_exponent()) << "\t" << to_binary(aquarter.unbiased_exponent()) << "\n";

    }
//    GIVEN("Two random floating point values")
//    {
//        float a = GENERATE(
//            take(10, random(float(1.0), std::numeric_limits<float>::max())));
//        float b = GENERATE(
//            take(10, random(float(1.0), std::numeric_limits<float>::max())));
//
//        using afloat = normalized_float<8, 23>;
//        afloat aa{a};
//        afloat ab{b};
//
//        float f_res_add = a + b;
//        float f_res_mul = a * b;
//        float f_res_sub = a - b;
//        float f_res_div = a / b;
//
//        afloat a_res_add = add(aa, ab);
//        afloat a_res_mul = mul(aa, ab);
//        afloat a_res_sub = sub(aa, ab);
//        afloat a_res_div = div(aa, ab);
//
//        CHECK(f_res_add == static_cast<float>(a_res_add));
//        CHECK(f_res_sub == static_cast<float>(a_res_sub));
//        CHECK(f_res_mul == static_cast<float>(a_res_mul));
//        REQUIRE(f_res_div == static_cast<float>(a_res_div));
//    }
}
//
// TEMPLATE_TEST_CASE_SIG("IEEE-754 arithmetic comparision: addition",
//                       "[normalized_float][arithmetic][addition][ieee-754]",
//                       ((typename F, typename A), F, A),
//                       (float, normalized_float<8,23>),
//                       (double, normalized_float<11,52>)
//                       )
//{
//    std::cout << "works\n";
////    FloatType a = GENERATE(take(
////        10, random(std::numeric_limits<FloatType>::min(),
/// std::numeric_limits<FloatType>::max()))); /    FloatType b = GENERATE(take( /        10,
/// random(std::numeric_limits<FloatType>::min(), std::numeric_limits<FloatType>::max())));
////
////    AarithFloat aa{a};
////    AarithFloat ab{b};
//}
