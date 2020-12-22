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
template <typename I> void print_int(const I& n, const std::string& name)
{
    std::cout << aarith::to_binary(n) << "\t" << n << "\t(" << name << ")\n";
}

/**
 * @brief Function to iterate over all inputs for the DSP calling a provided functor on each vector
 *
 * Each generated input vector is forwarded to the provided functor realizing the desired
 * functionality such as, for example, feeding the vector to the DSP to actually compute the
 * multiple multiplications.
 *
 * @tparam T
 * @param functor The functionality that is called on each input vector.
 */
template <typename T, size_t WWidth = 4, size_t AWidth = 4> void iterate_inputs(T& functor)
{
    for (aarith::integer<WWidth> w1 : aarith::integer_range<aarith::uinteger<WWidth>>())
    {
        for (aarith::integer<WWidth> w2 : aarith::integer_range<aarith::uinteger<WWidth>>())
        {
            for (aarith::uinteger<AWidth> a1 : aarith::integer_range<aarith::uinteger<AWidth>>())
            {
                for (aarith::uinteger<AWidth> a2 :
                     aarith::integer_range<aarith::uinteger<AWidth>>())
                {
                    functor(w1, w2, a1, a2);
                }
            }
        }
    }
}

/**
 * @brief Prints all inputs in the correct order as used in the experiment.
 *
 * @param print_header If true, print column header
 */
template <size_t WWidth = 4, size_t AWidth = 4> void print_inputs(const bool print_header = false)
{
    auto output = [](auto a1, auto a2, auto w1, auto w2) {
        std::cout << aarith::to_binary(a1) << " " << aarith::to_binary(a2) << " "
                  << aarith::to_binary(w1) << " " << aarith::to_binary(w2) << "\n";
    };

    if (print_header)
    {
        std::cout << "a1   a2   w1   w2\n";
    }

    iterate_inputs<decltype(output), WWidth, AWidth>(output);
}

} // namespace dsp_packing