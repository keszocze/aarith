#pragma once

#include <aarith/posit/posit_additional_operators.hpp>
#include <aarith/posit_no_operators.hpp>

namespace aarith {
using posit_operators::operator*;
using posit_operators::operator*=;
using posit_operators::operator++;
using posit_operators::operator+;
using posit_operators::operator+=;
using posit_operators::operator--;
using posit_operators::operator-;
using posit_operators::operator-=;
using posit_operators::operator/;
using posit_operators::operator/=;
} // namespace aarith

namespace aarith {
using quire_operators::operator*;
using quire_operators::operator*=;
using quire_operators::operator+;
using quire_operators::operator+=;
using quire_operators::operator-;
using quire_operators::operator-=;
using quire_operators::operator/;
using quire_operators::operator/=;
} // namespace aarith
