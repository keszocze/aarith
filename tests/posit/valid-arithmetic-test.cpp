#include <algorithm>
#include <initializer_list>
#include <set>
#include <tuple>

#include <catch.hpp>

#include "../test-signature-ranges.hpp"
#include "gen_tile.hpp"
#include <aarith/posit.hpp>

template <size_t N, size_t ES, typename WT>
static bool any_irregular(const std::initializer_list<aarith::tile<N, ES, WT>>& ts)
{
    using Tile = aarith::tile<N, ES, WT>;

    /**
    * @brief Class to access private members in Valid
    *
    * Unfortunately there is no easy way to access private members, even in test
    * cases. This class serves that purpose.
    */
    class valid_friend final : public aarith::valid<N, ES, WT>
    {
    public:
        [[nodiscard]] static constexpr aarith::tile<N, ES, WT> get_open_pos_inf()
        {
            return aarith::valid<N, ES, WT>::open_pos_inf();
        }

        [[nodiscard]] static constexpr aarith::tile<N, ES, WT> get_closed_pos_inf()
        {
            return aarith::valid<N, ES, WT>::closed_pos_inf();
        }
    };

    const Tile oinf = valid_friend::get_open_pos_inf();
    const Tile cinf = valid_friend::get_closed_pos_inf();

    const auto is_irreg = [&](auto& t) -> bool { return t == oinf || t == cinf || t.is_nar(); };
    return std::any_of(begin(ts), end(ts), is_irreg);
}

template <size_t N, size_t ES, typename WT>
static bool any_irregular(const std::initializer_list<aarith::valid<N, ES, WT>>& vs)
{
    const auto is_irreg = [](auto& v) -> bool { return v.is_nar() || v.is_empty(); };
    return std::any_of(begin(vs), end(vs), is_irreg);
}

TEMPLATE_TEST_CASE_SIG("addition and subtraction regular case", "[valid][posit][template]",
                       ((size_t N, size_t ES), N, ES), AARITH_POSIT_TEST_TEMPLATE_EXHAUSTABLE)
{
    using namespace aarith;

    using Valid = valid<N, ES>;
    using Tile = tile<N, ES>;

    // Computing {a, b} ± {c, d} = {x, y}

    const Tile a = GENERATE(take(10, random_tile<Tile>()));
    const Tile b = GENERATE(take(10, random_tile<Tile>()));
    const Tile c = GENERATE(take(10, random_tile<Tile>()));
    const Tile d = GENERATE(take(10, random_tile<Tile>()));

    const Valid v = Valid::from(a, b);
    const Valid w = Valid::from(c, d);

    const Valid sum = v + w;
    const Valid diff = v - w;

    // While we generate a lot of test cases, some of them require
    // different tests. We only care about the regular cases here.
    // Which means lots of filtering out.

    if (any_irregular({a, b, c, d}) || any_irregular({v, w, sum, diff}))
    {
        return;
    }

    // Finally we can run the real test case.

    {
        const Tile& x = sum.get_start();
        const Tile& y = sum.get_end();

        REQUIRE(x.value() == (a.value() + b.value()));
        REQUIRE(y.value() == (c.value() + d.value()));
    }

    {
        const Tile& x = diff.get_start();
        const Tile& y = diff.get_end();

        REQUIRE(x.value() == (a.value() - d.value()));
        REQUIRE(y.value() == (b.value() - c.value()));
    }
}
