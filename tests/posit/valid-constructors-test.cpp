#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_posit.hpp"

TEMPLATE_TEST_CASE_SIG("valid zero constructors and constants", "[posit][valid][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Posit = posit<N, ES>;
    using Valid = valid<N, ES>;

    {
        const Valid v = Valid();

        REQUIRE(v.is_zero());
        REQUIRE_FALSE(v.is_nar());
        REQUIRE_FALSE(v.is_empty());
        REQUIRE(v.contains(Posit::zero()));
        REQUIRE_FALSE(v.contains(Posit::one()));
    }

    {
        const Valid v = Valid::zero();

        REQUIRE(v.is_zero());
        REQUIRE_FALSE(v.is_nar());
        REQUIRE_FALSE(v.is_empty());
        REQUIRE(v.contains(Posit::zero()));
        REQUIRE_FALSE(v.contains(Posit::one()));
    }

    {
        const Valid v = Valid::one();

        REQUIRE_FALSE(v.is_zero());
        REQUIRE_FALSE(v.is_nar());
        REQUIRE_FALSE(v.is_empty());
        REQUIRE_FALSE(v.contains(Posit::zero()));
        REQUIRE(v.contains(Posit::one()));
    }

    {
        const Valid v = Valid::nar();

        REQUIRE_FALSE(v.is_zero());
        REQUIRE(v.is_nar());
        REQUIRE_FALSE(v.is_empty());
        REQUIRE_FALSE(v.contains(Posit::zero()));
        REQUIRE_FALSE(v.contains(Posit::one()));
    }

    {
        const Valid v = Valid::empty();

        REQUIRE_FALSE(v.is_zero());
        REQUIRE_FALSE(v.is_nar());
        REQUIRE(v.is_empty());
        REQUIRE_FALSE(v.contains(Posit::zero()));
        REQUIRE_FALSE(v.contains(Posit::one()));
    }
}

TEMPLATE_TEST_CASE_SIG("valid special cases", "[posit][valid][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Valid = valid<N, ES>;

    {
        const Valid v = Valid::nar();

        REQUIRE_FALSE(v.is_zero());
        REQUIRE_FALSE(v.is_empty());
        REQUIRE_FALSE(v.is_all_reals());
        REQUIRE_FALSE(v.is_full());
        REQUIRE_FALSE(v.is_exact_real());
        REQUIRE(v.is_nar());
        REQUIRE_FALSE(v.is_regular());
        REQUIRE(v.is_irregular());
    }

    {
        const Valid v = Valid::empty();

        REQUIRE_FALSE(v.is_zero());
        REQUIRE(v.is_empty());
        REQUIRE_FALSE(v.is_all_reals());
        REQUIRE_FALSE(v.is_full());
        REQUIRE_FALSE(v.is_exact_real());
        REQUIRE_FALSE(v.is_nar());
        REQUIRE(v.is_regular());
        REQUIRE_FALSE(v.is_irregular());
    }

    {
        const Valid v = Valid::full();

        REQUIRE_FALSE(v.is_zero());
        REQUIRE_FALSE(v.is_empty());
        REQUIRE_FALSE(v.is_all_reals());
        REQUIRE(v.is_full());
        REQUIRE_FALSE(v.is_exact_real());
        REQUIRE_FALSE(v.is_nar());
        REQUIRE_FALSE(v.is_regular());
        REQUIRE(v.is_irregular());
    }

    {
        const Valid v = Valid::all_reals();

        REQUIRE_FALSE(v.is_zero());
        REQUIRE_FALSE(v.is_empty());
        REQUIRE(v.is_all_reals());
        REQUIRE_FALSE(v.is_full());
        REQUIRE_FALSE(v.is_exact_real());
        REQUIRE_FALSE(v.is_nar());
        REQUIRE(v.is_regular());
        REQUIRE_FALSE(v.is_irregular());
    }

    {
        const Valid v = Valid::zero();

        REQUIRE(v.is_zero());
        REQUIRE_FALSE(v.is_empty());
        REQUIRE_FALSE(v.is_all_reals());
        REQUIRE_FALSE(v.is_full());
        REQUIRE(v.is_exact_real());
        REQUIRE_FALSE(v.is_nar());
        REQUIRE(v.is_regular());
        REQUIRE_FALSE(v.is_irregular());
    }

    {
        const Valid v = Valid::one();

        REQUIRE_FALSE(v.is_zero());
        REQUIRE_FALSE(v.is_empty());
        REQUIRE_FALSE(v.is_all_reals());
        REQUIRE_FALSE(v.is_full());
        REQUIRE(v.is_exact_real());
        REQUIRE_FALSE(v.is_nar());
        REQUIRE(v.is_regular());
        REQUIRE_FALSE(v.is_irregular());
    }

    {
        const Valid v = Valid::min();

        REQUIRE_FALSE(v.is_zero());
        REQUIRE_FALSE(v.is_empty());
        REQUIRE_FALSE(v.is_all_reals());
        REQUIRE_FALSE(v.is_full());
        REQUIRE_FALSE(v.is_exact_real());
        REQUIRE_FALSE(v.is_nar());
        REQUIRE(v.is_regular());
        REQUIRE_FALSE(v.is_irregular());
    }

    {
        const Valid v = Valid::max();

        REQUIRE_FALSE(v.is_zero());
        REQUIRE_FALSE(v.is_empty());
        REQUIRE_FALSE(v.is_all_reals());
        REQUIRE_FALSE(v.is_full());
        REQUIRE_FALSE(v.is_exact_real());
        REQUIRE_FALSE(v.is_nar());
        REQUIRE(v.is_regular());
        REQUIRE_FALSE(v.is_irregular());
    }
}

TEMPLATE_TEST_CASE_SIG("valid concrete constructor", "[posit][valid][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Posit = posit<N, ES>;
    using Valid = valid<N, ES>;

    const Posit& p = GENERATE(take(1000, random_posit<Posit>()));
    const Valid v(p);

    REQUIRE(v.contains(p));
    REQUIRE(v.is_nar() == p.is_nar());
}

TEMPLATE_TEST_CASE_SIG("valid concrete constructor contains correct values",
                       "[posit][valid][template]", ((size_t N, size_t ES), N, ES),
                       AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Posit = posit<N, ES>;
    using Valid = valid<N, ES>;

    // First we throw the dice some concrete posit value. From this value we
    // create a valid.

    const Posit& concrete = GENERATE(take(10, random_posit<Posit>()));
    const Valid v(concrete);

    // Then we get another random posit. If per chance both random posits are
    // equal, the created valid should contain that given posit value.  If the
    // two random posits are different (most cases), the "other" posit should
    // not be contained in "v".

    const Posit& other = GENERATE(take(100, random_posit<Posit>()));

    if (concrete == other)
    {
        REQUIRE(v.contains(other));
    }
    else
    {
        REQUIRE_FALSE(v.contains(other));
    }
}

SCENARIO("canonical valid construction")
{
    using namespace aarith;

    using Posit = posit<3, 1>;
    using Valid = valid<3, 1>;

    GIVEN("endpoints")
    {
        const Posit four = Posit(4.0);
        const Posit one = Posit(1.0);
        const Posit fourth = Posit(0.25);
        const Posit zero = Posit(0.0);

        constexpr auto open = interval_bound::OPEN;
        constexpr auto closed = interval_bound::CLOSED;

        THEN("assert that valids are as expected")
        {
            {
                const Valid v = Valid::from(one, closed, four, closed);
                REQUIRE(v.in_interval_notation() == "[1, 4]");
            }

            {
                const Valid v = Valid::from(fourth, closed, four, closed);
                REQUIRE(v.in_interval_notation() == "[0.25, 4]");
            }

            {
                const Valid v = Valid::from(zero, closed, four, closed);
                REQUIRE(v.in_interval_notation() == "[0, 4]");
            }

            {
                const Valid v = Valid::from(one, open, four, open);
                REQUIRE(v.in_interval_notation() == "(1, 4)");
            }

            {
                const Valid v = Valid::from(fourth, open, four, open);
                REQUIRE(v.in_interval_notation() == "(0.25, 4)");
            }

            {
                const Valid v = Valid::from(zero, open, four, open);
                REQUIRE(v.in_interval_notation() == "(0, 4)");
            }

            {
                const Valid v = Valid::from(one, closed, four, open);
                REQUIRE(v.in_interval_notation() == "[1, 4)");
            }

            {
                const Valid v = Valid::from(fourth, open, four, closed);
                REQUIRE(v.in_interval_notation() == "(0.25, 4]");
            }

            {
                const Valid v = Valid::from(zero, closed, four, open);
                REQUIRE(v.in_interval_notation() == "[0, 4)");
            }
        }
    }
}
