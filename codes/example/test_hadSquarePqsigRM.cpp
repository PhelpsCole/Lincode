#include "pqsigRM.h"

void testCicle() {
    for (size_t m = 4; m < 30; ++m) {
        for (size_t r = 2; r < m; ++r) {
            codes::Lincode rmCode(codes::pqsigRMGenerator(r, m));
            codes::Lincode pqsigrmCode = codes::hadamardProduct(rmCode, rmCode);
            if (pqsigrmCode.size() != pqsigrmCode.len()) {
                std::cout << "r = " << r << ", m = " << m << std::endl;
                rmCode.printCodeSizes();
                pqsigrmCode.printCodeSizes();
                if (pqsigrmCode.size() + 1 != pqsigrmCode.len()) {
                    pqsigrmCode.dual();
                    pqsigrmCode.printCodeSizes();
                }
                std::cout << std::endl;
                break;
            }
        }
    }
}

void testOnce(size_t r, size_t m) {
    std::cout << "r = " << r << ", m = " << m << std::endl;
    codes::Lincode rmCode(codes::RMCode(r, m).toMatrix());
    rmCode.basisView();
    std::cout << "RM code:" << std::endl;
    rmCode.printCodeSizes();
    codes::Lincode pqsigrmCode(codes::pqsigRMGenerator(r, m));
    codes::Lincode hadSquarePqsigRM = codes::hadamardProduct(pqsigrmCode, pqsigrmCode);
    std::cout << "pqsigRM code:" << m << std::endl;
    pqsigrmCode.printCodeSizes();
    std::cout << "Had square of pqsigRM code:" << std::endl;
    hadSquarePqsigRM.printCodeSizes();
    codes::Lincode dual = hadSquarePqsigRM;
    dual.dual();
    std::cout << "Dual of Had square of pqsigRM code:" << std::endl;
    dual.basisView();
    dual.printCodeSizes();
    dual = dual.hull();
    std::cout << "Hull of Had square of pqsigRM code:" << std::endl;
    dual.basisView();
    dual.printCodeSizes();
    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    //testCicle();
    size_t iter_num = 20;
    size_t r = 2, m = 7;
    if (argc == 2) {
        iter_num = std::stoi(argv[1]);
    } else if (argc == 3) {
        m = std::stoi(argv[1]);
        iter_num = std::stoi(argv[2]);
    } else if (argc >= 4) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        iter_num = std::stoi(argv[3]);
    }
    for (size_t i = 0; i < iter_num; ++i) {
        testOnce(r, m);
    }
    //testOnce(2, 8);

    return 0;
}