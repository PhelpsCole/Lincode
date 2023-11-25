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

void printCode(const codes::Lincode &c, std::string line, size_t mode) {
    std::cout << line << std::endl;
    if (mode == 1) {
        c.printCodeSizes();
    } else {
        c.printVisualCode(4);
    }

}

void testOnce(size_t r, size_t m, size_t mode) {
    std::cout << "r = " << r << ", m = " << m << std::endl;
    codes::Lincode rmCode(codes::RMCode(r, m).toMatrix());
    rmCode.basisView();
    //printCode(rmCode, "RM code:", mode);
    codes::Lincode pqsigrmCode(codes::pqsigRMGenerator(r, m));

    //pqsigrmCode.basisView();
    printCode(pqsigrmCode, "pqsigRM code:", mode);

    //codes::Lincode hadSquareRM(codes::RMCode(2*r, 7).toMatrix());// = codes::hadamardProduct(rmCode, rmCode);
    //hadSquareRM.basisView();
    //printCode(hadSquareRM, "2*r of RM code:", mode);

    //hadSquareRM = codes::hadamardProduct(rmCode, rmCode);
    //hadSquareRM.basisView();
    //printCode(hadSquareRM, "Had square of RM code:", mode);

    //codes::Lincode hadSquarePqsigRM = codes::hadamardProduct(pqsigrmCode, pqsigrmCode);
    //printCode(hadSquarePqsigRM, "Had square of pqsigRM code:", mode);

    //codes::Lincode dual = hadSquarePqsigRM;
    //dual.dual();
    //dual.basisView();
    //printCode(dual, "Dual of Had square of pqsigRM code:", mode);

    //dual = dual.hull();
    //dual.basisView();
    //printCode(dual, "Hull of Had square of pqsigRM code:", mode);

    codes::Lincode maxSubRM(pqsigrmCode);
    codes::maxRMSubMatrPqsigRM(maxSubRM);
    printCode(maxSubRM, "maxSubRM of pqsigRM code:", mode);

    codes::Lincode hullMaxSubRM = maxSubRM.hull();
    hullMaxSubRM.basisView();
    printCode(hullMaxSubRM, "Hull of maxSubRM of pqsigRM code:", mode);
}

int main(int argc, char *argv[]) {
    //testCicle();
    size_t iter_num = 1;
    size_t r = 2, m = 7;
    size_t mode = 1;
    if (argc < 3) {
        std::cout << "Input args in format: r m mode iter_num" << std::endl;
        std::cout << "Where mode=0 -- printVisualCode, mode=1 -- printCodeSizes" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 2: r m " << mode << " " << iter_num << std::endl;
        std::cout << "By 3: r m mode " << iter_num << std::endl;
        return 0;
    } else if (argc == 3) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    } else if (argc == 4) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        mode = std::stoi(argv[3]);
    } else if (argc >= 5) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        mode = std::stoi(argv[3]);
        iter_num = std::stoi(argv[4]);
    }
    for (size_t i = 0; i < iter_num; ++i) {
        testOnce(r, m, mode);
    }
    return 0;
}