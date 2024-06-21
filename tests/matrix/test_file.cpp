#include "matrix.h"

// Problem with Cmake linkage
void testMatrixFromFile(char c = ' ', char r = '\n') {
    std::string filename;
    std::cout << "STARTS testMatrixFromFile" << std::endl;
    //std::cout << "Matrix from " << filename << std::endl;
    std::cout << "Input full path to your file with matrix:" << std::endl;
    std::cin >> filename;
    std::cout << filename << std::endl;
    matrix::Matrix m = matrix::matrFromFile(filename, c, r);
    m.printMatrix();
}

int main() {
    testMatrixFromFile();
    return 0;
}