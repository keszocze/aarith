#include <catch.hpp>
#include <set>

#include <aarith/posit.hpp>

#include "valid_3_1.hpp"

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
