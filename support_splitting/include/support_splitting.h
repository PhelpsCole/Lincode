#ifndef SUPPORT_SPLITTING_H
#define SUPPORT_SPLITTING_H

#include <functional>
#include <set>
#include "matrix.h"
#include "codes.h"

namespace codes {
namespace invariants {

std::string invariantWeightBasis(const codes::Lincode &code);
//Bad for RM
std::string invariantWeightHull(const codes::Lincode &code);
//Bad
std::string invariantWeightHullBasis(const codes::Lincode &code);
//Bad
std::string invariantHullSize(const codes::Lincode &code);
//Bad
std::string invariantHullHadSquareSize(const codes::Lincode &code);
std::string invariantWeightHullHadSquare(const codes::Lincode &code);

} //namespace invariants

std::vector<size_t> support_splitting(const codes::Lincode &c1, const codes::Lincode &c2,
                                      std::function<std::string(const codes::Lincode &)>
                                      invariant);

} //namespace codes

#endif // SUPPORT_SPLITTING_H