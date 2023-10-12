#ifndef SSA_H
#define SSA_H

#include <functional>
#include <set>
#include "codes.h"

namespace codes {

struct SSAData {
    std::vector<size_t> found1;
    std::vector<size_t> to_find1;
    std::vector<size_t> found2;
    std::vector<size_t> to_find2;
};

std::vector<size_t> support_splitting(const codes::Lincode &, const codes::Lincode &,
                                      std::function<std::string(const codes::Lincode &,
                                                                const std::vector<size_t> &)>);
} //namespace codes

#endif // SSA_H