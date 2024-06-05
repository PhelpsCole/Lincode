#include "code_attacks.h"
#include "algorithms.h"

void printVector(const std::vector<unsigned long long> &v) {
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void testRunner(matrix::Matrix modRM,
                unsigned long long k1, unsigned long long k2,
                bool mode, unsigned long long iterationNumber) {
    std::cout << "START testRunner" << std::endl;
    std::cout << k1 << " " << k2 << " " << modRM.rows() << std::endl;
    if (mode) {
        std::cout << "sublock with RM(r, m) and RM(r-1, m)^s2:" << std::endl;
        modRM.printVisualMatrix(3);
    }
    while (iterationNumber--) {
        matrix::Matrix rmCode(modRM);
        rmCode.gaussElimination(false);
        /*while (rmCode.gaussElimination(false,
                                       algorithms::generateCols(rmCode.cols(),
                                                                rmCode.rows())).size()
               < rmCode.rows()) {
            rmCode = modRM;
        }*/

        //std::cout << "START separatingGaussElimination" << std::endl;
        //std::vector<std::vector<unsigned long long>>
        //result(codes::attackSupporters::separatingGaussElimination(rmCode, k1, k2));
        //std::cout << "END separatingGaussElimination" << std::endl;

        if (mode) {
            std::cout << "Sublock:" << std::endl;
            rmCode.printVisualMatrix(3, true);
        }
        /*rmCode = codes::Lincode(rmCode).hull().toMatrix();
        if (mode) {
            std::cout << "Sublock's hull:" << std::endl;
            std::cout << rmCode.rows() << std::endl;
            rmCode.printVisualMatrix(3, true);
        }*/

        std::vector<std::vector<char>> matrixVV = rmCode.toVectors();
        //algorithms::sorts::mergeSort(matrixVV, [](const std::vector<char> &row1,
        //                                          const std::vector<char> &row2)
        //                                       { return algorithms::hammingWeight(row1)
        //                                                <= algorithms::hammingWeight(row2); });
        //rmCode = matrix::Matrix(matrixVV);

        std::vector<unsigned long long> counter(rmCode.cols() * rmCode.cols());
        for (size_t i = 0;
             i < matrixVV.size();
             ++i) {
            matrix::Matrix tmp = matrix::hadamardProduct(matrix::Matrix(matrixVV[i]), rmCode);
            tmp.convertToBasis();
            if (tmp.rows() + 1 < rmCode.rows()) {
                std::cout << i << ": " << tmp.rows() << std::endl;
                if (mode) {
                    tmp.printVisualMatrix(3, true);
                }
            }
        }
    }

    std::cout << "END testRunner" << std::endl;
}

int main(int argc, char *argv[]) {
    size_t r = 2, m = 7;
    size_t permMode = 0;
    bool mode = true;
    unsigned long long iterNum = 1;
    if (argc < 3) {
        std::cout << "Input args in format: r m permMode mode iterNum" << std::endl;
        std::cout << "Where permMode=1 -- *= P and permMode=0 --default" << std::endl;
        std::cout << "Where mode=1 -- print visual, mode=0 -- print sizes" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 2: r m " << permMode << " " << mode << " " << iterNum << std::endl;
        std::cout << "By 3: r m permMode " << mode << " " << iterNum << std::endl;
        std::cout << "By 4: r m permMode mode " << iterNum << std::endl;
        return 0;
    } else if (argc == 3) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
    } else if (argc == 4) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        permMode = std::stoi(argv[3]);
    } else if (argc == 5) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        permMode = std::stoi(argv[3]);
        mode = std::stoi(argv[4]);
    } else if (argc >= 6) {
        r = std::stoi(argv[1]);
        m = std::stoi(argv[2]);
        permMode = std::stoi(argv[3]);
        mode = std::stoi(argv[4]);
        iterNum = std::stoi(argv[5]);
    }
    testRunner(codes::pqsigRMSubblockGenerator(r, m, permMode),
               codes::codeSizeFromRM(r, m),
               codes::codeSizeFromRM(r - 1, m),
               mode, iterNum);
    return 0;
}