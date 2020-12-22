#pragma once

#include "DSP.hpp"
#include "util.hpp"

namespace dsp_packing {

/**
 * @brief Helper function the extracts the individual results from the DSP's output bitstring
 * @param result The result computed by the DSP
 * @return The individual extracted results
 */
template <size_t WWidth = 4, size_t AWidth = 4>
[[nodiscard]] static std::array<aarith::integer<AWidth + WWidth>, 4>
extract_results(const PPort result)
{

    using ret_val = aarith::integer<AWidth + WWidth>;

    static constexpr size_t MAX_COMBINED_WIDTH = 12;

    static_assert(WWidth + AWidth < MAX_COMBINED_WIDTH,
                  "Using more than 11 bits (combined) for the weights and "
                  "activation values would lead to overlapping values");

    using E = ret_val;

    static constexpr size_t n_guards = 11 - (WWidth + AWidth);
    static constexpr size_t offset = (WWidth + AWidth) - 1;

    const E a1w1{aarith::bit_range<offset, 0>(result)};
    const E a2w1{aarith::bit_range<11 + offset, 11>(result)};
    const E a1w2{aarith::bit_range<22 + offset, 22>(result)};
    const E a2w2{aarith::bit_range<33 + offset, 33>(result)};

    return {a2w2, a1w2, a2w1, a1w1};
}

template <typename Packing, size_t WWidth = 4, size_t AWidth = 4> class DUT
{
public:
    using ret_val = aarith::integer<AWidth + WWidth>;
    using w_val = aarith::integer<WWidth>;
    using a_val = aarith::uinteger<AWidth>;
    using post_proc_t = std::function<std::array<ret_val, 4>(
        const w_val&, const w_val&, const a_val&, const a_val&, const PPort&)>;

private:
    Packing packing;
    post_proc_t post_proc;

public:
    static constexpr size_t W = WWidth;
    static constexpr size_t A = AWidth;

    /**
     * @brief Simply extracts the results from the DSP result without any further post-processing
     * @param dsp_result
     * @return
     */
    static std::array<ret_val, 4> default_post_proc([[maybe_unused]] const w_val&,
                                                    [[maybe_unused]] const w_val&,
                                                    [[maybe_unused]] const a_val&,
                                                    [[maybe_unused]] const a_val&,
                                                    const PPort& dsp_result)
    {
        const auto [a2w2, a1w2, a2w1, a1w1] = extract_results(dsp_result);
        return std::array<ret_val, 4>{a2w2, a1w2, a2w1, a1w1};
    }

    /**
     * @brief DUT with a user specified post-processing procedure (defaults to only extracting the
     * products without any further post-processing).
     */
    explicit DUT(Packing packing, post_proc_t post_proc = default_post_proc)
        : packing(packing)
        , post_proc(post_proc)
    {
    }

    std::array<aarith::integer<WWidth + AWidth>, 4> operator()(const w_val& w1, const w_val& w2,
                                                               const a_val& a1, const a_val& a2)
    {

        const DSPInput in = packing(w1, w2, a1, a2);

        const PPort dsp_result = dsp(in);

        return post_proc(w1, w2, a1, a2, dsp_result);
    }
};

template <typename Design> class DefaultEvaluator
{
    using val = aarith::integer<Design::A + Design::W>;
    using val_w = aarith::integer<Design::W>;
    using val_a = aarith::uinteger<Design::A>;
    size_t n_errors = 0;
    size_t n_individual_errors = 0;
    size_t n_tests = 0;
    val max_error{0};
    bool print_individual_results = false;

    Design design;

public:
    void setup()
    {
        // does nothing right now but we might come up with something interesting
    }
    void teardown()
    {
        std::cout << n_errors << ";" << n_tests << ";" << (static_cast<float>(n_errors) / static_cast<float>(n_tests)) << ";"
                  << n_individual_errors << ";" << max_error << "\n";
    }

    explicit DefaultEvaluator(Design design, const bool print_individual_results = false)
        : design(design)
        , print_individual_results(print_individual_results)
    {
    }

    void operator()(const val_w& w1, const val_w& w2, const val_a a1, const val_a a2)
    {

        val w1_{w1};
        val w2_{w2};

        val a1_{a1};
        val a2_{a2};

        const std::array<val, 4> res_correct = {a2_ * w2_, a1_ * w2_, a2_ * w1_, a1_ * w1_};

        const std::array<val, 4> res_dsp = design(w1, w2, a1, a2);

        bool error = false;

        for (size_t i = 0; i < 4; ++i)
        {

            if (res_correct[i] != res_dsp[i])
            {
                error = true;
                ++n_individual_errors;
                const auto diff = (res_correct[i] - res_dsp[i]);
                //                if (diff > val{1}) {
                //                    print_int(w1, "w1");
                //                    print_int(w2, "w2");
                //                    print_int(a1, "a1");
                //                    print_int(a2, "a2");
                //                    print_int(res_correct[i], "korrekt");
                //                    print_int(res_dsp[i], "dsp");
                //                }
                if (abs(diff) > abs(max_error))
                {
                    max_error = diff;
                }
            }
        }

        if (print_individual_results)
        {
            //                    std::cout << to_binary(a1) << " " << to_binary(a2) << " " <<
            //                    to_binary(w1) << " "
            //                    << to_binary(w2) << " ";
            std::cout << to_binary(res_dsp[3]) << " " << to_binary(res_dsp[2]) << " "
                      << to_binary(res_dsp[1]) << " " << to_binary(res_dsp[0]) << "\n";
        }

        if (error)
        {
            ++n_errors;
        }
        ++n_tests;
    }
};

template <template <typename> typename Evaluator, typename Design>
void evaluate(Design design, const bool print_output = false)
{
    Evaluator<Design> e{design, print_output};
    e.setup();
    iterate_inputs(e);
    e.teardown();
}

template <typename D> void evaluate(D design, const bool print_output = false)
{
    evaluate<DefaultEvaluator, D>(design, print_output);
}

} // namespace dsp_packing