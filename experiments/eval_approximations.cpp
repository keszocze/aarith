#include <aarith/float.hpp>
#include <aarith/float/approx_operations.hpp>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <random>
#include <sstream>

using namespace aarith;

using F = normalized_float<8, 23>;

int main()
{

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(std::numeric_limits<float>::lowest()/100.0f,
                                               std::numeric_limits<float>::max()/100.0f);

    std::cout << std::numeric_limits<float>::lowest() << "\t" << std::numeric_limits<float>::min()
              << "\t" << std::numeric_limits<float>::max() << "\n";

    for (size_t i = 0; i < 100; ++i)
    {

        float val_{dist(mt)};
        F val{val_};

        std::cout << (val_ < 0) << "\t" << val_ << "\t" << val << "\n";
    }

    return EXIT_SUCCESS;
}
