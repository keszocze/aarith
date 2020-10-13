#include <aarith/float.hpp>
#include <bitset>
#include <catch.hpp>
using namespace aarith;

TEMPLATE_TEST_CASE_SIG("Comparisons should match the native counterparts",
                       "[normalized_float][comparison][utility]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    using F = normalized_float<E, M>;

    GIVEN("A normalized_float created from native data types")
    {

        Native a_native = GENERATE(take(15, random<Native>(std::numeric_limits<Native>::min(),
                                                           std::numeric_limits<Native>::max())));
        F a{a_native};

        AND_GIVEN("The NaN value")
        {
            Native quite_nan_native = std::numeric_limits<Native>::quiet_NaN();

            F quite_nan{quite_nan_native};

            THEN("All comparisons should return false")
            {

                bool eq = (a == quite_nan);
                bool eq_native(a_native == quite_nan_native);
                CHECK(eq == eq_native);
                CHECK_FALSE(eq);

                bool neq = (a != quite_nan);
                bool neq_native(a_native != quite_nan_native);
                CHECK(neq == neq_native);
                CHECK(neq); // should actually be true!

                bool leq = (a <= quite_nan);
                bool leq_native(a_native <= quite_nan_native);
                CHECK(leq == leq_native);
                CHECK_FALSE(leq);

                bool lt = (a < quite_nan);
                bool lt_native(a_native < quite_nan_native);
                CHECK(lt == lt_native);
                CHECK_FALSE(lt);

                bool geq = (a >= quite_nan);
                bool geq_native(a_native >= quite_nan_native);
                CHECK(geq == geq_native);
                CHECK_FALSE(geq);

                bool gt = (a > quite_nan);
                bool gt_native(a_native > quite_nan_native);
                CHECK(gt == gt_native);
                REQUIRE_FALSE(gt);
            }
        }

        AND_GIVEN("Another normalized float generated from native data types")
        {
            Native b_native =
                GENERATE(take(15, random<Native>(std::numeric_limits<Native>::min(),
                                                 std::numeric_limits<Native>::max())));
            F b{b_native};

            WHEN("Comparing these numbers")
            {

                THEN("The result should match its native counterpart")
                {

                    bool eq = (a == b);
                    bool eq_native(a_native == b_native);
                    CHECK(eq == eq_native);

                    bool neq = (a != b);
                    bool neq_native(a_native != b_native);
                    CHECK(neq == neq_native);

                    bool leq = (a <= b);
                    bool leq_native(a_native <= b_native);
                    CHECK(leq == leq_native);

                    bool lt = (a < b);
                    bool lt_native(a_native < b_native);
                    CHECK(lt == lt_native);

                    bool geq = (a >= b);
                    bool geq_native(a_native >= b_native);
                    CHECK(geq == geq_native);

                    bool gt = (a > b);
                    bool gt_native(a_native > b_native);
                    CHECK(gt == gt_native);
                }
            }
        }
    }
}

TEMPLATE_TEST_CASE_SIG("Constants should be stored correctly",
                       "[normalized_float][comparison][utility][foo]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, double))
{
    using F = normalized_float<E, M>;

    GIVEN("The number zero in the native data type and as aarith float")
    {

        F zero_aarith = F::zero();
        Native zero_native{0.0f};

        THEN("The numbers should match")
        {
            CHECK(zero_aarith == F{zero_native});
            REQUIRE(static_cast<Native>(zero_aarith) == zero_native);
        }
    }

    GIVEN("The number one in the native data type and as aarith float")
    {

        F one_aarith = F::one();
        Native one_native{1.0f};

        THEN("The numbers should match")
        {
            CHECK(one_aarith == F{one_native});
            REQUIRE(static_cast<Native>(one_aarith) == one_native);
        }
    }
}