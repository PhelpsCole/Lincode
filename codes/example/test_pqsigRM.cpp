#include "pqsigRM.h"

std::string spectrHull(const codes::Lincode &code) {
    codes::Lincode hull = code.hull();
    return hull.stringSpectrum();
    //return std::to_string(hull.size());
}

std::string spectrMinRM(const codes::Lincode &code) {
    std::vector<size_t> rmSizes = codes::rmSizes(code);
    size_t r = rmSizes[0], m = rmSizes[1];
    codes::Lincode newCode(code);
    if (m == 0) {
        return "Not RM-code for invariant";
    }
    if (r != 1) {
        if (2*r + 1 > m) {
            r = m - r - 1;
            newCode.dual();
        }
        size_t q = m / r;
        if (q * r == m) {
            --q;
        }
        if (m - r * q != 1 && m < r * (q - 1) + 1) {
            newCode = codes::hadPower(code, q);
            newCode.dual();
        }
    }
    return newCode.stringSpectrum();
    
}

int main() {
    //std::cout << "Print 1 if you want to use default parameters" << std::endl;
    //char c;
    //std::cin >> c;
    //size_t r, m;
    //if (c != '1') {
    //    std::cout << "Input parameters of your pqsigRM code:" << std::endl;
    //    std::cout << "r: ";
    //    std::cin >> r;
    //    std::cout << "m: ";
    //    std::cin >> m;
    //} else {
    //    r = 3;
    //    m = 5;
    //}
    //matrix::Matrix matr = codes::pqsigRMGenerator(r, m);
    //matr.printMatrix();
    //std::cout << "Block view:" << std::endl;
    //matr.printMatrixInBlocks(matr.rows(), matr.cols() / 4);
    size_t r = 3, m = 10;
    for (size_t i = 0; i < 20; ++i) {
        codes::RMCode rm(r, m);
        std::cout << "RM-code:" << std::endl;
        codes::Lincode rmCode(rm.toMatrix());
        rmCode.printCodeSizes();
        //rmCode.printCode();
        //std::cout << spectrMinRM(rmCode) << std::endl;
        std::cout << "Generated pqsigRM:" << std::endl;
        matrix::Matrix matr = codes::pqsigRMGenerator(r, m);
        codes::Lincode code(matr);
        code.printCodeSizes();
        //code.printCode();
        //std::cout << spectrMinRM(code) << std::endl;
        std::cout << "McEliec on pqsigRM:" << std::endl;
        code = mcEliece(code);
        code.printCodeSizes();
        //code.printCode();
        //std::cout << spectrMinRM(code) << std::endl;
        std::cout << "Echelon form:" << std::endl;
        code.basisView();
        code.printCodeSizes();
        //code.printCode();
        //std::cout << spectrMinRM(code) << std::endl;
    }

    return 0;
}