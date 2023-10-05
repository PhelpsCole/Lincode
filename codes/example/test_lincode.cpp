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
    std::cout << "Code spectrum: ";
    std::vector<size_t> v = code.spectrum();
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << static_cast<int>(v[i]) << " ";
    }
    std::cout << std::endl;
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
    std::cout << "Hull of C:" << std::endl;
    codes::Lincode res(code.hull());
    res.printCode();
    std::cout << "Dual code C':" << std::endl;
    codes::Lincode dual(code);
    dual.dual();
    dual.printCode();
    std::cout << "Sum of C and C':" << std::endl;
    res = codes::sum(code, dual);
    res.printCode();
    std::cout << "Intersection of C and C':" << std::endl;
    res = codes::intersect(code, dual);
    res.printCode();
}

void testChecker(std::vector<std::vector<char>> &basis) {
    std::cout << "STARTS testChecker" << std::endl;
    if (codes::incorrect_basis(basis)) {
        std::cout << "Incorrect basis" << std::endl;
    } else {
        std::cout << "Correct basis" << std::endl;
    }
}

void testAddToVectors() {
    std::vector<char> v = {1, 0, 1, 1};
    codes::addToBinVector(v, 3);
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << static_cast<int>(v[i]) << " ";
    }
    std::cout << std::endl;
}

void testEncode(codes::Lincode &c) {
    std::cout << "STARTS testEncode" << std::endl;
    std::cout << "Inputted code:" << std::endl;
    c.printCode();
    std::cout << "All words of this code:" << std::endl;
    std::vector<char> v(c.size());
    for (size_t i = 0; i < static_cast<size_t>((1 << v.size()) - 1); ++i) {
        codes::addToBinVector(v, 1);
        std::vector<char> res = c.encode(v);
        for (size_t i = 0; i < v.size(); ++i) {
            std::cout << static_cast<int>(v[i]) << " ";
        }
        std::cout << std::endl;
    }
}

void testPuncturing(codes::Lincode &c, std::vector<size_t> columns) {
    std::cout << "STARTS testPuncturing" << std::endl;
    std::cout << "Inputted code:" << std::endl;
    c.printCode();
    std::cout << "Puncture columns:" << std::endl;
    for (size_t i = 0; i < columns.size(); ++i) {
        std::cout << columns[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Punctured code:" << std::endl;
    c.puncture(columns);
    c.printCode();
}

int main(void) {
    std::vector<std::vector<char>> basis = {{1, 0, 0, 0, 1, 0, 1},
                                            {0, 1, 0, 0, 1, 1, 1},
                                            {0, 0, 1, 0, 1, 1, 0},
                                            {0, 0, 0, 1, 0, 1, 1},},
                                   basis2 = {{1, 0, 0, 0, 1, 0, 1},
                                             {0, 0, 0, 0, 1, 0, 1},
                                             {1, 0, 0, 0, 0, 0, 0},},
                                   basis3 = {{1, 1},
                                             {0, 1}};
    testLincodeConstructorsAndSizes(basis);
    codes::Lincode code(basis);
    codes::Lincode code2(basis3);
    testLincodeConverters(code);
    testLincodeConverters(code2);
    testChecker(basis);
    testChecker(basis2);
    testAddToVectors();
    std::vector<std::vector<char>> basis4 = {{1, 0, 0, 0, 1, 0, 1},
                                            {0, 1, 0, 0, 1, 0, 1},
                                            {0, 1, 1, 0, 1, 0, 1},
                                            {0, 1, 0, 0, 1, 0, 1},};
    codes::Lincode code3(basis4);
    testEncode(code3);
    testPuncturing(code3, {1, 3, 6});
    return 0;
}