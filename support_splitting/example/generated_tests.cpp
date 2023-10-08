#include "support_splitting.h"
#include "test_printers.h"

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

void randomPermTest(const matrix::Matrix &matrCode,
                    std::function<std::string(const codes::Lincode &)> invariant,
                    std::function<codes::Lincode(const matrix::Matrix &,
                                                 matrix::Matrix &)> permFunc) {
    //std::cout << "STARTS randomTest" << std::endl;
    codes::Lincode c(matrCode);
    matrix::Matrix p(matrCode.cols(), matrCode.cols());
    codes::Lincode c_perm = permFunc(matrCode, p);
    std::vector<size_t> v = codes::support_splitting(c, c_perm, invariant);
    if (v.size()) {
        //std::cout << "Permutation vector:" << std::endl;
        codes::test_printers::printV(v);
    } else {
        std::cout << "Codes are not equal" << std::endl;
    }
}

void randomPermMatrTest(size_t k, size_t n,
                        std::function<std::string(const codes::Lincode &)> invariant,
                        std::function<codes::Lincode(const matrix::Matrix &,
                                                     matrix::Matrix &)> permFunc) {
    matrix::Matrix m = matrix::generateRandomNonSingular(k, n);
    randomPermTest(m, invariant, permFunc);
}

void randomPermRMTest(size_t r, size_t m,
                      std::function<std::string(const codes::Lincode &)> invariant,
                      std::function<codes::Lincode(const matrix::Matrix &,
                                                   matrix::Matrix &)> permFunc) {
    codes::RMCode rm(r, m);
    matrix::Matrix matr(rm.toMatrix());
    randomPermTest(matr, invariant, permFunc);
}

void randomTestSicles(size_t k, size_t n, size_t iterations,
                      std::function<std::string(const codes::Lincode &)> invariant,
                      std::function<codes::Lincode(const matrix::Matrix &,
                                                   matrix::Matrix &)> permFunc) {
    for (size_t i = 0; i < iterations; ++i) {
        randomPermMatrTest(k, n, invariant, permFunc);
    }
}

void randomTestSiclesRM(size_t r, size_t m, size_t iterations,
                        std::function<std::string(const codes::Lincode &)> invariant,
                        std::function<codes::Lincode(const matrix::Matrix &,
                                                     matrix::Matrix &)> permFunc) {
    for (size_t i = 0; i < iterations; ++i) {
        randomPermRMTest(r, m, invariant, permFunc);
    }
}

int main() {
    //randomTest(10, 20, codes::invariants::invariantWeightBasis);
    //randomTest(10, simplePerm20, codes::invariants::invariantHullSize);
    //std::cout << "invariantHullSize" << std::endl;
    //randomTestSicles(10, 20, 20, codes::invariants::invariantHullSize, simplePerm);
    //std::cout << "invariantWeightHull" << std::endl;
    //randomTestSicles(10, 20, 20, codes::invariants::invariantWeightHull, simplePerm);
    std::cout << "invariantWeightBasis" << std::endl;
    randomTestSicles(20, 64, 20, codes::invariants::invariantWeightBasis, simplePerm);
    //std::cout << "invariantHullSize for RM" << std::endl;
    //randomTestSiclesRM(2, 4, 20, codes::invariants::invariantHullSize, simplePerm);
    //std::cout << "invariantWeightHull for RM" << std::endl;
    //randomTestSiclesRM(2, 4, 20, codes::invariants::invariantWeightHull, simplePerm);
    //std::cout << "invariantWeightBasis for RM" << std::endl;
    //randomTestSiclesRM(2, 5, 20, codes::invariants::invariantWeightBasis, simplePerm);
    std::cout << "invariantHullHadSquareSize" << std::endl;
    randomTestSicles(10, 20, 20, codes::invariants::invariantHullHadSquareSize, simplePerm);
    std::cout << "invariantHullHadSquareSize for RM" << std::endl;
    randomTestSiclesRM(3, 6, 20, codes::invariants::invariantHullHadSquareSize, simplePerm);
    std::cout << "invariantWeightHull" << std::endl;
    randomTestSicles(10, 20, 20, codes::invariants::invariantWeightHull, simplePerm);
    std::cout << "invariantWeightHullBasis for RM" << std::endl;
    randomTestSiclesRM(2, 5, 20, codes::invariants::invariantWeightHullBasis, simplePerm);
}