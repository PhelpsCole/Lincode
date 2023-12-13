#include <stddef.h>
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <iostream>

namespace algorithms {

size_t hammingWeight(std::vector<char>);
size_t hammingDistance(std::vector<char>, std::vector<char>);
size_t factorial(size_t);
std::vector<std::vector<unsigned long long>> generatePermSequences(size_t, size_t);
std::vector<std::vector<unsigned long long>> permsOfVector(const std::vector<size_t> &, size_t);
std::vector<std::vector<unsigned long long>> permsOfVectorWithNew(const std::vector<size_t> &, size_t, size_t);
std::vector<size_t> sequence(size_t, size_t, size_t = 1);
void elemDeleter(std::vector<size_t> &, const std::vector<size_t> &);
void addToBinVector(std::vector<char> &v, size_t n);
void addToBinWeightVector(std::pair<std::vector<char>, unsigned long long> &v, size_t n);

} // namespace algorithms