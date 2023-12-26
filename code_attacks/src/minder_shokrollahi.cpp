#include "code_attacks.h"

namespace codes {
namespace attackSupporters {

// Minder-Shokrollahi algorithm of reduction RM(r, m) -> RM(r-1,m)
codes::Lincode rmReductor(const codes::Lincode &rm) {
    codes::Lincode B;
    std::vector<size_t> rmSizes = codes::rmSizes(rm);
    size_t size = codeSizeFromRM(rmSizes[0] - 1, rmSizes[1]);
    size_t weight = 1 << (rmSizes[1] - rmSizes[0]);
    return rm;
}

} //namespace attackSupporters
} // namespace codes
