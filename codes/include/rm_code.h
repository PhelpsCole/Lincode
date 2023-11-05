#ifndef RM_CODE_H
#define RM_CODE_H

#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include "matrix.h"
#include "linear_code.h"

namespace codes {

std::vector<std::vector<char>> generateRMCode(size_t r, size_t m);

class RMCode {
public:
    RMCode(size_t, size_t);
    RMCode(size_t, size_t, std::vector<std::vector<char>> &);

    size_t len();
    size_t size();
    size_t min_dist();

    const std::vector<std::vector<char>> &getBasis();

    matrix::Matrix toMatrix() const;

private:
    size_t r;
    size_t m;
    std::vector<std::vector<char>> basis;
};

std::vector<size_t> rmSizes(const codes::Lincode &);
std::vector<int> minRMVector(size_t r, size_t m);
codes::Lincode minRM(const codes::Lincode &);


} // namespace codes

#endif // RM_CODE_H