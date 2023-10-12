#include "codes.h"

namespace codes {
namespace invariants {
namespace indeep {

std::string invariantWeightBasis(const codes::Lincode &code,
                                 const std::vector<size_t> &columns);
// Bad for RM
std::string invariantWeightHull(const codes::Lincode &code,
                                const std::vector<size_t> &columns);
std::string invariantWeightHullBasis(const codes::Lincode &code,
                                     const std::vector<size_t> &columns);
//Bad invariant
std::string invariantHullSize(const codes::Lincode &code,
                              const std::vector<size_t> &columns);
//Bad invariant
std::string invariantHullHadSquareSize(const codes::Lincode &code,
                                       const std::vector<size_t> &columns);
std::string invariantWeightHullHadSquare(const codes::Lincode &code,
                                         const std::vector<size_t> &columns);
} //namespace indeep

std::string invariantWeightHull(const codes::Lincode &code,
                                const std::vector<size_t> &columns);
std::string invariantHullSize(const codes::Lincode &code,
                              const std::vector<size_t> &columns);
std::string invariantWeightBasis(const codes::Lincode &code,
                                 const std::vector<size_t> &columns);
} //namespace invariants
} //namespace codes
