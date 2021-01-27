#pragma once

#include <algorithm>
#include <initializer_list>

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
