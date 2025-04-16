#include <iostream>
#include <string>
#include <map>

#include "version.hpp"

int main()
{
    constexpr char program_name[] {"Ifinterp"};
    std::cout
        << "*** " << program_name << " v" << version_str() << " ***"
        << std::endl;
    std::cout << "Hello IF world!" << std::endl;

    return 0;
}
