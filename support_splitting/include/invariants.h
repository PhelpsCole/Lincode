#include "codes.h"

namespace codes {
namespace invariants {

typedef std::function<std::string(const codes::Lincode &,
                                  const std::vector<size_t> &)> invarType;

std::string invariantHullSize(const codes::Lincode &code,
                              const std::vector<size_t> &columns); // id = 0
std::string invariantWeightHull(const codes::Lincode &code,
                                const std::vector<size_t> &columns); // id = 1
std::string invariantSimpleHullSize(const codes::Lincode &code,
                                    const std::vector<size_t> &columns); // id = 2
std::string invariantHadSquareSize(const codes::Lincode &code,
                               const std::vector<size_t> &columns); // id = 3
std::string invariantWeightHadSquare(const codes::Lincode &code,
                                     const std::vector<size_t> &columns); // id = 4
std::string invariantHullHadSquareSize(const codes::Lincode &code,
                                       const std::vector<size_t> &columns); // id = 5
std::string invariantWeightHullHadSquare(const codes::Lincode &code,
                                         const std::vector<size_t> &columns); // id = 6
std::string invariantWeightBasis(const codes::Lincode &code,
                                 const std::vector<size_t> &columns); // id = 7

invarType returnInvarById(size_t id);
std::string returnInvarNameById(size_t id);

} //namespace invariants
} //namespace codes
