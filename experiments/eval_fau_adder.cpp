#include <aarith/integer.hpp>
#include <aarith/integer/approx_operations.hpp>
#include <sstream>

#include "../tests/integer/fau_adder.hpp"

template <size_t width, size_t lsp_width, size_t shared_bits = 0> void show_fau_adder_evaluation()
{
    const auto [AER, MED, ME] = aarith::eval_fau_adder<width, lsp_width, shared_bits>();

    std::cout << "Results for evaluating the FAU adder for \n";
    std::cout << "\ta bit width of " << width << "\n";
    std::cout << "\ta splitting point at " << lsp_width << "\n";
    std::cout << "\ta sharing of " << shared_bits << " bits\n";
    std::cout << "Results:\n";
    std::cout << "\t2^" << 2 * width << "* AER = " << AER << "\n";
    std::cout << "\tME = " << ME << "\n";
    std::cout << "\t2^" << 2 * width << "* MED = " << MED << "\n";
}

int main(int argc, char** argv)
{
    using namespace aarith;

    if (argc == 1)
    {
        show_fau_adder_evaluation<8, 4, 1>();
        std::cout << "\n\n";
        show_fau_adder_evaluation<8, 4, 3>();
        std::cout << "\n\n";
        show_fau_adder_evaluation<16, 8, 1>();
        std::cout << "\n\n";
        show_fau_adder_evaluation<16, 8, 3>();
        std::cout << "\n\n";
    }
    else
    {
        show_fau_adder_evaluation<32, 16, 1>();
        std::cout << "\n\n";
        show_fau_adder_evaluation<32, 16, 3>();
    }

    return 0;
}