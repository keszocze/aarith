#include <aarith/float.hpp>
#include <iostream>
#include <numeric>
#include <vector>

int main()
{
    using namespace aarith;
    using Native = float;

    using F = normalized_float<8, 23>;

    Native a_float{-1337.35f};
    Native b_float{420815.0f};

    constexpr Native delta = {0.123f};
    constexpr Native delta2 = {3.33f};

    for (size_t i = 0; i < std::numeric_limits<size_t>::max() - 1; ++i)
    {
        F a{a_float};
        F b{b_float};

        F res = a + b;
        Native res_float = a_float + b_float;

        F res_float_{res_float};
        Native res_ = static_cast<Native>(res);

        if (res_float_ != res)
        {

            std::cout << a << " + " << b << " = " << res << "\n";
            std::cout << to_binary(a) << " + " << to_binary(b) << " = " << to_binary(res) << "\n";
            std::cout << to_compute_string(a) << "\t+\t" << to_compute_string(b) << " = "
                      << to_compute_string(res) << "\n";

            std::cout << a_float << " + " << b_float << " = " << res_float << "\n";
            std::cout << to_binary(res) << "\n" << to_binary(res_float_) << "\n";
        }

        return 0;
    }
}