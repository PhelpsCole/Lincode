#ifndef MINE_SORTS_H
#define MINE_SORTS_H

#include <vector>
#include <functional>
#include <algorithm>

namespace algorithms {
// Template sorts, which returns vector of permutations
namespace sorts {

typedef std::function<bool(const int &, const int &)> intComparatorType;
typedef std::function<bool(const std::vector<char> &, const std::vector<char> &)> charVecCompType;

std::vector<unsigned long long> selectionSort(std::vector<int> &vv,
                                              intComparatorType comparator);
std::vector<unsigned long long> selectionSort(std::vector<std::vector<char>> &vv,
                                              charVecCompType comparator);
std::vector<unsigned long long> mergeSort(std::vector<std::vector<char>> &vv,
                                          std::function<bool(const std::vector<char> &,
                                                             const std::vector<char> &)>
                                          comparator);

} // namespace sorts
} // namespace algorithms

#endif // MINE_SORTS_H