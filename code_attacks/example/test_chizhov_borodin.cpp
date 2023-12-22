#include "code_attacks.h"

void testRunner(codes::Lincode rmCode) {
    std::cout << "RM(r, m) code:" << std::endl;
    //rmCode.printCodeSizes();
    rmCode.printVisualCode();
    matrix::Matrix permMatr(chizhov_borodin(rmCode));
    std::cout << "Perm matrix:" << std::endl;
    //permMatr.printMatrix();
    std::cout << "Perm matrix rank: " << permMatr.rank() << std::endl;
    std::cout << "RM(r, m)*P:" << std::endl;
    rmCode = rmCode.toMatrix() * permMatr;
    std::cout << rmCode.toMatrix().rank() << std::endl;
    rmCode.printVisualCode();
}

int main(int argc, char *argv[]) {
    size_t r = 2, m = 7;
    if (argc < 3) {
        std::cout << "Input args in format: r m" << std::endl;
        return 0;
    } else if (argc >= 3) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    }
    codes::RMCode rm(r, m);
    codes::Lincode rmCode(rm.toMatrix());
    testRunner(rmCode);

    std::uniform_int_distribution<int> distrib(1, rmCode.len());
    std::random_device rd;
    std::mt19937 gen(rd());
    matrix::Matrix P = matrix::generateRandomPermutation(rmCode.len(), 5);

    matrix::Matrix G(rmCode.toMatrix() * P);
    testRunner(codes::Lincode(G));
    return 0;
}