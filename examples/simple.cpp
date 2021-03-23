#include <aarith/float.hpp>
#include <aarith/integer.hpp>

int main()
{
    using aarith::uinteger;
    using aarith::floating_point;

    const uint64_t a = 10;
    const uint64_t b = 20;
    const uinteger<64> a_ = 10;
    const uinteger<64> b_ = 20;
    std::cout << "a + b = " << (a + b) << "\n";
    std::cout << "a_+ b_ = " << (a_ + b_) << "\n";

    const float x=3.0F;
    const float y=2.5F;
    const floating_point<8,23> x_{3.0F};
    const floating_point<8,23> y_{2.5F};

    std::cout << "x * y = " << (x * y) << "\n";
    std::cout << "x_ * y_ = " << (x_ * y_) << "\n";
}