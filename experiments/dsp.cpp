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

[[nodiscard]] constexpr DSPRes dsp(const DSPAddIn& A, const DSPAddIn& D, const DSPProdIn& B)
{

    const DSPAddIn sum = A + D;

    const DSPAddIn B_{B};

    // TODO ask Akif about preferred automatic width extension
    const DSPRes prod = sum * B_;

    return prod;
}

[[nodiscard]] std::array<aint8, 4> pack_dsp(const aint4& w1, const aint4& w2, const auint4& a1,
                                            const auint4& a2)
{

    const DSPAddIn A{w1};
    DSPAddIn D{w2};
    D <<= 22;

    const DSPProdIn A1{a1};
    DSPProdIn A2{a2};
    A2 <<= 11;
    const DSPProdIn B = A1 + A2;

    const DSPRes dsp_res = dsp(A, D, B);

    const aint8 a1w1{bit_range<7, 0>(dsp_res)};
    const aint8 a2w1{bit_range<18, 11>(dsp_res)};
    const aint8 a1w2{bit_range<29, 22>(dsp_res)};
    const aint8 a2w2{bit_range<40, 33>(dsp_res)};

    return {a2w2, a1w2, a2w1, a1w1};
}

bool test_single_packing(const aint4& w1, const aint4& w2, const auint4& a1, const auint4& a2)
{

    aint8 w1_{w1};
    aint8 w2_{w2};

    aint8 a1_{a1};
    aint8 a2_{a2};

    const std::array<aint8, 4> res_correct = {a2_ * w2_, a1_ * w2_, a2_ * w1_, a1_ * w1_};

    const std::array<aint8, 4> res_dsp = pack_dsp(w1, w2, a1, a2);

    bool error = false;

    for (size_t i = 0; i < 4; ++i)
    {
        error |= res_correct[i] != res_dsp[i];
        std::cout << res_correct[i] << ";" << res_dsp[i] << ";" << (res_correct[i] - res_dsp[i])
                  << ";";
    }

    std::cout << "\n";

    return error;
}

void test_packing()
{
    for (aint4 w1 : integer_range<aint4>(aint4::min(), 6))
    {
        std::cout << w1 << "\n";
    }
}

int main()
{

    const aint4 w1(-8);
    const aint4 w2 = aint4::zero();

    const auint4 a1 = auint4::one();
    const auint4 a2 = auint4::one();

    test_single_packing(w1, w2, a1, a2);
    test_packing();
    return 0;
}