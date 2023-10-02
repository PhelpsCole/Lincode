#include "rm_code.h"


int main() {
    unsigned int r = 2;
    unsigned int m = 4;

    codes::RMCode code(3, 5);
    matrix::Matrix matr = code.toMatrix();
    matr.printMatrix();
    auto basis = codes::generateRMCode(r, m);

    for (const auto& vec : basis) {
        for (const char& bit : vec) {
            std::cout << static_cast<int>(bit) << " ";
        }
        std::cout << std::endl;
    }

    return 0;
}