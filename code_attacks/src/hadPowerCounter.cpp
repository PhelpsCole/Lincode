#include "code_attacks.h"

namespace codes {
namespace attackSupporters {

void printVector(const std::vector<unsigned long long> &v, bool withNums = false) {
    for (unsigned long long i = 0; i < v.size(); ++i) {
        if (withNums) {
            std::cout << "[" << i << "]:";
        }
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

void printVector(const std::vector<char> &v) {
    for (unsigned long long i = 0; i < v.size(); ++i) {
        if (v[i]) {
            std::cout << "+";
        } else {
            std::cout << "-";
        }
    }
    std::cout << std::endl;
}

std::vector<std::vector<char>> minWeightWords(std::vector<std::vector<char>> matrixVV,
                                              unsigned long long minWeight,
                                              bool printData) {
    algorithms::sorts::mergeSort(matrixVV, [](const std::vector<char> &row1,
                                              const std::vector<char> &row2)
                                           { return algorithms::hammingWeight(row1)
                                                    <= algorithms::hammingWeight(row2); });
    std::vector<std::vector<char>> result;
    if (printData) {
        for (unsigned long long i = 0; i < matrixVV.size(); ++i) {
            std::cout << algorithms::hammingWeight(matrixVV[i]) << ": ";
            printVector(matrixVV[i]);
        }
    }
    for (unsigned long long i = 0;
         i < matrixVV.size() && algorithms::hammingWeight(matrixVV[i]) <= minWeight;
         ++i) {
        result.push_back(matrixVV[i]);
    }
    if (result.size() >= 2) {
        std::cout << result.size() << " ";
    }
    if (printData && result.size() != 0) {
        std::cout << result[0].size() << std::endl;
        for (unsigned long long i = 0; i < result.size(); ++i) {
            unsigned long long cnt = 0;
            for (unsigned long long j = 0; j < result[i].size(); ++j) {
                if (result[i][j]) {
                    std::cout << "+";
                    ++cnt;
                } else {
                    std::cout << "-";
                }
            }
            std::cout << " " << cnt << std::endl;
        }
    }
    return result;
}


std::vector<unsigned long long> hadPowerCounter(const codes::Lincode &modRM,
                                                unsigned long long k1,
                                                unsigned long long k2,
                                                unsigned long long minWeight,
                                                unsigned long long iterationNumber,
                                                bool printData) {
    std::vector<unsigned long long> counter(modRM.len());
    while (iterationNumber--) {
        if (printData) {
            printVector(counter);
            printVector(counter, true);
        }
        matrix::Matrix modRMMatrix(modRM.toMatrix());

        /*
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distrib(1, modRMMatrix.cols());
        matrix::Matrix P = matrix::generateRandomPermutation(modRMMatrix.cols(), distrib(gen));
        modRMMatrix *= P;
        codes::attackSupporters::separatingGaussElimination(modRMMatrix, k1, k2);
        //modRMMatrix.gaussElimination();
        */
        while (modRMMatrix.gaussElimination(false,
                                            algorithms::generateCols(modRMMatrix.cols(),
                                                                     modRMMatrix.rows())).size()
               < modRMMatrix.rows()) {
            modRMMatrix = modRM.toMatrix();
        }
        std::vector<std::vector<char>> codeWords(minWeightWords(modRMMatrix.toVectors(), minWeight, printData));
        if (codeWords.size() >=2) {
            modRMMatrix = matrix::Matrix(codeWords);
            //P.T();
            //modRMMatrix *= P;
            for (unsigned long long i = 0; i < modRMMatrix.rows() - 1; ++i) {
                for (unsigned long long j = i + 1; j < modRMMatrix.rows(); ++j) {
                    for (unsigned long long k = 0; k < modRMMatrix.cols(); ++k) {
                        if (modRMMatrix.at(i, k) & modRMMatrix.at(j, k)) {
                            ++counter[k];
                        }
                    }
                }
            }
            //
            for (unsigned long long i = 0; i < modRMMatrix.rows() - 2; ++i) {
                for (unsigned long long j = i + 1; j < modRMMatrix.rows() - 1; ++j) {
                    for (unsigned long long l = j + 1; l < modRMMatrix.rows(); ++l) {
                        for (unsigned long long k = 0; k < modRMMatrix.cols(); ++k) {
                            if (modRMMatrix.at(i, k) & modRMMatrix.at(j, k) & modRMMatrix.at(l, k)) {
                                ++counter[k];
                            }
                        }
                    }
                }
            }
        } else {
            ++iterationNumber;
        }
        /*
        //P.T();
        //modRMMatrix *= P;
        for (unsigned long long i = 0; i < modRMMatrix.rows() - 1; ++i) {
            for (unsigned long long j = i + 1; j < modRMMatrix.rows(); ++j) {
                for (unsigned long long k = 0; k < modRMMatrix.cols(); ++k) {
                    if (modRMMatrix.at(i, k) & modRMMatrix.at(j, k)) {
                        ++counter[k];
                    }
                }
            }
        }
        //
        for (unsigned long long i = 0; i < modRMMatrix.rows() - 2; ++i) {
            for (unsigned long long j = i + 1; j < modRMMatrix.rows() - 1; ++j) {
                for (unsigned long long l = j + 1; l < modRMMatrix.rows(); ++l) {
                    for (unsigned long long k = 0; k < modRMMatrix.cols(); ++k) {
                        if (modRMMatrix.at(i, k) & modRMMatrix.at(j, k) & modRMMatrix.at(l, k)) {
                            ++counter[k];
                        }
                    }
                }
            }
        }
        */
    }
    return counter;
}

} //namespace attackSupporters
} // namespace codes