#ifndef CODE_ATTACKS_H
#define CODE_ATTACKS_H

#include <cmath>
#include <set>
#include <algorithm>
#include "sorts.h"
#include "codes.h"
#include "encoder.h"
#include "support_splitting.h"
#include "algorithms.h"

namespace codes {
namespace attackSupporters {

struct ExtractBlockResult {
    matrix::Matrix block;
    matrix::Matrix sigmaP;
};

typedef std::vector<std::vector<unsigned long long>> VectorOfColumns;

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
std::vector<unsigned long long> findingBlockColumns(codes::Lincode,
                                                    size_t r, size_t m,
                                                    bool testRun = false);
ExtractBlockResult extractFirstBlock(const matrix::Matrix &,
                                     unsigned long long blockRowsSize = 0);
ExtractBlockResult extractLastBlock(const matrix::Matrix &,
                                    unsigned long long blockRowsSize = 0);

// Supporters from separate_RM_blocks
VectorOfColumns
separatingGaussElimination(matrix::Matrix &m, unsigned long long k1, unsigned long long k2);
VectorOfColumns
rowsIntersections(const matrix::Matrix &m, unsigned long long k1, unsigned long long k2);

} //namespace attackSupporters

matrix::Matrix minder_shokrollahi(codes::Lincode rm);
matrix::Matrix chizhov_borodin(const codes::Lincode &rm);
matrix::Matrix concatenateRM_attack(const codes::Lincode &rm, size_t blocksNum);
// Returns Matrix of code unlike other attacks
matrix::Matrix modRM_attack(const codes::Lincode &modRM, bool testRun = false);

std::vector<unsigned long long> separate_blocks(const codes::Lincode &c,
                                                size_t k, size_t n,
                                                size_t minWeight);

} // namespace codes

#endif // CODE_ATTACKS_H