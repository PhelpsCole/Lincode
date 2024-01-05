#include "code_attacks.h"

namespace codes {
namespace attackSupporters {

matrix::Matrix permByZeroRM(const matrix::Matrix &concMatr) {
    matrix::Matrix P(concMatr.cols(), concMatr.cols());
    std::vector<unsigned long long> cnt(concMatr.rows());
    for (size_t i = 0; i < cnt.size(); ++i) {
        cnt[i] = i * concMatr.cols() / concMatr.rows();
    }
    for (size_t j = 0; j < concMatr.cols(); ++j) {
        for (size_t i = 0; i < concMatr.rows(); ++i) {
            if (concMatr.at(i, j) == 1) {
                P.at(j, cnt[i]) = 1;
                ++cnt[i];
            }
        }
    }
    return P;
}

// Returns permutation which separates RMs
matrix::Matrix separateRMs(codes::Lincode concRM, size_t r, size_t m) {
    size_t q;
    while (r != 0 && m % r != 0) {
        q = m / r;
        r = m - r*q - 1;
        concRM = codes::hadPower(concRM, q);
        concRM.dual();
    }
    if (r != 0) {
        q = m / r - 1;
        codes::Lincode temp(codes::hadPower(concRM, q));
        temp.dual();
        concRM = codes::hadamardProduct(codes::hadPower(concRM, q - 1), temp);
        concRM.dual();
    }
    matrix::Matrix concMatr(concRM.toMatrix());
    concMatr.gaussElimination();
    return permByZeroRM(concMatr);
}

} //namespace attackSupporters

// Return perm which makes (r,m)|(r,m)|....
matrix::Matrix concatenateRM_attack(const codes::Lincode &concRM, size_t blocksNum) {
    std::vector<size_t> rmSizes(codes::rmSizes(concRM.size() / blocksNum, concRM.len() / blocksNum));
    matrix::Matrix separatorPerm(attackSupporters::separateRMs(concRM, rmSizes[0], rmSizes[1]));
    matrix::Matrix separated(concRM.toMatrix() * separatorPerm);

    std::vector<unsigned long long> cols(separated.cols() / blocksNum);
    std::vector<unsigned long long> rows(separated.rows() / blocksNum);
    std::vector<matrix::Matrix> permBlockVec;
    for (size_t i = 0; i < blocksNum; ++i) {
        std::iota(cols.begin(), cols.end(), i * separated.cols() / blocksNum);
        unsigned long long rowsPos;
        for (size_t j = 0; j < blocksNum; ++j) {
            if (separated.at(j * separated.rows() / blocksNum, cols[0]) == 1) {
                rowsPos = j * separated.rows() / blocksNum;
                break;
            }
        }
        std::iota(rows.begin(), rows.end(), rowsPos);
        matrix::Matrix subMatr(separated.submatrix(rows, cols));
        permBlockVec.push_back(codes::chizhov_borodin(subMatr));
    }
    matrix::Matrix P(matrix::blockDiag(permBlockVec));
    return separatorPerm * P;
}

} // namespace codes