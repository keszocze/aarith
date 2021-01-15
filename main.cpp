#include <aarith/float.hpp>
#include <aarith/integer.hpp>
#include <cmath>
#include <iostream>


int main()
{
    using namespace aarith;

    single_precision<uint64_t> test=single_precision<uint64_t>::neg_one();

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

    return 0;
}

