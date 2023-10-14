#include "support_splitting.h"
#include "test_printers.h"

typedef std::function<std::string(const codes::Lincode &,
                                  const std::vector<size_t> &)> fInvar;
typedef std::function<std::vector<size_t>(const codes::Lincode &,
                                          const codes::Lincode &,
                                          fInvar)> fSSA;

void SSArunner(const codes::Lincode &c1, const codes::Lincode &c2,
               fInvar invariant, fSSA support_splitting) {
    std::cout << "STARTS SSArunner" << std::endl;
    //std::cout << "Inputted codes:" << std::endl;
    //c1.printCode();
    //c2.printCode();
    //std::cout << std::endl;
    std::vector<size_t> perm = support_splitting(c1, c2, invariant);
    if  (perm.size()) {
        std::cout << "Permutation vector:" << std::endl;
        codes::test_printers::printV(perm);
    } else {
        std::cout << "Codes are not equal" << std::endl;
    }
    std::cout << std::endl;
}

void manualTests(fInvar invariant, fSSA support_splitting) {
    codes::Lincode code({{1, 0, 0, 0, 1, 0, 1},
                         {0, 1, 0, 0, 1, 1, 1},
                         {0, 0, 1, 0, 1, 1, 0},
                         {0, 0, 0, 1, 0, 1, 1},});
    codes::Lincode code2({{1, 1, 1, 0},
                         {0, 1, 1, 1},
                         {1, 0, 1, 0},});
    codes::Lincode code3({{0, 0, 1, 1},
                         {1, 0, 1, 1},
                         {1, 1, 0, 1},});
    codes::Lincode code4({{0, 1, 1, 0, 1},
                         {0, 1, 0, 1, 1},
                         {0, 1, 1, 1, 0},
                         {1, 0, 1, 0, 1},
                         {1, 1, 1, 1, 0},});
    codes::Lincode code5({{1, 0, 1, 0, 1},
                          {0, 0, 1, 1, 1},
                          {1, 0, 0, 1, 1},
                          {1, 1, 1, 0, 0},
                          {1, 1, 0, 1, 1},});
    codes::RMCode rm(2, 3);
    codes::RMCode rm2(2, 3);
    codes::Lincode rm_code(rm.getBasis());
    codes::Lincode rm_code2(rm2.getBasis());
    std::string str = "1 0 0 0 1 1 1\n0 1 0 0 1 1 0\n0 0 1 0 1 0 1\n0 0 0 1 0 1 1";
    std::string str2 = "1 0 0 0 1 1 1\n0 1 0 0 1 0 1\n0 0 1 0 0 1 1\n0 0 0 1 1 1 0";
    std::string str3 = "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1\n1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0\n1 1 0 0 1 1 0 0 1 1 0 0 1 1 0 0\n1 0 0 1 1 0 0 1 1 0 0 1 1 0 0 1";
    std::string str4 = "1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1\n1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0\n1 1 0 0 1 1 0 0 1 1 0 0 1 0 0 0\n1 0 0 1 1 0 0 1 1 0 0 1 1 0 0 1";
    codes::Lincode code6(str, ' ');
    codes::Lincode code7(str2, ' ');
    codes::Lincode code8(str3, ' ');
    codes::Lincode code9(str4, ' ');
    //Equiv; 1 step; spectr, {3 1 4 2}
    SSArunner(code2, code3, invariant, support_splitting);
    //Not equal
    SSArunner(code2, code4, invariant, support_splitting);
    //Equiv; 2 steps {1 2 } <-> {0 4 }; spectr, {2 5 1 4 3}
    SSArunner(code4, code5, invariant, support_splitting);
    //Equiv; 2 steps {0 4 } <-> {1 2 }; spectr, {3 1 5 4 2}
    SSArunner(code5, code4, invariant, support_splitting);
    //Same; 3 steps {3 5 6 } <-> {3 5 6 } && {1 2 4 } <->;
    //{3 5 } <-> {3 5 } && {1 2 } <-> {1 2 }
    SSArunner(rm_code, rm_code2, invariant, support_splitting);
    //Eq; 2 steps; spectr, {1 2 3 4 5 6 7} BUT it's incorrect (Hamming code) ans: {1 2 3 4 7 6 5}
    SSArunner(code6, code7, invariant, support_splitting);
    //Not equal, zero steps
    SSArunner(code8, code9, invariant, support_splitting);
    //Mcliece Random result
    SSArunner(rm_code, codes::mcEliece(rm_code), invariant, support_splitting);
}

int main(void) {
    std::cout << "SAA" << std::endl;
//    std::cout << "invariantWeightHull" << std::endl;
//    manualTests(codes::invariants::invariantWeightHull, codes::orig::support_splitting);
//    std::cout << "invariantHullSize" << std::endl;
//    manualTests(codes::invariants::invariantHullSize, codes::orig::support_splitting);
    std::cout << "invariantWeightBasis" << std::endl;
    manualTests(codes::invariants::invariantWeightBasis, codes::support_splitting);
    std::cout << "InDeep SAA" << std::endl;
//    std::cout << "invariantWeightHull" << std::endl;
//    manualTests(codes::invariants::invariantWeightHull, codes::indeep::support_splitting);
//    std::cout << "invariantHullSize" << std::endl;
//    manualTests(codes::invariants::invariantHullSize, codes::indeep::support_splitting);
    std::cout << "invariantWeightBasis" << std::endl;
    manualTests(codes::invariants::invariantWeightBasis, codes::indeep::support_splitting);
    return 0;
}