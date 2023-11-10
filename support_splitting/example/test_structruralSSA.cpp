#include "support_splitting.h"
#include "test_printers.h"

void testPqsigRM(size_t r, size_t m,
                 std::function<std::string(const codes::Lincode &,
                                           const std::vector<size_t> &)> invariant,
                 std::function<matrix::Matrix(size_t, size_t)> genFunc) {
    matrix::Matrix pqsigRM = genFunc(r, m);
    codes::Lincode c_perm(pqsigRM);
    codes::SSAStructure s = codes::ssaStructure(c_perm, invariant);
    codes::printSSAStructure(s);
}

void testMcElieceRM(size_t r, size_t m,
                    std::function<std::string(const codes::Lincode &,
                                              const std::vector<size_t> &)> invariant) {
    codes::RMCode rm(r, m);
    matrix::Matrix matr(rm.toMatrix());
    codes::Lincode c_prev(matr);
    codes::Lincode rmMcEliece = codes::mcEliece(c_prev);
    codes::SSAStructure s = codes::ssaStructure(rmMcEliece, invariant);
    codes::printSSAStructure(s);
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
                                      const std::vector<size_t> &)> invariant) {
    codes::SSAStructure s = codes::ssaStructure(c, invariant);
    codes::printSSAStructure(s);
}

void randomTestSiclesPqsigRM(size_t r, size_t m, size_t iterations,
                             std::function<std::string(const codes::Lincode &,
                                                       const std::vector<size_t> &)> invariant,
                        std::function<matrix::Matrix(size_t, size_t)> genFunc) {
    for (size_t i = 0; i < iterations; ++i) {
        testPqsigRM(r, m, invariant, genFunc);
    }
}

void randomTestSiclesMcElieceRM(size_t r, size_t m, size_t iterations,
                                std::function<std::string(const codes::Lincode &,
                                                          const std::vector<size_t> &)> invariant) {
    for (size_t i = 0; i < iterations; ++i) {
        testMcElieceRM(r, m, invariant);
    }
}

void testByInvariant(std::function<std::string(const codes::Lincode &,
                                               const std::vector<size_t> &)> invariant) {
    //std::cout << "invariantWeightBasis pqsigRM" << std::endl;
    //randomTestSiclesPqsigRM(2, 5, 1, invariant, codes::pqsigRMMcEliece);
    //std::cout << "invariantWeightBasis McEliece pqsigRM" << std::endl;
    //randomTestSiclesMcElieceRM(2, 5, 1, invariant);
    //std::cout << "invariantWeightBasis pqsigRM" << std::endl;
    randomTestSiclesPqsigRM(3, 7, 1, invariant, codes::pqsigRMMcEliece);
    //std::cout << "invariantWeightBasis McEliece pqsigRM" << std::endl;
    randomTestSiclesMcElieceRM(3, 7, 1, invariant);
    //std::cout << "invariantWeightBasis pqsigRM" << std::endl;
    //randomTestSiclesPqsigRM(3, 8, 1, invariant, codes::pqsigRMMcEliece);
    //std::cout << "invariantWeightBasis McEliece pqsigRM" << std::endl;
    //randomTestSiclesMcElieceRM(3, 8, 1, invariant);
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
    runSSA(code, invariant);
    //codes::RMCode rm(2, 5);
    //codes::Lincode code2(rm.toMatrix());
    //runSSA(code2, invariant);
}

int main() {
    testByInvariant(codes::invariants::invariantHullSize);
    testByInvariant(codes::invariants::invariantWeightHull);
    //testByInvariant(codes::invariants::invariantWeightMinRM);
    return 0;
}