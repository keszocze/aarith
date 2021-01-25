#include <catch.hpp>

#include <aarith/posit.hpp>
#include <set>
#include <vector>

using namespace aarith;

// For the following test cases, we look at <3, 1> posits. We instantiate all
// possible values and related tiles here. This makes the following tests more
// concise.

static const size_t N = 3;
static const size_t ES = 1;

using Posit = posit<N, ES>;
using Tile = tile<N, ES>;
using Valid = valid<N, ES>;

// Posit pᵢ is the i-th posit if you start with p₀ = NaR and then
// progress counter-clockwise on the projective reals.

static const Posit p0 = Posit::from(0b100);
static const Posit p1 = Posit::from(0b101);
static const Posit p2 = Posit::from(0b110);
static const Posit p3 = Posit::from(0b111);
static const Posit p4 = Posit::from(0b000);
static const Posit p5 = Posit::from(0b001);
static const Posit p6 = Posit::from(0b010);
static const Posit p7 = Posit::from(0b011);

// Tile tᵢ is the tile with value pᵢ and u-bit set to 0. It is the
// tile that represents pᵢ exactly.

static const Tile t0 = Tile::from(p0, false);
static const Tile t1 = Tile::from(p1, false);
static const Tile t2 = Tile::from(p2, false);
static const Tile t3 = Tile::from(p3, false);
static const Tile t4 = Tile::from(p4, false);
static const Tile t5 = Tile::from(p5, false);
static const Tile t6 = Tile::from(p6, false);
static const Tile t7 = Tile::from(p7, false);

// Tile uᵢ is the tile with value pᵢ and u-bit set to 1. It is the
// tile that represents the open interval (pᵢ, incremented(pᵢ)).

static const Tile u0 = Tile::from(p0, true);
static const Tile u1 = Tile::from(p1, true);
static const Tile u2 = Tile::from(p2, true);
static const Tile u3 = Tile::from(p3, true);
static const Tile u4 = Tile::from(p4, true);
static const Tile u5 = Tile::from(p5, true);
static const Tile u6 = Tile::from(p6, true);
static const Tile u7 = Tile::from(p7, true);

static const std::vector<Tile> all_tiles = {t0, t1, t2, t3, t4, t5, t6, t7,
                                            u0, u1, u2, u3, u4, u5, u6, u7};

// The following valids should cover most use cases. We have open and closed
// bounds. Some intervals that cross infinity and some do not.

static const Valid v0 = Valid::from(t4, t6);
static const Valid v1 = Valid::from(t3, t4);
static const Valid v2 = Valid::from(t2, t6);
static const Valid v3 = Valid::from(t7, t1);
static const Valid v4 = Valid::from(t7, t1);
static const Valid v5 = Valid::from(u0, u1);
static const Valid v6 = Valid::from(u5, u6);
static const Valid v7 = Valid::from(t2, u4);
static const Valid v8 = Valid::from(u3, t6);

template <typename T>
static void require_same(const std::vector<T>& actual, const std::vector<T>& expected)
{
    CAPTURE(actual, expected);

    const std::set<T> actual_set(begin(actual), end(actual));
    const std::set<T> expected_set(begin(expected), end(expected));

    REQUIRE(actual.size() == expected.size());
    REQUIRE(actual_set == expected_set);
}

template <size_t N, size_t ES, typename WT>
static void require_contains(const valid<N, ES, WT>& v,
                             const std::vector<tile<N, ES, WT>>& expected)
{
    for (const auto value : expected)
    {
        CAPTURE(v, value);
        REQUIRE(v.contains(value));
    }
}

SCENARIO("Valids contain the correct tiles")
{
    GIVEN("Special case valids")
    {
        const Valid zero = Valid::zero();
        const Valid one = Valid::one();
        const Valid empty = Valid::empty();
        const Valid nar = Valid::nar();

        THEN("Assert that they cotain the correct values")
        {
            require_same(all_values_in(zero), {Tile::zero()});
            require_same(all_values_in(one), {Tile::one()});
            require_same(all_values_in(empty), {});
            require_same(all_values_in(nar), {Tile::nar()});
        }
    }

    GIVEN("Some arbitrary valid endpoints")
    {
        THEN("Assert that some arbitrary valids contain the correct entries")
        {
            require_same(all_values_in(v0), {t4, u4, t5, u5, t6});
            require_contains(v0, {t4, u4, t5, u5, t6});

            require_same(all_values_in(v1), {t3, u3, t4});
            require_contains(v1, {t3, u3, t4});

            require_same(all_values_in(v2), {t2, u2, t3, u3, t4, u4, t5, u5, t6});
            require_contains(v2, {t2, u2, t3, u3, t4, u4, t5, u5, t6});

            require_same(all_values_in(v3), {t7, u7, t0, u0, t1});
            require_contains(v3, {t7, u7, t0, u0, t1});

            require_same(all_values_in(v4), {t7, u7, t0, u0, t1});
            require_contains(v4, {t7, u7, t0, u0, t1});

            require_same(all_values_in(v5), {u0, t1, u1});
            require_contains(v5, {u0, t1, u1});

            require_same(all_values_in(v6), {u5, t6, u6});
            require_contains(v6, {u5, t6, u6});

            require_same(all_values_in(v7), {t2, u2, t3, u3, t4, u4});
            require_contains(v7, {t2, u2, t3, u3, t4, u4});

            require_same(all_values_in(v8), {u3, t4, u4, t5, u5, t6});
            require_contains(v8, {u3, t4, u4, t5, u5, t6});
        }
    }
}

SCENARIO("Valid Comparison")
{
    GIVEN("Special case valids")
    {
        const Valid zero = Valid::zero();
        const Valid one = Valid::one();
        const Valid empty = Valid::empty();
        const Valid nar = Valid::nar();

        THEN("Assert that they compare as expected")
        {
            REQUIRE_FALSE(zero < zero);
            REQUIRE(zero < one);
            REQUIRE_FALSE(zero < empty);
            REQUIRE_FALSE(zero < nar);

            REQUIRE(one > zero);
            REQUIRE_FALSE(one < one);
            REQUIRE_FALSE(one < empty);
            REQUIRE_FALSE(one < nar);

            REQUIRE_FALSE(empty < zero);
            REQUIRE_FALSE(empty < one);
            REQUIRE_FALSE(empty < empty);
            REQUIRE_FALSE(empty < nar);

            REQUIRE_FALSE(nar < zero);
            REQUIRE_FALSE(nar < one);
            REQUIRE_FALSE(nar < empty);
            REQUIRE_FALSE(nar < nar);
        }
    }

    GIVEN("Some arbitrary valid endpoints")
    {
        THEN("Assert that they compare as expected")
        {
            REQUIRE_FALSE(v0 < v0);
            REQUIRE_FALSE(v0 < v1);
            REQUIRE_FALSE(v0 < v2);
            REQUIRE_FALSE(v0 < v3);
            REQUIRE_FALSE(v0 < v4);
            REQUIRE_FALSE(v0 < v5);
            REQUIRE_FALSE(v0 < v6);
            REQUIRE_FALSE(v0 < v7);
            REQUIRE_FALSE(v0 < v8);

            REQUIRE_FALSE(v1 < v0);
            REQUIRE_FALSE(v1 < v1);
            REQUIRE_FALSE(v1 < v2);
            REQUIRE_FALSE(v1 < v3);
            REQUIRE_FALSE(v1 < v4);
            REQUIRE_FALSE(v1 < v5);
            REQUIRE(v1 < v6);
            REQUIRE_FALSE(v1 < v7);
            REQUIRE_FALSE(v1 < v8);

            REQUIRE_FALSE(v2 < v0);
            REQUIRE_FALSE(v2 < v1);
            REQUIRE_FALSE(v2 < v2);
            REQUIRE_FALSE(v2 < v3);
            REQUIRE_FALSE(v2 < v4);
            REQUIRE_FALSE(v2 < v5);
            REQUIRE_FALSE(v2 < v6);
            REQUIRE_FALSE(v2 < v7);
            REQUIRE_FALSE(v2 < v8);

            REQUIRE_FALSE(v3 < v0);
            REQUIRE_FALSE(v3 < v1);
            REQUIRE_FALSE(v3 < v2);
            REQUIRE_FALSE(v3 < v3);
            REQUIRE_FALSE(v3 < v4);
            REQUIRE_FALSE(v3 < v5);
            REQUIRE_FALSE(v3 < v6);
            REQUIRE_FALSE(v3 < v7);
            REQUIRE_FALSE(v3 < v8);

            REQUIRE_FALSE(v4 < v0);
            REQUIRE_FALSE(v4 < v1);
            REQUIRE_FALSE(v4 < v2);
            REQUIRE_FALSE(v4 < v3);
            REQUIRE_FALSE(v4 < v4);
            REQUIRE_FALSE(v4 < v5);
            REQUIRE_FALSE(v4 < v6);
            REQUIRE_FALSE(v4 < v7);
            REQUIRE_FALSE(v4 < v8);

            REQUIRE(v5 < v0);
            REQUIRE(v5 < v1);
            REQUIRE(v5 < v2);
            REQUIRE_FALSE(v5 < v3);
            REQUIRE_FALSE(v5 < v4);
            REQUIRE_FALSE(v5 < v5);
            REQUIRE(v5 < v6);
            REQUIRE(v5 < v7);
            REQUIRE(v5 < v8);

            REQUIRE_FALSE(v6 < v0);
            REQUIRE_FALSE(v6 < v1);
            REQUIRE_FALSE(v6 < v2);
            REQUIRE_FALSE(v6 < v3);
            REQUIRE_FALSE(v6 < v4);
            REQUIRE_FALSE(v6 < v5);
            REQUIRE_FALSE(v6 < v6);
            REQUIRE_FALSE(v6 < v7);
            REQUIRE_FALSE(v6 < v8);

            REQUIRE_FALSE(v7 < v0);
            REQUIRE_FALSE(v7 < v1);
            REQUIRE_FALSE(v7 < v2);
            REQUIRE_FALSE(v7 < v3);
            REQUIRE_FALSE(v7 < v4);
            REQUIRE_FALSE(v7 < v5);
            REQUIRE(v7 < v6);
            REQUIRE_FALSE(v7 < v7);
            REQUIRE_FALSE(v7 < v8);

            REQUIRE_FALSE(v8 < v0);
            REQUIRE_FALSE(v8 < v1);
            REQUIRE_FALSE(v8 < v2);
            REQUIRE_FALSE(v8 < v3);
            REQUIRE_FALSE(v8 < v4);
            REQUIRE_FALSE(v8 < v5);
            REQUIRE_FALSE(v8 < v6);
            REQUIRE_FALSE(v8 < v7);
            REQUIRE_FALSE(v8 < v8);
        }
    }
}
