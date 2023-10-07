#include "support_splitting.h"
#include "test_printers.h"

void sSArunner(const codes::Lincode &c1, const codes::Lincode &c2,
               std::function<std::string(const codes::Lincode &)> invariant) {
    std::vector<size_t> perm = codes::support_splitting(c1, c2, invariant);
    if  (perm.size()) {
        codes::test_printers::printV(perm);
    } else {
        std::cout << "Codes are not equal" << std::endl;
    }
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
    sSArunner(code2, code3, codes::invariants::invariant_weight);
    sSArunner(code2, code4, codes::invariants::invariant_weight);
    sSArunner(code4, code5, codes::invariants::invariant_weight);
    sSArunner(code5, code4, codes::invariants::invariant_weight);
    sSArunner(rm_code, rm_code2, codes::invariants::invariant_weight);
}