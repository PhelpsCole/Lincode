#ifndef CODE_ATTACKS_H
#define CODE_ATTACKS_H

#include <cmath>
#include <set>
#include <algorithm>
#include "sorts.h"
#include "codes.h"
#include "encoder.h"
#include "support_splitting.h"

namespace codes {
namespace attackSupporters {

// Supporters from minder_shokrollahi 
std::vector<std::vector<unsigned long long>>
decomposeToColumnSets(const codes::Lincode &c0, size_t r, size_t m,
                      unsigned long long M = 0);
codes::Lincode rmReducer(const codes::Lincode &rm, unsigned long long M = 0);
matrix::Matrix simplePerm(matrix::Matrix);

// Supporters from chizhov_borodin
codes::Lincode nodRM(const codes::Lincode &rm, int a, int b);

// Supporters from modRM_attack
enum { PREPROC_SIMPLE_ID = 0, INVARIANT_SIZE_ID = 2 };
std::vector<unsigned long long> findingFirstBlockColumns(codes::Lincode,
                                                         bool testRun = false);
std::pair<matrix::Matrix, matrix::Matrix>
extractFirstBlock(const matrix::Matrix &,
                  std::vector<unsigned long long>,
                  unsigned long long blockRowsSize = 0);

} //namespace attackSupporters

matrix::Matrix minder_shokrollahi(codes::Lincode rm);
matrix::Matrix chizhov_borodin(const codes::Lincode &rm);
// Returns Matrix of code unlike other attacks
matrix::Matrix modRM_attack(const codes::Lincode &modRM);

} // namespace codes

#endif // CODE_ATTACKS_H