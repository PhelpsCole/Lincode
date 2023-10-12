#ifndef INDEEP_SSA_H
#define INDEEP_SSA_H

#include <functional>
#include <set>
#include "matrix.h"
#include "codes.h"

namespace codes {
namespace indeep {

struct SSAData {
    std::vector<size_t> dif1;
    std::vector<size_t> used1;
    std::vector<size_t> dif2;
    std::vector<size_t> used2;
};

struct SpectVectData {
    std::string spectr;
    size_t dif;
};

struct ComplEqClData {
    std::set<size_t> dif_set1;
    std::set<size_t> dif_set2;
    size_t new_used1;
    size_t new_used2;
};

struct AnsParamSet {
    std::vector<size_t> ans;
    size_t cntFound;
    std::vector<SSAData> newEquivClasses;
};

std::vector<size_t> support_splitting(const codes::Lincode &, const codes::Lincode &,
                                      std::function<std::string(const codes::Lincode &,
                                                                const std::vector<size_t> &)>);

} //namespace indeep
} //namespace codes

#endif // INDEEP_SSA_H