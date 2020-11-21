#include <aarith/integer.hpp>

// TODO allow for wider activation values

#include "DSP.hpp"

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

/**
 * @brief Helper function the extracts the individual results from the DSP's output bitstring
 * @param result The result computed by the DSP
 * @return The individual extracted results
 */
[[nodiscard]] std::array<aint8, 4> extract_results(const PPort result)
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
    const DSPInput params = packing(w1, w2, a1, a2);

    const PPort dsp_res = dsp(params);

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

    const DSPInput params = packing(w1, w2, a1, a2);

    print_int(params.A, "A");
    print_int(params.D, "D");
    print_int(params.B, "B");
    print_int(params.C, "C");

    const PPort dsp_res = dsp(params);

    print_int(dsp_res, "P");

    aint8 w1_{w1};
    aint8 w2_{w2};

    aint8 a1_{a1};
    aint8 a2_{a2};

    const std::array<aint8, 4> res_correct = {a2_ * w2_, a1_ * w2_, a2_ * w1_, a1_ * w1_};
    const std::array<aint8, 4> res_dsp = extract_results(dsp_res);

    print_int(res_correct[0], "a2w2 correct");
    print_int(res_correct[1], "a1w2 correct");
    print_int(res_correct[2], "a2w1 correct");
    print_int(res_correct[3], "a1w1 correct");

    print_int(res_dsp[0], "a2w2 dsp");
    print_int(res_dsp[1], "a1w2 dsp");
    print_int(res_dsp[2], "a2w1 dsp");
    print_int(res_dsp[3], "a1w1 dsp");
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
std::tuple<size_t, size_t, aint8>
test_single_packing(const aint4& w1, const aint4& w2, const auint4& a1, const auint4& a2,
                    Packing packing, const bool show_intermediate_results = false)
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
    }

    if (show_intermediate_results)
    {
        //        std::cout << to_binary(a1) << " " << to_binary(a2) << " " << to_binary(w1) << " "
        //        << to_binary(w2) << " ";
        std::cout << to_binary(res_dsp[3]) << " " << to_binary(res_dsp[2]) << " "
                  << to_binary(res_dsp[1]) << " " << to_binary(res_dsp[0]) << "\n";
    }

    return {error, num_error, max_error};
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
template <typename T> void iterate_inputs(T& functor)
{
    for (aint4 w1 : integer_range<auint4>())
    {
        for (aint4 w2 : integer_range<auint4>())
        {
            for (auint4 a1 : integer_range<auint4>())
            {
                for (auint4 a2 : integer_range<auint4>())
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
void print_inputs(const bool print_header = false)
{
    auto output = [](auto a1, auto a2, auto w1, auto w2) {
        std::cout << to_binary(a1) << " " << to_binary(a2) << " " << to_binary(w1) << " "
                  << to_binary(w2) << "\n";
    };

    if (print_header)
    {
        std::cout << "a1   a2   w1   w2\n";
    }

    iterate_inputs(output);
}

template <typename Packing> class D
{
    size_t n_errors = 0;
    size_t n_individual_errors = 0;
    size_t n_tests = 0;
    aint8 max_error{0};
    Packing packing;
    bool print_individual_result = false;

public:
    void setup()
    {
        // does nothing right now but we might come up with something interesting
    }
    void teardown()
    {
        std::cout << n_errors << ";" << n_tests << ";" << ((float)n_errors / (float)n_tests) << ";"
                  << n_individual_errors << ";" << max_error << "\n";
    }

    D(Packing packing, const bool print_individual_results = false)
        : packing(packing)
        , print_individual_result(print_individual_results)
    {
    }

    void operator()(const aint4& w1, const aint4& w2, const auint4 a1, const auint4 a2)
    {
        const auto res = test_single_packing(w1, w2, a1, a2, packing, print_individual_result);
        n_errors += std::get<0>(res);
        n_individual_errors += std::get<1>(res);
        max_error = max(max_error, std::get<2>(res));
        ++n_tests;
    }
};

template <typename Packing>
void test_packing(Packing packing, const bool print_individual_results = false)
{
    D t{packing, print_individual_results};
    t.setup();
    iterate_inputs(t);
    t.teardown();
}

}

int main()
{

    using namespace dsp_packing;

    //    aint4 w1{0};
    //    aint4 w2{1};
    //    auint4 a1{0};
    //    auint4 a2{8};
    //        generic_investigate(w1, w2, a1, a2, pack_xilinx);

    test_packing(pack_xilinx, false);
    test_packing(pack_akif, true);

    return 0;
}