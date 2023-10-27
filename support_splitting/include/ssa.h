#ifndef SSA_H
#define SSA_H

#include <functional>
#include <set>
#include "codes.h"

namespace codes {

//typedef std::pair<std::vector<std::vector<size_t>>, std::vector<std::vector<size_t>>> VVPair;
//typedef std::map<size_t, std::pair<std::vector<size_t>, std::string>> SSAStructure;

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
typedef std::pair<std::vector<size_t>, std::string> SSAStep;
typedef std::vector<std::vector<SSAStep>> SSAStructure;
typedef std::map<std::vector<size_t>, std::vector<SSAStep>> SSANStructure;

std::vector<size_t> support_splitting(const codes::Lincode &, const codes::Lincode &,
                                      std::function<std::string(const codes::Lincode &,
                                                                const std::vector<size_t> &)>);
SSAStructure ssaStructure(const codes::Lincode &,
                          std::function<std::string(const codes::Lincode &,
                                                    const std::vector<size_t> &)>);
SSANStructure ssaNStructure(const codes::Lincode &,
                            std::function<std::string(const codes::Lincode &,
                                                      const std::vector<size_t> &)>,
                            size_t = 1);
void printSSAStructure(const SSAStructure &);
void printSSANStructure(const SSANStructure &);
} //namespace codes

#endif // SSA_H