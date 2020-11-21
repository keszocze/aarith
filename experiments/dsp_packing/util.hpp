#pragma once

#include <aarith/integer.hpp>

#include <iostream>
#include <string>

namespace dsp_packing {
/**
 * @brief Helper method for printing out numbers in decimal and binary at once (including a name)
 * @tparam I The data type to display
 * @param n The number to display
 * @param name
 */
template <typename I> void print_int(const I& n, const std::string name)
{
    std::cout << aarith::to_binary(n) << "\t" << n << "\t(" << name << ")\n";
}
}