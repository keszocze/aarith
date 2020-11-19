#include <aarith/integer.hpp>

// TODO allow for wider activation values

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
 * @brief Helper method for printing out numbers in decimal and binary at once (including a name)
 * @tparam I The data type to display
 * @param n The number to display
 * @param name
 */
template <typename I> void print_int(const I& n, const std::string name)
{
    std::cout << to_binary(n) << "\t" << n << "\t(" << name << ")\n";
}

/**
 * @brief Struct storing the inputs to the DSP
 */
struct DSPIn
{
    DSPAddIn A;                //! First of the summands
    DSPAddIn D;                //! Second of the summands
    DSPProdIn B;               //! The other half of the multiplication
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
[[nodiscard]] DSPRes dsp(const DSPAddIn& A, const DSPAddIn& D, const DSPProdIn& B,
                         const DSPRes C = DSPRes::zero())
{

    const DSPAddIn sum = A + D;

    //    print_int(sum, "A+D");

    const DSPAddIn B_{B};

    //    print_int(B_, "B_");

    // TODO ask Akif about preferred automatic width extension
    const DSPRes prod = expanding_mul(sum, B_);

    //    print_int(sum * B_, "*");
    //    print_int(prod, "(A+D)*B");

    const DSPRes result = prod + C;

    return result;
}

/**
 * @brief Helper function calling the DSP with a given input struct
 * @param input The input parameters to the DSP packed in a strut
 * @return ((A+D)*B)+C
 */
[[nodiscard]] DSPRes dsp(const DSPIn& input)
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
        //        C.set_bit(33, true); // <- this increases the error rate again
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

/**
 * @brief Generic function for DSP packing for the use of multiple multiplications in a single DSP.
 *
 * The function packs the weights and activation variables using the provided packing function.
 * Afterwards, the data is fed to the DSP. The result is then extracted and returned.
 *
 * @tparam Packing Function template parameter for the packing to use
 * @param w1 Weight 1
 * @param w2 Weight 2
 * @param a1 Activation value 1
 * @param a2 Activation value 2
 * @param packing The concrete packing that is used to feed the DSP
 * @return Individual results
 */
template <typename Packing>
[[nodiscard]] std::array<aint8, 4> generic_approach(const aint4& w1, const aint4& w2,
                                                    const auint4& a1, const auint4& a2,
                                                    Packing packing)
{
    const DSPIn params = packing(w1, w2, a1, a2);

    const DSPRes dsp_res = dsp(params);

    return extract_results(dsp_res);
}

/**
 * @brief Helper function for debugging.
 *
 * It is unfinished because I found one error that Akif pointed out already.
 *
 * @tparam Packing
 * @param w1
 * @param w2
 * @param a1
 * @param a2
 * @param packing
 */
template <typename Packing>
void generic_investigate(const aint4& w1, const aint4& w2, const auint4& a1, const auint4& a2,
                         Packing packing)
{

    print_int(w1, "w1");
    print_int(w2, "w2");
    print_int(a1, "a1");
    print_int(a2, "a2");

    const DSPIn params = packing(w1, w2, a1, a2);

    print_int(params.A, "A");
    print_int(params.D, "D");
    print_int(params.B, "B");
    print_int(params.C, "C");

    const DSPRes dsp_res = dsp(params);

    print_int(dsp_res, "P");

    aint8 w1_{w1};
    aint8 w2_{w2};

    aint8 a1_{a1};
    aint8 a2_{a2};

    const std::array<aint8, 4> res_correct = {a2_ * w2_, a1_ * w2_, a2_ * w1_, a1_ * w1_};
    const std::array<aint8, 4> res_dsp = extract_results(dsp_res);
}

/**
 * @brief Evaluates a single packed input
 * @tparam Packing
 * @param w1 Weight 1
 * @param w2 Weight 2
 * @param a1 Activation value 1
 * @param a2 Activation value 2
 * @param packing The concrete packing that is used to feed the DSP
 * @param show_intermediate_results If true, all computations are printed to standard out
 * @return (1 iff an error occured, #individual errors, max error in that line)
 */
template <typename Packing>
std::tuple<size_t, size_t, aint8> test_single_packing(const aint4& w1, const aint4& w2, const auint4& a1,
                                              const auint4& a2, Packing packing,
                                              const bool show_intermediate_results = false)
{

    aint8 w1_{w1};
    aint8 w2_{w2};

    aint8 a1_{a1};
    aint8 a2_{a2};

    const std::array<aint8, 4> res_correct = {a2_ * w2_, a1_ * w2_, a2_ * w1_, a1_ * w1_};

    const std::array<aint8, 4> res_dsp = generic_approach(w1, w2, a1, a2, packing);

    bool error = false;
    int num_error = 0;
    aint8 max_error{0};

    for (size_t i = 0; i < 4; ++i)
    {
        error |= res_correct[i] != res_dsp[i];
        num_error += res_correct[i] != res_dsp[i];
        const auto diff = (res_correct[i] - res_dsp[i]);

        max_error = max(max_error, diff);

        if (show_intermediate_results)
        {

            std::cout << res_correct[i] << ";" << res_dsp[i] << ";" << diff << ";";
        }
    }

    if (show_intermediate_results)
    {
        std::cout << "\n";
    }

    return {error, num_error, max_error};
}

/**
 * @brief Fully evaluates a packing for the given bit-widths.
 * @tparam Packing
 * @param packing The concrete packing that is used to feed the DSP
 * @param show_intermediate_results
 */
template <typename Packing>
void test_packing(Packing packing, const bool show_intermediate_results = false)
{
    size_t n_errors = 0;
    size_t n_individual_errors = 0;
    size_t n_tests = 0;
    aint8 max_error{0};
    for (aint4 w1 : integer_range<aint4>())
    {

        //        std::cout << to_binary(w1) << "\n";

        for (aint4 w2 : integer_range<aint4>())
        {
            for (auint4 a1 : integer_range<auint4>())
            {
                for (auint4 a2 : integer_range<auint4>())
                {

                    //                    std::cout << to_binary(w1) << " " << to_binary(w2) << " "
                    //                    << to_binary(a1) << " " << to_binary(a2) << "\n";

                    const auto res =
                        test_single_packing(w1, w2, a1, a2, packing, show_intermediate_results);
                    n_errors += std::get<0>(res);
                    n_individual_errors += std::get<1>(res);
                    max_error = max(max_error, std::get<2>(res));
                    ++n_tests;
                }
            }
        }
    }
    std::cout << n_errors << ";" << n_tests << ";" << ((float)n_errors / (float)n_tests) << ";"
              << n_individual_errors << ";" << max_error <<  "\n";
}

int main()
{

    //    aint4 w1{1};
    //    aint4 w2{1};
    //    auint4 a1{1};
    //    auint4 a2{1};
    //
    //    generic_investigate(w1,w2,a1,a2,pack_xilinx);

    std::cout << "erroneous lines;number of inputs;percentage of errneous lines;number of individual errors;maximal errorr\n";
    test_packing(pack_xilinx);
    test_packing(pack_akif);
    return 0;
}