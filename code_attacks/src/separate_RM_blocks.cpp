#include "code_attacks.h"

namespace codes {
namespace attackSupporters {


void printVector(const std::vector<unsigned long long> &v) {
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << std::endl;
}

// Returns vector of two vectors {cols, rows}
// usedCols already contains startCols columns
std::vector<std::vector<unsigned long long>>
separatingGaussEliminationStep(matrix::Matrix &m,
                           std::vector<unsigned long long> &usedRows,
                           const std::vector<unsigned long long> &usedCols,
                           unsigned long long k,
                           const std::vector<unsigned long long> &startCols = {}) {
    std::vector<std::vector<unsigned long long>> result(2);
    result[0].reserve(k);
    result[1].reserve(k);
    // Start columns are in usedCols
    unsigned long long indRows = 0;
    for (unsigned long long j = 0; j < startCols.size() && result[0].size() != k; ++j) {
        for (unsigned long long i = 0; i < m.rows(); ++i) {
            if (indRows < usedRows.size() && i == usedRows[indRows]) {
                ++indRows;
                continue;
            }
            if (m.at(i, startCols[j]) == 1) {
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
                result[0].push_back(startCols[j]);
                result[1].push_back(i);
                usedRows.push_back(i);
                for (size_t l = 0; l < m.rows(); ++l) {
                    if (m.at(l, startCols[j]) == 1 && l != i) {
                        for (size_t p = 0; p < m.cols(); ++p) {
                            m.at(l, p) ^= m.at(i, p);
                        }
                    }
                }
                break;
            }
        }
    }
    unsigned long long indCols = 0;
    indRows = 0;
    for (unsigned long long j = 0; j < m.cols() && result[0].size() != k; ++j) {
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
                result[0].push_back(j);
                result[1].push_back(i);
                usedRows.push_back(i);
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


// Returns columns and rows of blocks
// col1, row1, col2, row2, col3, row3
std::vector<std::vector<unsigned long long>>
separatingGaussElimination(matrix::Matrix &m,
                           unsigned long long k1,
                           unsigned long long k2) {
    std::vector<std::vector<unsigned long long>> result(6);
    std::vector<unsigned long long> usedRows;
    std::vector<unsigned long long> usedCols;
    for (size_t i = 0; i < 3; ++i) {
        unsigned long long k = i == 2 ? k2 : k1;
        std::vector<std::vector<unsigned long long>>
        tmp = separatingGaussEliminationStep(m, usedRows, usedCols, k);
        result[2 * i] = tmp[0];
        usedCols.insert(usedCols.end(), result[2 * i].begin(), result[2 * i].end());
        result[2 * i + 1] = tmp[1];
        algorithms::sorts::mergeSort(usedRows,
                                     [](const unsigned long long &a, const unsigned long long &b)
                                     { return a <= b; });
        algorithms::sorts::mergeSort(result[2 * i + 1],
                                     [](const unsigned long long &a, const unsigned long long &b)
                                     { return a <= b; });
    }
    return result;
}

std::vector<unsigned long long>
rowsIntersectionsStep(const std::vector<std::vector<char>> &vv,
                      const std::vector<unsigned long long> &row1,
                      const std::vector<unsigned long long> &row2) {
    std::vector<char> columnValidator(vv[row1[0]].size(), 0);
    for (size_t i = 0; i < row1.size(); ++i) {
        for (size_t j = 0; j < row2.size(); ++j) {
            for (size_t p = 0; p < columnValidator.size(); ++p) {
                if (columnValidator[p] == 0 && vv[row1[i]][p] == 1 && vv[row2[j]][p] == 1) {
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
                          const std::vector<unsigned long long> &row) {
    std::vector<char> columnValidator(vv[row[0]].size(), 0);
    for (size_t i = 0; i < row.size() - 1; ++i) {
        for (size_t j = i + 1; j < row.size(); ++j) {
            for (size_t p = 0; p < columnValidator.size(); ++p) {
                if (columnValidator[p] == 0 && vv[row[i]][p] == 1 && vv[row[j]][p] == 1) {
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
std::vector<std::vector<unsigned long long>> rowsIntersections(const matrix::Matrix &m,
                                                               std::vector<unsigned long long> row1,
                                                               std::vector<unsigned long long> row2,
                                                               std::vector<unsigned long long> row3) {
    std::vector<std::vector<char>> vv = m.toVectors();
    std::vector<std::vector<unsigned long long>> result;
    std::vector<unsigned long long> columnValidator(m.cols(), 1);
    // (1, 1)
    result.push_back(rowsIntersectionsStepSame(vv, row1));
    // (1, 2)
    result.push_back(rowsIntersectionsStep(vv, row1, row2));
    // (1, 3)
    result.push_back(rowsIntersectionsStep(vv, row1, row3));
    // (2, 2)
    result.push_back(rowsIntersectionsStepSame(vv, row2));
    // (2, 3)
    result.push_back(rowsIntersectionsStep(vv, row2, row3));
    // (3, 3)
    result.push_back(rowsIntersectionsStepSame(vv, row3));
    return result;
}


// Split minWeight codewords to two classes by criteria (0|0|x) and (x'|0|x')/(0|x'|x')
// interPair sorted by count
// Returns vector of rows and cols from different classes
// {cols1, rows1, cols2, rows2}
std::vector<std::vector<unsigned long long>>
splittingToClassesByWords(const std::vector<std::vector<unsigned long long>> &minWeightWordsSupports,
                          std::map<unsigned long long, size_t> &interCnt,
                          unsigned long long minWeight) {
    std::vector<unsigned long long> firstCols;
    std::vector<unsigned long long> firstRows;
    std::vector<unsigned long long> lastCols;
    std::vector<unsigned long long> lastRows;
    for (size_t i = 0; i < minWeightWordsSupports.size(); ++i) {
        unsigned long long diagColumn;
        std::vector<unsigned long long> firstColsTmp;
        std::vector<unsigned long long> lastColsTmp;
        std::vector<std::pair<unsigned long long, size_t>> notFounded;
        // Fullfil candidates to classes
        combinationStepVector vecCombinations;
        for (size_t j = 0; j < minWeightWordsSupports[i].size(); ++j) {
            if (interCnt.find(minWeightWordsSupports[i][j]) != interCnt.end()) {
                bool found = false;
                for (size_t l = 0; !found && l < firstCols.size(); ++l) {
                    if (minWeightWordsSupports[i][j] == firstCols[l]) {
                        firstColsTmp.push_back(minWeightWordsSupports[i][j]);
                        found = true;
                    }
                }
                for (size_t l = 0; !found && l < lastCols.size(); ++l) {
                    if (minWeightWordsSupports[i][j] == lastCols[l]) {
                        lastColsTmp.push_back(minWeightWordsSupports[i][j]);
                        found = true;
                    }
                }
                if (!found) {
                    bool isInserted = false;
                    for (auto iter = notFounded.begin(); !isInserted && iter != notFounded.end(); ++iter) {
                        if (interCnt[minWeightWordsSupports[i][j]] < iter->second) {
                            notFounded.push_back(std::make_pair(minWeightWordsSupports[i][j],
                                                                interCnt[minWeightWordsSupports[i][j]]));
                            isInserted = true;
                        }
                    }
                    if (!isInserted) {
                        notFounded.push_back(std::make_pair(minWeightWordsSupports[i][j],
                                                            interCnt[minWeightWordsSupports[i][j]]));
                    }
                }
            } else {
                diagColumn = j;
            }
        }
        unsigned long long separateVal = minWeight / 2;
        std::vector<unsigned long long> notFoundedVector(notFounded.size());
        for (size_t i = 0; i < notFounded.size(); ++i) {
            notFoundedVector[i] = notFounded[i].first;
        }
        if (lastColsTmp.size() > separateVal) {
            if (firstColsTmp.size() == 0) {
                for (size_t l = 0; l < notFoundedVector.size(); ++l) {
                    lastColsTmp.push_back(notFoundedVector[l]);
                }
                lastColsTmp.push_back(diagColumn);
                // Need to update
                lastRows.push_back(i);
            } else {
                // Bad case, refind
            }
        } else if (firstColsTmp.size() > separateVal) {
            // Bad case, refind
        } else {
            // Fork case, need to be updated
            unsigned long long p1;
            unsigned long long p2;
            combinationStepType combStep = std::make_pair(algorithms::combinationsOfSeparatingSets(p1, p2, notFoundedVector), 0);
            if (firstColsTmp.size() == 0) {
                combStep.first.insert(combStep.first.begin(), std::make_pair(notFoundedVector, std::vector<unsigned long long>()));
                // Need to update
                lastRows.push_back(i);
            } else {
                // Need to update
                firstRows.push_back(i);
            }
            for (size_t i = 0; i < combStep.first[combStep.second].first.size(); ++i) {
                firstCols.push_back(combStep.first[combStep.second].first[i]);
            }
            for (size_t i = 0; i < combStep.first[combStep.second].second.size(); ++i) {
                lastCols.push_back(combStep.first[combStep.second].second[i]);
            }
        }
    }
    return {firstCols, firstRows, lastCols, lastRows};
}


} //namespace attackSupporters

// Return columns of block with sizes k x n and minWeight hamWeight basis
std::vector<unsigned long long> separate_blocks(const codes::Lincode &c,
                                                size_t k, size_t n,
                                                size_t minWeight) {
}

} // namespace codes
