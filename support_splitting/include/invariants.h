#include "codes.h"

namespace codes {
namespace invariants {

std::string invariantWeightHull(const codes::Lincode &code,
                                const std::vector<size_t> &columns);
std::string invariantHullSize(const codes::Lincode &code,
                              const std::vector<size_t> &columns);
std::string invariantWeightBasis(const codes::Lincode &code,
                                 const std::vector<size_t> &columns);
std::string invariantWeightHullHadSquare(const codes::Lincode &code,
                                         const std::vector<size_t> &columns);
} //namespace invariants
} //namespace codes
