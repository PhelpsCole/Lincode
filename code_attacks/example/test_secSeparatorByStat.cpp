#include "code_attacks.h"
#include "algorithms.h"

bool testRunner(const matrix::Matrix &rmCode,
                unsigned long long k1,
                unsigned long long k2,
                unsigned long long coutNum,
                unsigned long long minWeight,
                bool printMatrix) {
    std::cout << "START testRunner" << std::endl;
    std::cout << k1 << " " << k2 << std::endl;
    std::cout << coutNum << " " << minWeight << std::endl;
    std::cout << "START secSeparatorByStat" << std::endl;
    std::vector<std::vector<unsigned long long>>
    classes(codes::attackSupporters::secSeparatorByStat(rmCode, minWeight, coutNum, printMatrix));
    std::cout << "END secSeparatorByStat" << std::endl;
    if (classes[0].size() == 0) {
        return false;
    }
    std::cout << "1-2 blocks:" << std::endl;
    for (size_t i = 0; i < classes[0].size(); ++i) {
        std::cout << classes[0][i] << " ";
    }
    std::cout << std::endl;
    std::cout << "3 block:" << std::endl;
    for (size_t i = 0; i < classes[1].size(); ++i) {
        std::cout << classes[1][i] << " ";
    }
    std::cout << std::endl;
    std::cout << "END testRunner" << std::endl;
    return true;
}

int main(int argc, char *argv[]) {
    size_t r = 2, m = 7;
    bool printMatrix = false;
    bool permMode = false;
    size_t coutNum = 0;
    size_t step = 0;
    size_t iterNum = 1;
    if (argc < 3) {
        std::cout << "Input args in format: r m printMatrix "
                  << "permMode coutNum step iterNum" << std::endl;
        //std::cout << "Where mode=0 -- subblocks 2-4, mode=1 -- modRM matr" << std::endl;
        std::cout << "Where permMode=1 -- *= P and permMode=0 --default" << std::endl;
        std::cout << "If step!=0 it continues inf times with step" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 2: r m " << printMatrix << " " << permMode
                  << " " << coutNum << " " << step << iterNum << std::endl;
        std::cout << "By 3: r m printMatrix " << permMode << " " << coutNum
                  << " " << step << iterNum << std::endl;
        std::cout << "By 4: r m printMatrix permMode " << coutNum
                  << " " << step << iterNum << std::endl;
        std::cout << "By 5: r m printMatrix permMode coutNum " << step << iterNum << std::endl;
        return 0;
    } else if (argc == 3) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    } else if (argc == 4) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        printMatrix = std::stoi(argv[3]);
    } else if (argc == 5) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        printMatrix = std::stoi(argv[3]);
        permMode = std::stoi(argv[4]);
    } else if (argc == 6) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        printMatrix = std::stoi(argv[3]);
        permMode = std::stoi(argv[4]);
        coutNum = std::stoi(argv[5]);
    } else if (argc >= 7) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        printMatrix = std::stoi(argv[3]);
        permMode = std::stoi(argv[4]);
        coutNum = std::stoi(argv[5]);
        step = std::stoi(argv[6]);
    } else if (argc >= 8) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        printMatrix = std::stoi(argv[3]);
        permMode = std::stoi(argv[4]);
        coutNum = std::stoi(argv[5]);
        step = std::stoi(argv[6]);
        iterNum = std::stoi(argv[7]);
    }
    for (size_t i = 0; i < iterNum; ++i) {
        matrix::Matrix subblocks(codes::pqsigRMSubblockGenerator(r, m, permMode));
        if (codes::attackSupporters::testSSASubblocks(subblocks, r, m)) {
            std::cout << "Found by SSA" << std::endl;
            ++iterNum;
            continue;
        }
        unsigned long long k1, k2, minWeight;
        k1 = codes::codeSizeFromRM(r, m);
        k2 = codes::codeSizeFromRM(r - 1, m);
        minWeight = 1 << (m - r + 1);
        if (!step) {
            testRunner(subblocks, k1, k2, coutNum, minWeight, printMatrix);
        }
        else {
            while (coutNum != 100000) {
                std::cout << "Counter = " << coutNum << std::endl;
                coutNum += step;
                if(testRunner(subblocks, k1, k2, coutNum, minWeight, printMatrix)) {
                    std::cout << "Permutation found!" << std::endl;
                    break;
                }
            }
        }
    }
    return 0;
}