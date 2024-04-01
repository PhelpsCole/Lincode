#include "code_attacks.h"

namespace codes {
namespace attackSupporters {

    bool equal(const std::vector<char> &v1, const std::vector<char> &v2) {
        if (v1.size() != v2.size()) {
            return false;
        }
        for (unsigned long long i = 0; i < v1.size(); ++i) {
            if (v1[i] != v2[i]) {
                return false;
            }
        }
        return true;
    }

    bool finder(std::vector<char> &usedRows,
                const std::vector<char> &first,
                const std::vector<char> &second,
                const matrix::Matrix &B,
                unsigned long long ind) {
        if (ind == usedRows.size()) {
            std::vector<char> ans(second);
            for (unsigned long long i = 0; i < usedRows.size(); ++i) {
                if (usedRows[i] == 1) {
                    std::vector<char> row(B.row(i));
                    for (unsigned long long j = 0; j < ans.size(); ++j) {
                        ans[j] ^= row[j];
                    }
                }
            }
            return equal(first, ans);
        }
        if (finder(usedRows, first, second, B, ind + 1)) {
            return true;
        }
        usedRows[ind] = 1;
        return finder(usedRows, first, second, B, ind + 1);
    }

    // Input matr: A|A'
    //             0|B
    // Return matr: A|A
    //              0|B
    void clearFirstRow(matrix::Matrix &blockTriangle, unsigned long long k1) {
        matrix::Matrix triangle(blockTriangle);
        // I|A1|A2|I|A1'|A2'
        // 0|0 |0 |0|I'|B
        triangle.gaussElimination();
        for (unsigned long long i = 0; i < k1; ++i) {
            std::vector<char> first(triangle.row(i));
            std::vector<char> second;
            auto middle = first.begin();
            std::advance(middle, first.size() / 2);
            second.insert(second.end(), middle, first.end());
            first.resize(first.size() / 2);

            std::vector<char> usedRows(triangle.rows() - k1);
            std::vector<unsigned long long> rows(triangle.rows() - k1);
            std::iota(rows.begin(), rows.end(), triangle.rows() - k1);
            std::vector<unsigned long long> cols(triangle.cols() / 2);
            std::iota(cols.begin(), cols.end(), cols.size() / 2);
            if (!finder(usedRows, first, second, triangle.submatrix(rows, cols), 0)) {
                std::cerr << "Block wasn't found" << std::endl;
            }
            for (unsigned long long j = 0; j < usedRows.size(); ++j) {
                if (usedRows[j] == 1) {
                    blockTriangle.addRow(i, triangle.row(k1 + j));
                }
            }
        }
    }

    void clearRows23(matrix::Matrix &modRMMatrix,
                      unsigned long long k1,
                      unsigned long long k2,
                      bool testRun) {
        std::vector<unsigned long long> rows(modRMMatrix.rows() - k1 - k2);
        std::iota(rows.begin(), rows.end(), k1 + k2);
        std::vector<unsigned long long> cols(modRMMatrix.cols() / 2);
        std::iota(cols.begin(), cols.end(), cols.size());
        matrix::Matrix sub34(modRMMatrix.submatrix(rows, cols));
        // (3, 4)
        clearFirstRow(sub34, k2);
        if (testRun) {
            std::cerr << "Cleared (3,4) subblock" << std::endl;
        }

        auto midRow = rows.begin();
        std::advance(midRow, k2);
        auto midCol = cols.begin();
        std::advance(midCol, cols.size() / 2);
        std::iota(rows.begin(), midRow, k1);
        std::iota(cols.begin(), midCol, cols.size() / 2);
        matrix::Matrix sub24(modRMMatrix.submatrix(rows, cols));
        // (2, 4)
        clearFirstRow(sub24, k2);
        if (testRun) {
            std::cerr << "Cleared (2,4) subblock" << std::endl;
        }
//FIX HERE!!!! SOMETHING WENT WRONG!
        modRMMatrix.copyToSubMatrix(k1, modRMMatrix.cols() / 4 * 3,
                                    sub24, 0, sub24.cols() / 2,
                                    k2, modRMMatrix.cols() / 4);
        modRMMatrix.copyToSubMatrix(k1 + k2, modRMMatrix.cols() / 4 * 3,
                                    sub34, 0, sub34.cols() / 2,
                                    k2, modRMMatrix.cols() / 4);
    }

    void clearRow1(matrix::Matrix &modRMMatrix,
                      unsigned long long k1,
                      unsigned long long k2,
                      bool testRun) {
        std::vector<unsigned long long> rows(k1 + k2);
        std::iota(rows.begin(), rows.end(), 0);
        std::vector<unsigned long long> cols(modRMMatrix.cols() / 2);
        std::iota(cols.begin(), cols.end(), 0);
        matrix::Matrix sub12(modRMMatrix.submatrix(rows, cols));
        //(1, 2)
        clearFirstRow(sub12, k1);
        if (testRun) {
            std::cerr << "Cleared (1,2) subblock" << std::endl;
        }

        auto midRow = rows.begin();
        std::advance(midRow, k1);
        auto midCol = cols.begin();
        std::advance(midCol, cols.size() / 2);
        std::iota(midRow, rows.end(), k1 + k2);
        std::iota(midCol, cols.end(), cols.size());
        matrix::Matrix sub13(modRMMatrix.submatrix(rows, cols));
        //(1, 3)
        clearFirstRow(sub13, k1);
        if (testRun) {
            std::cerr << "Cleared (1,3) subblock" << std::endl;
        }

        rows.resize(modRMMatrix.rows());
        std::iota(rows.begin(), rows.end(), 0);
        cols.resize(modRMMatrix.cols() / 2);
        midCol = cols.begin();
        std::advance(midCol, cols.size() / 2);
        std::iota(midCol, cols.end(), modRMMatrix.cols() / 4 * 3);
        matrix::Matrix sub14(modRMMatrix.submatrix(rows, cols));
        //(1, 4)
        clearFirstRow(sub14, k1);
        if (testRun) {
            std::cerr << "Cleared (1,4) subblock" << std::endl;
        }

        modRMMatrix.copyToSubMatrix(0, modRMMatrix.cols() / 4,
                                    sub12, 0, sub12.cols() / 2,
                                    k1, modRMMatrix.cols() / 4);
        modRMMatrix.copyToSubMatrix(0, modRMMatrix.cols() / 4 * 2,
                                    sub13, 0, sub13.cols() / 2,
                                    k1, modRMMatrix.cols() / 4);
        modRMMatrix.copyToSubMatrix(0, modRMMatrix.cols() / 4 * 3,
                                    sub14, 0, sub14.cols() / 2,
                                    k1, modRMMatrix.cols() / 4);
    }

} // namespace attackSupporters

codes::Lincode modRM_attack(const codes::Lincode &modRM, bool testRun, bool findSigmas) {
    std::vector<size_t> rmSizes = codes::rmSizes(modRM);
    unsigned long long k1 = codes::codeSizeFromRM(rmSizes[0], rmSizes[1] - 2);
    unsigned long long k2 = codes::codeSizeFromRM(rmSizes[0] - 1, rmSizes[1] - 2);
    std::vector<matrix::Matrix> res(modRM_remover(modRM, testRun, findSigmas, rmSizes));
    matrix::Matrix modRMMatrix = modRM.toMatrix() * res[0];
    matrix::Matrix sigma1 = res[1];
    matrix::Matrix sigma2 = res[2];
    // Clearing 2 and 3 rows from row 4 
    attackSupporters::clearRows23(modRMMatrix, k1, k2, testRun);
    matrix::Matrix P = matrix::blockDiag(4, sigma1);
    modRMMatrix *= P;
    attackSupporters::clearRow1(modRMMatrix, k1, k2, testRun);
    P.T();
    modRMMatrix *= P;

    return modRM;
}

} // namespace codes