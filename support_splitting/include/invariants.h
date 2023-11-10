#ifndef INVARIANTS_H
#define INVARIANTS_H

#include "codes.h"

namespace codes {
namespace invariants {

typedef std::function<std::string(const codes::Lincode &,
                                  const std::vector<size_t> &)> invarType;

typedef std::function<void(codes::Lincode &)> preprocType;

std::string invariantHullSize(const codes::Lincode &code,
                              const std::vector<size_t> &columns);
std::string invariantWeightHull(const codes::Lincode &code,
                                const std::vector<size_t> &columns);
std::string invariantSize(const codes::Lincode &code,
                          const std::vector<size_t> &columns);
std::string invariantWeight(const codes::Lincode &code,
                            const std::vector<size_t> &columns);
std::string invariantWeightBasis(const codes::Lincode &code,
                                 const std::vector<size_t> &columns);

void preprocSimple(codes::Lincode &);
void preprocHull(codes::Lincode &);
void preprocHadPower(codes::Lincode &);
void preprocHadPowerHull(codes::Lincode &);

invarType returnInvarById(size_t id);
preprocType returnPreprocById(size_t id);
std::string returnInvarNameById(size_t id);
std::string returnPreprocNameById(size_t id);

} //namespace invariants
} //namespace codes


#endif // INVARIANTS_H