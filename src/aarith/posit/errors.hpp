#pragma once

#include <aarith/posit.hpp>

namespace aarith {

inline posit_error::posit_error(const std::string& whatarg)
    : std::runtime_error(whatarg)
{
}

inline nar_error::nar_error(const std::string& whatarg)
    : posit_error(whatarg)
{
}

} // namespace aarith
