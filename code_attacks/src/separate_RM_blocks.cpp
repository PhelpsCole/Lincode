#include "code_attacks.h"

namespace codes {
namespace attackSupporters {


void printVector(const std::vector<unsigned long long> &v) {
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
}

std::vector<unsigned long long> 
separatingGaussElimination(matrix::Matrix &m,
                           std::vector<unsigned long long> &usedRows,
                           const std::vector<unsigned long long> &usedCols,
                           unsigned long long k) {
    std::vector<unsigned long long> result;
    result.reserve(k);
    unsigned long long indRows = 0, indCols = 0;
    for (unsigned long long j = 0; j < m.cols() && result.size() != k; ++j) {
        //std::cout << "matrix on (i, j) = (" << " ," << j << ")" << std::endl;
        //m.printVisualMatrix(3);
        //    printVector(result[0]);
        //    printVector(result[1]);
        if (indCols < usedCols.size() && j == usedCols[indCols]) {
            ++indCols;
            continue;
        }
        for (unsigned long long i = 0; i < m.rows(); ++i) {
            if (indRows < usedRows.size() && i == usedRows[indRows]) {
                ++indRows;
                continue;
            }
            if (m.at(i, j) == 1) {
                bool isUsed = false;
                for (size_t l = 0; l < usedRows.size(); ++l) {
                    if (usedRows[l] == i) {
                        isUsed = true;
                        continue;
                    }
                }
                if (isUsed) {
                    continue;
                }
                usedRows.push_back(i);
                result.push_back(j);
                for (size_t l = 0; l < m.rows(); ++l) {
                    if (m.at(l, j) == 1 && l != i) {
                        for (size_t p = 0; p < m.cols(); ++p) {
                            m.at(l, p) ^= m.at(i, p);
                        }
                    }
                }
                break;
            }
        }
    }
    return result;
}


// Returns columns of 1, 2 and 3 rows
VectorOfColumns separatingGaussElimination(matrix::Matrix &m,
                                           unsigned long long k1,
                                           unsigned long long k2) {
    VectorOfColumns result(3);
    std::vector<unsigned long long> usedRows;
    std::vector<unsigned long long> usedCols;
    result[0] = separatingGaussElimination(m, usedRows, usedCols, k1);
    usedCols.insert(usedCols.end(), result[0].begin(), result[0].end());
    algorithms::sorts::mergeSort(usedRows,
                                 [](const unsigned long long &a, const unsigned long long &b)
                                 { return a <= b; });
    printVector(usedCols);
    printVector(usedRows);
    std::cout << std::endl;
    result[1] = separatingGaussElimination(m, usedRows, usedCols, k1);
    usedCols.insert(usedCols.end(), result[1].begin(), result[1].end());
    algorithms::sorts::mergeSort(usedRows,
                                 [](const unsigned long long &a, const unsigned long long &b)
                                 { return a <= b; });
    printVector(usedCols);
    printVector(usedRows);
    std::cout << std::endl;
    result[2] = separatingGaussElimination(m, usedRows, usedCols, k2);
    usedCols.insert(usedCols.end(), result[2].begin(), result[2].end());
    algorithms::sorts::mergeSort(usedRows,
                                 [](const unsigned long long &a, const unsigned long long &b)
                                 { return a <= b; });
    printVector(usedCols);
    printVector(usedRows);
    std::cout << std::endl;
    return result;
}

std::vector<unsigned long long>
rowsIntersectionsStep(const std::vector<std::vector<char>> &vv,
                      unsigned long long blockStart1, unsigned long long blockEnd1,
                      unsigned long long blockStart2, unsigned long long blockEnd2) {
    std::vector<char> columnValidator(vv[blockStart1].size(), 0);
    for (size_t i = blockStart1; i < blockEnd1; ++i) {
        for (size_t j = blockStart2; j < blockEnd2; ++j) {
            for (size_t p = 0; p < columnValidator.size(); ++p) {
                if (columnValidator[p] == 0 && vv[i][p] == 1 && vv[j][p] == 1) {
                    columnValidator[p] = 1;
                }
            }
        }
    }
    std::vector<unsigned long long> result;
    for (size_t i = 0; i < columnValidator.size(); ++i) {
        if (columnValidator[i] == 1) {
            result.push_back(i);
        }
    }
    return result;
}

std::vector<unsigned long long>
rowsIntersectionsStepSame(const std::vector<std::vector<char>> &vv,
                          unsigned long long blockStart, unsigned long long blockEnd) {
    std::vector<char> columnValidator(vv[blockStart].size(), 0);
    for (size_t i = blockStart; i < blockEnd - 1; ++i) {
        for (size_t j = i + 1; j < blockEnd; ++j) {
            for (size_t p = 0; p < columnValidator.size(); ++p) {
                if (columnValidator[p] == 0 && vv[i][p] == 1 && vv[j][p] == 1) {
                    columnValidator[p] = 1;
                }
            }
        }
    }
    std::vector<unsigned long long> result;
    for (size_t i = 0; i < columnValidator.size(); ++i) {
        if (columnValidator[i] == 1) {
            result.push_back(i);
        }
    }
    return result;
}

// Returns columns of intersection pairs between 1, 2 and 3
VectorOfColumns rowsIntersections(const matrix::Matrix &m,
                                  unsigned long long k1,
                                  unsigned long long k2) {
    std::vector<std::vector<char>> vv = m.toVectors();
    VectorOfColumns result;
    std::vector<unsigned long long> columnValidator(m.cols(), 1);
    // (1, 1)
    result.push_back(rowsIntersectionsStepSame(vv, 0, k1));
    // (1, 2)
    result.push_back(rowsIntersectionsStep(vv, 0, k1, k1, 2 * k1));
    // (1, 3)
    result.push_back(rowsIntersectionsStep(vv, 0, k1, 2 * k1, 2 * k1 + k2));
    // (2, 2)
    result.push_back(rowsIntersectionsStepSame(vv, k1, 2 * k1));
    // (2, 3)
    result.push_back(rowsIntersectionsStep(vv, k1, 2 * k1, 2 * k1, 2 * k1 + k2));
    // (3, 3)
    result.push_back(rowsIntersectionsStepSame(vv, 2 * k1, 2 * k1 + k2));
    return result;
}

} //namespace attackSupporters

// Return columns of block with sizes k x n and minWeight hamWeight basis
std::vector<unsigned long long> separate_blocks(const codes::Lincode &c,
                                                size_t k, size_t n,
                                                size_t minWeight) {
}

} // namespace codes
