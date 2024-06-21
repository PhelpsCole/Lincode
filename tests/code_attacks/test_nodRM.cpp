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
    rmCode.printCodeSizes();
    //rmCode.printCode();
    std::vector<size_t> rmSizes = codes::rmSizes(rmCode);
    std::vector<int> ans = algorithms::extendedGcd(rmSizes[1] - 1, rmSizes[0]);
    size_t d = ans[0];
    codes::Lincode rm_d = codes::attackSupporters::nodRM(rmCode, ans[1], ans[2]);
    std::cout << "d = " << d << std::endl;
    std::cout << "RM(d, m) code:" << std::endl;
    rm_d.printCodeSizes();
    //rm_d.printCode();
    return 0;
}