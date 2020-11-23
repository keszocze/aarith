#include <aarith/integer.hpp>

// TODO allow for wider activation values

#include "DUT.hpp"

namespace dsp_packing {

using namespace aarith;

using aint8 = integer<8>;
using aint4 = integer<4>;
using auint4 = uinteger<4>;

/**
 * @brief Packs the two signed weights and two unsigned activation values as proposed by Xilinx
 *
 * Note that this packing is incorrect
 *
 * @param w1 Weight 1
 * @param w2 Weight 2
 * @param a1 Activation value 1
 * @param a2 Activation value 2
 * @return Input packed so that it can be fed to the DSP
 */
DSPInput pack_xilinx(const aint4& w1, const aint4& w2, const auint4& a1, const auint4& a2)
{
    const APort A{w1};
    APort D{w2};
    D <<= 22;

    const BPort A1{a1};
    BPort A2{a2};
    A2 <<= 11;
    const BPort B = A1 + A2;

    return {A, D, B};
}

/**
 * @brief Packs the two signed weights and two unsigned activation values as proposed by Akif
 *
 * The idea is to perform a simple error correction using the carry input of the DSP. The packing
 * of the other ports is as done by Xilinx.
 *
 * @param w1 Weight 1
 * @param w2 Weight 2
 * @param a1 Activation value 1
 * @param a2 Activation value 2
 * @return Input packed so that it can be fed to the DSP
 */
DSPInput pack_akif(const aint4& w1, const aint4& w2, const auint4& a1, const auint4& a2)
{
    DSPInput params = pack_xilinx(w1, w2, a1, a2);

    CPort C = CPort::zero();
    C.set_bit(11, w1.msb());
    C.set_bit(22, w1.msb());
    C.set_bit(33, w2.msb());

    params.C = C;
    return params;
}

// TODO Implement other simple addition correction
// TODO Implement Akif's correction circuit

} // namespace dsp_packing


int main()
{

    using namespace dsp_packing;

    DUT design_under_test(pack_akif);


    evaluate(design_under_test, true);

    return 0;
}