#include <algorithm>
#include <set>
#include <vector>

#include <catch.hpp>

#include <aarith/posit.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_posit.hpp"

template <size_t N, size_t ES, typename WT>
static std::vector<aarith::valid<N, ES, WT>> all_valids(const aarith::posit<N, ES, WT>& start,
                                                        const aarith::posit<N, ES, WT>& end)
{
    using namespace aarith;

    using Valid = valid<N, ES, WT>;

    constexpr auto open = interval_bound::OPEN;
    constexpr auto closed = interval_bound::CLOSED;

    return {Valid::from(start, open, end, open), Valid::from(start, open, end, closed),
            Valid::from(start, closed, end, open), Valid::from(start, closed, end, closed)};
}

TEMPLATE_TEST_CASE_SIG("multiplying one does not change the result (random)",
                       "[valid][posit][template]", ((size_t N, size_t ES), N, ES),
                       AARITH_POSIT_TEST_TEMPLATE_BIG)
{
    using namespace aarith;

    using Posit = posit<N, ES>;
    using Valid = valid<N, ES>;

    const Posit start = GENERATE(take(5, random_posit<Posit>()));
    const Posit end = GENERATE(take(5, random_posit<Posit>()));

    const Valid one = Valid::one();

    for (const Valid& v : all_valids(start, end))
    {
        if (v.is_nar())
        {
            REQUIRE((v * one) == Valid::nar());
            REQUIRE((one * v) == Valid::nar());
        }
        else if (v.is_regular())
        {
            REQUIRE((v * one) == v);
            REQUIRE((one * v) == v);
        }
    }
}

TEMPLATE_TEST_CASE_SIG("multiplying zero works as expected (random)", "[valid][posit][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_FULL)
{
    using namespace aarith;

    using Posit = posit<N, ES>;
    using Valid = valid<N, ES>;

    const Posit start = GENERATE(take(5, random_posit<Posit>()));
    const Posit end = GENERATE(take(5, random_posit<Posit>()));

    const Valid zero = Valid::zero();

    for (const Valid& v : all_valids(start, end))
    {
        if (v.is_nar())
        {
            REQUIRE((v * zero) == Valid::nar());
            REQUIRE((zero * v) == Valid::nar());
        }
        else if (v.is_empty())
        {
            REQUIRE((v * zero) == Valid::empty());
            REQUIRE((zero * v) == Valid::empty());
        }
        else if (v.is_full())
        {
            REQUIRE((v * zero) == Valid::full());
            REQUIRE((zero * v) == Valid::full());
        }
        else if (v.is_regular())
        {
            REQUIRE((v * zero) == zero);
            REQUIRE((zero * v) == zero);
        }
    }
}

SCENARIO("multiplying arbitrary interval valids")
{
    using namespace aarith;

    using Posit = posit<5, 1>;
    using Valid = valid<5, 1>;

    GIVEN("arbitrary valids")
    {
        constexpr auto open = interval_bound::OPEN;
        constexpr auto closed = interval_bound::CLOSED;

        THEN("assert that their product is as expected")
        {
            {
                const Valid v = Valid::from(Posit(3), closed, Posit(3), closed); // [3, 3] == 3

                const Valid actual = v * v;
                const Valid expected = Valid::from(Posit(8), open, Posit(16), open); // (8, 16)

                REQUIRE(actual == expected);
            }

            {
                const Valid v = Valid::from(Posit(2), closed, Posit(4), closed);      // [2, 4]
                const Valid w = Valid::from(Posit(0.25), closed, Posit(0.5), closed); // [1/4, 1/2]

                const Valid actual = v * w;
                const Valid expected =
                    Valid::from(Posit(0.5), closed, Posit(2), closed); // [1/2, 2]

                REQUIRE(actual == expected);
            }

            {
                const Valid v = Valid::from(Posit(2), open, Posit(4), closed);        // (2, 4]
                const Valid w = Valid::from(Posit(0.25), closed, Posit(0.5), closed); // [1/4, 1/2]

                const Valid actual = v * w;
                const Valid expected = Valid::from(Posit(0.5), open, Posit(2), closed); // (1/2, 2]

                REQUIRE(actual == expected);
            }

            {
                const Valid v = Valid::from(Posit(2), closed, Posit(4), closed);    // [2, 4]
                const Valid w = Valid::from(Posit(0.25), open, Posit(0.5), closed); // (1/4, 1/2]

                const Valid actual = v * w;
                const Valid expected = Valid::from(Posit(0.5), open, Posit(2), closed); // (1/2, 2]

                REQUIRE(actual == expected);
            }

            {
                const Valid v = Valid::from(Posit(2), closed, Posit(4), open);        // [2, 4)
                const Valid w = Valid::from(Posit(0.25), closed, Posit(0.5), closed); // [1/4, 1/2]

                const Valid actual = v * w;
                const Valid expected = Valid::from(Posit(0.5), closed, Posit(2), open); // [1/2, 2)

                REQUIRE(actual == expected);
            }

            {
                const Valid v = Valid::from(Posit(2), closed, Posit(4), closed);    // [2, 4]
                const Valid w = Valid::from(Posit(0.25), closed, Posit(0.5), open); // [1/4, 1/2)

                const Valid actual = v * w;
                const Valid expected = Valid::from(Posit(0.5), closed, Posit(2), open); // [1/2, 2)

                REQUIRE(actual == expected);
            }

            {
                const Valid v = Valid::from(Posit(2), open, Posit(4), open);          // (2, 4)
                const Valid w = Valid::from(Posit(0.25), closed, Posit(0.5), closed); // [1/4, 1/2]

                const Valid actual = v * w;
                const Valid expected = Valid::from(Posit(0.5), open, Posit(2), open); // (1/2, 2)

                REQUIRE(actual == expected);
            }

            {
                const Valid v = Valid::from(Posit(2), open, Posit(4), open);      // (2, 4)
                const Valid w = Valid::from(Posit(0.25), open, Posit(0.5), open); // (1/4, 1/2)

                const Valid actual = v * w;
                const Valid expected = Valid::from(Posit(0.5), open, Posit(3), open); // (1/2, 2)

                REQUIRE(actual == expected);
            }

            {
                const Valid v = Valid::from(Posit(-2), closed, Posit(4), closed); // [-2, 4]
                const Valid w =
                    Valid::from(Posit(-0.25), closed, Posit(0.5), closed); // [-1/4, 1/2]

                const Valid actual = v * w;
                const Valid expected = Valid::from(Posit(-1), closed, Posit(2), closed); // [-1, 2]

                REQUIRE(actual == expected);
            }

            {
                const Valid v = Valid::from(Posit(-2), closed, Posit(4), closed); // (-2, 4)
                const Valid w =
                    Valid::from(Posit(-0.25), closed, Posit(0.5), closed); // [-1/4, 1/2]

                const Valid actual = v * w;
                const Valid expected = Valid::from(Posit(-1), closed, Posit(2), closed); // (-1, 2)

                REQUIRE(actual == expected);
            }

            {
                // Taken from "The End of Error", p. 128.
                //
                // This is a tricky one. Multiplying v = [-1, 2) with w = (-1/2, 1] means
                // we have the following choices:
                //
                //   (-1)    * (-1/2 + ε) = 1/2 - ε,                 (I)
                //   (-1)    * (-1)       = -1,                      (II)
                //   (2 - ε) * (-1/2 + η) = -1 + 2η + ε/2 - εη,      (III)
                //   (2 - ε) * (1)        = 2 - ε                    (IV)
                //
                // Finding the maximum is obvious, it is (IV). But what about
                // the lower bound?  Both (II) and (III) have end point (-1),
                // but (III) has awkward infinitesimals which we are not at
                // all sure about whether they are positive or negative. So we
                // have to make a conservative guess an return the result
                //
                //   v * w = {(III); (IV)} = (Pred(-1), 2)
                //
                // which is the best thing we could possibly return.

                const Valid v = Valid::from(Posit(-1), closed, Posit(2), open);   // [-1, 2)
                const Valid w = Valid::from(Posit(-0.5), open, Posit(1), closed); // (-1/2, 1]

                const Valid actu = v * w;
                const Valid expec = Valid::from(Posit(-1).decremented_real(), open, Posit(2), open);

                REQUIRE(actu == expec);
            }
        }
    }

    GIVEN("posit exact valids that can be multiplied w/o rounding")
    {
        THEN("assert that their product is as expected")
        {
            const std::set<double> values51 = {
                64.0,       16.0,        8.0,        4.0,        3.0,        2.0,        3.0 / 2.0,
                1.0,        3.0 / 4.0,   1.0 / 2.0,  3.0 / 8.0,  1.0 / 4.0,  1.0 / 8.0,  1.0 / 16.0,
                1.0 / 64.0, 0.0,         -64.0,      -16.0,      -8.0,       -4.0,       -3.0,
                -2.0,       -3.0 / 2.0,  -1.0,       -3.0 / 4.0, -1.0 / 2.0, -3.0 / 8.0, -1.0 / 4.0,
                -1.0 / 8.0, -1.0 / 16.0, -1.0 / 64.0};

            const auto type51_has_value = [&](double x) -> bool {
                return std::find(values51.begin(), values51.end(), x) != values51.end();
            };

            for (const double v_value : values51)
            {
                for (const double w_value : values51)
                {
                    const double expected = v_value * w_value;

                    if (!type51_has_value(expected))
                    {
                        // We cannot represent the product of v and w exactly,
                        // so don't even bother trying.
                        continue;
                    }

                    const Valid v = Valid(Posit(v_value));
                    const Valid w = Valid(Posit(w_value));
                    const Valid product = v * w;

                    REQUIRE(product.is_exact());

                    const double actual = static_cast<double>(product.as_exact());
                    REQUIRE(actual == expected);
                }
            }
        }
    }
}

SCENARIO("multiplying arbitrary low res valids")
{
    using namespace aarith;

    using Posit = posit<3, 1>;
    using Valid = valid<3, 1>;

    GIVEN("arbitrary valids")
    {
        constexpr auto open = interval_bound::OPEN;
        constexpr auto closed = interval_bound::CLOSED;

        THEN("assert that their product is as expected")
        {
            {
                const Valid v = Valid::from(Posit(-1), closed, Posit(0), closed);   // [-1, 0]
                const Valid w = Valid::from(Posit(-4), closed, Posit(-0.25), open); // [-4, -1/4)

                const Valid actual = v * w;
                const Valid expected = Valid::from(Posit(0), closed, Posit(4), closed); // [0, 4]

                REQUIRE(actual == expected);
            }

            {
                const Valid v = Valid::from(Posit(0), closed, Posit(4), closed); // [0, 4]
                const Valid w = Valid(Posit(4));                                 // [4, 4]

                const Valid actual = v * w;
                const Valid expected = Valid::from(Posit(0), closed, Posit::nar(), open); // [0, ∞)

                REQUIRE(actual == expected);
            }

            {
                const Valid v = Valid::from(Posit(0), closed, Posit(4), open); // [0, 4)
                const Valid w = Valid(Posit(4));                               // [4, 4]

                const Valid actual = v * w;
                const Valid expected = Valid::from(Posit(0), closed, Posit::nar(), open); // [0, ∞)

                REQUIRE(actual == expected);
            }

            {
                const Valid v = Valid::from(Posit(0.25), closed, Posit(1), open);     // [1/4, 1)
                const Valid w = Valid::from(Posit(-4), closed, Posit(-0.25), closed); // [-4, -1/4]

                const Valid actual = v * w;
                const Valid expected = Valid::from(Posit(-4), open, Posit(0), open); // (-4, 0)

                REQUIRE(actual == expected);
            }
        }
    }
}
