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

void testRunner(matrix::Matrix rmCode,
                unsigned long long k1, unsigned long long k2,
                unsigned long long minWeight, bool mode) {
    std::cout << "START testRunner" << std::endl;
    std::cout << k1 << " " << k2 << " " << rmCode.rows() << std::endl;
    if (mode) {
        std::cout << "sublock with RM(r, m) and RM(r-1, m)^s2:" << std::endl;
        rmCode.printVisualMatrix(3);
    }

    std::cout << "START separatingGaussElimination" << std::endl;
    std::vector<std::vector<unsigned long long>>
    result(codes::attackSupporters::separatingGaussElimination(rmCode, k1, k2));
    std::cout << "END separatingGaussElimination" << std::endl;

    if (mode) {
        std::cout << "Sublock:" << std::endl;
        rmCode.printVisualMatrix(3, true);
    }
    std::cout << "First block columns [" << result[0].size() << "]:" << std::endl;
    printVector(result[0]);
    std::cout << "First block rows [" << result[1].size() << "]:" << std::endl;
    printVector(result[1]);
    std::cout << "Second block columns [" << result[2].size() << "]:" << std::endl;
    printVector(result[2]);
    std::cout << "Second block rows [" << result[3].size() << "]:" << std::endl;
    printVector(result[3]);
    std::cout << "Last block columns [" << result[4].size() << "]:" << std::endl;
    printVector(result[4]);
    std::cout << "Last block rows [" << result[5].size() << "]:" << std::endl;
    printVector(result[5]);
    std::vector<unsigned long long> lastblockCols(result[4]);
    std::cout << "Last block columns [" << lastblockCols.size() << "]:" << std::endl;
    printVector(lastblockCols);

    std::vector<std::vector<char>> matrixVV = rmCode.toVectors();
    algorithms::sorts::mergeSort(matrixVV, [](const std::vector<char> &row1,
                                              const std::vector<char> &row2)
                                           { return algorithms::hammingWeight(row1)
                                                    <= algorithms::hammingWeight(row2); });
    rmCode = matrix::Matrix(matrixVV);
    std::vector<std::vector<unsigned long long>> columnsBack;
    std::map<unsigned long long, size_t> interCnt;
    std::vector<std::vector<unsigned long long>> minWeightWordsSupports;
    size_t i = 0;
    for (;
         i < matrixVV.size() && algorithms::hammingWeight(matrixVV[i]) < 2 * minWeight;
         ++i) {
        std::vector<unsigned long long> columns;
        std::cout << "Columns: ";
        for (size_t j = 0; j < matrixVV[i].size(); ++j) {
            if (matrixVV[i][j] == 1) {
                columns.push_back(j);
                std::cout << j << " ";
                bool found = false;
                for (size_t l = 0; !found && l < columnsBack.size(); ++l) {
                    for (size_t k = 0; !found && k < columnsBack[l].size(); ++k) {
                        if (j == columnsBack[l][k]) {
                            if (interCnt.find(j) != interCnt.end()) {
                                ++interCnt[j];
                            } else {
                                interCnt[j] = 1;
                            }
                            found = true;
                        }
                    }
                }
            }
        }
        std::cout << "[" << columns.size() << "]" << std::endl;
        columnsBack.push_back(columns);

        if (algorithms::hammingWeight(matrixVV[i]) == minWeight) {
            minWeightWordsSupports.push_back(columns);
        }
    }
    std::cout << "Found " << i << " columns." << std::endl;
    std::cout << "Intersection of min vectors:" << std::endl;
    std::cout << "[" << interCnt.size() << "]: ";
    std::vector<std::pair<unsigned long long, size_t>> interPair;
    for (auto &elem : interCnt){
        std::cout << elem.first << "(" << elem.second << ") ";
        bool isInserted = false;
        // Inserting by order
        for (auto iter = interPair.begin(); !isInserted && iter != interPair.end(); ++iter) {
            if (elem.second < iter->second) {
                interPair.insert(iter, std::make_pair(elem.first, elem.second));
                isInserted = true;
            }
        }
        if (!isInserted) {
            interPair.push_back(std::make_pair(elem.first, elem.second));
        }
    }
    std::cout << std::endl;
    for (size_t j = 0; j < interPair.size(); ++j){
        if (j != 0 && j % (interPair.size() / 4) == 0) {
            std::cout << std::endl;
        }
        std::cout << interPair[j].first << "(" << interPair[j].second << ") ";
    }
    std::cout << std::endl;


    std::cout << "START separatingGaussElimination" << std::endl;
    result = codes::attackSupporters::splittingToClassesByWords(minWeightWordsSupports,
                                                                interCnt, minWeight);
    std::cout << "END separatingGaussElimination" << std::endl;
    std::cout << "END testRunner" << std::endl;
}

int main(int argc, char *argv[]) {
    size_t r = 2, m = 7;
    size_t permMode = 0;
    bool mode = true;
    size_t iterNum = 1;
    if (argc < 3) {
        std::cout << "Input args in format: r m permMode mode iterNum" << std::endl;
        std::cout << "Where permMode=0 -- *= P and permMode=1 --default" << std::endl;
        std::cout << "Where mode=1 -- print visual, mode=0 -- print sizes" << std::endl;
        std::cout << "Shortcuts:" << std::endl;
        std::cout << "By 2: r m " << permMode << " " << mode << " " << iterNum << std::endl;
        std::cout << "By 3: r m permMode" << mode << " " << iterNum << std::endl;
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
    for (size_t i = 0; i < iterNum; ++i) {
        testRunner(matrixGenerator(r, m, permMode),
                   codes::codeSizeFromRM(r, m), codes::codeSizeFromRM(r - 1, m), 1 << (m - (r - 1)), mode);
    }
    return 0;
}