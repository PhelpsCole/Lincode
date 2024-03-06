#include "code_attacks.h"
#include "algorithms.h"

matrix::Matrix matrixGenerator(size_t r, size_t m, size_t permMode) {
    std::cout << "START matrixGenerator" << std::endl;
    codes::RMCode rm(r, m);
    matrix::Matrix rmMatr(rm.toMatrix());
    matrix::Matrix zero(rmMatr.rows(), rmMatr.cols());
    matrix::Matrix subblock(rmMatr);
    matrix::Matrix row2(zero);
    subblock.concatenateByRows(zero);
    // (r,m)|0|(r,m)
    subblock.concatenateByRows(rmMatr);
    row2.concatenateByRows(rmMatr);
    // 0|(r,m)|(r,m)
    row2.concatenateByRows(rmMatr);
    subblock.concatenateByColumns(row2);

    rm = codes::RMCode(r - 1, m);
    rmMatr = rm.toMatrix();
    zero = matrix::Matrix(rmMatr.rows(), 2 * rmMatr.cols());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distrib(1, rmMatr.cols());
    matrix::Matrix P = matrix::generateRandomPermutation(rmMatr.cols(), distrib(gen));

    rmMatr = rmMatr * P;

    // 0|0|(r-1,m)
    zero.concatenateByRows(rmMatr);
    subblock.concatenateByColumns(zero);

    matrix::Matrix M = matrix::generateRandomNonSingular(subblock.rows(), subblock.rows());
    std::uniform_int_distribution<int> distrib2(1, subblock.cols());
    P = matrix::generateRandomPermutation(subblock.cols(), distrib2(gen));
    if (!permMode) {
        subblock = M * subblock * P;
    } else {
        subblock = M * subblock;
    }

    std::cout << "END matrixGenerator" << std::endl;
    return subblock;
}
void printVector(const std::vector<unsigned long long> &v, bool withNums = false) {
    for (size_t i = 0; i < v.size(); ++i) {
        if (withNums) {
            std::cout << "[" << i << "]:";
        }
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void testRunner(const matrix::Matrix &rmCode,
                unsigned long long k1,
                unsigned long long k2,
                unsigned long long coutNum,
                unsigned long long minWeight) {
    std::cout << "START testRunner" << std::endl;
    std::cout << k1 << " " << k2 << std::endl;
    std::cout << coutNum << " " << minWeight << std::endl;
    std::cout << "START hadPowerCounter" << std::endl;
    std::vector<unsigned long long>
    counter(codes::attackSupporters::hadPowerCounter(rmCode, k1, k2,
                                                     minWeight, coutNum, false));
    //std::vector<unsigned long long>
    //counter(codes::attackSupporters::hadPowerCounterSelectors(rmCode, coutNum, false));
    std::cout << std::endl;
    std::cout << "END hadPowerCounter" << std::endl;
    printVector(counter);
    printVector(counter, true);
    std::cout << "END testRunner" << std::endl;
}

int main(int argc, char *argv[]) {
    size_t r = 2, m = 7;
    bool mode = false;
    bool permMode = false;
    size_t coutNum = 0;
    size_t step = 0;
    if (argc < 3) {
        std::cout << "Input args in format: r m mode permMode coutNum step" << std::endl;
        std::cout << "Where mode=0 -- subblocks 2-4, mode=1 -- modRM matr" << std::endl;
        std::cout << "Where permMode=0 -- *= P and permMode=1 --default" << std::endl;
        std::cout << "If step!=0 it continues inf times with step" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 2: r m " << permMode << " " << mode << " " << coutNum << " " << step
                  << std::endl;
        std::cout << "By 3: r m mode " << permMode << " " << coutNum << " " << step << std::endl;
        std::cout << "By 4: r m mode permMode " << coutNum << " " << step << std::endl;
        std::cout << "By 5: r m mode permMode coutNum " << step << std::endl;
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
        permMode = std::stoi(argv[4]);
    } else if (argc == 6) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        mode = std::stoi(argv[3]);
        permMode = std::stoi(argv[4]);
        coutNum = std::stoi(argv[5]);
    } else if (argc >= 6) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        mode = std::stoi(argv[3]);
        permMode = std::stoi(argv[4]);
        coutNum = std::stoi(argv[5]);
        step = std::stoi(argv[6]);
    }
    unsigned long long k1, k2, minWeight;
    if (mode) {
        // Need to fix
        k1 = codes::codeSizeFromRM(r, m);
        k2 = 2 * codes::codeSizeFromRM(r - 1, m) + codes::codeSizeFromRM(r - 2, m);
        minWeight = 1 << (m - r + 2);
        matrix::Matrix pqsigRM = codes::pqsigRMGenerator(r, m);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distrib(1, pqsigRM.cols());

        matrix::Matrix M = matrix::generateRandomNonSingular(pqsigRM.rows(), pqsigRM.rows());
        if (permMode) {
            matrix::Matrix P = matrix::generateRandomPermutation(pqsigRM.cols(), distrib(gen));
            pqsigRM = M * pqsigRM * P;
        } else {
            pqsigRM = M * pqsigRM;
        }
        if (!step) {
            testRunner(pqsigRM, k1, k2, coutNum, minWeight);
        }
        else {
            while (coutNum != 100000) {
                std::cout << "Counter = " << coutNum << std::endl;
                coutNum += step;
                testRunner(pqsigRM, k1, k2, coutNum, minWeight);
            }
        }
    } else {
        k1 = codes::codeSizeFromRM(r, m);
        k2 = codes::codeSizeFromRM(r - 1, m);
        minWeight = 1 << (m - r + 1);
        if (!step) {
            testRunner(matrixGenerator(r, m, permMode), k1, k2, coutNum, minWeight);
        }
        else {
            matrix::Matrix subblocks = matrixGenerator(r, m, permMode);
            while (coutNum != 100000) {
                std::cout << "Counter = " << coutNum << std::endl;
                coutNum += step;
                testRunner(subblocks, k1, k2, coutNum, minWeight);
            }
        }
    }
    return 0;
}