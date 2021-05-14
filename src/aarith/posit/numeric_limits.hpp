#pragma once

#include <aarith/posit.hpp>

template <size_t N, size_t ES, typename WT> class std::numeric_limits<::aarith::posit<N, ES, WT>>
{
public:
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = true;
    static constexpr bool is_integer = false;
    static constexpr bool is_exact = false;
    static constexpr bool has_infinity = false;
    static constexpr bool has_quiet_NaN = true;
    static constexpr bool has_signaling_NaN = false;
    static constexpr bool is_bounded = true;
    static constexpr std::float_round_style round_style = std::round_to_nearest;
    static constexpr int radix = 2;
    static constexpr int digits = N;
    static constexpr bool traps = false;

    static constexpr ::aarith::posit<N, ES, WT> min() noexcept
    {
        return ::aarith::posit<N, ES, WT>::min();
    }

    static constexpr ::aarith::posit<N, ES, WT> lowest() noexcept
    {
        return ::aarith::posit<N, ES, WT>::min();
    }

    static constexpr ::aarith::posit<N, ES, WT> max() noexcept
    {
        return ::aarith::posit<N, ES, WT>::max();
    }

    static constexpr ::aarith::posit<N, ES, WT> epsilon() noexcept
    {
        return ::aarith::posit<N, ES, WT>::minpos();
    }

    static constexpr ::aarith::posit<N, ES, WT> quiet_NaN() noexcept
    {
        return ::aarith::posit<N, ES, WT>::nar();
    }
};
