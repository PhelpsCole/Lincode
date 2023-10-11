#ifndef INDEEP_SSA_H
#define INDEEP_SSA_H

#include <functional>
#include <set>
#include "matrix.h"
#include "codes.h"

namespace codes {
namespace indeep {
namespace invariants {

std::string invariantWeightBasis(codes::Lincode code,
                                 const std::vector<size_t> &columns);

// Bad for RM
std::string invariantWeightHull(codes::Lincode code,
                                const std::vector<size_t> &columns);

std::string invariantWeightHullBasis(codes::Lincode code,
                              const std::vector<size_t> &columns);

//Bad invariant
std::string invariantHullSize(codes::Lincode code,
                              const std::vector<size_t> &columns);

//Bad invariant
std::string invariantHullHadSquareSize(codes::Lincode code,
                                       const std::vector<size_t> &columns);

std::string invariantWeightHullHadSquare(codes::Lincode code,
                                         const std::vector<size_t> &columns);

} //namespace invariants

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
bool ansHaveZeroes(const std::vector<size_t> &ans);

std::vector<SpectVectData>
spectPunctVector(const codes::Lincode &c, std::vector<size_t> &used,
                 size_t i, std::vector<size_t> &dif, size_t set_size,
                 std::function<std::string(const codes::Lincode &)> invariant);

std::vector<size_t> support_splitting(const codes::Lincode &c1, const codes::Lincode &c2,
                                      std::function<std::string(codes::Lincode,
                                                                const std::vector<size_t> &)>
                                      invariant);

} //namespace indeep
} //namespace codes

#endif // INDEEP_SSA_H