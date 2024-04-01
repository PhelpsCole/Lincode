#ifndef SSA_TEST_H
#define SSA_TEST_H

#include "ssa.h"

namespace ssa_test {

void printSSAStructure(const codes::SSAStructure &s,
                       const std::string &filename);

void printSSANStructure(const codes::SSANStructure &s,
                        const std::string &filename);

bool check_signature(const codes::SSAStructure &s, size_t m);
// Need to fix
bool check_signature(const codes::SSANStructure &s, size_t m);

} //namespace ssa_test

#endif // SSA_TEST_H