#include "DUT.hpp"
#include <aarith/integer.hpp>

/**
 * @brief Experiments for evaluating multiple multiplications within a single DSP.
 *
 * The idea comes from the following whitepaper:
 *
 * Xilinx. 2020. „Convolutional Neural Network with INT4 Optimization on Xilinx Devices“.
 * Whitepaper. https://www.xilinx.com/support/documentation/white_papers/wp521-4bit-optimization.pdf
 */

namespace dsp_packing {

using aarith::integer;
using aarith::uinteger;

constexpr size_t weight_size = 5;
constexpr size_t activation_size = 4;

using weight_t = integer<weight_size>;
using activation_t = uinteger<activation_size>;
using res_t = integer<weight_size + activation_size>;

static constexpr size_t SND_POS = 11;
static constexpr size_t THRD_POS = 22;
static constexpr size_t FOURTH_POS = 33;

/**
 * @brief Packs the two signed weights and two unsigned activation values as proposed by Xilinx.
 *
 *
 *
 * Note that this packing is incorrect
 *
 * @param w1 Weight 1
 * @param w2 Weight 2
 * @param a1 Activation value 1
 * @param a2 Activation value 2
 * @return Input packed so that it can be fed to the DSP
 */
DSPInput pack_xilinx(const weight_t& w1, const weight_t& w2, const activation_t& a1,
                     const activation_t& a2)
{
    const APort A{w1};
    APort D{w2};
    D <<= THRD_POS;

    const BPort A1{a1};
    BPort A2{a2};
    A2 <<= SND_POS;
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
DSPInput pack_partially_correcting(const weight_t& w1, const weight_t& w2, const activation_t& a1,
                                   const activation_t& a2)
{
    DSPInput params = pack_xilinx(w1, w2, a1, a2);

    CPort C = CPort::zero();
    C.set_bit(SND_POS, w1.msb());
    C.set_bit(THRD_POS, w1.msb());
    C.set_bit(FOURTH_POS, w2.msb());

    params.C = C;
    return params;
}

// as C++ is not really a modern language, we have to manually wrap the function in this type
// ourselves
using post_proc_t = std::function<std::array<res_t, 4>(
    const weight_t&, const weight_t&, const activation_t&, const activation_t&, const PPort&)>;

/**
 * @brief A post-processing method that fixes all off-by-one errors introduced in the original
 * Xilinx implementation.
 */
post_proc_t fix_dsp_result = []([[maybe_unused]] const weight_t& w1, // NOLINT
                                [[maybe_unused]] const weight_t& w2,
                                [[maybe_unused]] const activation_t& a1,
                                [[maybe_unused]] const activation_t& a2, const PPort& dsp_result) {
    auto [a2w2, a1w2, a2w1, a1w1] = extract_results<weight_size, activation_size>(dsp_result);

    if (dsp_result.bit(10)) // NOLINT
    {
        a2w1 = add(a2w1, res_t::one());
    }

    if (dsp_result.bit(21)) // NOLINT
    {
        a1w2 = add(a1w2, res_t::one());
    }

    if (dsp_result.bit(32)) // NOLINT
    {
        a2w2 = add(a2w2, res_t::one());
    }

    return std::array<res_t, 4>{a2w2, a1w2, a2w1, a1w1};
};

} // namespace dsp_packing

int main()
{

    using namespace dsp_packing; // NOLINT

    DUT xilinx(pack_xilinx);

    DUT no_cost_correction(pack_partially_correcting);

    DUT fully_corrected(pack_xilinx, fix_dsp_result);

    evaluate(xilinx, false);
    evaluate(no_cost_correction, false);
    evaluate(fully_corrected, false);

    return 0;
}