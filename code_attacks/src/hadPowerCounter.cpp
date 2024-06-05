#include "code_attacks.h"

namespace codes {
namespace attackSupporters {

void printVector(const std::vector<unsigned long long> &v, bool withNums = false) {
    for (unsigned long long i = 0; i < v.size(); ++i) {
        if (i == v.size() / 3 * 2) {
            std::cout << "| ";
        }
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

std::vector<unsigned long long>
generateBlockCols(const matrix::Matrix &modRMMatrix, unsigned long long k2) {
    unsigned long long endBlock = modRMMatrix.cols() / 3 * 2;
    std::vector<unsigned long long>
    cols(algorithms::generateCols(endBlock, modRMMatrix.rows() - k2));
    std::vector<unsigned long long>
    cols2(algorithms::generateCols(modRMMatrix.cols() / 3, k2));
    for (unsigned long long i = 0; i < cols2.size(); ++i) {
        cols2[i] += endBlock;
    }
    cols.insert(cols.end(), cols2.begin(), cols2.end());
    //printVector(cols);
    return cols;
}


std::vector<unsigned long long>
hadPowerCounter(const codes::Lincode &modRM,
                unsigned long long iterationNumber,
                unsigned long long k2,
                bool hullMode,
                bool byMinWeight,
                unsigned long long minWeight,
                size_t fillingNumber,
                bool includeDependent) {
    std::vector<unsigned long long> counter(modRM.len());
    unsigned long long mid;
    unsigned long long border = modRM.len() / 3 - k2 + 1;
    unsigned long long wordsNumber = k2;
    if (hullMode) {
        border = modRM.len();
        wordsNumber = modRM.size();
    }
    std::cout << border << std::endl;
    matrix::Matrix modRMMatrix(0, 0);
    unsigned long long midSum = 0;
    std::vector<unsigned long long> v;
    std::vector<unsigned long long> cols;
    bool notFinding = false;
    for (size_t i = 0; i < fillingNumber; ) {
        mid = 0;
        modRMMatrix = modRM.toMatrix();
        cols = algorithms::generateCols(modRMMatrix.cols(), modRMMatrix.rows());
        v = modRMMatrix.gaussElimination(false,  cols);
        for (unsigned long long i = modRMMatrix.rows() - wordsNumber;
             i < modRMMatrix.rows(); ++i) {
            mid += algorithms::hammingWeight(modRMMatrix.row(i));
        }
        //std::cout << modRMMatrix.rows() << " " << v.size() << std::endl;
        //std::cout << mid / wordsNumber << " " << border << std::endl;
        //std::cout << std::endl;
        if ((!includeDependent && v.size() < modRMMatrix.rows()) || (mid / wordsNumber) > border) {
            continue;
        }
        //std::cout << "FOUND" << std::endl;
        ++i;
        midSum += mid;
    }
    border = midSum / (fillingNumber * wordsNumber);
    std::cout << "New: " << border << std::endl;
    while (iterationNumber--) {
        do {
            mid = 0;
            modRMMatrix = modRM.toMatrix();
            cols = algorithms::generateCols(modRMMatrix.cols(), modRMMatrix.rows());
            v = modRMMatrix.gaussElimination(false,  cols);
            for (unsigned long long i = modRMMatrix.rows() - wordsNumber;
                 i < modRMMatrix.rows(); ++i) {
                mid += algorithms::hammingWeight(modRMMatrix.row(i));
            }
            mid /= wordsNumber;
        } while ((v.size() < modRMMatrix.rows() && !includeDependent) || mid > border);
        //std::cout << "After gaussElimination:" << std::endl;
        //std::cout << mid << std::endl;
        //modRMMatrix.printVisualMatrix(3, true);
        /*
        for (size_t i = 0; i < cols.size(); ++i) {
            if (i == modRMMatrix.rows() - k2) {
                std::cout << "|";
            }
            std::cout << cols[i] << " ";
        }
        std::cout << std::endl;
        */
        std::vector<std::vector<char>> codeWords;
        if (byMinWeight) {
            codeWords = minWeightWords(modRMMatrix.toVectors(), minWeight, false);
            std::cout << codeWords.size() << " ";
        }
        if (!byMinWeight || codeWords.size() >=2) {
            if (byMinWeight) {
                modRMMatrix = matrix::Matrix(codeWords);
            }
            for (unsigned long long i = 0; i < modRMMatrix.rows() - 1; ++i) {
                for (unsigned long long j = i + 1; j < modRMMatrix.rows(); ++j) {
                    for (unsigned long long k = 0; k < modRMMatrix.cols(); ++k) {
                        if (modRMMatrix.at(i, k) & modRMMatrix.at(j, k)) {
                            ++counter[k];
                        }
                    }
                }
            }
        } else {
            ++iterationNumber;
        }
    }
    return counter;
}

} //namespace attackSupporters
} // namespace codes