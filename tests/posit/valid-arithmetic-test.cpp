#include <catch.hpp>
#include <set>
#include <tuple>

#include "../test-signature-ranges.hpp"
#include "gen_tile.hpp"

#include <aarith/posit.hpp>

/**
 * @brief Class to access private members in Valid
 *
 * Unfortunately there is no easy way to access private members, even in test
 * cases. This class serves that purpose.
 */
template <size_t N, size_t ES, typename WT = aarith::DefaultWordType>
class valid_friend final : public aarith::valid<N, ES, WT>
{
public:
    static aarith::tile<N, ES, WT> get_open_pos_inf()
    {
        return aarith::valid<N, ES, WT>::open_pos_inf();
    }

    static aarith::tile<N, ES, WT> get_closed_pos_inf()
    {
        return aarith::valid<N, ES, WT>::closed_pos_inf();
    }
};

TEMPLATE_TEST_CASE_SIG("valid sums", "[valid][posit][template]", ((size_t N, size_t ES), N, ES),
                       AARITH_POSIT_TEST_TEMPLATE_EXHAUSTABLE)
{
    using namespace aarith;

    using Valid = valid<N, ES>;
    using Tile = tile<N, ES>;

    // Computing {a, b} + {c, d} = {x, y}

    const Tile a = GENERATE(take(10, random_tile<Tile>()));
    const Tile b = GENERATE(take(10, random_tile<Tile>()));
    const Tile c = GENERATE(take(10, random_tile<Tile>()));
    const Tile d = GENERATE(take(10, random_tile<Tile>()));

    const Valid v = Valid::from(a, b);
    const Valid w = Valid::from(c, d);
    const Valid sum = v + w;

    // While we generate a lot of test cases, some of them require
    // different tests. We only care about the regular cases here.
    // Which means lots of filtering out.

    if (a.value().is_nar() || b.value().is_nar() || c.value().is_nar() || d.value().is_nar())
    {
        return;
    }

    if (a.is_nar() || b.is_nar() || c.is_nar() || d.is_nar())
    {
        return;
    }

    {
        const Tile oinf = valid_friend<N, ES>::get_open_pos_inf();

        if (a == oinf || b == oinf || c == oinf || d == oinf)
        {
            return;
        }
    }

    {
        const Tile cinf = valid_friend<N, ES>::get_closed_pos_inf();

        if (a == cinf || b == cinf || c == cinf || d == cinf)
        {
            return;
        }
    }

    if (sum.is_empty() || v.is_empty() || w.is_empty())
    {
        return;
    }

    // Finally we can run the real test case.

    const Tile& x = sum.get_start();
    const Tile& y = sum.get_end();

    REQUIRE(x.value() == (a.value() + b.value()));
    REQUIRE(y.value() == (c.value() + d.value()));
}
