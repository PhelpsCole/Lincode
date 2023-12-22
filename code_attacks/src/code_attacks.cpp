#include "code_attacks.h"

namespace codes {
namespace attackSupporters {
// Minder-Shokrollahi algorithm of reduction RM(r, m) -> RM(r-1,m)
codes::Lincode rmReductor(const codes::Lincode &rm) {
    //!
    return rm;
}


// Converts RM(r,m) -> RM(d,m)
codes::Lincode nodRM(const codes::Lincode &rm, int a, int b) {
    if (a == 0 && b == 1) {
        return rm;
    } else if (b != 0 && a * b <= 0) {
        if (b > 0) {
            a = 1 - a;
            b *= -1;
        }
        int q = -b / a;
        int s = q * a + b;
        codes::Lincode srm;
        if (s != 0) {
            srm = codes::hadPower(rm, s);
        }
        codes::Lincode qrm = codes::hadPower(rm, q);
        qrm.dual();
        qrm = codes::hadPower(qrm, a);
        if (s != 0) {
            codes::hadamardProduct(qrm, srm);
        }
        if (b > 0) {
            qrm.dual();
        }
        return qrm;
    } else {
        throw std::invalid_argument("Incorrect r and m sizes");
    }
}

bool compare(const std::vector<char> &row1, const std::vector<char> &row2) {
    if (row1.size() != row2.size()) {
        std::cout << row1.size() << " " << row2.size() << std::endl;
        throw std::invalid_argument("Incorrect rows sizes");
    }
    for (size_t i = 0; i < row1.size(); ++i) {
        if (row1[i] > row2[i]) {
            return true;
        } else if (row1[i] < row2[i]) {
            return false;
        }
    }
    return false;
}

std::vector<unsigned long long> sortMatrixPermutation(matrix::Matrix &a) {
    std::vector<unsigned long long> permVec(a.rows());
    std::iota(permVec.begin(), permVec.end(), 0);
    for (size_t i = 0; i < permVec.size() - 1; ++i) {
        size_t ind = i;
        std::vector<char> row = a.row(ind);
        for (size_t j = i + 1; j < permVec.size(); ++j) {
            if (compare(row, a.row(j))) {
                ind = j;
                row = a.row(ind);
            }
        }
        if (i != ind) {
            std::vector<char> tmpRow = row;
            a.insertRow(ind, a.row(i), true);
            a.insertRow(i, row, true);
            unsigned long long tmp = permVec[ind];
            permVec[ind] = permVec[i];
            permVec[i] = tmp;
        }
    }
    return permVec;
}

} //namespace attackSupporters

// Return sigma^-1 by RM(r,m)^sigma
matrix::Matrix chizhov_borodin(codes::Lincode rm) {
    //Step 1: finding d = NOD(r, m-1)'
    std::vector<size_t> rmSizes = codes::rmSizes(rm);
    std::vector<int> ans = algorithms::extendedGcd(rmSizes[1] - 1, rmSizes[0]);
    size_t d = ans[0];
    codes::Lincode rm_d = attackSupporters::nodRM(rm, ans[1], ans[2]);

    codes::Lincode trivialRM;
    if (d == 1) {
        trivialRM = rm_d;
    } else {
        // Step 2: (d,m) => (d-1,m)' !!
        codes::Lincode rm_d2 = attackSupporters::rmReductor(rm_d);

        // Step 3: f((d, m)',(d-1,m)') => (1, m)'
        rm_d.dual();
        trivialRM = codes::hadamardProduct(rm_d, rm_d2);
        trivialRM.dual();
    }

    // Step 4: (1, m)' => (1, m) (get sigma)
    // Get matrix with first row (1..1)
    matrix::Matrix trivialRMMatrix(trivialRM.toMatrix());
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

    // Permutation of sorting rows (tested)
    a.T();
    std::vector<unsigned long long> permVec = attackSupporters::sortMatrixPermutation(a);
    return matrix::permFromVector(permVec);
}

} // namespace codes