#include "code_attacks.h"

// Minder-Shokrollahi algorithm of reduction RM(r, m) -> RM(r-1,m)
codes::Lincode rmReductor(const codes::Lincode &rm) {
    //!
    return rm;
}

// Return sigma^-1 by RM(r,m)^sigma
matrix::Matrix chizhov_borodin(codes::Lincode rm) {
    //Step 1: finding d = NOD(r, m-1)' !!

    codes::Lincode rm_d = rm; //!
    std::vector<size_t> rmSizes = codes::rmSizes(rm_d); //!
    size_t d = rmSizes[0]; //!

    // Step 2: (d,m) => (d-1,m)' !!
    codes::Lincode trivialRM;
    if (d == 1) {
        trivialRM = rm_d;
    } else {
        codes::Lincode rm_d2 = rmReductor(rm_d);

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
    matrix::Matrix a(sol.size(), sol.size());
    a *= trivialRMMatrix;

    // Delete first row
    std::vector<unsigned long long> rows(a.rows() - 1);
    std::vector<unsigned long long> cols(a.cols());
    std::iota(rows.begin(), rows.end(), 1);
    std::iota(cols.begin(), cols.end(), 0);
    a = a.submatrix(rows, cols);

    // Permutation of sorting rows


    matrix::Matrix perm(0, 0); //!
    return perm;
}