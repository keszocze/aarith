#include <aarith/float.hpp>
#include <aarith/integer.hpp>
#include <aarith/core/core_number_utils.hpp>
#include <aarith/float/numeric_limits.hpp>
#include <iostream>

int main()
{
    using namespace aarith; // NOLINT

    single_precision test=single_precision::neg_one();

    std::cout << test << "\n";

    std::stringstream ss;
    ss << test;

    std::string s = ss.str();


    std::cout << "\"" << to_sci_string(test).substr(0,1) << "\"\n";
    std::cout << "\"" << to_sci_string(test).substr(1,1) << "\"\n";
    std::cout << "\"" << to_sci_string(test).substr(2,1) << "\"\n";

    std::cout << s << "\n";
    std::cout << "\"" << s.substr(0,1) << "\"\n";
    std::cout << "\"" << s.substr(1,1) << "\"\n";
    std::cout << "\"" << s.substr(2,1) << "\"\n";


    std::cout << static_cast<int>(-3.5f) << " " <<  ::aarith::floor<int>(-3.5f) << "\n";
    std::cout << static_cast<int>(3.5f) << " " <<  ::aarith::floor<int>(3.5f) << "\n";

    std::cout << single_precision::min_exp << "\n";

    std::cout << std::numeric_limits<single_precision>::min_exponent << "\n";
    std::cout << min_exponent<single_precision>() << "\n";

    return 0;
}
