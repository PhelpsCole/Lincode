#include "code_attacks.h"

namespace codes {
namespace attackSupporters {

void printVV(const std::vector<std::vector<unsigned long long>> &v, bool withNums = false) {
    for (size_t i = 0; i < v.size(); ++i) {
        for (size_t j = 0; j < v[i].size(); ++j) {
            if (withNums) {
                std::cout << "[" << i << "," << j << "]:";
            }
            std::cout << v[i][j] << " ";
        }
        std::cout << std::endl;
    }
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

std::vector<std::vector<unsigned long long>>
hadPowerCounterTwice(const codes::Lincode &modRM, unsigned long long k1,
                     unsigned long long k2, unsigned long long minWeight,
                     unsigned long long iterationNumber, bool printData) {
    std::vector<std::vector<unsigned long long>> counter;
    for (size_t i = 0; i < modRM.len(); ++i) {
        counter.push_back(std::vector<unsigned long long>(modRM.len()));
    }
    while (iterationNumber--) {
        if (printData) {
            printVV(counter);
            //printVV(counter, true);
        }
        matrix::Matrix modRMMatrix(modRM.toMatrix());
        while (modRMMatrix.gaussElimination(false,
                                            algorithms::generateCols(modRMMatrix.cols(),
                                                                     modRMMatrix.rows())).size()
               < modRMMatrix.rows()) {
            modRMMatrix = modRM.toMatrix();
        }
        //std::vector<std::vector<char>> codeWords(modRMMatrix.toVectors());
        std::vector<std::vector<char>> codeWords(minWeightWords(modRMMatrix.toVectors(), minWeight, false));
        if (codeWords.size() >=2) {
            modRMMatrix = matrix::Matrix(codeWords);
            for (unsigned long long i = 0; i < modRMMatrix.rows() - 1; ++i) {
                for (unsigned long long j = i + 1; j < modRMMatrix.rows(); ++j) {
                    std::vector<unsigned long long> supp;
                    for (unsigned long long k = 0; k < modRMMatrix.cols(); ++k) {
                        if (modRMMatrix.at(i, k) & modRMMatrix.at(j, k)) {
                            supp.push_back(k);
                        }
                    }
                    if (supp.size()) {
                        for (size_t k = 0; k < supp.size() - 1; ++k) {
                            for (size_t l = k + 1; l < supp.size(); ++l) {
                                ++counter[supp[k]][supp[l]];
                                ++counter[supp[l]][supp[k]];
                            }
                        }
                    }
                }
            }
        } else {
            ++iterationNumber;
        }
    }
    for (size_t k = 0; k < counter.size() - 1; ++k) {
        std::cout << k << ": ";
        for (size_t l = k + 1; l < counter[k].size(); ++l) {
            std::cout << counter[k][l] << "(" << l << ") ";
        }
        std::cout << std::endl;
    }
    return counter;
}

} //namespace attackSupporters
} // namespace codes