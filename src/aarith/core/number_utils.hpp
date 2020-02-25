#pragma once

namespace aarith {

template <unsigned Radix, unsigned Value> constexpr double log();

template <> constexpr double log<2, 10>()
{
    return 3.32192809489;
}

template <> constexpr double log<10, 2>()
{
    return 0.30102999566;
}

template <class Result> constexpr Result ceil(double num)
{
    return (static_cast<double>(static_cast<Result>(num)) == num)
               ? static_cast<Result>(num)
               : static_cast<Result>(num) + ((num > 0) ? 1 : 0);
}

} // namespace aarith