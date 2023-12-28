#include "code_attacks.h"

matrix::Matrix testRunner(matrix::Matrix rmCode, size_t mode) {
    std::cout << "START testRunner:" << std::endl;
    std::cout << "RM(r, m) code:" << std::endl;
    if (mode == 0) {
        rmCode.printVisualMatrix();
    } else {
        std::cout << rmCode.rows() << " " << rmCode.cols() << std::endl;
    }
    rmCode.printVisualMatrix(4);
    std::cout << "START modRM_attack" << std::endl;
    std::vector<unsigned long long> cols(rmCode.cols() / 4);
    std::iota(cols.begin(), cols.end(), 0);
    matrix::Matrix permMatr(codes::attackSupporters::permOfExtraction(rmCode, cols));
    std::cout << "END modRM_attack" << std::endl;
    if (mode == 0) {
        std::cout << "Perm matrix:" << std::endl;
        permMatr.printMatrix();
    } else {
        std::cout << "Perm matrix sizes:" << std::endl;
        std::cout << permMatr.rows() << " " << permMatr.rows() << std::endl;
    }
    std::cout << "Perm matrix rank: " << permMatr.rank() << std::endl;
    std::cout << "END testRunner" << std::endl;
    return permMatr;
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
    matrix::Matrix pqsigRMMixed(pqsigRM);
    matrix::Matrix P(0, 0);
    bool mixedTest = false;
    if (mixedTest) {
        matrix::Matrix M = matrix::generateRandomNonSingular(pqsigRMMixed.rows(),
                                                             pqsigRMMixed.rows());

        std::uniform_int_distribution<int> distrib(1, pqsigRMMixed.cols());
        std::random_device rd;
        std::mt19937 gen(rd());
        P = matrix::generateRandomPermutation(pqsigRMMixed.cols(), distrib(gen));

        pqsigRMMixed = M * pqsigRMMixed * P;
    }

    P = testRunner(pqsigRMMixed, mode);
    return 0;
}