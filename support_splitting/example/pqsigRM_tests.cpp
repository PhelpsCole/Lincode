#include "support_splitting.h"
#include "test_printers.h"

void testPqsigRM(size_t r, size_t m,
                 std::function<std::string(const codes::Lincode &)> invariant,
                 std::function<matrix::Matrix(size_t, size_t)> genFunc) {
    matrix::Matrix rm(codes::RMCode(r, m).toMatrix());
    codes::Lincode c(rm);
    matrix::Matrix pqsigRM = genFunc(r, m);
    codes::Lincode c_perm(pqsigRM);
    std::vector<size_t> v = codes::support_splitting(c, c_perm, invariant);
    if (v.size()) {
        //std::cout << "Permutation vector:" << std::endl;
        //if (!check_permutation(matrix::permFromVector(v), p)) {
        //    std::cout << "Incorrect permutation: " << std::endl;
        //}
        codes::test_printers::printV(v);
        //codes::test_printers::printVInd(v);
    } else {
        std::cout << "Codes are not equal" << std::endl;
    }
}

std::vector<size_t> generateNumVector(size_t start, size_t end) {
    if (end <= start) {
        return std::vector<size_t>(0);
    }
    std::vector<size_t> v(end - start);
    for (size_t i = 0; i < end - start; ++i) {
        v[i] = start + i;
    }
    return v;
}

void runSSA(const codes::Lincode &c1, const codes::Lincode &c2,
            std::function<std::string(const codes::Lincode &)> invariant) {
    std::vector<size_t> v = codes::support_splitting(c1, c2, invariant);
    if (v.size()) {
        //std::cout << "Permutation vector:" << std::endl;
        //if (!check_permutation(matrix::permFromVector(v), p)) {
        //    std::cout << "Incorrect permutation: " << std::endl;
        //}
        codes::test_printers::printV(v);
        //codes::test_printers::printVInd(v);
    } else {
        std::cout << "Codes are not equal" << std::endl;
    }
}

void testPqsigRMonSubmatr(size_t r, size_t m,
                          std::function<std::string(const codes::Lincode &)> invariant,
                          std::function<matrix::Matrix(size_t, size_t)> genFunc) {
    matrix::Matrix rm(codes::RMCode(r, m - 2).toMatrix());
    codes::Lincode c(rm);
    matrix::Matrix pqsigRM = genFunc(r, m);
    std::cout << "Block 1:" << std::endl;
    matrix::Matrix submatr = pqsigRM.submatrix(generateNumVector(0, rm.rows()),
                                               generateNumVector(0, rm.cols()));
    codes::Lincode c_perm(submatr);
    runSSA(c, c_perm, invariant);
    std::cout << "Block 2:" << std::endl;
    submatr = pqsigRM.submatrix(generateNumVector(0, rm.rows()),
                                generateNumVector(rm.cols(), 2 * rm.cols()));
    c_perm = codes::Lincode(submatr);
    runSSA(c, c_perm, invariant);
    std::cout << "Block 3:" << std::endl;
    submatr = pqsigRM.submatrix(generateNumVector(0, rm.rows()),
                                generateNumVector(2 * rm.cols(), 3 * rm.cols()));
    c_perm = codes::Lincode(submatr);
    runSSA(c, c_perm, invariant);
    std::cout << "Block 1:" << std::endl;
    submatr = pqsigRM.submatrix(generateNumVector(0, rm.rows()),
                                generateNumVector(3 * rm.cols(), 4 * rm.cols()));
    c_perm = codes::Lincode(submatr);
    runSSA(c, c_perm, invariant);
    std::cout << std::endl;
}

void randomTestSiclesPqsigRM(size_t r, size_t m, size_t iterations,
                        std::function<std::string(const codes::Lincode &)> invariant,
                        std::function<matrix::Matrix(size_t, size_t)> genFunc) {
    for (size_t i = 0; i < iterations; ++i) {
        testPqsigRM(r, m, invariant, genFunc);
    }
}

void randomTestSiclesPqsigRMSub(size_t r, size_t m, size_t iterations,
                        std::function<std::string(const codes::Lincode &)> invariant,
                        std::function<matrix::Matrix(size_t, size_t)> genFunc) {
    for (size_t i = 0; i < iterations; ++i) {
        testPqsigRMonSubmatr(r, m, invariant, genFunc);
    }
}

int main() {

    //std::cout << "invariantWeightBasis pqsigRM" << std::endl;
    //randomTestSiclesPqsigRM(3, 10, 20, codes::invariants::invariantWeightBasis);
    std::cout << "invariantWeightBasis pqsigRM on submatr" << std::endl;
    randomTestSiclesPqsigRMSub(3, 7, 20, codes::invariants::invariantWeightBasis, codes::pqsigRMGenerator);
    std::cout << "invariantWeightBasis pqsigRM with McEliece on submatr" << std::endl;
    randomTestSiclesPqsigRMSub(3, 7, 20, codes::invariants::invariantWeightBasis, codes::pqsigRMMcEliece);

    //std::cout << "invariantWeightHullHadSquare pqsigRM" << std::endl;
    //randomTestSiclesPqsigRM(3, 7, 10, codes::invariants::invariantWeightHullHadSquare);
    //std::cout << "invariantWeightHullHadSquare pqsigRM on submatr" << std::endl;
    //randomTestSiclesPqsigRMSub(3, 7, 20, codes::invariants::invariantWeightHullHadSquare);
    return 0;
}