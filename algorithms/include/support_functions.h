#include <stddef.h>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace algorithms {

size_t hammingWeight(std::vector<char>);
size_t hammingDistance(std::vector<char>, std::vector<char>);
size_t factorial(size_t);
std::vector<std::vector<size_t>> generatePermSequences(size_t, size_t);
std::vector<std::vector<size_t>> permsOfVector(const std::vector<size_t> &, size_t);
std::vector<size_t> sequence(size_t, size_t, size_t = 1);
void elemDeleter(std::vector<size_t> &, const std::vector<size_t> &);

} // namespace algorithms