#include <stddef.h>
#include <vector>
#include <algorithm>
#include <stdexcept>

namespace algorithms {

size_t hammingWeight(const std::vector<char> &);
size_t hammingDistance(const std::vector<char> &, const std::vector<char> &);
size_t factorial(size_t);
size_t gcd(size_t, size_t);
std::vector<int> extendedGcd(int x, int y);
std::vector<std::vector<unsigned long long>> generatePermSequences(size_t, size_t);
std::vector<std::vector<unsigned long long>> permsOfVector(const std::vector<size_t> &, size_t);
std::vector<std::vector<unsigned long long>> permsOfVectorWithNew(const std::vector<size_t> &, size_t, size_t);
std::vector<size_t> sequence(size_t, size_t, size_t = 1);
void elemDeleter(std::vector<size_t> &, const std::vector<size_t> &);
void addToBinVector(std::vector<char> &v, size_t n);
void addToBinWeightVector(std::pair<std::vector<char>, unsigned long long> &v, size_t n);

} // namespace algorithms