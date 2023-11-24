#ifndef PQSIGRM_H
#define PQSIGRM_H

#include <random>
#include "rm_code.h"

namespace codes {

matrix::Matrix pqsigRMGenerator(size_t r, size_t m);
matrix::Matrix pqsigRMMcEliece(size_t r, size_t m);
void maxRMSubMatrPqsigRM(codes::Lincode &pqsigRMcode);

} // namespace codes

#endif // PQSIGRM_H