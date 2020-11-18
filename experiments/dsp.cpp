#include <aarith/integer.hpp>

using namespace aarith;

static constexpr size_t DSPResWidth = 48;
static constexpr size_t DSPAddInWidth = 27;
static constexpr size_t DSPProdInWidth = 18;

using DSPRes = integer<DSPResWidth>;
using DSPAddIn = integer<DSPAddInWidth>;
using DSPProdIn = integer<DSPProdInWidth>;
using aint8 = integer<8>;
using aint4 = integer<4>;
using auint4 = uinteger<4>;

/**
 * @brief Struct storing the inputs to the DSP
 */
struct DSPIn
{
    DSPAddIn A;                //! First of the summands
    DSPAddIn D;                //! Second of the summands
    DSPProdIn B;               //!
    DSPRes C = DSPRes::zero(); //! The optional carry
};

/**
 * @brief Models the DSP's behaviour
 * @param A The first summand
 * @param D The second summand
 * @param B The other part of the product
 * @param C The carry
 * @return ((A+D)*B)+C
 */
[[nodiscard]] constexpr DSPRes dsp(const DSPAddIn& A, const DSPAddIn& D, const DSPProdIn& B,
                                   const DSPRes C = DSPRes::zero())
{

    const DSPAddIn sum = A + D;

    const DSPAddIn B_{B};

    // TODO ask Akif about preferred automatic width extension
    const DSPRes prod = sum * B_;

    const DSPRes result = prod + C;

    return result;
}

/**
 * @brief Helper function calling the DSP with a given input struct
 * @param input The input parameters to the DSP packed in a strut
 * @return ((A+D)*B)+C
 */
[[nodiscard]] constexpr DSPRes dsp(const DSPIn& input)
{
    return dsp(input.A, input.D, input.B, input.C);
}

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
DSPIn pack_xilinx(const aint4& w1, const aint4& w2, const auint4& a1, const auint4& a2)
{
    const DSPAddIn A{w1};
    DSPAddIn D{w2};
    D <<= 22;

    const DSPProdIn A1{a1};
    DSPProdIn A2{a2};
    A2 <<= 11;
    const DSPProdIn B = A1 + A2;

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
// TODO Check with Akif that this actually is what he proposed
DSPIn pack_akif(const aint4& w1, const aint4& w2, const auint4& a1, const auint4& a2)
{
    DSPIn params = pack_xilinx(w1, w2, a1, a2);

    DSPRes C = DSPRes::zero();

    if (w1.is_negative())
    {
        C.set_bit(11, true);
        C.set_bit(22, true);
        //        C.set_bit(33, true); <- this increases the error rate again
    }

    params.C = C;
    return params;
}

/**
 * @brief Helper function the extracts the individual results from the DSP's output bitstring
 * @param result The result computed by the DSP
 * @return The individual extracted results
 */
[[nodiscard]] std::array<aint8, 4> extract_results(const DSPRes result)
{
    const aint8 a1w1{bit_range<7, 0>(result)};
    const aint8 a2w1{bit_range<18, 11>(result)};
    const aint8 a1w2{bit_range<29, 22>(result)};
    const aint8 a2w2{bit_range<40, 33>(result)};

    return {a2w2, a1w2, a2w1, a1w1};
}

template <typename Packing>
[[nodiscard]] std::array<aint8, 4> generic_approach(const aint4& w1, const aint4& w2,
                                                    const auint4& a1, const auint4& a2,
                                                    Packing packing)
{
    const DSPIn params = packing(w1, w2, a1, a2);

    const DSPRes dsp_res = dsp(params);

    return extract_results(dsp_res);
}

template <typename Packing>
int test_single_packing(const aint4& w1, const aint4& w2, const auint4& a1, const auint4& a2,
                        Packing packing, const bool show_intermediate_results = false)
{

    aint8 w1_{w1};
    aint8 w2_{w2};

    aint8 a1_{a1};
    aint8 a2_{a2};

    const std::array<aint8, 4> res_correct = {a2_ * w2_, a1_ * w2_, a2_ * w1_, a1_ * w1_};

    const std::array<aint8, 4> res_dsp = generic_approach(w1, w2, a1, a2, packing);

    bool error = false;

    for (size_t i = 0; i < 4; ++i)
    {
        error |= res_correct[i] != res_dsp[i];
        if (show_intermediate_results)
        {

            std::cout << res_correct[i] << ";" << res_dsp[i] << ";" << (res_correct[i] - res_dsp[i])
                      << ";";
        }
    }

    if (show_intermediate_results)
    {
        std::cout << "\n";
    }

    return error;
}

template <typename Packing>
void test_packing(Packing packing, const bool show_intermediate_results = false)
{
    size_t n_errors = 0;
    size_t n_tests = 0;
    for (aint4 w1 : integer_range<aint4>())
    {
        for (aint4 w2 : integer_range<aint4>())
        {
            for (auint4 a1 : integer_range<auint4>())
            {
                for (auint4 a2 : integer_range<auint4>())
                {
                    n_errors +=
                        test_single_packing(w1, w2, a1, a2, packing, show_intermediate_results);
                    ++n_tests;
                }
            }
        }
    }
    std::cout << n_errors << ";" << n_tests << ";" << ((float)n_errors / (float)n_tests) << "\n";
}

int main()
{
    test_packing(pack_xilinx);
    test_packing(pack_akif);
    return 0;
}