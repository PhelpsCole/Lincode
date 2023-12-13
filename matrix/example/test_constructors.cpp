#include "matrix.h"

void testMatrixVectorConstructors(unsigned long long rows, unsigned long long cols, std::vector<char> &basis,
                            std::vector<std::vector<char>> &vec) {
    std::cout << "STARTS testMatrixVectorConstructors" << std::endl;
    std::cout << "Default constructor of " << rows << " x " << cols << " matrix:" << std::endl;
    matrix::Matrix m(rows, cols);
    m.printMatrix();
    std::cout << "Constructor with vector of values:" << std::endl;
    m = matrix::Matrix(rows, cols, basis);
    m.printMatrix();
    std::cout << "Constructor by vector of vectors:" << std::endl;
    m = matrix::Matrix(vec);
    m.printMatrix();
    std::cout << "Constructor by isTransposed vector:" << std::endl;
    m = matrix::Matrix(basis, true);
    m.printMatrix();
}

void testMatrixStringConstructorSeparators(std::string &str, const char tab = '\0',
                                           const char sep = '\n') {
    std::cout << "Constructor by string with separators:" << std::endl;
    matrix::Matrix m = matrix::Matrix(str, tab, sep);
    m.printMatrix();
}

void testMatrixStringConstructors(unsigned long long rows, unsigned long long cols, std::string &str,
                                  std::string &str2, const char tab = '\0', const char sep = '\n') {
    std::cout << "STARTS testMatrixStringConstructors" << std::endl;
    std::cout << "Constructor by string with matrix sizes:" << std::endl;
    matrix::Matrix m(rows, cols, str);
    m.printMatrix();
    testMatrixStringConstructorSeparators(str2, tab, sep);
}

// Problem with Cmake linkage
//void testMatrixFromFile(std::string &filename, char c = ' ', char r = '\n') {
//    std::cout << "STARTS testMatrixFromFile" << std::endl;
//    //std::cout << "Matrix from " << filename << std::endl;
//    std::cout << "Input full path to your file with matrix:" << std::endl;
//    std::cin >> filename;
//    std::cout << filename << std::endl;
//    matrix::Matrix m = matrix::matrFromFile(filename, c, r);
//    m.printMatrix();
//}


int main() {
	std::string str = "010100\n101010\n000011";
    std::vector<std::vector<char>> code = {{1, 0, 0, 0},
                                           {1, 1, 0, 1},
                                           {1, 1, 1, 1},
                                           {0, 0, 1, 1}},
                                   code2 = {{1, 0, 1},
                                            {0, 1, 1}};
    std::vector<char> vec = {1, 0, 0, 1}, vec2 = {1, 0};
    testMatrixVectorConstructors(2, 2, vec, code);
    std::string str2 = "010100101010000011";
    std::string str3 = "0 1 0 1 0 0\n1 0 1 0 1 0\n0 0 0 0 1 1";
    testMatrixStringConstructors(3, 6, str2, str, '\n');
    testMatrixStringConstructorSeparators(str3, ' ', '\n');
    //std::string filename = "./matr_1.txt";
    //testMatrixFromFile(filename);
	return 0;
}