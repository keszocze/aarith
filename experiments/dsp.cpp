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

    const DSPRes prod = sum * B_;

    return prod;
}

[[nodiscard]] std::array<aint8, 4> pack_dsp(const aint4& w1, const aint4& w2,
                                                      const auint4& a1, const auint4& a2)
{

    const DSPAddIn A{w1};
    DSPAddIn D{w2};
    D <<= 22;

    const DSPProdIn A1{a1};
    DSPProdIn A2{a2};
    A2 <<= 11;
    const DSPProdIn B = A1 + A2;

    std::cout << A << " " << to_binary(A) << "\n";
    std::cout << D << " " << to_binary(D) << "\n";
    std::cout << B << " " << to_binary(B) << "\n";

    const DSPRes dsp_res = dsp(A,D,B);

    const aint8 a1w1{bit_range<7,0>(dsp_res)};
    const aint8 a2w1{bit_range<18,11>(dsp_res)};
    const aint8 a1w2{bit_range<29,22>(dsp_res)};
    const aint8 a2w2{bit_range<40,33>(dsp_res)};

    std::cout << dsp_res << " " << to_binary(dsp_res) << "\n";

    std::cout << "a1w1 " <<  bit_range<7,0>(dsp_res) << "\n";
    std::cout << "a2w1 " <<  bit_range<18,11>(dsp_res) << "\n";
    std::cout << "a1w2 " <<  bit_range<29,22>(dsp_res) << "\n";
    std::cout << "a2w2 " <<  bit_range<40,33>(dsp_res) << "\n";

    return {a2w2,a1w2,a2w1,a1w1};
}

int main()
{

    const aint4 w1(-8);
    const aint4 w2=aint4::zero();

    const auint4 a1 = auint4::one();
    const auint4 a2 = auint4::one();

    const auto [r1, r2, r3, r4] = pack_dsp(w1, w2, a1, a2);

    std::cout << w1 << "\t" << w2 << "\t" << a1 << "\t" << a2 << "\t" << r1 << "\t" << r2 << "\t"
              << r3 << "\t" << r4 << "\n";

    return 0;
}