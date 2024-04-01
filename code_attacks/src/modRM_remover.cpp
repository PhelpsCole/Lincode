#include "code_attacks.h"
#include <fstream>
#include <chrono>
#include <ctime>

namespace codes {
namespace attackSupporters {
namespace attackTestSupporters {
void printSSAStructure(const codes::SSAStructure &s,
                       const std::string &filename) {
    std::ofstream out;
    out.open(filename);
    if (out.is_open()) {
        for (size_t i = 0; i != s.size(); ++i) {
            out << i << "(" << s[i].size() << "): ";
            for (size_t j = 0; j != s[i].size(); ++j) {
                out << " [";
                for (size_t k = 0; k != s[i][j].first.size(); ++k) {
                    out << s[i][j].first[k] << ", ";
                }
                out << "]:" << s[i][j].second;
            }
            out << std::endl;
        }
    }
    out.close();
}

std::string nameFile(size_t r, size_t m, std::string invarANDpreprocName) {
    std::string res = "modRM_(" + std::to_string(r) + ","
                      + std::to_string(m) + ")_" + invarANDpreprocName;
    return res;
}

} // namespace attackTestSupporters

bool check_signature(const codes::SSAStructure &s, size_t m,
                     std::vector<unsigned long long> &blockColumns,
                     bool returnMax) {
    std::map<std::string, std::pair<size_t, std::vector<unsigned long long>>> counter;
    for (size_t i = 0; i != s.size(); ++i) {
        // In pqsigRM no refinders, so s[i].size() == 1
        for (size_t j = 0; j != s[i].size(); ++j) {
            std::string tmp = s[i][j].second;
            if (counter.find(tmp) == counter.end()) {
                // In pqsigRM no refinders, so s[i][j].first.size() == 1
                counter[tmp]
                    = std::make_pair(1, std::vector<unsigned long long>({s[i][j].first[0]}));
            } else {
                ++counter[tmp].first;
                counter[tmp].second.push_back(s[i][j].first[0]);
            }
        }
    }
    std::vector<std::pair<size_t, std::vector<unsigned long long>>> sizes;
    for (auto iter = counter.begin(); iter != counter.end(); ++iter) {
        sizes.push_back(iter->second);
    }
    // Separated to 2^(m -2) and 2^m - 2^(m -2)
    size_t len = 1 << (m - 2);
    if (sizes.size() == 2) {
        if (sizes[0].first == len) {
            if (returnMax) {
                blockColumns = sizes[1].second;
            } else {
                blockColumns = sizes[0].second;
            }
            return true;
        } else if (sizes[1].first == len) {
            if (returnMax) {
                blockColumns = sizes[0].second;
            } else {
                blockColumns = sizes[1].second;
            }
            return true;
        }
    }
    return false;
}

// Support splitting algorithm on hadPower of modRM
std::vector<unsigned long long> findingBlockColumns(codes::Lincode pqsigRMcode,
                                                    size_t r, size_t m,
                                                    bool returnMax,
                                                    bool testRun) {
    std::string filename;
    codes::Lincode startCode;
    if (testRun) {
        startCode = pqsigRMcode;
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        filename = attackTestSupporters::nameFile(r, m, "invariantSize_iterativeMaxRM");
        std::cout << "Processing " << filename << "..." << std::endl;
        std::cout << "Started computation at " << std::ctime(&time) << std::endl;

    }
    // qRM(r, m-2) step
    size_t q = (m - 2) / r;
    std::string symb = std::to_string(q) + "|";
    if ((m - 2) % r == 0) {
        --q;
    }
    pqsigRMcode = codes::hadPower(pqsigRMcode, q);
    codes::SSAStructure s = codes::ssaStructure(pqsigRMcode, INVARIANT_SIZE_ID, PREPROC_SIMPLE_ID);
    if (testRun) {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Completed computation of " << symb
                  << " at " << std::ctime(&time) << std::endl;
    }
    std::vector<unsigned long long> blockColumns;
    if (check_signature(s, m, blockColumns, returnMax)) {
        if (testRun) {
            std::string tempFilename = filename + '_' + symb + "_found.txt";
            attackTestSupporters::printSSAStructure(s, tempFilename);
        }
        return blockColumns;
    }
    if (q * r != (m - 2) - 1) {
        // dual(qmodRM(r, m-2)) step
        symb += "-1|";
        pqsigRMcode.dual();

        // max(modRM(r, m-2)) step
        r = (m - 2) - q * r - 1;
        q = (m - 2) / r;
        symb += std::to_string(q) + "|";
        pqsigRMcode = codes::hadPower(pqsigRMcode, q);
        s = codes::ssaStructure(pqsigRMcode, INVARIANT_SIZE_ID, PREPROC_SIMPLE_ID);
        if (testRun) {
            auto now = std::chrono::system_clock::now();
            std::time_t time = std::chrono::system_clock::to_time_t(now);
            std::cout << "Completed computation of " << symb
                      << " at " << std::ctime(&time) << std::endl;
        }
        if (check_signature(s, m, blockColumns, returnMax)) {
            if (testRun) {
                std::string tempFilename = filename + '_' + symb + "_found.txt";
                attackTestSupporters::printSSAStructure(s, tempFilename);
            }
            return blockColumns;
        }
    }
    if (testRun) {
        std::string tempFilename = filename + '_' + symb + "_.txt";
        std::string unFoundMatrix = filename + '_' + symb + "_matrix.txt";
        attackTestSupporters::printSSAStructure(s, tempFilename);
        startCode.printCode(unFoundMatrix);
    }
    return blockColumns;
}

matrix::Matrix movingBlockPerm(std::vector<unsigned long long> &columns,
                               unsigned long long matrSize,
                               bool ordered=false) {
    matrix::Matrix P(matrSize, matrSize);
    if (!ordered) {
        algorithms::sorts::mergeSort(columns,
                                     [] (const unsigned long long &a,
                                         const unsigned long long &b)
                                     { return a <= b; });
    }
    size_t delta = 0;
    for (size_t i = 0; i < P.cols(); ++i) {
        if (i == columns[delta]) {
            P.at(i, delta++) = 1;
        } else {
            P.at(i, columns.size() + i - delta) = 1;
        }
    }
    return P;
}

// Finding sigma_1 and removing block without first row
// Returning pair (otherBlocks, sigma_1P_1)
codes::attackSupporters::ExtractBlockResult
extractFirstBlock(const matrix::Matrix &afterSSA,
                  bool findSigma,
                  unsigned long long blockRowsSize) {
    matrix::Matrix block(afterSSA);
    std::vector<unsigned long long> blockColumns(afterSSA.cols() >> 2);
    std::iota(blockColumns.begin(), blockColumns.end(), 0);
    std::vector<unsigned long long> blockRows(block.gaussElimination(true, blockColumns).size());
    if (blockRowsSize != 0 && blockRowsSize != blockRows.size()) {
        std::cerr << "Extracted block has wrong size:" << std::endl;
        std::cerr << "needed=" << blockRowsSize << ", counted=" << blockRows.size() << std::endl;
        return codes::attackSupporters::ExtractBlockResult({matrix::Matrix(0, 0),
                                                            matrix::Matrix(0, 0)});
    }
    std::iota(blockRows.begin(), blockRows.end(), 0);

    // RM(r,m-2)^sigma_1*P_1'
    matrix::Matrix RMBlock = block.submatrix(blockRows, blockColumns);
    // Finding sigma_1
    matrix::Matrix sigma_1P_1(0, 0);
    if (findSigma) {
        sigma_1P_1 = codes::chizhov_borodin(codes::Lincode(RMBlock));
    }

    // Removing other rows without zeroes in first row
    blockRows.resize(afterSSA.rows() - blockRows.size());
    std::iota(blockRows.begin(), blockRows.end(), afterSSA.rows() - blockRows.size());
    std::vector<unsigned long long> otherCols(afterSSA.cols() - blockColumns.size());
    std::iota(otherCols.begin(), otherCols.end(), blockColumns.size());
    return codes::attackSupporters::ExtractBlockResult({block.submatrix(blockRows, otherCols),
                                                        sigma_1P_1});
}

matrix::Matrix
separatorToLastBlocks(const std::vector<std::vector<unsigned long long>> &columnSets,
                      const matrix::Matrix &otherBlocks, unsigned long long codeSize,
                      unsigned long long matrSize) {
    std::vector<unsigned long long> rows(otherBlocks.rows());
    std::iota(rows.begin(), rows.end(), 0);
    size_t block4 = columnSets.size();

    //std::vector<unsigned long long> columns(otherBlocks.cols() / 3);
    //std::iota(columns.begin(), columns.end(), 0);
    std::vector<unsigned long long> columns;

    for (size_t i = 0; i < columnSets.size(); ++i) {
        if (block4 == columnSets.size()) {
            matrix::Matrix temp(otherBlocks.submatrix(rows, columnSets[i]));
            if (temp.gaussElimination(true).size() != codeSize) {
                block4 = i;
                continue;
            }
        }
        columns.insert(columns.end(), columnSets[i].begin(), columnSets[i].end());
    }
    columns.insert(columns.end(), columnSets[block4].begin(), columnSets[block4].end());
    return movingBlockPerm(columns, matrSize, true);
}

// Finding sigma_2 and removing block without first row
// Returning pair (concatenatedRM, sigma_2P_4)
codes::attackSupporters::ExtractBlockResult
extractLastBlock(const matrix::Matrix &afterSSA,
                 bool findSigma,
                 unsigned long long blockRowsSize) {
    matrix::Matrix block(afterSSA);
    std::vector<unsigned long long> blockColumns(afterSSA.cols() / 3 * 2);
    std::iota(blockColumns.begin(), blockColumns.end(), 0);
    block.gaussElimination(true, blockColumns);

    std::vector<unsigned long long> blockRows(blockRowsSize);
    std::iota(blockRows.begin(), blockRows.end(), 0);

    // (r-1,m-2)|0
    // 0|(r-1,m-2)
    matrix::Matrix concatenatedRM = block.submatrix(blockRows, blockColumns);

    // Finding sigma_2P_2
    blockRows.resize(afterSSA.rows() - blockRows.size());
    std::iota(blockRows.begin(), blockRows.end(), afterSSA.rows() - blockRows.size());
    blockColumns.resize(afterSSA.cols() - blockColumns.size());
    std::iota(blockColumns.begin(), blockColumns.end(), afterSSA.cols() - blockColumns.size());

    matrix::Matrix sigma_2P_2(0, 0);
    if (findSigma) {
        sigma_2P_2 = codes::chizhov_borodin(block.submatrix(blockRows, blockColumns));
    }
    return codes::attackSupporters::ExtractBlockResult({concatenatedRM, sigma_2P_2});
}

matrix::Matrix findRMmiddle(const matrix::Matrix &concRM) {
    std::vector<unsigned long long> rows(concRM.rows() / 2);
    std::iota(rows.begin(), rows.end(), 0);
    std::vector<unsigned long long> cols(concRM.cols() / 2);
    std::iota(cols.begin(), cols.end(), 0);
    matrix::Matrix first(codes::chizhov_borodin(concRM.submatrix(rows, cols)));
    std::iota(rows.begin(), rows.end(), rows.size());
    std::iota(cols.begin(), cols.end(), cols.size());
    return matrix::blockDiag({first, codes::chizhov_borodin(concRM.submatrix(rows, cols))});
}

} // namespace attackSupporters

// Attack for m >= 8 and 2 <= r < (m - 2) / 2
// Returns permutation matrix, sigma_1 and sigma_2
std::vector<matrix::Matrix>
modRM_remover(const codes::Lincode &modRM, bool testRun,
              bool findSigmas, std::vector<size_t> rmSizes) {
    if (testRun) {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Started computation at " << std::ctime(&time) << std::endl;
    }
    if (rmSizes.size() == 0) {
        rmSizes = codes::rmSizes(modRM);
    }
    if (rmSizes[1] < 8 || rmSizes[0] < 2 || 2 * rmSizes[0] >= rmSizes[1] - 2) {
        std::cerr << "Unable to analyse in case of r=" << rmSizes[0];
        std::cerr << " and m=" << rmSizes[1] << std::endl;
        return std::vector<matrix::Matrix>();
    }

    // Step 1: Finding first block columns with RM(r,m-2)^sigma_1
    // Works on 2r < m - 2
    std::vector<unsigned long long>
    firstBlockColumns(attackSupporters::findingBlockColumns(modRM, rmSizes[0], rmSizes[1]));

    if (testRun) {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Completed computation of " << "Step 1: firstBlockColumns"
                  << " at " << std::ctime(&time) << std::endl;
    }
    if (firstBlockColumns.size() != 1 << (rmSizes[1] - 2)) {
        std::cerr << "Block wasn't separated" << std::endl;
        std::cerr << firstBlockColumns.size() << std::endl;
        return std::vector<matrix::Matrix>();
    }

    // Step 2: Move first block columns to begining
    matrix::Matrix P1_1 = attackSupporters::movingBlockPerm(firstBlockColumns, modRM.len());
    // SSA-1|2-3-4
    matrix::Matrix afterSSA(modRM.toMatrix() * P1_1);

    if (testRun) {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Completed computation of " << "Step 2: movingBlockPerm"
                  << " at " << std::ctime(&time) << std::endl;
    }

    // Step 3: Separating first block from other matrix
    //         Returns sigma_1 * P_1' and 2-3-4 blocks without zeroes and first row
    codes::attackSupporters::ExtractBlockResult excratingResult
        = attackSupporters::extractFirstBlock(afterSSA, findSigmas);
    matrix::Matrix otherBlocks(excratingResult.block);
    matrix::Matrix sigma_1P_1(excratingResult.sigmaP);

    if (testRun) {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Completed computation of " << "Step 3: extractFirstBlock"
                  << " at " << std::ctime(&time) << std::endl;
    }

    // Step 4: Separating (r,m)x(r,m)xC construction in had square of matrix:
    // (r-1,m-2)|    0    |     (r-1,m-2)
    //     0    |(r-1,m-2)|     (r-1,m-2)
    //     0    |    0    |(r-2,m-2)^\sigma_2
    // To column sets
    // Works on 2r - 2 < m - 2
    std::vector<std::vector<unsigned long long>>
    columnSets(separateConcatenatedCodeToSets(matrix::hadPower(otherBlocks, 2)));

    if (testRun) {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Completed computation of " << "Step 4: separateConcatenatedCodeToSets"
                  << " at " << std::ctime(&time) << std::endl;
    }

    // Step 5: Finding 4 block set and permutation which separates and sorts 2|3|4 blocks:
    unsigned long long k2 = codes::codeSizeFromRM(rmSizes[0] - 1, rmSizes[0] - 2);
    matrix::Matrix P234_sub(attackSupporters::
                            separatorToLastBlocks(columnSets, otherBlocks, k2,
                                                  otherBlocks.cols()));
    otherBlocks *= P234_sub;
    matrix::Matrix P234(matrix::blockDiag({matrix::diag(firstBlockColumns.size(), 1),
                                           P234_sub}));

    if (testRun) {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Completed computation of " << "Step 5: separatorToLastBlocks"
                  << " at " << std::ctime(&time) << std::endl;
    }

    // Step 6: Separating 2-3 blocks from other matrix
    //         Returns sigma_2 * P_4' and 2-3 blocks without zeroes
    excratingResult = attackSupporters::extractLastBlock(otherBlocks, findSigmas, 2 * k2);
    matrix::Matrix sigma_2P_4(excratingResult.sigmaP);
    otherBlocks = excratingResult.block;

    if (testRun) {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Completed computation of " << "Step 6: extractLastBlock"
                  << " at " << std::ctime(&time) << std::endl;
     }

    // Step 7: Find perm on 2 and 3 blocks
    //         otherBlocks needs to be sorted
    matrix::Matrix P23(matrix::blockDiag({matrix::diag(firstBlockColumns.size(), 1),
                                          attackSupporters::findRMmiddle(otherBlocks),
                                          matrix::diag(firstBlockColumns.size(), 1)}));

    if (testRun) {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::cout << "Completed computation of " << "Step 7: findRMmiddle"
                  << " at " << std::ctime(&time) << std::endl;
     }

    // Known blocks 2-3 and partly found blocks 1 and 4
    std::vector<matrix::Matrix> ans;
    ans.push_back(P1_1 * P234 * P23);
    ans.push_back(sigma_1P_1);
    ans.push_back(sigma_2P_4);
    return ans;
}

} // namespace codes
