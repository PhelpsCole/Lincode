#ifndef CODE_ATTACKS_H
#define CODE_ATTACKS_H

#include <cmath>
#include <set>
#include <algorithm>
#include "sorts.h"
#include "codes.h"
#include "encoder.h"

namespace codes {
namespace attackSupporters {

// Supporters from minder_shokrollahi 
std::vector<std::vector<unsigned long long>>
decomposeToColumnSets(const codes::Lincode &c0, size_t r, size_t m,
                      unsigned long long M = 10);
codes::Lincode rmReductor(const codes::Lincode &rm);
matrix::Matrix simplePerm(matrix::Matrix);

// Supporters from chizhov_borodin
codes::Lincode nodRM(const codes::Lincode &rm, int a, int b);

} //namespace attackSupporters

matrix::Matrix chizhov_borodin(codes::Lincode);

} // namespace codes

#endif // CODE_ATTACKS_H