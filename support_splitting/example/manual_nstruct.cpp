#include "support_splitting.h"

int main(void) {
    codes::Lincode code({{0, 1, 1, 0, 1},
                        {0, 1, 0, 1, 1},
                        {0, 1, 1, 1, 0},
                        {1, 0, 1, 0, 1},
                        {1, 1, 1, 1, 0},});
    codes::SSANStructure s = codes::ssaNStructure(code, codes::invariants::invariantWeightBasis, 2);
    codes::SSAStructure s2 = codes::ssaStructure(code, codes::invariants::invariantWeightBasis);
    codes::printSSANStructure(s);
    std::cout << std::endl;
    codes::printSSAStructure(s2);
    return 0;
}