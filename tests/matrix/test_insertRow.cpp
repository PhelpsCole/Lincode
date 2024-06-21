#include "matrix.h"

void testInsertRow(matrix::Matrix matrix, const std::vector<char> &vec, unsigned long long pos) {
    std::cout << "STARTS testInsertRow" << std::endl;
    std::cout << "Inputed matrix:" << std::endl;
    matrix.printMatrix();
    std::cout << "Inputed row:" << std::endl;
    for (size_t i = 0; i < vec.size(); ++i) {
        std::cout << static_cast<unsigned long long>(vec[i]) << " ";
    }
    std::cout << std::endl;
    std::cout << "Inserting row in position " << pos << ":" << std::endl;
    matrix.insertRow(pos, vec);
    matrix.printMatrix();
}


int main() {
    std::string str = "010100\n101010\n000011";
    std::vector<std::vector<char>> code = {{1, 0, 0, 0},
                                           {1, 1, 0, 1},
                                           {1, 1, 1, 1},
                                           {0, 0, 1, 1}};
    matrix::Matrix m(code);
    std::vector<char> vec = {1, 1, 1, 0};
    testInsertRow(m, vec, 0);
    testInsertRow(m, vec, 1);
    testInsertRow(m, vec, 3);
    testInsertRow(m, vec, 4);
    return 0;
}