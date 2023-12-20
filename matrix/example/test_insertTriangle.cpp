#include "matrix.h"

void testinsertTriangle(matrix::Matrix &matrix, const std::vector<char> &vec) {
    std::cout << "STARTS testinsertTriangle" << std::endl;
    std::cout << "Inputed matrix:" << std::endl;
    matrix.printMatrix();
    std::cout << "Inputed row:" << std::endl;
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << static_cast<unsigned long long>(vec[i]) << " ";
    }
    std::cout << std::endl;
    std::cout << "Matrix after inserting row:" << std::endl;
    matrix.insertTriangle(vec);
    matrix.printMatrix();
}


int main() {
    std::string str = "010100\n101010\n000011";
    std::vector<std::vector<char>> code = {{1, 0, 0, 0},
                                           {1, 1, 0, 1},
                                           {1, 1, 1, 1},
                                           {0, 0, 1, 1}};
    matrix::Matrix m(0, 0);
    std::vector<char> vec = {1, 1, 1, 0};
    testinsertTriangle(m, code[3]);
    testinsertTriangle(m, vec);
    testinsertTriangle(m, code[1]);
    testinsertTriangle(m, {1, 0, 1, 0});
    testinsertTriangle(m, code[0]);
    testinsertTriangle(m, code[2]);
    return 0;
}