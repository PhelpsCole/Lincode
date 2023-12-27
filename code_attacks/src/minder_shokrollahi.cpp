#include "code_attacks.h"

namespace codes {
namespace attackSupporters {

std::vector<std::vector<unsigned long long>>
separateToSubClasses(const std::vector<std::vector<unsigned long long>> &counterIJ,
                     unsigned long long max) {
    std::vector<std::set<unsigned long long>> columnSetsVec;
    for (size_t i = 0; i < counterIJ.size(); ++i) {
        // Choosing equiv class
        bool foundColumn = false;
        unsigned long long ind = columnSetsVec.size();
        for (size_t x = 0; !foundColumn && x < columnSetsVec.size(); ++x) {
            for (unsigned long long elem: columnSetsVec[x]) {
                if (i == elem) {
                    foundColumn = true;
                    ind = x;
                    break;
                }
            }
        }
        if (!foundColumn) {
            columnSetsVec.push_back({i});
        }
        for (size_t j = i + 1; j < counterIJ[i].size(); ++j) {
            if (counterIJ[i][j] >= max) {
                columnSetsVec[ind].insert(j);
            }
        }
        // Updating equiv classes
        std::vector<std::set<unsigned long long>> tempColumnVecSet;
        std::set<unsigned long long> colizes(columnSetsVec[ind]);
        for (size_t x = 0; x < columnSetsVec.size(); ++x) {
            if (x == ind) {
                continue;
            }
            bool foundColize = false;
            for (unsigned long long prevSetElem: columnSetsVec[x]) {
                for (unsigned long long newSetElem: columnSetsVec[ind]) {
                    if (prevSetElem == newSetElem) {
                        foundColize = true;
                        break;
                    }
                }
                if (foundColize) {
                    break;
                }
            }
            if (foundColize) {
                colizes.insert(columnSetsVec[x].begin(), columnSetsVec[x].end());
            } else {
                tempColumnVecSet.push_back(columnSetsVec[x]);
            }
        }
        columnSetsVec = tempColumnVecSet;
        columnSetsVec.push_back(colizes);
    }
    std::vector<std::vector<unsigned long long>> ans;
    for (size_t i = 0; i < columnSetsVec.size(); ++i) {
        ans.push_back(std::vector<unsigned long long>(columnSetsVec[i].begin(),
                                                      columnSetsVec[i].end()));
    }
    return ans;
}

bool checkSeparatedSets(const std::vector<std::vector<unsigned long long>> &separatedSets,
                        unsigned long long len, unsigned long long count) {
    if (separatedSets.size() == count) {
        unsigned long long lenSub = len / count;
        for (unsigned long long i = 0; i < count; ++i) {
            if (separatedSets[i].size() != lenSub) {
                return false;
            }
        }
        return true;
    }
    return false;
}

// M != 0 maybe incorrect
std::vector<std::vector<unsigned long long>>
decomposeToColumnSets(const codes::Lincode &c0, size_t r, size_t m,
                      unsigned long long M) {
    unsigned long long degParam = 1 << (m - 2*r + 1);
    unsigned long long min_weight = 1 << (m - r);
    unsigned long long max_weight;
    if (M == 0) {
        max_weight = min_weight;
    } else {
        max_weight = floor(degParam * ((1 << r) - 1) *
                           std::sqrt(1 - 1 / static_cast<double>(degParam)));
    }
    unsigned long long M_step = M;
    codes::Encoder encoder(c0);
    std::vector<std::vector<unsigned long long>> counterIJ;
    for (size_t i = 0; i < c0.len() - 1; ++i) {
        counterIJ.push_back(std::vector<unsigned long long>(c0.len()));
    }
    while (!encoder.isEnd()) { //!!!!
        // Every sicle it cleans
        std::vector<std::vector<char>> selectedCodeWords;
        while (!encoder.isEnd() && (M == 0 || selectedCodeWords.size() < M_step)) {
            std::vector<char> codeWord = encoder.next();
            unsigned long long weight = algorithms::hammingWeight(codeWord);
            if (min_weight <= weight && weight <= max_weight) {
                selectedCodeWords.push_back(codeWord);
            }
        }
        for (size_t k = 0; k < selectedCodeWords.size(); ++k) {
            std::vector<char> tmpWord(selectedCodeWords[k]);
            for (size_t i = 0; i < tmpWord.size() - 1; ++i) {
                for (size_t j = i + 1; j < tmpWord.size(); ++j) {
                    if (tmpWord[i] == 1 && tmpWord[j] == 1) {
                        ++counterIJ[i][j];
                    }
                }
            }
        }
        unsigned long long c;
        bool simple = true;
        if (M == 0 || simple) {
            c = counterIJ[0][1];
            for (size_t i = 0; i < counterIJ.size(); ++i) {
                for (size_t j = i + 1; j < counterIJ[i].size(); ++j) {
                    if (c < counterIJ[i][j]) {
                        c = counterIJ[i][j];
                    }
                }
            }
        } else {
            if (selectedCodeWords.size() != M_step) {
                M -= M_step;
                M += selectedCodeWords.size();
            }
            c = floor(M * (degParam - 1) / static_cast<double>((1 << (m - 1)) - (1 << (r - 1)))) - 1;
        }
        std::vector<std::vector<unsigned long long>> separatedSets(separateToSubClasses(counterIJ, c));
        if (checkSeparatedSets(separatedSets, c0.len(), (1 << r) - 1)) {
            return separatedSets;
        }
        if (M != 0) {
            M += M_step;    
        }
    }
    std::cerr << "Code wasn't separated to classes" << std::endl;
    return std::vector<std::vector<unsigned long long>>();
}

std::vector<char> mergeCodeWords(const std::vector<char> &codeWord,
                                 std::vector<unsigned long long> &separatedColumns) {
    algorithms::sorts::selectionSort(separatedColumns,
                                     [](const unsigned long long &a, const unsigned long long &b)
                                     { return a <= b; });
    std::vector<char> result(codeWord.size());
    unsigned long long ind = 0;
    unsigned long long delta = 0;
    for (size_t i = 0; i < codeWord.size(); ++i) {
        if (codeWord[i] == 1) {
            result[i] = 1;
            ++delta;
        }
        if (ind != separatedColumns.size() && i - delta == separatedColumns[ind]) {
            result[i] = 1;
            ++ind;
        }
    }
    return result;
}

// Minder-Shokrollahi algorithm of reduction RM(r, m) -> RM(r-1,m) !!!
codes::Lincode rmReducer(const codes::Lincode &rm) {
    matrix::Matrix B(0, 0);
    std::vector<size_t> rmSizes = codes::rmSizes(rm);
    unsigned long long size = codeSizeFromRM(rmSizes[0] - 1, rmSizes[1]);
    unsigned long long weight = 1 << (rmSizes[1] - rmSizes[0]);
    std::vector<std::vector<char>> usedCodeWords;
    codes::Encoder encoder(rm);
    while (B.rows() != size) {
        std::vector<char> codeWord;
        // Finding word of minimum weight
        while (!encoder.isEnd()) {
            codeWord = encoder.next();
            if (algorithms::hammingWeight(codeWord) == weight) {
                usedCodeWords.push_back(codeWord);
                break;
            }
        }
        codes::Lincode truncated = rm.truncate(codeWord, true);
        std::vector<std::vector<unsigned long long>>
        // creating V_i(...)
        separatedColumnSets = decomposeToColumnSets(truncated, rmSizes[0], rmSizes[1], 0);
        for (size_t i = 0; i < separatedColumnSets.size(); ++i) {
            std::vector<char> newCodeWord(mergeCodeWords(codeWord, separatedColumnSets[i]));
            B.insertTriangle(newCodeWord);
        }
    }
    return codes::Lincode(B);
}

bool comparator(const std::vector<char> &row1, const std::vector<char> &row2) {
    if (row1.size() != row2.size()) {
        std::cout << row1.size() << " " << row2.size() << std::endl;
        throw std::invalid_argument("Incorrect rows sizes");
    }
    for (size_t i = 0; i < row1.size(); ++i) {
        if (row1[i] < row2[i]) {
            return true;
        } else if (row1[i] > row2[i]) {
            return false;
        }
    }
    return false;
}

// Return permutation (1, m)' => (1, m) where (1, m) is Reed-Muller code
matrix::Matrix simplePerm(matrix::Matrix trivialRMMatrix) {
    // Get matrix with first row (1..1)
    trivialRMMatrix.T();
    std::vector<char> b(trivialRMMatrix.rows(), 1);
    std::vector<char> sol = matrix::solution(trivialRMMatrix, b);
    std::vector<char> aVector(sol);
    bool removedNum = false;
    for (size_t i = 0; i < sol.size(); ++i) {
        if (removedNum || sol[i] == 0) {
            aVector.insert(aVector.end(), sol.begin(), sol.end());
            aVector[aVector.size() - sol.size() + i] ^= 1;
        } else {
            removedNum = true;
        }
    }
    trivialRMMatrix.T();
    matrix::Matrix a(sol.size(), sol.size(), aVector);
    a *= trivialRMMatrix;

    // Delete first row
    std::vector<unsigned long long> rows(a.rows() - 1);
    std::vector<unsigned long long> cols(a.cols());
    std::iota(rows.begin(), rows.end(), 1);
    std::iota(cols.begin(), cols.end(), 0);
    a = a.submatrix(rows, cols);

    // Permutation of sorting rows
    a.T();
    std::vector<std::vector<char>> vv(a.toVectors());
    std::vector<unsigned long long> permVec = algorithms::sorts::
                                              mergeSort(vv, comparator);
    return matrix::permFromVector(permVec, true);
}

} //namespace attackSupporters
} // namespace codes
