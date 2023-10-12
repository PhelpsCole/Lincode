#include "support_splitting.h"
#include "test_printers.h"

typedef std::function<std::string(const codes::Lincode &,
                                  const std::vector<size_t> &)> fInvar;

codes::Lincode mcEliece(const matrix::Matrix &matrCode, matrix::Matrix &p) {
    std::random_device rd;
    std::mt19937 gen(rd());
    
    size_t k = matrCode.rows();
    size_t n = matrCode.cols();
    matrix::Matrix M = matrix::generateRandomNonSingular(k, k);

    std::uniform_int_distribution<int> distrib(1, n);
    p = matrix::generateRandomPermutation(n, distrib(gen));
    matrix::Matrix res = M * matrCode * p;
    return codes::Lincode(res);
}

codes::Lincode simplePerm(const matrix::Matrix &matrCode, matrix::Matrix &p) {
    p = matrix::generateRandomPermutation(matrCode.cols(), matrCode.cols() / 4);
    matrix::Matrix m_perm = matrCode * p;
    return codes::Lincode(m_perm);
}

codes::Lincode equiv(const matrix::Matrix &matrCode, matrix::Matrix &p) {
    p = matrix::diag(matrCode.cols(), 1);
    return codes::Lincode(matrCode);
}

bool check_permutation(matrix::Matrix m, const matrix::Matrix &m2) {
    m.T();
    if (m.rank() != std::min(m.cols(), m.rows())) {
        // Not full pemutation
        return true;
    }
    m *= m2;
    return matrix::isEqual(m, matrix::diag(m.cols(), 1));
}

void randomPermTest(const matrix::Matrix &matrCode, fInvar invariant,
                    std::function<codes::Lincode(const matrix::Matrix &,
                                                 matrix::Matrix &)> permFunc) {
    //std::cout << "STARTS randomTest" << std::endl;
    codes::Lincode c(matrCode);
    matrix::Matrix p(matrCode.cols(), matrCode.cols());
    codes::Lincode c_perm = permFunc(matrCode, p);
    std::vector<size_t> v = codes::support_splitting(c, c_perm, invariant);
    //std::vector<size_t> v = codes::indeep::support_splitting(c, c_perm, invariant);
    if (v.size()) {
        //std::cout << "Permutation vector:" << std::endl;
        if (!check_permutation(matrix::permFromVector(v), p)) {
            std::cout << "Incorrect permutation: " << std::endl;
        }
        codes::test_printers::printV(v);
    } else {
        std::cout << "Codes are not equal" << std::endl;
    }
}

void testPqsigRM(size_t r, size_t m, fInvar invariant) {
    matrix::Matrix rm(codes::RMCode(r, m).toMatrix());
    codes::Lincode c(rm);
    matrix::Matrix pqsigRM = codes::pqsigRMGenerator(r, m);
    codes::Lincode c_perm(pqsigRM);
    std::vector<size_t> v = codes::support_splitting(c, c_perm, invariant);
    //std::vector<size_t> v = codes::indeep::support_splitting(c, c_perm, invariant);
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

void randomPermMatrTest(size_t k, size_t n, fInvar invariant,
                        std::function<codes::Lincode(const matrix::Matrix &,
                                                     matrix::Matrix &)> permFunc) {
    matrix::Matrix m = matrix::generateRandomNonSingular(k, n);
    randomPermTest(m, invariant, permFunc);
}

void randomPermRMTest(size_t r, size_t m, fInvar invariant,
                      std::function<codes::Lincode(const matrix::Matrix &,
                                                   matrix::Matrix &)> permFunc) {
    codes::RMCode rm(r, m);
    matrix::Matrix matr(rm.toMatrix());
    randomPermTest(matr, invariant, permFunc);
}

void randomTestSicles(size_t k, size_t n, size_t iterations, fInvar invariant,
                      std::function<codes::Lincode(const matrix::Matrix &,
                                                   matrix::Matrix &)> permFunc) {
    for (size_t i = 0; i < iterations; ++i) {
        randomPermMatrTest(k, n, invariant, permFunc);
    }
}

void randomTestSiclesRM(size_t r, size_t m, size_t iterations, fInvar invariant,
                        std::function<codes::Lincode(const matrix::Matrix &,
                                                     matrix::Matrix &)> permFunc) {
    for (size_t i = 0; i < iterations; ++i) {
        randomPermRMTest(r, m, invariant, permFunc);
    }
}

void randomTestSiclesPqsigRM(size_t r, size_t m, size_t iterations, fInvar invariant) {
    for (size_t i = 0; i < iterations; ++i) {
        testPqsigRM(r, m, invariant);
    }
}

int main() {
    //std::cout << "invariantHullSize" << std::endl;
    //randomTestSicles(10, 20, 20, codes::invariants::invariantHullSize, simplePerm);
    //std::cout << "invariantHullSize for RM" << std::endl;
    //randomTestSiclesRM(2, 4, 20, codes::invariants::indeep::invariantHullSize, simplePerm);

    //std::cout << "invariantWeightHull" << std::endl;
    //randomTestSicles(10, 20, 20, codes::invariants::indeep::invariantWeightHull, simplePerm);
    //std::cout << "invariantWeightHull for RM" << std::endl;
    //randomTestSiclesRM(2, 4, 20, codes::invariants::indeep::invariantWeightHull, simplePerm);

    //std::cout << "invariantWeightBasis" << std::endl;
    //randomTestSicles(20, 64, 20, codes::invariants::indeep::invariantWeightBasis, simplePerm);
    //std::cout << "invariantWeightBasis for RM" << std::endl;
    //randomTestSiclesRM(2, 5, 20, codes::invariants::indeep::invariantWeightBasis, simplePerm);

    //std::cout << "invariantHullHadSquareSize" << std::endl;
    //randomTestSicles(10, 20, 20, codes::invariants::indeep::invariantHullHadSquareSize, mcEliece);
    //std::cout << "invariantHullHadSquareSize for RM" << std::endl;
    //randomTestSiclesRM(3, 6, 20, codes::invariants::indeep::invariantHullHadSquareSize, mcEliece);

    //std::cout << "invariantWeightHull" << std::endl;
    //randomTestSicles(10, 20, 20, codes::invariants::invariantWeightHull, simplePerm);
    //std::cout << "invariantWeightHull" << std::endl;
    //randomTestSicles(10, 20, 20, codes::invariants::invariantWeightHull, mcEliece);
    //std::cout << "invariantWeightHull RM equal" << std::endl;
    //randomTestSiclesRM(3, 6, 1, codes::invariants::invariantWeightHull, equiv);
    //std::cout << "invariantWeightHull RM simple" << std::endl;
    //randomTestSiclesRM(3, 6, 1, codes::invariants::invariantWeightHull, simplePerm);
    //std::cout << "invariantWeightHull" << std::endl;
    //randomTestSiclesRM(3, 6, 1, codes::invariants::invariantWeightHull, mcEliece);
    //std::cout << "invariantWeightHullBasis for RM" << std::endl;
    //randomTestSiclesRM(2, 5, 20, codes::invariants::indeep::invariantWeightHullBasis, simplePerm);

    //std::cout << "invariantWeightBasis RM equal" << std::endl;
    //randomTestSiclesRM(3, 7, 1, codes::invariants::indeep::invariantWeightBasis, equiv);
    //std::cout << "invariantWeightBasis RM simple" << std::endl;
    //randomTestSiclesRM(3, 8, 1, codes::invariants::indeep::invariantWeightBasis, simplePerm);
    ////std::cout << "invariantWeightBasis pqsigRM" << std::endl;
    //randomTestSiclesPqsigRM(3, 7, 10, codes::invariants::indeep::invariantWeightBasis);

    //std::cout << "invariantWeightHullHadSquare" << std::endl;
    //randomTestSicles(10, 20, 20, codes::invariants::invariantWeightHullHadSquare, mcEliece);
    std::cout << "invariantWeightHullHadSquare RM equal" << std::endl;
    randomTestSiclesRM(3, 7, 1, codes::invariants::invariantWeightHullHadSquare, equiv);
    std::cout << "invariantWeightHullHadSquare RM simple" << std::endl;
    randomTestSiclesRM(3, 7, 2, codes::invariants::invariantWeightHullHadSquare, simplePerm);
    std::cout << "invariantWeightHullHadSquare pqsigRM" << std::endl;
    randomTestSiclesPqsigRM(3, 7, 2, codes::invariants::invariantWeightHullHadSquare);
}