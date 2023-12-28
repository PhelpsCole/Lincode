#include "code_attacks.h"

matrix::Matrix testRunner(codes::Lincode rmCode, size_t mode) {
    std::cout << "START testRunner:" << std::endl;
    std::cout << "RM(r, m) code:" << std::endl;
    if (mode == 0) {
        rmCode.printVisualCode();
    } else {
        rmCode.printCodeSizes();
    }
    std::cout << "START chizhov_borodin" << std::endl;
    matrix::Matrix permMatr(chizhov_borodin(rmCode));
    std::cout << "END chizhov_borodin" << std::endl;
    if (mode == 0) {
        std::cout << "Perm matrix:" << std::endl;
        permMatr.printMatrix();
    }
    std::cout << "Perm matrix rank: " << permMatr.rank() << std::endl;
    std::cout << "RM(r, m)*P:" << std::endl;
    rmCode = rmCode.toMatrix() * permMatr;
    std::cout << rmCode.toMatrix().rank() << std::endl;
    if (mode == 0) {
        rmCode.printVisualCode();
    } else {
        rmCode.printCodeSizes();
    }
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
    codes::RMCode rm(r, m);
    codes::Lincode rmCode(rm.toMatrix());
    
    std::uniform_int_distribution<int> distrib(1, rmCode.len());
    std::random_device rd;
    std::mt19937 gen(rd());
    matrix::Matrix P = matrix::generateRandomPermutation(rmCode.len(), distrib(gen));

    matrix::Matrix G(rmCode.toMatrix() * P);
    P = testRunner(codes::Lincode(G), mode);
    if (!codes::isEquivalent(rmCode, G*P)) {
        std::cout << "Same basises" << std::endl;
    } else {
        std::cout << "Basises are not the same" << std::endl;
    }
    return 0;
}