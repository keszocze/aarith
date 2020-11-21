#pragma once

#include "DSP.hpp"
#include "util.hpp"

namespace dsp_packing {

template <typename Packing, typename Postprocessing, size_t WWidth = 4, size_t AWidth = 4>
PPort DUT(const aarith::integer<WWidth>& w1, const aarith::integer<WWidth>& w2,
          const aarith::uinteger<AWidth> a1, const aarith::uinteger<AWidth> a2, Packing pack,
          Postprocessing post_proc)
{

    const DSPInput in = pack(w1, w2, a1, a2);

    const PPort dsp_result = dsp(in);

    return post_proc(dsp_result);
};

template <typename Packing, size_t WWidth = 4, size_t AWidth = 4>
PPort DUT(const aarith::integer<WWidth>& w1, const aarith::integer<WWidth>& w2,
          const aarith::uinteger<AWidth> a1, const aarith::uinteger<AWidth> a2, Packing pack)
{
    return DUT(w1, w2, a1, a2, pack, [](const PPort p) { return p; });
}

/**
 * @brief Helper function the extracts the individual results from the DSP's output bitstring
 * @param result The result computed by the DSP
 * @return The individual extracted results
 */
template <size_t WWidth = 4, size_t AWidth = 4>
[[nodiscard]] std::array<aarith::integer<WWidth + AWidth>, 4> extract_results(const PPort result)
{

    static_assert(WWidth + AWidth < 12, "Using more than 11 bits (combined) for the weights and "
                                        "activation values would lead to overlapping values");

    using E = aarith::integer<WWidth + AWidth>;

    static constexpr size_t n_guards = 11 - (WWidth + AWidth);
    static constexpr size_t offset = (WWidth + AWidth) - 1;

    const E a1w1{aarith::bit_range<offset, 0>(result)};
    const E a2w1{aarith::bit_range<11 + offset, 11>(result)};
    const E a1w2{aarith::bit_range<22 + offset, 22>(result)};
    const E a2w2{aarith::bit_range<33 + offset, 33>(result)};

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
template <typename Packing, typename PostProcessing, size_t WWidth = 4, size_t AWidth = 4>
[[nodiscard]] std::array<aarith::integer<WWidth + AWidth>, 4>
call_dut(const aarith::integer<WWidth>& w1, const aarith::integer<WWidth>& w2,
                 const aarith::uinteger<AWidth>& a1, const aarith::uinteger<AWidth>& a2,
                 Packing packing, PostProcessing post_proc)
{
    return extract_results(DUT(w1,w2,a1,a2,packing,post_proc));
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
template <typename Packing, size_t WWidth = 4, size_t AWidth = 4>
[[nodiscard]] std::array<aarith::integer<WWidth + AWidth>, 4>
call_dut(const aarith::integer<WWidth>& w1, const aarith::integer<WWidth>& w2,
                 const aarith::uinteger<AWidth>& a1, const aarith::uinteger<AWidth>& a2,
                 Packing packing)
{
    return extract_results(DUT(w1,w2,a1,a2,packing));
}



//template <typename Packing> class DefaultEvaluator
//{
//    size_t n_errors = 0;
//    size_t n_individual_errors = 0;
//    size_t n_tests = 0;
//    aint8 max_error{0};
//    Packing packing;
//    bool print_individual_result = false;
//
//public:
//    void setup()
//    {
//        // does nothing right now but we might come up with something interesting
//    }
//    void teardown()
//    {
//        std::cout << n_errors << ";" << n_tests << ";" << ((float)n_errors / (float)n_tests) << ";"
//                  << n_individual_errors << ";" << max_error << "\n";
//    }
//
//    D(Packing packing, const bool print_individual_results = false)
//        : packing(packing)
//        , print_individual_result(print_individual_results)
//    {
//    }
//
//    void operator()(const aint4& w1, const aint4& w2, const auint4 a1, const auint4 a2)
//    {
//        const auto res = test_single_packing(w1, w2, a1, a2, packing, print_individual_result);
//        n_errors += std::get<0>(res);
//        n_individual_errors += std::get<1>(res);
//        max_error = max(max_error, std::get<2>(res));
//        ++n_tests;
//    }
//};
//
//
//
//template <typename D, template<size_t, size_t> typename Evaluator, size_t WWidth = 4, size_t AWidth = 4>
//void evaluate(D design, const bool print_output=false) {
//    Evaluator<WWidth, AWidth> e{design, print_output};
//    e.setup();
//    iterate_inputs(e);
//    e.teardown();
//}
//
//
//template <typename D, size_t WWidth = 4, size_t AWidth = 4>
//void evaluate(D design, const bool print_output=false) {
//    evaluate<D,DefaultEvaluator, WWidth, AWidth>(design, print_output);
//}

} // namespace dsp_packing