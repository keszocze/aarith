#pragma once

#include <aarith/posit.hpp>

namespace aarith {

template <size_t N, size_t ES, typename WT>
valid_tile<N, ES, WT>::valid_tile()
    : uncertain(false)
{
}

template <size_t N, size_t ES, typename WT>
valid_tile<N, ES, WT>::valid_tile(const valid_tile<N, ES, WT>& other)
    : posit<N, ES, WT>(other)
    , uncertain(other.uncertain)
{
}

template <size_t N, size_t ES, typename WT>
valid_tile<N, ES, WT>& valid_tile<N, ES, WT>::operator=(const valid_tile<N, ES, WT>& other)
{
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid_tile<N, ES, WT>::operator==(const valid_tile& other) const
{
    return (value() == other.value()) && (uncertain == other.uncertain);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid_tile<N, ES, WT>::operator!=(const valid_tile& other) const
{
    return !(*this == other);
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid_tile<N, ES, WT>::operator<(const valid_tile& other) const
{
    throw std::logic_error("valid_tile::operator< not implemented");
    return false;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid_tile<N, ES, WT>::operator<=(const valid_tile& other) const
{
    throw std::logic_error("valid_tile::operator<= not implemented");
    return false;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid_tile<N, ES, WT>::operator>(const valid_tile& other) const
{
    throw std::logic_error("valid_tile::operator> not implemented");
    return false;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] constexpr bool valid_tile<N, ES, WT>::operator>=(const valid_tile& other) const
{
    throw std::logic_error("valid_tile::operator>= not implemented");
    return false;
}

template <size_t N, size_t ES, typename WT> void valid_tile<N, ES, WT>::mark_as_uncertain()
{
    uncertain = true;
}

template <size_t N, size_t ES, typename WT> void valid_tile<N, ES, WT>::mark_as_certain()
{
    uncertain = false;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] posit<N, ES, WT>& valid_tile<N, ES, WT>::value()
{
    return *this;
}

template <size_t N, size_t ES, typename WT>
[[nodiscard]] const posit<N, ES, WT>& valid_tile<N, ES, WT>::value() const
{
    return *this;
}

} // namespace aarith
