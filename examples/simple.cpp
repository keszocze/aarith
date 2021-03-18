#include <aarith/float.hpp>
#include <aarith/integer.hpp>

int main()
{
    using namespace aarith;
    uint64_t a = 10, b = 20;
    uinteger<64> a_ = 10, b_ = 20;
    std::cout << "a + b = " << (a + b) << "\n";
    std::cout << "a_+ b_ = " << (a_ + b_) << "\n";

    float x=3.0F, y=2.5F;
    floating_point<8,23> x_{3.0F}, y_{2.5F};
    std::cout << "x * y = " << (x * y) << "\n";
    std::cout << "x_ * y_ = " << (x_ * y_) << "\n";
}