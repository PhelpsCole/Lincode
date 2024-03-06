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
std::vector<unsigned long long>
hadPowerCounterSelectors(const codes::Lincode &modRM,
                         unsigned long long iterationNumber,
                         bool printData) {
    std::vector<unsigned long long> counter(modRM.len());
    std::vector<std::vector<char>> minimalVectors;
    while (iterationNumber--) {
        if (printData) {
            printVector(counter);
            printVector(counter, true);
        }
        matrix::Matrix modRMMatrix(modRM.toMatrix());
        while (modRMMatrix.gaussElimination(false,
                                            algorithms::generateCols(modRMMatrix.cols(),
                                                                     modRMMatrix.rows())).size()
               < modRMMatrix.rows()) {
            modRMMatrix = modRM.toMatrix();
        }
        std::vector<std::vector<char>> newVectors(modRMMatrix.toVectors());
        unsigned long long colInd = 0;
        for (unsigned long long l = 0; l < newVectors.size(); ++l) {
            bool isFound = false;
            unsigned long long i = 0;
            for (; i < minimalVectors.size(); ++i) {
                if (newVectors[l][colInd] > minimalVectors[i][colInd]) {
                    break;
                }
                while (colInd < newVectors[l].size()
                       && newVectors[l][colInd] == minimalVectors[i][colInd]) {
                    ++colInd;
                }
                if (colInd == newVectors[l].size()) {
                    isFound = true;
                    break;
                }
            }
            if (!isFound) {
                minimalVectors.insert(minimalVectors.begin() + i, newVectors[l]);
            }
        }
        std::cout << minimalVectors.size() << " ";
    }
    std::cout << std::endl;
    for (unsigned long long i = 0; i < minimalVectors.size() - 1; ++i) {
        for (unsigned long long j = i + 1; j < minimalVectors.size(); ++j) {
            for (unsigned long long l = j + 1; l < minimalVectors.size(); ++l) {
                for (unsigned long long k = 0; k < minimalVectors[i].size(); ++k) {
                    //if (minimalVectors[i][k] & minimalVectors[j][k]) {
                    if (minimalVectors[i][k] & minimalVectors[j][k] & minimalVectors[l][k]) {
                        ++counter[k];
                    }
                }
            }
        }
    }
    return counter;
}

} //namespace attackSupporters
} // namespace codes