#include "code_attacks.h"

namespace codes {
namespace attackSupporters {

} //namespace attackSupporters
void printVector(const std::vector<unsigned long long> &vec) {
    for (unsigned long long i = 0; i < vec.size(); ++i) {
        std::cout << vec[i] << " ";
    }
    std::cout << std::endl;
}

void printRow(const std::vector<char> &row) {
    for (unsigned long long i = 0; i < row.size(); ++i) {
        if (row[i] == 1) {
            std::cout << "+";
        } else {
            std::cout << "-";
        }
    }
    std::cout << std::endl;
}

std::vector<std::vector<unsigned long long>>
separateConcatenatedCodeToSets(const codes::Lincode &concatCode) {
    matrix::Matrix concatCodeMatr(concatCode.toMatrix());
    concatCodeMatr.gaussElimination();
    // Default to zeroes
    std::vector<unsigned long long> colsToSets(concatCode.len());
    size_t startSetInd = 1;
    for (unsigned long long i = 0; i < concatCodeMatr.rows(); ++i) {
        std::vector<unsigned long long> tempSet;
        size_t curSet = 0;
        for (unsigned long long j = 0; j < concatCodeMatr.cols(); ++j) {
            if (concatCodeMatr.at(i, j) == 1) {
                if (colsToSets[j] != 0) {
                    if (curSet == 0) {
                        curSet = colsToSets[j];
                    } else {
                        for (unsigned long long l = 0; l < concatCodeMatr.cols(); ++l) {
                            if (curSet == colsToSets[l]) {
                                colsToSets[l] = colsToSets[j];
                            }
                        }
                        curSet = colsToSets[j];
                    }
                } else {
                    tempSet.push_back(j);
                }
            }
        }
        if (curSet == 0) {
            curSet = startSetInd++;
        }
        for (unsigned long long ind = 0; ind < tempSet.size(); ++ind) {
            colsToSets[tempSet[ind]] = curSet;
        }
    }
    std::map<size_t, std::vector<unsigned long long>> separator;
    for (unsigned long long i = 0; i < colsToSets.size(); ++i) {
        if (separator.find(colsToSets[i]) == separator.end()) {
            separator[colsToSets[i]] = {i};
        } else {
            separator[colsToSets[i]].push_back(i);
        }
    }
    std::vector<std::vector<unsigned long long>> ans;
    for (auto iter = separator.begin(); iter != separator.end(); ++iter) {
        ans.push_back(iter->second);
    }
    return ans;
}

matrix::Matrix separateConcatenatedCodes(const codes::Lincode &concatCode) {
    std::vector<std::vector<unsigned long long>>
    columnSets(separateConcatenatedCodeToSets(concatCode));
    matrix::Matrix P(concatCode.len(), concatCode.len());
    unsigned long long ind = 0;
    for (unsigned long long i = 0; i < columnSets.size(); ++i) {
        for (unsigned long long j = 0; j < columnSets[i].size(); ++j) {
            if (ind == P.cols()) {
                throw std::invalid_argument("Wrong separator or inputted matrix");
            }
            P.at(columnSets[i][j], ind++) = 1;
        }
    }
    return P;
}

} // namespace codes