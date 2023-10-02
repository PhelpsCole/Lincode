#include "pqsigRM.h"


int main() {
    std::cout << "Print 1 if you want to use default parameters" << std::endl;
    char c;
    std::cin >> c;
    size_t r, m;
    if (c != '1') {
        std::cout << "Input parameters of your pqsigRM code:" << std::endl;
        std::cout << "r: ";
        std::cin >> r;
        std::cout << "m: ";
        std::cin >> m;
    } else {
        r = 3;
        m = 5;
    }
    matrix::Matrix matr = codes::pqsigRMGenerator(r, m);
    matr.printMatrix();

    return 0;
}