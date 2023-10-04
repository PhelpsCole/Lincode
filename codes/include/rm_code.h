#include <iostream>
#include <vector>
#include <list>
#include <utility>
#include "matrix.h"

namespace codes {

std::vector<std::vector<char>> generateRMCode(size_t r, size_t m);

class RMCode {
public:
    RMCode(size_t, size_t);
    RMCode(size_t, size_t, std::vector<std::vector<char>> &);

    size_t len();
    size_t size();
    size_t min_dist();

    std::vector<std::vector<char>> &getBasis();

    matrix::Matrix toMatrix() const;

private:
    size_t r;
    size_t m;
    std::vector<std::vector<char>> basis;
};


} // namespace codes