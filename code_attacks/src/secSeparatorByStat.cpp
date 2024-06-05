#include "code_attacks.h"

namespace codes {
namespace attackSupporters {

void printVV(const std::vector<std::vector<unsigned long long>> &v, bool withNums = false) {
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << i << ": ";
        for (size_t j = 0; j < v[i].size(); ++j) {
            if (withNums) {
                std::cout << "[" << i << "," << j << "]:";
            }
            if (j == v[i].size() / 3 * 2 ||
                j == v[i].size() / 3) {
                std::cout << "|";
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
    /*
        for (unsigned long long i = 0; i < matrixVV.size(); ++i) {
            std::cout << algorithms::hammingWeight(matrixVV[i]) << ": ";
            printVector(matrixVV[i]);
        }
    */
    for (unsigned long long i = 0;
         i < matrixVV.size() && algorithms::hammingWeight(matrixVV[i]) <= minWeight;
         ++i) {
        result.push_back(matrixVV[i]);
    }
    if (printData && result.size() >= 2) {
        std::cout << result.size() << " ";
    }
    /*
        if (result.size() != 0) {
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
    */
    return result;
}

std::vector<std::vector<unsigned long long>> 
findingCounter(const codes::Lincode &modRM,
               unsigned long long minWeight,
               unsigned long long iterationNumber,
               bool printData) {
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
        //std::vector<std::vector<char>> codeWords(minWeightWords(modRMMatrix.toVectors(), minWeight, printData));
        //if (codeWords.size() >=2) {
        //    modRMMatrix = matrix::Matrix(codeWords);
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
        //} else {
        //    ++iterationNumber;
        //}
    }
    std::vector<unsigned long long> summary(counter.size());
    for (size_t i = 0; i < counter.size(); ++i) {
        std::vector<unsigned long long> tmp(counter[i]);
        algorithms::sorts::mergeSort(tmp,
                                     [](const unsigned long long &a, const unsigned long long &b)
                                     { return a >= b; });
        for (size_t j = 0; j < counter[i].size(); ++j) {
            if (counter[i][j] != tmp[0] && counter[i][j] != tmp[1]) {
                summary[j] += counter[i][j];
            }
        }

    }
    std::cout << "Sum by all columns:" << std::endl;
    unsigned long long max = 0;
    bool correct = true;
    for (unsigned long long i = 0; i < summary.size(); ++i) {
        if (i != 0 && i % (summary.size() / 3) == 0) {
            std::cout << "|";
        }
        if (i < 2 * summary.size() / 3) {
            if (summary[i] > max) {
                max = summary[i];
            }
        } else {
            if (max > summary[i]) {
                correct = false;
            }
        }
        std::cout << summary[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "Maximum in blocks 1-2 is " << max << std::endl;
    if (correct) {
        std::cout << "Sum correctly separates blocks" << std::endl;
    } else {
        std::cout << "Sum doesn't separates blocks" << std::endl;
    }
    /*
    std::vector<unsigned long long>
    permVec = algorithms::sorts::selectionSort(summary,
                                               [](const unsigned long long &a,
                                                  const unsigned long long &b)
                                               { return a >= b; });
    for (unsigned long long i = 0; i < permVec.size(); ++i) {
        if (i != 0 && i % (permVec.size() / 3) == 0) {
            std::cout << "|";
        }
        std::cout << permVec[i] << " ";
    }
    std::cout << std::endl;
    */
    return counter;
}

std::vector<std::vector<unsigned long long>>
secSeparatorByStat(const codes::Lincode &modRM, unsigned long long minWeight,
                   unsigned long long iterationNumber, bool printData) {
    std::vector<std::vector<unsigned long long>>
    counter(findingCounter(modRM.hull(), minWeight, iterationNumber, printData));

    std::map<unsigned long long, std::vector<unsigned long long>> triples;
    std::vector<unsigned long long> value(counter.size());
    for (unsigned long long i = 0; i < counter.size(); ++i) {
        std::vector<unsigned long long> tmp(counter[i]);
        algorithms::sorts::mergeSort(tmp,
                                     [](const unsigned long long &a, const unsigned long long &b)
                                     { return a >= b; });
        std::cout << tmp[0] << " " << tmp[1] << " " << tmp[2] << "| ";
        unsigned long long max1 = counter.size(), max2 = counter.size();
        size_t cnt = 2;
        for (unsigned long long j = 0; cnt != 0 && j < counter[i].size(); ++j) {
            if (counter[i][j] == tmp[0] && max1 == counter.size()) {
                max1 = j;
                --cnt;
            } else if (counter[i][j] == tmp[1]) {
                max2 = j;
                --cnt;
            }
        }
        std::cout << i << " " << max1 << " " << max2 << "| ";
        unsigned long long min = std::min(std::min(max1, max2), i);
        if (triples.find(min) == triples.end()) {
            triples[min] = {i, max1, max2};
        } else {
            for (size_t j = 0; j < 3; ++j) {
                if (triples[min][j] != i && triples[min][j] != max1
                    && triples[min][j] != max2) {
                    std::cerr << "Mistakes:" << std::endl;
                    std::cerr << triples[min][0] << " " << triples[min][1]
                              << " " << triples[min][2] << std::endl;
                    std::cerr << i << " " << max1 << " " << max2 << std::endl;
                    //throw std::invalid_argument("Found conflict in triples");
                    return {{},{}};
                }

            }
        }
        unsigned long long sum = 0;
        unsigned long long count = counter[i].size() / 3;
        unsigned long long sep = 2;
        for (size_t i = sep; i < sep + count; ++i) {
            sum += tmp[i];
        }
        value[i] = sum / count;
        std::cout << value[i] << std::endl;
    }
    std::vector<unsigned long long>
    permVec = algorithms::sorts::selectionSort(value,
                                               [](const unsigned long long &a,
                                                  const unsigned long long &b)
                                               { return a >= b; });
    unsigned long long cnt = 0;
    std::vector<char> used(permVec.size(), 0);
    std::vector<std::vector<unsigned long long>> twoClasses(2);
    for (unsigned long long i = 0; cnt != permVec.size() / 3 && i < permVec.size(); ++i) {
        if (used[permVec[i]] == 0) {
            // used[i] = 1;
            ++cnt;
            bool found = false;
            for (const auto &elem: triples) {
                for (size_t j = 0; !found && j < elem.second.size(); ++j) {
                    if (elem.second[j] == permVec[i]) {
                        found = true;
                        used[permVec[i]] = 1;
                        twoClasses[1].push_back(permVec[i]);
                    }
                }
                if (found) {
                    for (size_t j = 0; j < elem.second.size(); ++j) {
                        if (elem.second[j] != permVec[i]) {
                            used[elem.second[j]] = 1;
                            twoClasses[0].push_back(elem.second[j]);
                        }
                    }
                    triples.erase(elem.first);
                    break;
                }
            }
            if (!found) {
                throw std::invalid_argument("Some other mistake in triples");
            }
        }
    }
    algorithms::sorts::mergeSort(twoClasses[0],
                                 [](const unsigned long long &a,
                                    const unsigned long long &b)
                                 { return a <= b; });
    algorithms::sorts::mergeSort(twoClasses[1],
                                 [](const unsigned long long &a,
                                    const unsigned long long &b)
                                 { return a <= b; });
    return twoClasses;
}

} //namespace attackSupporters
} // namespace codes