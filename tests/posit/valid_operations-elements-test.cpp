#include <catch.hpp>

#include <aarith/posit.hpp>
#include <set>
#include <vector>

template <typename T>
static void require_same(const std::vector<T>& actual, const std::vector<T>& expected)
{
    CAPTURE(actual, expected);

    const std::set<T> actual_set(begin(actual), end(actual));
    const std::set<T> expected_set(begin(expected), end(expected));

    REQUIRE(actual.size() == expected.size());
    REQUIRE(actual_set == expected_set);
}

SCENARIO("Valid Tile Comparisons")
{
    using namespace aarith;

    static const size_t N = 3;
    static const size_t ES = 1;

    using Posit = posit<N, ES>;
    using Tile = tile<N, ES>;
    using Valid = valid<N, ES>;

    GIVEN("Special case valids")
    {
        const Valid zero = Valid::zero();
        require_same(all_values_in(zero), { Tile::zero() });

        const Valid one = Valid::one();
        require_same(all_values_in(one), { Tile::one() });

        const Valid empty = Valid::empty();
        require_same(all_values_in(empty), {});

        const Valid nar = Valid::nar();
        require_same(all_values_in(nar), { Tile::nar() });
    }

    GIVEN("Some arbitrary valid endpoints")
    {
        // Posit pᵢ is the i-th posit if you start with p₀ = NaR and then
        // progress counter-clockwise on the projective reals.

        const Posit p0 = Posit::from(0b100);
        const Posit p1 = Posit::from(0b101);
        const Posit p2 = Posit::from(0b110);
        const Posit p3 = Posit::from(0b111);
        const Posit p4 = Posit::from(0b000);
        const Posit p5 = Posit::from(0b001);
        const Posit p6 = Posit::from(0b010);
        const Posit p7 = Posit::from(0b011);

        // Tile tᵢ is the tile with value pᵢ and u-bit set to 0. It is the
        // tile that represents pᵢ exactly.

        const Tile t0 = Tile::from(p0, false);
        const Tile t1 = Tile::from(p1, false);
        const Tile t2 = Tile::from(p2, false);
        const Tile t3 = Tile::from(p3, false);
        const Tile t4 = Tile::from(p4, false);
        const Tile t5 = Tile::from(p5, false);
        const Tile t6 = Tile::from(p6, false);
        const Tile t7 = Tile::from(p7, false);

        // Tile uᵢ is the tile with value pᵢ and u-bit set to 1. It is the
        // tile that represents the open interval (pᵢ, incremented(pᵢ)).

        const Tile u0 = Tile::from(p0, true);
        const Tile u1 = Tile::from(p1, true);
        const Tile u2 = Tile::from(p2, true);
        const Tile u3 = Tile::from(p3, true);
        const Tile u4 = Tile::from(p4, true);
        const Tile u5 = Tile::from(p5, true);
        const Tile u6 = Tile::from(p6, true);
        const Tile u7 = Tile::from(p7, true);

        THEN("Assert that some arbitrary valids contain the correct entries")
        {
            const Valid v0 = Valid::from(t4, t6);
            require_same(all_values_in(v0), { t4, u4, t5, u5, t6 });

            const Valid v1 = Valid::from(t3, t4);
            require_same(all_values_in(v1), { t3, u3, t4 });

            const Valid v2 = Valid::from(t2, t6);
            require_same(all_values_in(v2), { t2, u2, t3, u3, t4, u4, t5, u5, t6 });

            const Valid v3 = Valid::from(t7, t1);
            require_same(all_values_in(v3), { t7, u7, t0, u0, t1 });

            const Valid v4 = Valid::from(t7, t1);
            require_same(all_values_in(v4), { t7, u7, t0, u0, t1 });

            const Valid v5 = Valid::from(u0, u1);
            require_same(all_values_in(v5), { u0, t1, u1 });

            const Valid v6 = Valid::from(u5, u6);
            require_same(all_values_in(v6), { u5, t6, u6 });

            const Valid v7 = Valid::from(t2, u4);
            require_same(all_values_in(v7), { t2, u2, t3, u3, t4, u4 });

            const Valid v8 = Valid::from(u3, t6);
            require_same(all_values_in(v8), { u3, t4, u4, t5, u5, t6 });
        }
    }
}
