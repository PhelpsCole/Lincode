#include "code_attacks.h"

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
    std::cout << "RM(r, m) code:" << std::endl;
    //rmCode.printCodeSizes();
    rmCode.printCode();
    std::cout << std::endl;
    rmCode = codes::attackSupporters::rmReductor(rmCode);
    std::cout << "RM(r-1, m) code:" << std::endl;
    rmCode.printCodeSizes();
    rmCode.printCode();
    return 0;
}