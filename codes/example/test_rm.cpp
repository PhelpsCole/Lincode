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

    std::vector<size_t> sizes = codes::rmSizes(codes::Lincode(matr));
    std::cout << sizes[0] << " " << sizes[1] << std::endl;

    for (size_t m = 3; m < 21; ++m) {
        for (size_t r = 1; r < m; ++r) {
            std::vector<int> mrm  = codes::minRMVector(r, m);
            std::cout << "(" << r << " " << m << "): ";
            for (size_t i = 0; i < mrm.size(); ++i) {
                std::cout << mrm[i] << " ";
            }
            std::cout << std::endl;
        }
    }
    codes::RMCode rm(1, 5);
    codes::Lincode rmCode(rm.toMatrix());
    std::cout << "Hadamard power 3 of (1, 5) rm code:" << std::endl;
    rmCode = hadPower(rmCode, 3);
    rmCode.printCode();
    std::cout << "Hadamard power 4 of (1, 5) rm code:" << std::endl;
    rmCode = hadPower(rm.toMatrix(), 4);
    rmCode.printCode();

    std::cout << "Hadamard power 2 of (2, 6) rm code:" << std::endl;
    rmCode = hadPower(codes::RMCode(2, 6).toMatrix(), 2);
    rmCode.printCodeSizes();

    size_t k = codes::codeSizeFromRM(4, 12);
    std::cout << "Code size from RM(4, 12):" << std::endl;
    std::cout << k << std::endl;

    return 0;
}