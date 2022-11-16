#pragma once

#include <aarith/posit_no_operators.hpp>
#include <sstream>
#include <string>

namespace aarith {

template <size_t N, size_t ES, typename WT> std::string to_binary(const posit<N, ES, WT>& p)
{
    return to_binary(p.get_bits());
}

template <size_t N, size_t ES, typename WT>
std::string to_binary(const posit<N, ES, WT>& p, const char* delim)
{
    if (!delim)
    {
        delim = "";
    }

    const size_t nregime = get_num_regime_bits(p);
    const size_t nexp = get_num_exponent_bits(p);

    const std::string bs = to_binary(p.get_bits());

    std::stringstream ss;
    size_t i = N - 1;

    for (const char c : bs)
    {
        ss << c;

        if (i == 0)
        {
            // do not append additional separators at the end of the returned
            // string
            break;
        }

        if (i == N - 1)
        {
            // sign / regime gap
            ss << delim;
        }

        if (i == N - 1 - nregime)
        {
            // regime / exponent gap
            ss << delim;
        }

        if (i == N - 1 - nregime - nexp)
        {
            // exponent / fraction gap
            ss << delim;
        }

        i -= 1;
    }

    return ss.str();
}

} // namespace aarith