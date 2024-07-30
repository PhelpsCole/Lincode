#include "code_attacks.h"
#include "algorithms.h"

void printVector(const std::vector<unsigned long long> &v, bool withNums = false) {
    for (size_t i = 0; i < v.size(); ++i) {
        if (withNums) {
            std::cout << "[" << i << "]:";
        }
        if (i == v.size() / 3 * 2 ||
            i == v.size() / 3) {
            std::cout << "|";
        }
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void printVV(const std::vector<std::vector<unsigned long long>> &v,
             bool printMatrix = false, bool withNums = false) {
    std::vector<unsigned long long> ans(v.size());
    if (printMatrix) {
        for (size_t i = 0; i < v.size(); ++i) {
            std::cout << i << ": ";
            for (size_t j = 0; j < v[i].size(); ++j) {
                if (j == v[i].size() / 3 * 2 ||
                    j == v[i].size() / 3) {
                    std::cout << "|";
                }
                std::cout << v[i][j];
                if (withNums) {
                    std::cout << "(" << j << ")";
                }
                std::cout << " ";
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
    for (size_t i = 0; i < v.size(); ++i) {
        if (i % (v[i].size() / 3) == 0 && i != 0) {
            std::cout << std::endl;
        }
        std::cout << i << ": ";
        {
            std::vector<unsigned long long> tmp(v[i]);
            algorithms::sorts::mergeSort(tmp,
                                         [](const unsigned long long &a, const unsigned long long &b)
                                         { return a >= b; });
            std::cout << tmp[0] << " " << tmp[1] << " " << tmp[2] << " ";
            unsigned long long sum = 0;
            unsigned long long count = v[i].size() / 3;
            unsigned long long sep = 2;
            for (size_t i = sep; i < sep + count; ++i) {
                sum += tmp[i];
            }
            ans[i] = sum / count;
            std::cout << ans[i] << std::endl;
        }
    }
    std::cout << std::endl;
    printVector(ans);
    algorithms::sorts::mergeSort(ans,
                                 [](const unsigned long long &a, const unsigned long long &b)
                                 { return a >= b; });
    printVector(ans);

}

void testRunner(const matrix::Matrix &rmCode,
                unsigned long long k1,
                unsigned long long k2,
                unsigned long long coutNum,
                unsigned long long minWeight,
                bool printMatrix) {
    std::cout << "START testRunner" << std::endl;
    std::cout << k1 << " " << k2 << std::endl;
    std::cout << coutNum << " " << minWeight << std::endl;
    std::cout << "START findingCounter" << std::endl;
    std::vector<std::vector<unsigned long long>>
    counter(codes::attackSupporters::findingCounter(rmCode, minWeight, coutNum, false));
    std::cout << std::endl;
    std::cout << "END findingCounter" << std::endl;
    printVV(counter, printMatrix);
    //printVV(counter, true);
    std::cout << "END testRunner" << std::endl;
}

int main(int argc, char *argv[]) {
    size_t r = 2, m = 7;
    bool mode = false;
    bool printMatrix = false;
    bool permMode = false;
    size_t coutNum = 0;
    size_t step = 0;
    if (argc < 3) {
        std::cout << "Input args in format: r m printMatrix permMode coutNum step" << std::endl;
        //std::cout << "Where mode=0 -- subblocks 2-4, mode=1 -- modRM matr" << std::endl;
        std::cout << "Where permMode=1 -- *= P and permMode=0 --default" << std::endl;
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
    } else if (argc >= 6) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        printMatrix = std::stoi(argv[3]);
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

        matrix::Matrix M = matrix::generateRandomNonSingular(pqsigRM.rows(), pqsigRM.rows());
        if (permMode) {
            matrix::Matrix P = permutation::generateRandomPermutation(pqsigRM.cols()).matrix();
            pqsigRM = M * pqsigRM * P;
        } else {
            pqsigRM = M * pqsigRM;
        }
        if (!step) {
            testRunner(pqsigRM, k1, k2, coutNum, minWeight, printMatrix);
        }
        else {
            while (coutNum != 100000) {
                std::cout << "Counter = " << coutNum << std::endl;
                coutNum += step;
                testRunner(pqsigRM, k1, k2, coutNum, minWeight, printMatrix);
            }
        }
    } else {
        k1 = codes::codeSizeFromRM(r, m);
        k2 = codes::codeSizeFromRM(r - 1, m);
        minWeight = 1 << (m - r + 1);
        if (!step) {
            testRunner(codes::pqsigRMSubblockGenerator(r, m, permMode),
                       k1, k2, coutNum, minWeight, printMatrix);
        }
        else {
            matrix::Matrix subblocks = codes::pqsigRMSubblockGenerator(r, m, permMode);
            while (coutNum != 100000) {
                std::cout << "Counter = " << coutNum << std::endl;
                coutNum += step;
                testRunner(subblocks, k1, k2, coutNum, minWeight, printMatrix);
            }
        }
    }
    return 0;
}