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

        AND_GIVEN("Positive/negative infinity") {
            THEN("Comparison with inf should be possible and correct") {
                F pos{F::pos_infinity()};
                F neg{F::neg_infinity()};
                CHECK(pos > a);
                CHECK(pos != a);
                CHECK(neg < a);
                REQUIRE(neg != a);
            }
        }

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
                       "[normalized_float][comparison][utility]",
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

TEMPLATE_TEST_CASE_SIG("Comparing differently sized floating points works as expected",
                       "[normalized_float][casting][constructor]",
                       ((size_t E, size_t M, typename Native), E, M, Native), (8, 23, float),
                       (11, 52, float), (11, 52, double))
{
    using F = normalized_float<E, M>;
    using G = normalized_float<E + 80, M>;
    using H = normalized_float<M, M + 80>;
    using J = normalized_float<E + 80, M + 80>;

    GIVEN("A random floating point value")
    {
        Native f_native = GENERATE(take(30, random<Native>(std::numeric_limits<Native>::min()/4,
                                                          0.0f)));


        F f{f_native};

        G g{f};
        H h{f};
        J j{f};

        THEN("The value should not have changed")
        {
            CHECK(f == g);
            CHECK(f == h);
            CHECK(g == f);
            CHECK(h == f);
            CHECK(j == f);
            REQUIRE(f == j);
        }
        AND_THEN("Inequality should not be true")
        {
            CHECK_FALSE(f != g);
            CHECK_FALSE(f != h);
            CHECK_FALSE(g != f);
            CHECK_FALSE(h != f);
            CHECK_FALSE(j != f);
            REQUIRE_FALSE(f != j);
        }
        AND_GIVEN("A larger floating point value")
        {
            // the ranges are chosen in such a way that,  hopefully, adding the second number
            // actually changes the value
            Native fs_native = GENERATE(take(10, random<Native>(std::numeric_limits<Native>::max()/20,
                std::numeric_limits<Native>::max()/4)));

            F f_{fs_native + f_native};
            G g_{f_};
            H h_{f_};
            J j_{f_};

            THEN("The value should have changed")
            {
                if (f == f_) {
                    std::cout << f.is_nan() << "\t" << f.is_inf() << "\n";
                    std::cout << to_binary(f) << "(" << f << ")\n";
                    std::cout << to_binary(f_) << "(" << f_ << ")\n";
                    std::cout << to_binary(g_) << "(" << g_ << ")\n";
                }
                REQUIRE(f != f_);
                REQUIRE(f != g_);
                CHECK(f != h_);
                CHECK(g_ != f);
                CHECK(h_ != f);
                CHECK(j_ != f);
                REQUIRE(f != j_);
            }
            AND_THEN("Inequality should be true")
            {
                REQUIRE(f != g_);
                CHECK(f != h_);
                CHECK(g_ != f);
                CHECK(h_ != f);
                CHECK(j_ != f);
                REQUIRE(f != j_);
            }
        }
    }
}