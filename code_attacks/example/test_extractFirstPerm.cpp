#include "code_attacks.h"

void testRunner(matrix::Matrix modRMMatr, size_t mode) {
    std::cout << "START testRunner:" << std::endl;
    std::cout << "RM(r, m) code:" << std::endl;
    if (mode == 0) {
        modRMMatr.printVisualMatrix(4);
    } else {
        std::cout << modRMMatr.rows() << " " << modRMMatr.cols() << std::endl;
    }
    std::vector<unsigned long long> cols(modRMMatr.cols() / 4);
    std::iota(cols.begin(), cols.end(), 0);
    std::cout << "START extractFirstPerm" << std::endl;
    matrix::Matrix afterExtruction(codes::attackSupporters::extractFirstPerm(modRMMatr, cols));
    std::cout << "END extractFirstPerm" << std::endl;
    if (mode == 0) {
        std::cout << "modRMMatr:" << std::endl;
        modRMMatr.printVisualMatrix(4);
        std::cout << "afterExtruction:" << std::endl;
        afterExtruction.printVisualMatrix(4);
        std::cout << "afterExtruction.basisView():" << std::endl;
        afterExtruction.convertToBasis();
        afterExtruction.printVisualMatrix(4);
    } else {
        std::cout << modRMMatr.rows() << " " << modRMMatr.cols() << std::endl;
        std::cout << afterExtruction.rows() << " " << afterExtruction.cols() << std::endl;
    }
}

int main(int argc, char *argv[]) {
    size_t r = 2, m = 7;
    size_t mode = 1;
    if (argc < 3) {
        std::cout << "Input args in format: r m mode" << std::endl;
        std::cout << "Where mode=0 -- print visual, mode=1 -- print sizes" << std::endl;
        return 0;
    } else if (argc == 3) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    } else if (argc >= 4) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        mode = std::stoi(argv[3]);
    }
    matrix::Matrix pqsigRM = codes::pqsigRMGenerator(r, m);
    testRunner(pqsigRM, mode);
    return 0;
}