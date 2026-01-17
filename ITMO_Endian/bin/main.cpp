#include <lib/number.h>
#include <iostream>

int main() {
    uint239_t value1 = FromInt(8, 0);
    std::cout << value1<< std::endl;
    value1 = FromInt(8, 239);
    std::cout << value1<< std::endl;
    //RightShift(value1, 239);
    //std::cout << value1<< std::endl;

    return 0;
}
