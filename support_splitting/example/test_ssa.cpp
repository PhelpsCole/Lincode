#include "support_splitting.h"
#include "test_printers.h"

void SSArunner(const codes::Lincode &c1, const codes::Lincode &c2,
               std::function<std::string(const codes::Lincode &)> invariant) {
    std::cout << "STARTS SSArunner" << std::endl;
    //std::cout << "Inputted codes:" << std::endl;
    //c1.printCode();
    //c2.printCode();
    //std::cout << std::endl;
    std::vector<size_t> perm = codes::support_splitting(c1, c2, invariant);
    if  (perm.size()) {
        std::cout << "Permutation vector:" << std::endl;
        codes::test_printers::printV(perm);
    } else {
        std::cout << "Codes are not equal" << std::endl;
    }
    std::cout << std::endl;
}

int main() {
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
    SSArunner(code2, code3, codes::invariants::invariantWeightBasis);
    //Not equal
    SSArunner(code2, code4, codes::invariants::invariantWeightBasis);
    //Equiv; 2 steps {1 2 } <-> {0 4 }; spectr, {2 5 1 4 3}
    SSArunner(code4, code5, codes::invariants::invariantWeightBasis);
    //Equiv; 2 steps {0 4 } <-> {1 2 }; spectr, {3 1 5 4 2}
    SSArunner(code5, code4, codes::invariants::invariantWeightBasis);
    //Same; 3 steps {3 5 6 } <-> {3 5 6 } && {1 2 4 } <->;
    //{3 5 } <-> {3 5 } && {1 2 } <-> {1 2 }
    SSArunner(rm_code, rm_code2, codes::invariants::invariantWeightBasis);
    //Eq; 2 steps; spectr, {1 2 3 4 5 6 7} BUT it's incorrect (Hamming code) ans: {1 2 3 4 7 6 5}
    SSArunner(code6, code7, codes::invariants::invariantWeightBasis);
    //Not equal, zero steps
    SSArunner(code8, code9, codes::invariants::invariantWeightBasis);
    //Mcliece Random result
    SSArunner(rm_code, codes::mcEliece(rm_code), codes::invariants::invariantWeightBasis);
}