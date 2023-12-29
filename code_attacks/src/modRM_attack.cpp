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
                     std::vector<unsigned long long> &blockColumns) {
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
            blockColumns = sizes[0].second;
            return true;
        } else if (sizes[1].first == len) {
            blockColumns = sizes[1].second;
            return true;
        }
    }
    return false;
}

// Support splitting algorithm on hadPower of modRM
std::vector<unsigned long long> findingFirstBlockColumns(codes::Lincode pqsigRMcode,
                                                         bool testRun) {
    std::string filename;
    codes::Lincode startCode;
    std::vector<size_t> rmSizes = codes::rmSizes(pqsigRMcode);
    if (testRun) {
        startCode = pqsigRMcode;
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        filename = attackTestSupporters::nameFile(rmSizes[0], rmSizes[1], "invariantSize_iterativeMaxRM");
        std::cout << "Processing " << filename << "..." << std::endl;
        std::cout << "Started computation at " << std::ctime(&time) << std::endl;

    }
    // qRM(r, m-2) step
    size_t q = (rmSizes[1] - 2) / rmSizes[0];
    std::string symb = std::to_string(q) + "|";
    if ((rmSizes[1] - 2) % rmSizes[0] == 0) {
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
    if (check_signature(s, rmSizes[1], blockColumns)) {
        if (testRun) {
            std::string tempFilename = filename + '_' + symb + "_found.txt";
            attackTestSupporters::printSSAStructure(s, tempFilename);
        }
        return blockColumns;
    }
    if (q * rmSizes[0] != (rmSizes[1] - 2) - 1) {
        // dual(qmodRM(r, m-2)) step
        symb += "-1|";
        pqsigRMcode.dual();

        // max(modRM(r, m-2)) step
        rmSizes[0] = (rmSizes[1] - 2) - q * rmSizes[0] - 1;
        q = (rmSizes[1] - 2) / rmSizes[0];
        symb += std::to_string(q) + "|";
        pqsigRMcode = codes::hadPower(pqsigRMcode, q);
        s = codes::ssaStructure(pqsigRMcode, INVARIANT_SIZE_ID, PREPROC_SIMPLE_ID);
        if (testRun) {
            auto now = std::chrono::system_clock::now();
            std::time_t time = std::chrono::system_clock::to_time_t(now);
            std::cout << "Completed computation of " << symb
                      << " at " << std::ctime(&time) << std::endl;
        }
        if (check_signature(s, rmSizes[1], blockColumns)) {
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

// Finding sigma_1 and from matrix
// (RM(r,m-2)^sigma_1|RM(r,m-2)^sigma_1|RM(r,m-2)^sigma_1|RM(r,m-2)^sigma_1) * P
// Removing sigma_1 and writing into firstBlock
// blockRowsSize is for checking subBlock size (is it (r,m-2)-code or not)
// Returning pair (firstBlock, sigma_1)
std::pair<matrix::Matrix, matrix::Matrix>
extractFirstBlock(const matrix::Matrix &modRMMatrix,
                  std::vector<unsigned long long> blockColumns,
                  unsigned long long blockRowsSize) {
    matrix::Matrix block(modRMMatrix);
    std::vector<unsigned long long> blockRows(block.gaussElimination(true, blockColumns).size());
    if (blockRowsSize != 0 && blockRowsSize != blockRows.size()) {
        std::cerr << "Extracted block has wrong size:" << std::endl;
        std::cerr << "needed=" << blockRowsSize << ", counted=" << blockRows.size() << std::endl;
        return std::make_pair(matrix::Matrix(0, 0), matrix::Matrix(0, 0));
    }
    std::iota(blockRows.begin(), blockRows.end(), 0);
    std::vector<unsigned long long> allColumns(modRMMatrix.cols());
    std::iota(allColumns.begin(), allColumns.end(), 0);
    // (RM(r,m-2)^sigma_1||4) * P
    matrix::Matrix firstBlock = block.submatrix(blockRows, allColumns);

    // RM(r,m-2)^sigma_1
    block = block.submatrix(blockRows, blockColumns);
    // Finding sigma_1
    matrix::Matrix sigma_1 = codes::chizhov_borodin(codes::Lincode(block));

    // Removing sigma_1 from firstBlock
    matrix::Matrix O(sigma_1.rows(), sigma_1.cols());
    matrix::Matrix row1(sigma_1);
    matrix::Matrix row2(O);
    // O|P
    row2.concatenateByRows(sigma_1);
    // P|O
    row1.concatenateByRows(O);
    // O|O
    O.concatenateByRows(O);
    matrix::Matrix row3(O);
    // O|O|P|O
    row3.concatenateByRows(row1);
    matrix::Matrix row4(O);
    // O|O|O|P
    row4.concatenateByRows(row2);
    // P|O|O|O
    row1.concatenateByRows(O);
    // O|P|O|O
    row2.concatenateByRows(O);

    row1.concatenateByColumns(row2);
    row1.concatenateByColumns(row3);
    row1.concatenateByColumns(row4);
    // A|A|A|A * blockDiag(P, 4)
    firstBlock *= row1;
    return std::make_pair(firstBlock, sigma_1);
}

} // namespace attackSupporters

// Attack for m >= 8 and 2 <= r < (m - 2) / 2
// Returns secret key
matrix::Matrix modRM_attack(const codes::Lincode &modRM) {
    std::vector<size_t> rmSizes = codes::rmSizes(modRM);
    if (rmSizes[1] < 8 || rmSizes[0] < 2 || 2 * rmSizes[0] >= rmSizes[1] - 2) {
        std::cerr << "Unable to analyse in case of r=" << rmSizes[0];
        std::cerr << " and m=" << rmSizes[1] << std::endl;
        return modRM.toMatrix();
    }

    // Step 1: Finding first block columns with RM(r,m-2)^sigma_1
    std::vector<unsigned long long>
    blockColumns(attackSupporters::findingFirstBlockColumns(modRM));

    // Step 2: Separating first block
    //         Returns (RM(r,m-2)|RM(r,m-2)|RM(r,m-2)|RM(r,m-2)) * P and sigma_1
    std::pair<matrix::Matrix, matrix::Matrix>
    excratingResult = attackSupporters::extractFirstBlock(modRM.toMatrix(), blockColumns);
    matrix::Matrix firstBlock(excratingResult.first);
    matrix::Matrix sigma_1(excratingResult.second);


    // Step 3: Removing P from firstBlock by attacking concatenated code

    // Step 4: modRM * P and ^sigma_1 on subblocks

    // Step 5: Finding sigma_2 and returning secret key
    return firstBlock;
}

} // namespace codes