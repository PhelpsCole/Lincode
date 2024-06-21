#include "code_attacks.h"

void testRunner(matrix::Matrix modRMMatr, size_t mode) {
    std::cout << "START testRunner:" << std::endl;
    std::cout << "RM(r, m) code:" << std::endl;
    if (mode == 0) {
        modRMMatr.printVisualMatrix(4);
    } else {
        std::cout << modRMMatr.rows() << " " << modRMMatr.cols() << std::endl;
    }
    std::cout << "START extractFirstBlock" << std::endl;
    codes::attackSupporters::ExtractBlockResult
    afterExtruction(codes::attackSupporters::extractFirstBlock(modRMMatr));
    std::cout << "END extractFirstBlock" << std::endl;
    if (mode == 0) {
        std::cout << "modRMMatr:" << std::endl;
        modRMMatr.printVisualMatrix(4);
        std::cout << "block:" << std::endl;
        afterExtruction.block.printVisualMatrix(4);
        std::cout << "sigmaP:" << std::endl;
        afterExtruction.sigmaP.printVisualMatrix(4);
    } else {
        std::cout << modRMMatr.rows() << " " << modRMMatr.cols() << std::endl;
        std::cout << afterExtruction.block.rows()
                  << " " << afterExtruction.block.cols() << std::endl;
        std::cout << afterExtruction.sigmaP.rows()
                  << " " << afterExtruction.sigmaP.cols() << std::endl;
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