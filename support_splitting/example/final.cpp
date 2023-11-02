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

void testPqsigRM_N(size_t r, size_t m,
                   std::function<std::string(const codes::Lincode &,
                                             const std::vector<size_t> &)> invariant,
                   std::function<matrix::Matrix(size_t, size_t)> genFunc,
                   size_t n_sign) {
    matrix::Matrix pqsigRM = genFunc(r, m);
    codes::Lincode c_perm(pqsigRM);
    codes::SSANStructure s = codes::ssaNStructure(c_perm, invariant, n_sign);
    codes::printSSANStructure(s);
}

void testMcElieceRM_N(size_t r, size_t m,
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

void testByInvariant(std::function<std::string(const codes::Lincode &,
                                               const std::vector<size_t> &)> invariant) {
    std::cout << "pqsigRM (3, 5)" << std::endl;
    testPqsigRM(3, 5, invariant, codes::pqsigRMMcEliece);
    std::cout << "McEliece pqsigRM (3, 5)" << std::endl;
    testMcElieceRM(3, 5, invariant);
    std::cout << "pqsigRM (2, 6)" << std::endl;
    testPqsigRM(2, 6, invariant, codes::pqsigRMMcEliece);
    std::cout << "McEliece pqsigRM (2, 6)" << std::endl;
    testMcElieceRM(2, 6, invariant);
    //td::cout << "pqsigRM (3, 10)" << std::endl;
    //estPqsigRM(3, 10, invariant, codes::pqsigRMMcEliece);
    //td::cout << "McEliece pqsigRM (3, 10)" << std::endl;
    //estMcElieceRM(3, 10, invariant);

    std::cout << "pqsigRM (3, 5) 2-puncture" << std::endl;
    testPqsigRM_N(3, 5, invariant, codes::pqsigRMMcEliece, 2);
    std::cout << "McEliece pqsigRM (3, 5) 2-puncture" << std::endl;
    testMcElieceRM_N(3, 5, invariant, 2);
    //std::cout << "pqsigRM (2, 6) 2-puncture" << std::endl;
    //testPqsigRM_N(2, 6, invariant, codes::pqsigRMMcEliece, 2);
    //std::cout << "McEliece pqsigRM (2, 6) 2-puncture" << std::endl;
    //testMcElieceRM_N(2, 6, invariant, 2);
    //std::cout << "pqsigRM (3, 10) 2-puncture" << std::endl;
    //testPqsigRM_N(3, 10, invariant, codes::pqsigRMMcEliece, 2);
    //std::cout << "McEliece pqsigRM (3, 10) 2-puncture" << std::endl;
    //testMcElieceRM_N(3, 10, invariant, 2);

    std::cout << "pqsigRM (3, 5) 4-puncture" << std::endl;
    testPqsigRM_N(3, 5, invariant, codes::pqsigRMMcEliece, 4);
    std::cout << "McEliece pqsigRM (3, 5) 4-puncture" << std::endl;
    testMcElieceRM_N(3, 5, invariant, 4);
    //std::cout << "pqsigRM (2, 6) 4-puncture" << std::endl;
    //testPqsigRM_N(2, 6, invariant, codes::pqsigRMMcEliece, 4);
    //std::cout << "McEliece pqsigRM (2, 6) 4-puncture" << std::endl;
    //testMcElieceRM_N(2, 6, invariant, 4);
    //std::cout << "pqsigRM (3, 10) 4-puncture" << std::endl;
    //testPqsigRM_N(3, 10, invariant, codes::pqsigRMMcEliece, 4);
    //std::cout << "McEliece pqsigRM (3, 10) 4-puncture" << std::endl;
    //testMcElieceRM_N(3, 10, invariant, 4);
}

int main() {
    //std::cout << "invariantHullSize" << std::endl;
    //testByInvariant(codes::invariants::invariantHullSize);
    //std::cout << "invariantWeightHullMinRM" << std::endl;
    //testByInvariant(codes::invariants::invariantWeightHullMinRM);
    std::cout << "invariantWeightHull" << std::endl;
    testByInvariant(codes::invariants::invariantWeightHull);
    return 0;
}