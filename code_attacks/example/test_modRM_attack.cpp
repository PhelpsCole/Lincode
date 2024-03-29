#include "code_attacks.h"
#include <chrono>
#include <ctime>

matrix::Matrix testRunner(codes::Lincode rmCode, size_t mode) {
    std::cout << "START testRunner:" << std::endl;
    std::cout << "RM(r, m) code:" << std::endl;
    if (mode == 0) {
        rmCode.printVisualCode();
    } else {
        rmCode.printCodeSizes();
    }
    std::cout << "START modRM_attack" << std::endl;
    std::vector<matrix::Matrix> ans(codes::modRM_attack(rmCode, true));
    if (ans.size() == 0) {
        std::cerr << "Wrong matrix and zero result" << std::endl;
        return matrix::Matrix(0, 0);
    }
    matrix::Matrix permMatr = ans[0];
    std::cout << "END modRM_attack" << std::endl;
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
    {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Started generating keys at " << std::ctime(&time) << std::endl;
    }
    matrix::Matrix pqsigRM = codes::pqsigRMGenerator(r, m);
    codes::Lincode pqsigRMcode(pqsigRM);
    
    std::uniform_int_distribution<int> distrib(1, pqsigRMcode.len());
    std::random_device rd;
    std::mt19937 gen(rd());
    matrix::Matrix P = matrix::generateRandomPermutation(pqsigRMcode.len(), distrib(gen));

    matrix::Matrix G(pqsigRMcode.toMatrix() * P);
    {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Ended generating keys at " << std::ctime(&time) << std::endl;
    }
    P = testRunner(codes::Lincode(G), mode);
    if (!codes::isEquivalent(pqsigRMcode, G*P)) {
        std::cout << "Same basises" << std::endl;
    } else {
        std::cout << "Basises are not the same" << std::endl;
    }
    return 0;
}