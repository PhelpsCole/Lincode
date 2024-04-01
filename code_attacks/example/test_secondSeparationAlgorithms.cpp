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

void printVector(const std::vector<unsigned long long> &v) {
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void testRunner(matrix::Matrix modRM,
                unsigned long long k1, unsigned long long k2,
                unsigned long long minWeight, bool mode,
                unsigned long long iterationNumber) {
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
        std::cout << "Where permMode=0 -- *= P and permMode=1 --default" << std::endl;
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
    testRunner(matrixGenerator(r, m, permMode),
               codes::codeSizeFromRM(r, m),
               codes::codeSizeFromRM(r - 1, m),
               1 << (m - (r - 1)), mode,
               iterNum);
    return 0;
}