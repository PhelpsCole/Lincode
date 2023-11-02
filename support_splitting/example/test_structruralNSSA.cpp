#include "support_splitting.h"
#include "test_printers.h"

void testPqsigRM(size_t r, size_t m,
                 std::function<std::string(const codes::Lincode &,
                                           const std::vector<size_t> &)> invariant,
                 std::function<matrix::Matrix(size_t, size_t)> genFunc,
                 size_t n_sign) {
    matrix::Matrix pqsigRM = genFunc(r, m);
    codes::Lincode c_perm(pqsigRM);
    codes::SSANStructure s = codes::ssaNStructure(c_perm, invariant, n_sign);
    codes::printSSANStructure(s);
}

void testMcElieceRM(size_t r, size_t m,
                    std::function<std::string(const codes::Lincode &,
                                              const std::vector<size_t> &)> invariant,
                    size_t n_sign) {
    codes::RMCode rm(r, m);
    matrix::Matrix matr(rm.toMatrix());
    codes::Lincode c_prev(matr);
    codes::Lincode rmMcEliece = codes::mcEliece(c_prev);
    codes::SSANStructure s = codes::ssaNStructure(rmMcEliece, invariant, n_sign);
    codes::printSSANStructure(s);
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

void runSSA(const codes::Lincode &c,
            std::function<std::string(const codes::Lincode &,
                                      const std::vector<size_t> &)> invariant,
            size_t n_sign) {
    codes::SSANStructure s = codes::ssaNStructure(c, invariant, n_sign);
    codes::printSSANStructure(s);
}

void randomTestSiclesPqsigRM(size_t r, size_t m, size_t iterations,
                             std::function<std::string(const codes::Lincode &,
                                                       const std::vector<size_t> &)> invariant,
                        std::function<matrix::Matrix(size_t, size_t)> genFunc,
                        size_t n_sign) {
    for (size_t i = 0; i < iterations; ++i) {
        testPqsigRM(r, m, invariant, genFunc, n_sign);
    }
}

void randomTestSiclesMcElieceRM(size_t r, size_t m, size_t iterations,
                                std::function<std::string(const codes::Lincode &,
                                                          const std::vector<size_t> &)> invariant,
                                size_t n_sign) {
    for (size_t i = 0; i < iterations; ++i) {
        testMcElieceRM(r, m, invariant, n_sign);
    }
}

void testByInvariant(std::function<std::string(const codes::Lincode &,
                                               const std::vector<size_t> &)> invariant,
                     size_t n_sign) {
    //std::cout << "invariantWeightBasis pqsigRM" << std::endl;
    randomTestSiclesPqsigRM(2, 4, 1, invariant, codes::pqsigRMMcEliece, n_sign);
    //std::cout << "invariantWeightBasis McEliece pqsigRM" << std::endl;
    randomTestSiclesMcElieceRM(2, 4, 1, invariant, n_sign);
    //std::cout << "invariantWeightBasis pqsigRM" << std::endl;
    randomTestSiclesPqsigRM(3, 7, 1, invariant, codes::pqsigRMMcEliece, n_sign);
    //std::cout << "invariantWeightBasis McEliece pqsigRM" << std::endl;
    randomTestSiclesMcElieceRM(3, 7, 1, invariant, n_sign);
    //std::cout << "invariantWeightBasis pqsigRM" << std::endl;
    randomTestSiclesPqsigRM(3, 8, 1, invariant, codes::pqsigRMMcEliece, n_sign);
    //std::cout << "invariantWeightBasis McEliece pqsigRM" << std::endl;
    randomTestSiclesMcElieceRM(3, 8, 1, invariant, n_sign);
    std::string str = std::string("1&0&0&0&0&0&0&0&0&0&1&0&1&0&1&0&1&1&1&1\n") +
                      std::string("0&1&0&0&0&0&0&0&0&0&0&1&0&1&1&1&1&1&0&0\n") +
                      std::string("0&0&1&0&0&0&0&0&0&0&1&0&0&1&1&0&0&1&0&1\n") +
                      std::string("0&0&0&1&0&0&0&0&0&0&1&1&1&1&1&1&0&1&0&1\n") +
                      std::string("0&0&0&0&1&0&0&0&0&0&0&1&0&0&0&0&1&0&0&0\n") +
                      std::string("0&0&0&0&0&1&0&0&0&0&0&0&0&0&0&0&1&1&1&1\n") +
                      std::string("0&0&0&0&0&0&1&0&0&0&0&1&0&1&0&0&0&1&1&0\n") +
                      std::string("0&0&0&0&0&0&0&1&0&0&0&0&1&1&1&0&0&1&1&1\n") +
                      std::string("0&0&0&0&0&0&0&0&1&0&0&1&1&0&0&1&0&0&0&0\n") +
                      std::string("0&0&0&0&0&0&0&0&0&1&0&0&1&1&1&0&0&0&1&0");
    codes::Lincode code(str, '&', '\n');
    //runSSA(code, invariant, 2);
    //codes::RMCode rm(2, 5);
    //codes::Lincode code2(rm.toMatrix());
    //runSSA(code2, invariant, 2);
}

int main() {
    //testByInvariant(codes::invariants::invariantHullSize, 3);
    //testByInvariant(codes::invariants::invariantWeightHull, 4);
    testByInvariant(codes::invariants::invariantWeightHullMinRM, 2);
    return 0;
}