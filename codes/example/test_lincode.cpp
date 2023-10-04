#include "linear_code.h"

void testLincodeConstructorsAndSizes(std::vector<std::vector<char>> basis) {
    std::cout << "STARTS testLincodeConstructorsAndSizes" << std::endl;
    codes::Lincode code(1, 1);
    std::cout << "Default constructor with 1, 1 parameters:" << std::endl; 
    code.printCode();
    std::cout << "Vector of vectors constructor:" << std::endl; 
    code = codes::Lincode(basis);
    code.printCode();
    std::cout << "size = " << code.size() << ", len  = " << code.len() << std::endl;
    std::cout << "min_dist = " << code.min_dist() << std::endl;
    std::cout << "Copy constructor:" << std::endl; 
    codes::Lincode copy(code);
    copy.printCode();
    std::cout << "Matrix constructor:" << std::endl; 
    matrix::Matrix matr(basis);
    code = codes::Lincode(matr);
    code.printCode();
}

void testLincodeConverters(codes::Lincode &code) {
    std::cout << "STARTS testLincodeConverters" << std::endl;
    std::cout << "Inputted code C:" << std::endl;
    code.printCode();
    std::cout << "Dual code C':" << std::endl;
    code.dual();
    code.printCode();
}

int main(void) {
    std::vector<std::vector<char>> basis = {{1, 0, 0, 0, 1, 0, 1},
                                            {0, 1, 0, 0, 1, 1, 1},
                                            {0, 0, 1, 0, 1, 1, 0},
                                            {0, 0, 0, 1, 0, 1, 1},};
    testLincodeConstructorsAndSizes(basis);
    codes::Lincode code(basis);
    testLincodeConverters(code);
    return 0;
}