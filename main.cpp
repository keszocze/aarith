#include <aarith/float/float_comparisons.hpp>
#include <aarith/float/float_operations.hpp>
#include <aarith/float/normalized_float.hpp>

#include <aarith/core/string_utils.hpp>
#include <aarith/float/string_utils.hpp>

#include <iostream>
#include <string>

using nf_t = aarith::normalized_float<3, 5>;

// something like this would be useful in aarith
// using newly committed conversions here
template <size_t exp_bits, size_t man_bits> inline float to_float(nf_t x)
{
    std::cout << "to_float: M=" << x.get_mantissa() << " E=" << x.get_exponent()
              << " S=" << x.get_sign() << " B=" << x.get_bias() << std::endl;
    const size_t float_man_len = 23;
    const size_t float_exp_len = 8;
    const int float_bias = 127;

    float res;
    uint32_t float_rep = 0U;

    if (x.get_sign())
        // set sign bit
        float_rep |= (1 << 31);

    uint32_t mantissa_part;
    // get them bits
    mantissa_part = static_cast<uint32_t>(x.get_mantissa());
    // truncate to given size - maybe we can omit this?
    mantissa_part &= ((1 << man_bits) - 1);
    // adjust mantissa with left shift
    static_assert(man_bits <= float_man_len);
    // make old format MSBs to new format MSBs
    mantissa_part <<= (float_man_len - man_bits);
    float_rep &= mantissa_part;

    uint32_t exponent_part, exponent_bias;
    // same for exponent
    exponent_part = static_cast<uint32_t>(x.get_exponent());
    exponent_bias = static_cast<uint32_t>(x.get_bias());
    exponent_part &= ((1 << exp_bits) - 1);
    // remove old biasing
    exponent_part -= exponent_bias;
    // rebias for float32
    exponent_part += float_bias;
    static_assert(exp_bits <= float_exp_len);
    // CAREFUL - skip mantissa bits
    exponent_part <<= float_man_len;
    float_rep &= exponent_part;

    // bitwise manipulation with different types sucks
    // for now work around using std::memcpy
    // compiler should optimize this
    std::memcpy(&res, &float_rep, sizeof(float));
    return res;
}

int main()
{
    using namespace aarith;

    // We have numbers
    const float number_a_f = 0.5;
    const float number_b_f = 0.25;
    const double number_a_d = 0.5;
    const double number_b_d = 0.25;

    // we convert them to some normfloat values
    const nf_t nf_a_f(number_a_f);
    const nf_t nf_b_f(number_b_f);

    const nf_t nf_a_d(number_a_d);
    const nf_t nf_b_d(number_b_d);

    // some fancy calculation, whatever
    nf_t nf_c_f = add(nf_a_f, nf_b_f);
    nf_t nf_c_d = add(nf_a_d, nf_b_d);

    std::cout << "floats: " << number_a_f << "\t" << number_b_f << "\n";
    std::cout << "doubles: " << number_a_d << "\t" << number_b_d << "\n";
    std::cout << "from floats: " << nf_a_f << " (" << to_binary(nf_a_f) << ")\t" << nf_b_f << " ("
              << to_binary(nf_b_f) << ")\n";
    std::cout << "from doubles: " << nf_a_d << "\t" << nf_b_d << "\n";

    std::cout << "\n\n";

    std::cout << "float: " << number_a_f << " + " << number_b_f << " = "
              << (number_a_f + number_b_f) << std::endl;
    std::cout << "normalized_float (from float): " << nf_a_f << " + " << nf_b_f << " = " << nf_c_f
              << std::endl;
    std::cout << "compute representation: " << to_compute_string(nf_c_f) << std::endl;
    std::cout << "as binary: " << to_binary(nf_c_f) << "\n";

    std::cout << "\n\n";

    std::cout << "double: " << number_a_d << " + " << number_b_d << " = "
              << (number_a_d + number_b_d) << std::endl;
    std::cout << "normalized_float (from double): " << nf_a_d << " + " << nf_b_d << " = " << nf_c_d
              << std::endl;
    // NOTE: compute output here is still broken
    std::cout << "compute representation: " << to_compute_string(nf_c_d) << std::endl;
    std::cout << "as binary: " << to_binary(nf_c_d) << "\n";

    std::cout << "\n\n";

    // trying to access values as float again
    std::cout << "reference value: " << number_a_d + number_b_d << std::endl;
    std::cout << "Float conversion: " << to_float<3, 5>(nf_c_f) << std::endl;
    std::cout << "Double conversion: " << to_float<3, 5>(nf_c_d) << std::endl;

    std::cout << "\n\n";

    std::cout << to_binary(nf_a_f) << "\n";
    std::cout << to_compute_string(nf_a_f) << "\n";
    std::cout << static_cast<float>(nf_a_f) << "\n";
    std::cout << static_cast<double>(nf_a_f) << "\n";
    std::cout << static_cast<float>(nf_b_f) << "\n";
    std::cout << static_cast<float>(nf_c_f) << "\n";

    return 0;
}
