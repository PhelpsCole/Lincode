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

struct ExtractBlockResult {
    matrix::Matrix block;
    matrix::Matrix sigmaP;
};
typedef std::pair<std::vector<std::pair<std::vector<unsigned long long>,
                                        std::vector<unsigned long long>>>,
                 unsigned long long> combinationStepType;
typedef std::vector<combinationStepType> combinationStepVector;


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
                                                    bool returnMax = false,
                                                    bool testRun = false);
ExtractBlockResult extractFirstBlock(const matrix::Matrix &,
                                     bool findSigma = true,
                                     unsigned long long blockRowsSize = 0);
ExtractBlockResult extractLastBlock(const matrix::Matrix &,
                                    bool findSigma = true,
                                    unsigned long long blockRowsSize = 0);

// Supporters from separate_RM_blocks
std::vector<std::vector<unsigned long long>>
separatingGaussElimination(matrix::Matrix &m, unsigned long long k1, unsigned long long k2);
std::vector<std::vector<unsigned long long>>
rowsIntersections(const matrix::Matrix &m,
                  std::vector<unsigned long long> row1,
                  std::vector<unsigned long long> row2,
                  std::vector<unsigned long long> row3);
std::vector<std::vector<unsigned long long>>
splittingToClassesByWords(const std::vector<std::vector<unsigned long long>> &minWeightWordsSupports,
                          std::map<unsigned long long, size_t> &interCnt,
                          unsigned long long minWeight);

std::vector<unsigned long long> hadPowerCounter(const codes::Lincode &modRM,
                                                unsigned long long k1,
                                                unsigned long long k2,
                                                unsigned long long minWeight,
                                                unsigned long long iterationNumber,
                                                bool printData = true);
std::vector<unsigned long long>
hadPowerCounterSelectors(const codes::Lincode &modRM,
                         unsigned long long iterationNumber,
                         bool printData = true);

} //namespace attackSupporters

matrix::Matrix minder_shokrollahi(codes::Lincode rm);
matrix::Matrix chizhov_borodin(const codes::Lincode &rm);
matrix::Matrix concatenateRM_attack(const codes::Lincode &rm, size_t blocksNum);
std::vector<std::vector<unsigned long long>>
separateConcatenatedCodeToSets(const codes::Lincode &concatCode);
matrix::Matrix separateConcatenatedCodes(const codes::Lincode &concatCode);
// Returns perm 1 x P_{2-3} x 1, P'_1 and P'_4
std::vector<matrix::Matrix>
modRM_attack(const codes::Lincode &modRM, bool testRun = false);

std::vector<unsigned long long> separate_blocks(const codes::Lincode &c,
                                                size_t k, size_t n,
                                                size_t minWeight);

} // namespace codes

#endif // CODE_ATTACKS_H