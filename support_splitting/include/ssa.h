#ifndef SSA_H
#define SSA_H

#include <functional>
#include <set>
#include "codes.h"
#include "invariants.h"

namespace codes {

struct SSAData {
    std::vector<size_t> found1;
    std::vector<size_t> to_find1;
    std::vector<size_t> found2;
    std::vector<size_t> to_find2;
};
struct SSASingleData {
    std::vector<size_t> found;
    std::vector<size_t> to_find;
};
typedef std::pair<std::vector<unsigned long long>, std::string> SSAStep;
typedef std::vector<std::vector<SSAStep>> SSAStructure;
typedef std::map<std::vector<unsigned long long>, std::vector<SSAStep>> SSANStructure;

SSAStructure ssaStructure(codes::Lincode,
                          size_t invarId,
                          size_t preprocId = 0);
SSANStructure ssaNStructure(codes::Lincode,
                            size_t invarId,
                            size_t = 1,
                            size_t preprocId = 0);
} //namespace codes

#endif // SSA_H