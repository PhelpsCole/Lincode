#ifndef SUPPORT_SPLITTING_H
#define SUPPORT_SPLITTING_H

#include <functional>
#include <set>
#include "matrix.h"
#include "linear_code.h"
#include "rm_code.h"

namespace codes {

namespace support_ssa {

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

bool inSSDataFirst(const SSAData &d, size_t elem);
bool inSSDataSecond(const SSAData &d, size_t elem);

bool isBetterCandidate(const AnsParamSet &, const AnsParamSet &);

std::vector<SpectVectData>
spectPunctVector(const codes::Lincode &c, std::vector<size_t> &used,
                 size_t i, std::vector<size_t> &dif, size_t set_size,
                 std::function<std::string(const codes::Lincode &)> invariant);

} //namespace support_ssa

namespace invariants {

std::string invariantWeightBasis(const codes::Lincode &code);
std::string invariantWeightHull(const codes::Lincode &code);
std::string invariantHullSize(const codes::Lincode &code);

} //namespace invariants

std::vector<size_t> support_splitting(const codes::Lincode &c1, const codes::Lincode &c2,
                                      std::function<std::string(const codes::Lincode &)>
                                      invariant);

} //namespace codes

#endif // SUPPORT_SPLITTING_H