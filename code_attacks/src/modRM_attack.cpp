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

std::vector<unsigned long long> findingFirstBlock(codes::Lincode pqsigRMcode,
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
    if (testRun) {
        std::string tempFilename = filename + '_' + symb + "_.txt";
        std::string unFoundMatrix = filename + '_' + symb + "_matrix.txt";
        attackTestSupporters::printSSAStructure(s, tempFilename);
        startCode.printCode(unFoundMatrix);
    }
    return blockColumns;
}

matrix::Matrix permOfExtraction(const matrix::Matrix &modRMMatrix,
                                const std::vector<unsigned long long> &blockColumns) {
    matrix::Matrix block(modRMMatrix);
    std::vector<unsigned long long> blockRows(block.gaussElimination(true, blockColumns));
    std::cout << blockRows.size() << " " << codes::codeSizeFromRM(2, 7) << std::endl;
    std::cout << "blockRows:" << std::endl;
    for (size_t i = 0; i < blockRows.size(); ++i) {
        std::cout << blockRows[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "blockColumns:" << std::endl;
    for (size_t i = 0; i < blockColumns.size(); ++i) {
        std::cout << blockColumns[i] << " ";
    }
    std::cout << std::endl;
    block.printVisualMatrix(4);
    block = block.submatrix(blockRows, blockColumns);
    std::cout << block.rank() << std::endl;
    std::cout << block.rows() << " " << block.cols() << std::endl;
    block.printVisualMatrix();
    block = modRMMatrix.submatrix(blockRows, blockColumns);
    block.convertToBasis();
    std::cout << block.rank() << std::endl;
    std::cout << block.rows() << " " << block.cols() << std::endl;
    block.printVisualMatrix();
    return matrix::Matrix(0, 0);
    matrix::Matrix P(codes::chizhov_borodin(codes::Lincode(block)));

    // Removing sigma_1 from modRM
    matrix::Matrix I(matrix::diag(P.rows(), 1));
    P.concatenateByRows(I);
    P.concatenateByRows(I);
    // (P|I|I|I)
    P.concatenateByRows(I);
    I = P;
    P.concatenateByColumns(I);
    P.concatenateByColumns(I);
    P.concatenateByColumns(I);
    return P;
}

} // namespace attackSupporters

// Attack for m >= 8 and 2 <= r < (m - 2) / 2
matrix::Matrix modRM_attack(const codes::Lincode &modRM) {
    // Step 1: Separating first block with RM(r,m-2)^sigma_1
    std::vector<unsigned long long> blockColumns(attackSupporters::findingFirstBlock(modRM));

    // Step 2: Finding sigma_1 permutation and removing it
    matrix::Matrix P = attackSupporters::permOfExtraction(modRM.toMatrix(), blockColumns);
    matrix::Matrix block = modRM.toMatrix();
    block *= P;

    // Step 3: Separating last block with RM(r-2,m-2)^sigma_2
    
    return P;
}

} // namespace codes