#include "pqsigRM.h"
#include "codes.h"

void printCode(const codes::Lincode &c, std::string line, size_t mode) {
    return;
    std::cout << line << std::endl;
    if (mode == 1) {
        c.printCodeSizes();
    } else {
        c.printVisualCode(4);
    }

}

void testOnce(size_t r, size_t m, size_t mode, const std::string &paramsOut) {
    //std::cout << "r = " << r << ", m = " << m << std::endl;
    size_t ind = 0;
    //printCode(rmCode, "RM code:", mode);
    codes::Lincode pqsigrmCode(codes::pqsigRMGenerator(r, m));
    std::vector<size_t> ans(7);
    ans[2] = pqsigrmCode.size();

    std::vector<unsigned long long> colsSub(pqsigrmCode.len() / 4);
    for (size_t i = 0; i < colsSub.size(); ++i) {
        colsSub[i] = i;
    }
    std::vector<unsigned long long> rowsSub(pqsigrmCode.size());
    for (size_t i = 0; i < rowsSub.size(); ++i) {
        rowsSub[i] = i;
    }
    if (paramsOut[ind++] == '1') {
        printCode(pqsigrmCode, "pqsigRM code:", mode);
    }
    if (paramsOut[ind++] == '1') {
        printCode(codes::Lincode(pqsigrmCode.toMatrix().submatrix(rowsSub, colsSub)), "pqsigRM first block:", mode);
    }

    if (paramsOut[ind++] == '1') {
        pqsigrmCode.basisView();
        printCode(pqsigrmCode, "basisView pqsigRM code:", mode);
    }

    codes::Lincode dual = pqsigrmCode;
    if (paramsOut[ind++] == '1') {
        dual.dual();
        dual.basisView();
        printCode(dual, "Dual of pqsigRM code:", mode);
        ans[3] = dual.size();
    }

    if (paramsOut[ind++] == '1') {
        rowsSub.resize(dual.size());
        codes::Lincode tmp(codes::Lincode(dual.toMatrix().submatrix(rowsSub, colsSub)));
        tmp.basisView();
        printCode(tmp, "Dual first block:", mode);
        ans[4] = tmp.size();
    }

    if (paramsOut[ind++] == '1') {
        dual = codes::hadamardProduct(pqsigrmCode, dual);
        dual.basisView();
        printCode(dual, "pqsigRM^-1 * pqsigRM:", mode);
    }

    codes::Lincode hull = pqsigrmCode.hull();
    hull.basisView();
    if (paramsOut[ind++] == '1') {
        printCode(hull, "Hull of pqsigRM code:", mode);
        ans[5] = hull.size();
    }

    if (paramsOut[ind++] == '1') {
        rowsSub.resize(hull.size());
        codes::Lincode tmp(codes::Lincode(hull.toMatrix().submatrix(rowsSub, colsSub)));
        tmp.basisView();
        printCode(tmp, "Hull's first block:", mode);
        ans[6] = tmp.size();
    }
    ans[0] = r;
    ans[1] = m;
    std::cout << "(r, m) & modRM & dual & hull" << std::endl;
    std::cout << "(" << ans[0] << ", " << ans[1] << ") & " << ans[2] << " & "
              << ans[3] << " & " << ans[5] << " \\\\\\hline" << std::endl;
    std::cout << "(r, m) & modRM & dual_1 & hull_1" << std::endl;
    std::cout << "(" << ans[0] << ", " << ans[1] << ") & " << ans[2] << " & * & "
              << ans[4] << " & " << ans[6] << " \\\\\\hline" << std::endl;
    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    size_t iter_num = 1;
    size_t r = 2, m = 7;
    size_t mode = 1;
    std::string paramsOut = "11111011";
    if (argc < 3) {
        std::cout << "Input args in format: r m mode iter_num paramsOut" << std::endl;
        std::cout << "Where mode=0 -- printVisualCode, mode=1 -- printCodeSizes" << std::endl;
        std::cout << "Len of paramsOut must be " << paramsOut.size() << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 2: r m " << mode << " " << iter_num << paramsOut << std::endl;
        std::cout << "By 3: r m mode " << iter_num << paramsOut << std::endl;
        std::cout << "By 4: r m mode iter_num " << paramsOut << std::endl;
        return 0;
    } else if (argc == 3) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    } else if (argc == 4) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        mode = std::stoi(argv[3]);
    } else if (argc == 5) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        mode = std::stoi(argv[3]);
        iter_num = std::stoi(argv[4]);
    } else if (argc >= 6) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        mode = std::stoi(argv[3]);
        iter_num = std::stoi(argv[4]);
        paramsOut = argv[5];
    }
    for (size_t i = 0; i < iter_num; ++i) {
        testOnce(r, m, mode, paramsOut);
    }
    return 0;
}