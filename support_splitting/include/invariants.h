#ifndef INVARIANTS_H
#define INVARIANTS_H

#include "codes.h"

namespace codes {
namespace invariants {

std::string invariantHullSize(const codes::Lincode &code,
                              const std::vector<unsigned long long> &columns);
std::string invariantWeightHull(const codes::Lincode &code,
                                const std::vector<unsigned long long> &columns);
std::string invariantSize(const codes::Lincode &code,
                          const std::vector<unsigned long long> &columns);
std::string invariantWeight(const codes::Lincode &code,
                            const std::vector<unsigned long long> &columns);
std::string invariantWeightBasis(const codes::Lincode &code,
                                 const std::vector<unsigned long long> &columns);

void preprocHull(codes::Lincode &);
void preprocHadPower(codes::Lincode &);
void preprocHadPowerHull(codes::Lincode &);
void preprocMaxRMSubMatrPqsigRM(codes::Lincode &);
void preprocMaxRMSubMatrPqsigRMSafe(codes::Lincode &);

void runPreproc(codes::Lincode &, size_t id);
std::string runInvariant(const codes::Lincode &,
                         const std::vector<unsigned long long> &columns,
                         size_t id);
std::string returnInvarNameById(size_t id);
std::string returnPreprocNameById(size_t id);

} //namespace invariants
} //namespace codes


#endif // INVARIANTS_H